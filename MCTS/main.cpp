#include <iostream>
#include <vector>	// �洢�ӽڵ�
#include <math.h>	// ����UCB���õ�
#include <set>
#include <ctime>
using namespace std;
// ����ʶ��
typedef int Player;
// �궨��player
#define NOBODY 0
#define HUMAN 1
#define AI 6	// ����ʤ���Ƚ�����
const int n_in_row = 5;
struct Point {
	// ��Ļ���Ͻ�ΪԶ�㣬����Ϊx������Ϊy
	int x;	// x����
	int y;	// y����
	// ���캯��
	Point(int x = 0, int y = 0, Player player = NOBODY):x(x), y(y) {}
	Point(const Point&point): x(point.x), y(point.y) {}
	// ������������,==�Ļ�����ҲҪдһ�°�
	friend ostream&operator<<(ostream&out, Point&point) {
		out << point.x << ", " << point.y;
		return out;
	}
};

class Board {
protected:
	int boardSize;
	// ע����Ҫ�Լ�д���ƹ��캯������Ȼ���ڵ��childrenҲ�ᴫ����
	vector<vector<Player>>board;		// ���̵ĵ��һ����άvector
	

	Player check(int&sum) {
		if (sum / HUMAN == n_in_row) {
			return HUMAN;
		}
		else if (sum / AI == n_in_row) {
			return AI;
		}
		return NOBODY;
	}
	int left;
public:
	
	Board(int boardSize = 10) {
		this->boardSize = boardSize;
		left = boardSize * boardSize;
		this->board = vector<vector<Player>>(boardSize, vector<Player>(boardSize, NOBODY));
	}
	// �жϴ˵��Ƿ����
	bool canMove(const Point&point) const {
		int x = point.x;
		int y = point.y;
		// ���ж�λ���Ƿ�Ϸ�
		if (x < 0 || y < 0 || x >= boardSize || y >= boardSize) return false;
		//  ���жϴ˵��player����
		if (board[y][x] == NOBODY) {
			return true;
		}
		else {
			return false;
		}
	}
	// ����pointλ�õ�player
	void setPlayer(const Point &point, Player player) {
		int x = point.x;
		int y = point.y;
		board[y][x] = player;
		--left;
	}
	// �õ����̴�С
	int getSize() const {
		return boardSize;
	}

	bool isFull() {
		return left <= 0;
	}

	// �ж��Ƿ���winner����������Ҳ��Ϊ��ʤ
	Player hasWinner();
	void display();
	// ��Ȼ��Ҫ����=/(��o��)/~~
	const Board&operator=(const Board&board) {
		this->board = board.board;
		this->left = board.left;
		return *this;
	}
};

class Human {
public:
	// ��ȡ�����ߵ㣬ʧ�ܼ����ݹ��ȡ
	Point getMove(const Board&board) {
		int x, y;
		cout << "Please enter your place(x y): ";
		cin >> x >> y;
		Point point(x, y, HUMAN);
		if (board.canMove(point)) {
			return point;
		}
		else {
			cout << "Sorry, illegal place." << endl;
			return this->getMove(board);
		}
	}
};

class AIBoard{
private:
	static int limitTimes;
	static int allTimes;
	Point point;	// ���ζ�����Ǹ���
	vector<Point>childrenMove;		// ���ߵ�
	vector<AIBoard>children;		// ָ���ӽڵ�ļ��ϣ�һάvector
	int win;		// �˽ڵ�ʤ������
	int all;		// �˽ڵ����н��д���
	const static int width = 1;

	// ��ȡ�ٽ��ĵ�
	void getNeighbor(vector<Point>&points, const Point&center, set<int>&points_set) {
		Point point;
		int size = board.getSize();
		for (int row_off = -width; row_off <= width; ++row_off) {
			for (int col_off = -width; col_off <= width; ++col_off) {
				point.x = center.x + col_off;
				point.y = center.y + row_off;
				if (board.canMove(point)) {
					if (points_set.find(point.y * size + point.x) == points_set.end()) {
						points.push_back(point);
						points_set.insert(point.y * size + point.x);
					}
				}
			}
		}
	}
public:
	Board board;	// ����
	// ��MCTS��ʼ����
	AIBoard(const Board&board):board(board), win(1), all(1){
		children = vector<AIBoard>();
		//childrenMove = getCanMove();
	}
	// �Լ����ó�ʼ����
	AIBoard(const Board&board, const Point&point, Player player) :board(board), win(1), all(1), point(point) {
		children = vector<AIBoard>();
		this->board.setPlayer(point, player);
		//childrenMove = getCanMove();
	}

	// ��ȡUCBֵ
	double getUCB(int parentAll) {
		return 1.0 * win / all + sqrt(2 * log(parentAll) / all);
	}

	// ��ȡʤ��
	double getRate() {
		return 1.0 * win / all;
	}

	// ��ȡ�����̶���Ǹ���
	Point getPoint() {
		return point;
	}

	// ��ȡ��õĵ㣬���ֻ��ҪMCTS����
	Point getBestMove() {
		int best = 0;
		for (int i = 0; i < children.size(); ++i) {
			if (children[i].getRate() > children[best].getRate()) {
				best = i;
			}
		}
		return children[best].getPoint();
	}

	// ��ȡ��Χ���ӣ��о���Ӧ���Ǹ��Լ��ĺ���
	vector<Point> getCanMove() {
		Point point;
		set<int> points_set;
		vector<Point>points;
		for (int row = 0; row < board.getSize(); ++row) {
			for (int col = 0; col < board.getSize(); ++col) {
				point.x = col;
				point.y = row;
				// �����ﲻ����˵�����ˣ������ȡ��Χ��λ��
				if (!board.canMove(point)) {
					getNeighbor(points, point, points_set);	// ����Ϊ���ã�ֱ���޸�points��board�ǹ��е�
				}
			}
		}
		return points;
	}

	// ģ�⿪ʼ��ֱ����һ��ʤ������������ʤ����player���о��Ǹ��ߵ�pointҲҪ���أ����罨һ���ӽڵ�
	Player silmulation(Player player) {
		++allTimes;
		Board tempBoard(board);		// temp�ȱ������ڵ����̣���ģ���껻��ȥ
		Point point;
		vector<Point> canMove;
		srand(time(nullptr));
		while (board.hasWinner() == NOBODY && !board.isFull()) {
			canMove = getCanMove();
			// �����ѡһ����
			unsigned int _rand = ((unsigned)rand()) % canMove.size();
			//cout << "index: " << _rand << " size: " << canMove.size() << endl;
			point = canMove[_rand];
			board.setPlayer(point, player);
			player = AI + HUMAN - player;
			//board.display();
		}
		player = board.hasWinner();
		board = tempBoard;
		if (player == AI) {
			++win;
		}
		++all;
		return player;
	}

	// ���򴫲��Ĳ���ֱ��д��getMove����ȥ�����ý�����������
	// ���ֻ����ģ�⣬�������ȡ�����������̵Ľ��
	Player getMove(Player player) {
		// ģ��
		while (allTimes <= limitTimes) {
			int best = 0;
			for (int i = 0; i < children.size(); ++i) {
				if (children[i].getUCB(all) > children[best].getUCB(all)) {
					best = i;
				}
			}
			Player p = children[best].silmulation(AI);
			if (p == AI) {
				++win;
			}
			++all;
		}
		return player;
	}

	void setBoard(const Board&board) {
		this->board = board;
		childrenMove.clear();
		childrenMove = getCanMove();	// �˴���������
		// ��childrenҲ������
		children.clear();
		for (int i = 0; i < childrenMove.size(); ++i) {
			children.push_back(AIBoard(board, childrenMove[i], AI));
		}
	}

	// ���һ�������̵����
	void displayChildren() {
		for (int i = 0; i < children.size(); ++i) {
			cout << "Point: " << children[i].point << " rate: " << children[i].getRate() << endl;
		}
	}

	// ����һ�´���
	static void setTimes(int LimitTimes, int AllTimes) {
		limitTimes = LimitTimes;
		allTimes = AllTimes;
	}
};
// �����ʼ��
int AIBoard::allTimes = 0;
int AIBoard::limitTimes = 0;

class MCTS {
private:
	// AI�ڲ��࣬�����Լ�����
	AIBoard myboard;
public:
	// ���ڹ��캯�������봫��һ��Board�����Կ����ڵ����´��ģ����ľ����ڲ�����Ǹ�board
	MCTS(const Board&board) : myboard(board) { }

	Point getMove(const Board&board) {
		// ���������ģ��10^5��
		AIBoard::setTimes(100, 0);
		this->myboard.setBoard(board);
		myboard.getMove(AI);
		myboard.displayChildren();
		return myboard.getBestMove();
	}
};

int main(void) {
	Board board(6);			// ��ʼ������
	Player player = HUMAN;	// ���õ�һ��������
	Human human;			// ��ʼ�����
	MCTS ai(board);			// ��ʼ��AI
	Point move(0, 0);		// ÿ������һ����ʱ����
	board.display();

	while (board.hasWinner() == NOBODY) {
		if (player == HUMAN) {
			move = human.getMove(board);
		}
		else {
			move = ai.getMove(board);
		}
		board.setPlayer(move, player);	// ����λ��
		// ����ߵ�λ��
		cout << ((player == HUMAN) ? "Human: " : "AI:   ") << move << endl;
		board.display();
		// �л����
		player = HUMAN + AI - player;
	}
	cout << endl;
	cout << "The winner is " << ((board.hasWinner() == HUMAN) ? "HUMAN" : "AI")  << endl;

	return 0;
}

// �ж��Ƿ���winner
Player Board::hasWinner() {
	int x = 0, y = 0;
	// ���жϺ���
	int sum = 0;
	for (int row = 0; row < boardSize; ++row) {
		sum = 0;
		y = row;
		for (int i = 0; i < n_in_row; ++i) {
			sum += board[y][i];
		}
		if (check(sum) != NOBODY) return check(sum);
		for (int start = 0; start < boardSize - n_in_row; ++start) {
			sum = sum - board[y][start] + board[y][start + n_in_row];
			if (check(sum) != NOBODY) return check(sum);
		}
	}
	// �ж�����
	for (int col = 0; col < boardSize; ++col) {
		sum = 0;
		x = col;
		for (int i = 0; i < n_in_row; ++i) {
			sum += board[i][x];
		}
		if (check(sum) != NOBODY) return check(sum);
		for (int start = 0; start < boardSize - n_in_row; ++start) {
			sum = sum - board[start][x] + board[start + n_in_row][x];
			if (check(sum) != NOBODY) return check(sum);
		}
	}

	// �����ж�б��
	// �ж����ϵ����·���
	// ���Ⱥſ�����   /(��o��)/~~
	for (int row = 0; row <= boardSize - n_in_row; ++row) {
		sum = 0;
		for (int col = 0; col <= boardSize - n_in_row; ++col) {
			x = col;
			y = row;
			sum = 0;
			for (int i = 0; i < n_in_row; ++i) {
				sum += board[y][x];
				x++;
				y++;
			}
			if (sum / HUMAN == n_in_row) {
				return HUMAN;
			}
			else if (sum / AI == 5) {
				return AI;
			}
		}
	}
	// �ж����µ����Ϸ���
	for (int col = n_in_row - 1; col < boardSize; ++col) {
		for (int row = 0; row <= boardSize - n_in_row; ++row) {
			x = col;
			y = row;
			sum = 0;
			for (int i = 0; i < n_in_row; ++i) {
				sum += board[y][x];
				x--;
				y++;
			}
			if (sum / HUMAN == n_in_row) {
				return HUMAN;
			}
			else if (sum / AI == n_in_row) {
				return AI;
			}
		}
	}
	return NOBODY;
}
// ������̵���Ļ
void Board::display() {
	cout << "  ";
	for (int i = 0; i < boardSize; ++i) {
		cout << i << " ";
	}
	cout << endl;
	for (int row = 0; row < boardSize; ++row) {
		cout << row << " ";
		for (int col = 0; col < boardSize; ++col) {
			if (board[row][col] == HUMAN) {
				cout << "o ";
			}
			else if (board[row][col] == AI) {
				cout << "x ";
			}
			else {
				cout << "_ ";
			}
		}
		cout << endl;
	}
	cout << "Human: o" << endl;
	cout << "AI:    x" << endl;
	cout << endl;
}

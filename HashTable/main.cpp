#include "table.h"   
using namespace std;

int main()
{
	int len = 10;
	HashTable<int> hashTest(len);
	hashTest.insertHash(10);
	hashTest.insertHash(1);
	hashTest.insertHash(9);
	hashTest.insertHash(5);
	hashTest.insertHash(5);
	hashTest.insertHash(2);
	hashTest.insertHash(18);
	hashTest.insertHash(65);
	hashTest.insertHash(21);
	cout << "���е�Ԫ��Ϊ��   ";
	for (int i = 0; i<len; i++)
	{
		cout << hashTest.getData(i) << "  ";
	}
	cout << endl;
	cout << "Ԫ��9��λ�ã�";
	int pos = hashTest.getHash(9);
	if (pos == -1)
	{
		cout << "�޴�Ԫ��" << endl;
	}
	else
	{
		cout << pos << endl;
	}
	system("pause");
	return 0;
}

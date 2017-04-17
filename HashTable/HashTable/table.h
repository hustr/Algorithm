#pragma once
#include "iostream"    
using namespace std;

template<class DataType>
class HashTable
{
public:
	HashTable(int size)
	{
		maxSize = size;
		count = 0;
		element = new DataType[size];
		if (element == NULL)
		{
			exit(1);
		}
		else
		{
			for (int i = 0; i<size; i++)
			{
				element[i] = NULL;
			}
		}
	}
	~HashTable()
	{
		delete[] element;
	}
	DataType getData(int i)
	{
		if (i < 0 || i >= maxSize)
		{
			exit(1);
		}
		else
		{
			return element[i];
		}

	}

	int getNum()
	{
		return count;
	}
	//��ϣ����������ndata��(�������е�)�洢λ��    
	int Hash(DataType nData);
	//�������,����newData����ϣ����    
	int insertHash(DataType newData);
	//��ѰnData�ĵ�ǰ�洢λ��    
	int getHash(DataType nData);
private:
	int maxSize;
	int count;
	DataType *element;
};


template<class DataType>
int HashTable<DataType>::Hash(DataType newData)
{
	return newData%maxSize;//��������    
}

//����nData�Ŀհ״洢λ��    
template<class DataType>
int HashTable<DataType>::insertHash(DataType newData)
{
	if (getNum() == maxSize)
	{
		cerr << "��ϣ���Ѿ���" << endl;
	}
	int pos = Hash(newData);
	if (element[pos] == NULL)
	{
		element[pos] = newData;
		count++;
		return 1;
	}
	else
	{
		int rpos = (pos + 1) % maxSize;
		while (rpos != pos)
		{
			if (element[rpos] == NULL)//Ѱ��û�д洢���ݵ�λ��    
			{
				element[rpos] = newData;
				count++;
				return 1;
			}
			rpos = (rpos + 1) % maxSize;
		}

		if (rpos == pos)//������ն�û���ҵ���λ��ֱ��ǿ�и�ֵ    
		{
			element[rpos] = newData;
			count++;
			return 1;
		}
	}
}

//��ѰnData�ĵ�ǰ�洢λ��    
template<class DataType>
int HashTable<DataType>::getHash(DataType newData)
{
	int pos = Hash(newData);
	if (element[pos] == newData)
	{
		return pos;
	}
	else
	{
		int rpos = (pos + 1) % maxSize;
		while (rpos != pos)
		{
			if (element[rpos] == newData)
			{
				return rpos;
			}
			rpos = (rpos + 1) % maxSize;
		}
		if (rpos == pos)
		{
			cerr << "�޷�����ָ��Ԫ�صĴ洢λ��" << endl;
			return -1;
		}
		else
		{
			return rpos;
		}
	}
}
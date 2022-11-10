#ifndef _MPQ_H
#define _MPQ_H
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

// to track the fullness of rows, to send to MPQ
struct row_info
{
	string name;
	int block;
	int row;
	int full_seat;
	int index;
	

	row_info(): block(0),row(0),full_seat(0),index(0),name(""){};

	row_info(int b,int r, int i)
	{
		block = b;
		row = r;
		full_seat = i;
		index = 0;
	}

	row_info(int b,int r, int i,string n)
	{
		block = b;
		row = r;
		full_seat = i;
		index = 0;
		name = n;
	}

	bool operator<(const row_info& rhs)
	{
		if( this->full_seat < rhs.full_seat )
			return true;

		else if( this->full_seat == rhs.full_seat )
		{
			if( this->block < rhs.block )
				return true;

			return false;
		}

		else return false;
	}

	bool operator>(const row_info& rhs)
	{
		if( this->full_seat > rhs.full_seat )
			return true;

		else if( this->full_seat == rhs.full_seat )
		{
			if( this->block > rhs.block )
				return true;

			return false;
		}

		else return false;
	}

	row_info& operator=(const row_info& rhs)
	{
		this->block = rhs.block;
		this->row = rhs.row;
		this->full_seat = rhs.full_seat;
		this->index = rhs.index;
		return *this;
	}
};

template <class T> class MPQ
{
	public:
		MPQ();
		MPQ(int size);
		MPQ(const MPQ&);
		~MPQ();

		bool isEmpty();
		bool isFull();

		void Insert(T, vector<unordered_map<string,row_info>>&, vector<string>&);
		void getMin(T&min, vector<unordered_map<string,row_info>>&, vector<string>&);

		void Remove(int position, vector<unordered_map<string,row_info>>&, vector<string>&);
		void donothing();

		MPQ<T>& operator=(const MPQ<T> &);
		

	private:
		T *q;
		int capacity;
		int current_size;
		void percolateDown(int hole, vector<unordered_map<string,row_info>>&, vector<string>&);
};

//#include "MPQ.cpp" // I didn't use this method, it didn't link my header to cpp for template class
//I included both cpp and header in the source instead
#endif

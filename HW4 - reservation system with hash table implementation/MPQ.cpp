#include "MPQ.h"

// there are several cout's commented out among the functions, those were for self checks
template <class T> MPQ<T>::MPQ()
{
	// cout << "\ndefault\n" ;

	q = NULL;
	return;
}

template <class T> MPQ<T>::MPQ(int size)
{
	// cout << "\nwith paramete\n" ;

	q = new T[size+1];
	capacity = size;
	current_size = 0;
}

template <class T> MPQ<T>::MPQ(const MPQ<T>& rhs)
{
	//cout << "\ncopy const\n" ;

	capacity = rhs.capacity;
	current_size = rhs.current_size;
	
	q = new T[capacity+1];
	for( int i=1;i<=capacity;i++)
		q[i] = rhs.q[i];
}

template <class T> MPQ<T>::~MPQ()
{
	//T temp = q[0];
	//cout << "\ndest\n" ;
	if( q!=NULL )
		delete[] q;
}

template <class T> bool MPQ<T>::isEmpty()
{
	if( current_size == 0) return true;

	return false;
}

template <class T> bool MPQ<T>::isFull()
{
	if( current_size == capacity) return true;

	return false;
}

template <class T> void MPQ<T>::Insert(T item, vector<unordered_map<string,row_info>> &Row_fullness, vector<string> &Row_vec)
{
	// cout << "0 " << q[0].block << " " << q[0].row << " " << q[0].full_seat << " " << q[0].index << endl;
	// cout << "1 " << q[1].block << " " << q[1].row << " " << q[1].full_seat << " " << q[1].index << endl;
	// cout << "2 " << q[2].block << " " << q[2].row << " " << q[2].full_seat << " " << q[2].index << endl;
	// cout << "3 " << q[3].block << " " << q[3].row << " " << q[3].full_seat << " " << q[3].index << endl;
	
	if( !isFull() )
	{
		int hole = ++current_size;
		for(;hole>1 && item < q[hole/2]; hole/=2)
		{
			q[hole] = q[hole/2];
			Row_fullness[q[hole/2].block][Row_vec[q[hole/2].row]].index = hole/2; // update the new index to data stored in row list

		}

		q[hole] = item;
		Row_fullness[q[hole].block][Row_vec[q[hole].row]].index = hole; // update the new index to data stored in row list
		q[hole].index = hole;
		// cout << "inserted " << q[hole].block << " " << q[hole].row << " " << q[hole].full_seat << " " << q[hole].index << endl;
	}
}

template <class T> void MPQ<T>::getMin(T &min, vector<unordered_map<string,row_info>> &Row_fullness, vector<string> &Row_vec )
{
	if( !isEmpty() )
	{
		min = q[1];
		q[1] = q[current_size--];
		Row_fullness[q[1].block][Row_vec[q[1].row]].index = 1; // update the new index to data stored in row list
		percolateDown(1,Row_fullness,Row_vec);
	}
}

template <class T> void MPQ<T>::percolateDown(int hole, vector<unordered_map<string,row_info>> &Row_fullness, vector<string> &Row_vec)
{
	int child;
	T temp = q[hole];
	
	for(;hole*2<=current_size; hole = child)
	{
		child = hole*2;
		if( child!=current_size && q[child+1]<q[child])
			child++;
		if( q[child]<temp )
		{
			q[hole] = q[child];
			Row_fullness[q[hole].block][Row_vec[q[hole].row]].index = hole; // update the new index to data stored in row list
		}
		else
			break;
	}
	q[hole] = temp;
	Row_fullness[q[hole].block][Row_vec[q[hole].row]].index = hole; // update the new index to data stored in row list
}

template <class T> void MPQ<T>::Remove(int position, vector<unordered_map<string,row_info>> &Row_fullness, vector<string> &Row_vec)
{
	q[position] = q[current_size--];	
	Row_fullness[q[position].block][Row_vec[q[position].row]].index = position; // update the new index to data stored in row list
	percolateDown(position,Row_fullness,Row_vec);
}

template <class T> MPQ<T>& MPQ<T>::operator=(const MPQ<T>& rhs)
{
	//cout << "\nassign\n" ;
	capacity = rhs.capacity;
	current_size = rhs.current_size;
	if( q!=NULL )
		delete[] q;
	
	q = new T[capacity+1];
	for( int i=1;i<=capacity;i++)
		q[i] = rhs.q[i];
	return *this;
}	

template <class T> void MPQ<T>::donothing() // for self check
{
	T c = q[0];
	cout << "0 " << q[0].block << " " << q[0].row << " " << q[0].full_seat << " " << q[0].index << endl;
	T d = q[1];
	cout << "1 " << q[1].block << " " << q[1].row << " " << q[1].full_seat << " " << q[1].index << endl;
	T e = q[2];
	cout << "2 " << q[2].block << " " << q[2].row << " " << q[2].full_seat << " " << q[2].index << endl;
	T f = q[3];
	cout << "3 " << q[3].block << " " << q[3].row << " " << q[3].full_seat << " " << q[3].index << endl;
}


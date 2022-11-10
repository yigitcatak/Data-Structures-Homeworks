#include "MPQ.h"

template <class T> MPQ<T>::MPQ(int size)
{
	q = new T[size];
	capacity = size;
	current_size = 0;
	front = 0;
	back = -1;
}

template <class T> MPQ<T>::~MPQ()
{
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

template <class T> void MPQ<T>::Insert(T item)
{
	if( isEmpty() )
	{
		current_size++;
		back++;
		q[front] = item;
	}
	else if( !isFull() )
	{
		current_size++;
		back++;
		for( int i = 0; i<back; i++ )
		{
			if( item < q[i] ) 
			{
				for( int j = back; j>=i; j--) // shift the rest one right
				{
					q[j] = q[j-1];
				}
				q[i] = item;
				return;
			}
			else if( item == q[i] ) // if the items have same comparison value, keep the firstly inserted one in front
			{
				for( int j = back; j>i; j--) // shift the rest one right
				{
					q[j] = q[j-1];
				}
				q[i+1] = item;
				return;
			}
		}
		q[back] = item; // if not inserted in between, insert to back
	}
}

template <class T> T MPQ<T>::Remove(int ID)
{
	for(int i=0; i<=back; i++)
	{
		if( q[i].ID == ID ) 
		{
			T temp = q[i];
			for(int j=i; j<back; j++) // carry everything it he left of to be deleted object to 1 left
				q[j] = q[j+1];

			//used size and region deducted by 1;
			back--;
			current_size--; 
			return temp;
		}
	}
}

template <class T> T MPQ<T>::GetMax()
{
	return q[back];
}

template <class T> T MPQ<T>::Dequeue()
{
	T temp = q[0];
	for(int i=0; i<back; i++)
		q[i] = q[i+1];

	back--;
	current_size--;
	return temp;
}
#ifndef _MPQ_H
#define _MPQ_H

typedef enum {RightCorner, LeftCorner} corner; 

struct BuildingCorner
{
	// struct for, building corners

	int x; // coordinate
	int ID; // building no
	bool start; // left or right, if LeftCorner is given as input, it forces enum value 1 to bool value 1, so start == true if leftcorner

	BuildingCorner() {};
	BuildingCorner(int x_in, int ID_in, corner pos_in) 
	{
		x = x_in;
		ID = ID_in;
		start = pos_in; // right corner will be false when turned into bool and left will be true
	};


	// operator assignments for template priority queue class, comparisons
	bool operator<(const BuildingCorner& rhs)
	{
		if( this->x < rhs.x )
			return true;

		else return false;
	}

	bool operator>(const BuildingCorner& rhs)
	{
		if( this->x > rhs.x )
			return true;

		else return false;
	}

	bool operator==(const BuildingCorner& rhs)
	{
		if( this->x == rhs.x )
			return true;

		else return false;
	}

	void operator=(const BuildingCorner& rhs)
	{
		this->x = rhs.x;
		this->ID = rhs.ID;
		this->start = rhs.start;
	}
};

struct SightedBuilding
{
	int height;
	int ID; // building no

	SightedBuilding() {};
	SightedBuilding(int height_in, int ID_in)
	{
		height = height_in;
		ID =ID_in ;
	};
	

	// operator assignments for template priority queue class, comparisons
	bool operator<(const SightedBuilding& rhs)
	{
		if( this->height < rhs.height )
			return true;

		else return false;
	}

	bool operator>(const SightedBuilding& rhs)
	{
		if( this->height > rhs.height )
			return true;

		else return false;
	}

	bool operator==(const SightedBuilding& rhs)
	{
		if( this->height == rhs.height )
			return true;

		else return false;
	}

	void operator=(const SightedBuilding& rhs)
	{
		this->height = rhs.height;
		this->ID = rhs.ID;
	}

};

template <class T> class MPQ
{
	public:
		MPQ(int);
		~MPQ();

		bool isEmpty();
		bool isFull();
		void Insert(T);
		T Remove(int);
		T GetMax();

		T Dequeue();
		

	private:
		T *q;
		int front;
		int back;
		int capacity;
		int current_size;

};

//#include "MPQ.cpp" // I didn't use this method, it didn't link my header to cpp for template class
//I included both cpp and header in the source instead
#endif

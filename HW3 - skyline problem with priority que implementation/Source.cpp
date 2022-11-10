#include "MPQ.h"
#include "MPQ.cpp" // for template class linking
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

void Read(const string &text);
void GetOutline(MPQ<BuildingCorner> &BuildingCorners, int *Heights, const int &size);

void Read(const string &text)
{
	ifstream file;
	file.open(text);

	string line,subString;
	getline(file,line); //first line is building count
	int building_count = stoi(line); 

	// Priority Queue to track the corners of buildings (left and right), sorted ascending based on x coordinate
	// so we can easily sweep from left to right by just dequeueing from this Priority Queue
	MPQ<BuildingCorner> BuildingCorners(building_count*2);  

	int *Heights = new int[building_count]; // array to store height of building number i at index i
	// as I implemented template class priority queue with operator implemented structs
	// heights could have added to structs and still be reached easily, but did like this for sake of homework documentation



	int count = 0;
	while( getline(file,line) )
	{
		istringstream stream(line);
		int x1,x2,y, ID;

		ID = count;
		stream>>subString;
		x1 = stoi(subString);

		stream>>subString;
		y = stoi(subString);

		stream>>subString;
		x2 = stoi(subString);

		BuildingCorner a(x1,ID,LeftCorner), b(x2,ID,RightCorner); // create corners structs
		
		BuildingCorners.Insert(a); // Insert corners
		BuildingCorners.Insert(b);
		Heights[count] = y; // Insert the height to heights register
		
		count++;
	}

	GetOutline(BuildingCorners, Heights, building_count);
}

void GetOutline(MPQ<BuildingCorner> &BuildingCorners, int *Heights, const int &size)
{
	MPQ<SightedBuilding> Buildings_In_Sight(size); // Priority Queue to track heights of buildings in sight
	bool first=true;
	int PreviousHeight = 0; // to not print if height didn't change

	while( !BuildingCorners.isEmpty() )
	{

		// GET ONE FROM THE QUEUE AND DO PROPER OPERATION
		BuildingCorner Corner = BuildingCorners.Dequeue();
		int ID = Corner.ID;

		if( Corner.start ) // if it is left corner, start is set to true, add to sight
		{
			SightedBuilding Building( Heights[ID], ID );
			Buildings_In_Sight.Insert(Building);
		}
		else // the right corner of a building is reached, remove from sight
			Buildings_In_Sight.Remove(ID);


		// CHECK IF THERE ARE MORE BUILDING CORNERS AT SAME X COORDINATE
		bool Check = true;
		while( Check && !BuildingCorners.isEmpty() )
		{
			BuildingCorner Temp = BuildingCorners.Dequeue();
			
			if( Temp.x == Corner.x ) // if same coordinate
			{

				int TempID = Temp.ID;
				if( Temp.start )
				{
					SightedBuilding TempBuilding( Heights[TempID], TempID );
					Buildings_In_Sight.Insert(TempBuilding);
				}
				else
					Buildings_In_Sight.Remove(TempID);

			}

			else // if not same coordinate insert it back and get out
			{
				BuildingCorners.Insert(Temp);
				Check = false;
			}			
		}
		

		// PRINTS

		// if nothing is printed yet 
		if ( first ) 
		{
			if( Corner.x != 0) // if buildings does not start from x=0
				cout << "0 0\n";
				

			// if there is a building in sight
			else if( !Buildings_In_Sight.isEmpty() ) 
			{
				int h = Buildings_In_Sight.GetMax().height; // get the max height in view
				if( h != PreviousHeight ) // if height changed
				{
					cout << Corner.x << " " << h << endl; 
					PreviousHeight = h;
				}
			}

			first = false;
		}

		// if there is a building in sight
		if( !Buildings_In_Sight.isEmpty() )
		{
			int h = Buildings_In_Sight.GetMax().height; // get the max height in view
			if( h != PreviousHeight ) // if height changed
			{
				cout << Corner.x << " " << h << endl; 
				PreviousHeight = h;
			}
		}

		// if no building in sight
		else if( PreviousHeight != 0 ) 
		{
			cout << Corner.x <<" 0\n";
			PreviousHeight = 0;
		}
	}	

	delete[] Heights;
}

int main()
{
	Read("input.txt"); // added text input here so you can change easily, it calls GetOutline()

	return 0;
}

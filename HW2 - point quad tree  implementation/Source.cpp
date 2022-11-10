#include "quadtree.h"
#include <fstream>
#include <sstream>
#include <algorithm>


void Read_Cities(QuadTree *MyTree, const string &text)
{
	ifstream file;
	file.open(text);

	enum reader{Name, X, Y, Xmax, Ymax};
	string line, subString;
	bool init = false;
	reader position;


	while( getline(file, line) )
	{
		unsigned int x, y;
		string name;
		istringstream stream(line);

		if( init ) //if tree is initialised
			position = Name;
		else
			position = Xmax;

		QuadTreeNode *temp = new QuadTreeNode;

		while(stream>>subString)
		{
			switch(position)
			{
				case Name:
					temp->name = subString;
					position = X;
					break;

				case X:
					x = stoi(subString);
					temp->x = x;
					position = Y;
					break;

				case Y:
					y = stoi(subString);
					temp->y = y;
					MyTree->Insert(temp);
					break;

				case Xmax:
					x = stoi(subString);
					MyTree->SetMax(x,"x");
					position = Ymax;
					break;

				case Ymax:
					y = stoi(subString);
					MyTree->SetMax(y,"y");
					init = true;
					break;
			}
		}
	}
	MyTree->Print();
}

void Read_Query(QuadTree *MyTree, const string &text)
{
	cout  << endl << endl;
	ifstream file;
	file.open(text);

	string line, subString;

	while( getline(file, line) )
	{
		line.erase(remove(line.begin(),line.end(),','),line.end());
		istringstream stream(line);

		unsigned int x, y, r;

		enum reader{X, Y, R};
		reader position = X;

		while(stream>>subString)
		{
			switch(position)
			{
				case X:
					x = stoi(subString);
					position = Y;
					break;
				case Y:
					y = stoi(subString);
					position = R;
					break;
				case R:
					r = stoi(subString);
					MyTree->Search(x,y,r);
					break;
			}
		}
	}	
}

int main()
{
	QuadTree MyTree;
	Read_Cities(&MyTree,"cities.txt"); // added file names here to change them without going to function
	Read_Query(&MyTree,"queries2.txt");

	return 0;
}

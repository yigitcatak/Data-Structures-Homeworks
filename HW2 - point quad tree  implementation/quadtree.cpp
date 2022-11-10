#include "quadtree.h"

QuadTree::~QuadTree()
{
	if(tree_root == NULL)
		return;

	Destruct(tree_root);
}

void QuadTree::Destruct(QuadTreeNode *root)
{
	if( root != NULL )
	{
		Destruct(root->SE);
		Destruct(root->SW);
		Destruct(root->NE);
		Destruct(root->NW);
		delete root;
	}
}

void QuadTree::SetMax(unsigned int a, string axis)
{
	if(axis == "x")
		max_x = a;
	else
		max_y = a;
}

void  QuadTree::Insert(QuadTreeNode *node) // public
{
	if(node->x > max_x || node->y > max_y)
		return;

	if( tree_root == NULL )
		tree_root = node;

	else
		Insert(node, tree_root); // private
}

void QuadTree::Insert(QuadTreeNode *node, QuadTreeNode *root) // private
{

	if(node->x < root->x) //insert to west
	{

		if(node->y < root->y) // insert to south
		{
			if(root->SW == NULL)
			{
				root->SW = node;
				return;
			}
			Insert(node, root->SW);
		}
		else//insert to north
		{
			if(root->NW == NULL)
			{
				root->NW = node;
				return;
			}
			Insert(node, root->NW);
		}
	}

	else //insert to east
	{
		if(node->y < root->y) // insert to south
		{
			if(root->SE == NULL)
			{
				root->SE = node;
				return;
			}
			Insert(node, root->SE);
		}

		else //insert to north
		{
			if(root->NE == NULL)
			{
				root->NE = node;
				return;
			}
			Insert(node, root->NE);
		}		
	}	
}

void QuadTree::Print() const // public
{
	if(tree_root == NULL)
		return;

	Print(tree_root);
}

void QuadTree::Print(QuadTreeNode *root) const // private
{
	if(root != NULL)
	{
		cout << root->name << endl;
		Print(root->SE);
		Print(root->SW);
		Print(root->NE);
		Print(root->NW);
	}
}

void QuadTree::Search(unsigned int x,unsigned int y,unsigned int r) const// public
{
	/*
		for better performance during search (to not search entire tree blindly)
		first determine the maximum and minimum reaches of the search circle, 
		
		for example:
		if query is given as 50 30 10; the maximum reaches are x: 40 60 and y: 20 40
		therefore if the root is below 20 we don't need to look to south of root 
		as the root's south is alread outside of the search circle's reach

	*/

	if(tree_root == NULL)
		return;

	vector<string> track;
	vector<string> track2;
	int E_limit = x + r, W_limit = x - r, N_limit = y + r, S_limit = y-r;
	Search(S_limit, W_limit, N_limit, E_limit, x, y, r, tree_root, &track, &track2);
	
	// Print
	if (track2.empty())
		cout << "<None>\n";
	else
	{
		for(int i=0;i<track2.size()-1;i++)
				cout << track2.at(i) << ", ";
		cout << track2.at(track2.size()-1) << endl;
	}

	for(int i=0;i<track.size()-1;i++)
			cout << track.at(i) << ", ";
	cout << track.at(track.size()-1) << endl << endl;

}

void QuadTree::Search(int S_limit, int W_limit, int N_limit,int E_limit, unsigned int x, unsigned int y, unsigned int r,\
					  QuadTreeNode *root,vector<string> *track,vector<string> *track2) const// private
{
	if(root == NULL)
		return;

	// this is for "unsigned int > negative int" giving false result due to type mismatch (negative int is wrapped around to "Unsigned max range - |negative int|")
	int root_x = root->x, root_y = root->y;

	int difx = (x - root_x)*(x - root_x);
	int dify = (y - root_y)*(y - root_y);
	int rr = r*r;
	bool inrange=false, SE=true , SW=true, NE=true, NW=true;

	//if( root->name == "Buffalo" )
	//	int a = 0;

	if( difx + dify <= rr) //if node is in the search range
	{
		track2->push_back(root->name); 
		track->push_back(root->name); 
		inrange = true;
	}
	else
		track->push_back(root->name); 

	// this is to check if city is to *direction* of the center and not in range, its *direction* shouldn't be explored
	if( !inrange ) 
	{
		if( root_y > y) // city is to north of the center
		{
			if( root_x > x ) // city is to east of the center
				NE = false;
			else if( root_x < x ) // city is to west of the center
				NW = false;
		}
		else if( root_y < y ) // city is to south of the center
		{
			if( root_x > x ) // city is to east of the center
				SE = false;
			else if( root_x < x ) // city is to west of the center
				SW = false;
		}
	}

	if( root_y > S_limit ) // can search to south
	{
		if( root_x < E_limit && SE) // can search to east
			Search(S_limit, W_limit, N_limit, E_limit, x, y, r, root->SE,track,track2);
		
		if( root_x > W_limit && SW ) // can search to west 
			Search(S_limit, W_limit, N_limit, E_limit, x, y, r, root->SW,track,track2);
	}

	if( root_y < N_limit ) // can search to north
	{
		if( root_x < E_limit && NE) // can search to east
			Search(S_limit, W_limit, N_limit, E_limit, x, y, r, root->NE,track,track2);

		if( root_x > W_limit && NW ) // can search to west
			Search(S_limit, W_limit, N_limit, E_limit, x, y, r, root->NW,track,track2);
	}
}

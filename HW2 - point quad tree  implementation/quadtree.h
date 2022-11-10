#ifndef _QUADTREE_H
#define _QUADTREE_H
#include <string>
#include <iostream>
#include <vector>
using namespace std;


struct QuadTreeNode
{
	QuadTreeNode(): SE(NULL), SW(NULL), NE(NULL), NW(NULL){};
	string name;
	unsigned int x;
	unsigned int y;

	QuadTreeNode *SE;
	QuadTreeNode *SW;
	QuadTreeNode *NE;
	QuadTreeNode *NW;
};


class QuadTree
{
	public:
		QuadTree(): tree_root(NULL){};
		~QuadTree(); // has private counterpart as well

		void SetMax(unsigned int, string);

		// functions with private counterparts to not expose tree_root or other pointers to user
		void Insert(QuadTreeNode*);
		void Print() const;
		void Search(unsigned int,unsigned int,unsigned int) const;

	private:
		QuadTreeNode *tree_root;
		unsigned int max_x;
		unsigned int max_y;

		// private counterparts of the public member functions
		void Destruct(QuadTreeNode*);
		void Insert(QuadTreeNode*, QuadTreeNode*); 
		void Print(QuadTreeNode*) const;
		void Search(int,int,int,int,unsigned int,unsigned int,unsigned int,QuadTreeNode*,vector<string>*,vector<string>*) const;
};
#endif
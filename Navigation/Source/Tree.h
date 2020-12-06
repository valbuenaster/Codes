/*
 * Tree.h
 *
 *  Created on: Oct 9, 2020
 *      Author: luis
 */

#ifndef SOURCE_TREE_H_
#define SOURCE_TREE_H_

#include <vector>
#include <list>
#include <math.h>
#include "Map.h"

//const long long int DeltaX[] = {0, 1, 1, 1, 0, -1, -1, -1};
//const long long int DeltaY[] = {1, 1, 0, -1, -1, -1, 0, 1};

//const long long int DeltaX[] = {1, 1, -1, -1, 0, 1, 0, -1};
//const long long int DeltaY[] = {1, -1, -1, 1, 1, 0, -1, 0};

const long long int DeltaX[] = {-1, 1, 0, 0, 1, -1, 1, -1};
const long long int DeltaY[] = {0, 0, 1, -1, 1, -1, -1, 1,};

const long long int DeltaXBA[] = {0, 1, 1, 1, 0, -1, -1, -1};
const long long int DeltaYBA[] = {1, 1, 0, -1, -1, -1, 0, 1};

struct Nd{
	long long int px=0;
	long long int py=0;
	long long int Level = 0;

	bool visited = false;
	bool flagA = false;
	bool flagB = false;
	bool flagC = false;

	double cummulativeDistance = 0.0;

	struct Nd * Parent = nullptr;
	std::vector<struct Nd *>Connections;
}typedef Node;

class Tree
{
	long long int initPosx=0;
	long long int initPosy=0;

	std::list<Node*> setofRoads;

	const int Nconst = 8;
public:
	Node * Root;
	Tree(long long int Ipx, long long int Ipy);
	Node * createNode(Node * Pointer, long long int Ipx, long long int Ipy);
	Node * exploreMapbyDepth(Map * M, Node *Pointer);//Return the node that has the destination;
	Node * exploreMapbyBreadth(Map * M, Node *Pointer);
	void printTree();

	~Tree();
};

#endif /* SOURCE_TREE_H_ */

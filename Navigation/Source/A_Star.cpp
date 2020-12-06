/*
 * A_Star.cpp
 *
 *  Created on: Nov 4, 2020
 *      Author: luis
 *
 */

#include "A_Star.h"
#include "Map.h"



A_Star::A_Star(long long int Sx,long long int Sy,long long int Ex,long long int Ey)
{
	this->StartNode = new NodeS;
	this->EndNode = new NodeS;

	this->StartNode->px = Sx;
	this->StartNode->py = Sy;
	this->StartNode->Level = 0;
	this->StartNode->cummulativeDistance = 0.0;

	this->EndNode->px = Ex;
	this->EndNode->py = Ey;

	this->StartNode->stateNeighbors = vector<char> (N_CONN_S,' ');
	this->StartNode->Connections = vector<NodeS *>(N_CONN_S,nullptr);
	this->EndNode->stateNeighbors = vector<char> (N_CONN_S,' ');
	this->EndNode->Connections = vector<NodeS *>(N_CONN_S,nullptr);


	this->mapWHAT2MOVE= { {0, {1, 2, 8, 6, 7}},
						  {1, {2, 8, 0}},
						  {2, {3, 4, 8, 0, 1}},
						  {3, {4, 8, 2}},
						  {4, {5, 6, 8, 2, 3}},
						  {5, {6, 8, 4}},
						  {6, {7, 0, 8, 4, 5}},
						  {7, {0, 8, 6}}};

	this->mapWHERE2MOVE = { {0, {2, 3, 4, 5, 6}},
							{1, {4, 5, 6}},
							{2, {4, 5, 6, 7, 0}},
							{3, {6, 7, 0}},
							{4, {6, 7, 0, 1, 2}},
							{5, {0, 1, 2}},
							{6, {0, 1, 2, 3, 4,}},
							{7, {2, 3, 4}}};

	this->mapConnInsOrder = {{0,{1, 7, 2, 6, 3, 5, 4}},
			                 {1,{2, 0, 3, 7, 4, 6, 5}},
							 {2,{3, 1, 4, 0, 5, 7, 6}},
							 {3,{4, 2, 5, 1, 6, 0, 7}},
							 {4,{5, 3, 6, 2, 7, 1, 0}},
							 {5,{6, 4, 7, 3, 0, 2, 1}},
							 {6,{7, 5, 0, 4, 1, 3, 2}},
							 {7,{0, 6, 1, 5, 2, 4, 3}}};
}

int A_Star::exploreNeighbors(Map* M, NodeS* ptr, long long int Vx, long long int Vy)
{
	int retVal = -1;
	double maxVal = 0.0;
	double tempDotProduct = 0.0;
	char mapState = ' ';
	bool visited = false;
	//cout<<"Exploring Neighbor Nodes for ("<< ptr->px <<","<< ptr->py <<")..."<<endl;
	for(int ii=0;ii<N_CONN_S;ii++)
	{
		tempDotProduct = ((DeltaXS[ii]*Vx) + (DeltaYS[ii]*Vy))/(DifferentialCost[ii]*sqrt( pow(Vx,2) + pow(Vy,2) ));
		mapState = M->returnStateMap( ptr->px+DeltaXS[ii], ptr->py+DeltaYS[ii]);
		visited = M->returnVisitStatus(ptr->px+DeltaXS[ii], ptr->py+DeltaYS[ii]);

		ptr->stateNeighbors[ii] = mapState;

		if((( mapState==EMPTY_SPACE || mapState==DESTINATION))&&(ptr->Connections[ii]==nullptr)&& !visited)
		{
			ptr->Connections[ii]= this->createNode(ptr,ii);
			//cout<<"\t Node ("<< ptr->Connections[ii]->px <<","<< ptr->Connections[ii]->py <<") created."<<endl;
		}
		//if( ( maxVal < tempDotProduct )&&( mapState==EMPTY_SPACE || mapState==DESTINATION) && !visited)//RELAX THIS CONDITION FOR LATER DECISION
		if( ( maxVal < tempDotProduct ))
		{
			retVal = ii;
			maxVal = tempDotProduct;
		}
	}

	for(int ii=0;ii<N_CONN_S;ii++)
	{
		createConnections(ptr, ii);
	}

	/*
	cout<<"All Neighbors are"<<endl;
	for(auto elem: ptr->Connections)
	{
		if(elem!=nullptr) cout<<"\t ("<< elem->px <<","<< elem->py <<")"<<endl;
	}
	*/

	return retVal;
}

void A_Star::createConnections(NodeS * ptrParent, int index)
{
	vector<long long int> whatTomove = this->mapWHAT2MOVE[index];
	vector<long long int> whereTomove = this->mapWHERE2MOVE[index];
	NodeS * ptr = ptrParent->Connections[index];

	int Size = whatTomove.size();

	if(ptr!=nullptr)
	{
		for(int ii=0;ii<Size;ii++)
		{
			if(whatTomove[ii]==N_CONN_S)
			{
				ptr->Connections[whereTomove[ii]]=ptrParent;
			}else{
				if((ptr->Connections[whereTomove[ii]]==nullptr)&&(ptrParent->Connections[whatTomove[ii]]!=nullptr))
				{
					ptr->Connections[whereTomove[ii]]=ptrParent->Connections[whatTomove[ii]];
				}
			}
		}
	}
}

NodeS * A_Star::createNode(NodeS * ptrParent,int index)
{
	NodeS* ptr = new NodeS;
	ptr->px = ptrParent->px + DeltaXS[index];
	ptr->py = ptrParent->py + DeltaYS[index];
	ptr->Parent = ptrParent;
	ptr->cummulativeDistance = ptrParent->cummulativeDistance + DifferentialCost[index];
	ptr->stateNeighbors = vector<char> (N_CONN_S,' ');
	ptr->Connections = vector<NodeS *>(N_CONN_S,nullptr);

	return ptr;
}

bool A_Star::evolve(Map* M)
{
	bool retVal = false;
	list<NodeS*> bag;
	long long int Vx;
	long long int Vy;
	int ii_grow;
	NodeS * Pointer = nullptr;
	bag.push_front(this->StartNode);
	vector<long long int> vectElem;

	while(bag.size()>0)
	{
		Pointer = bag.front();
		bag.pop_front();
		Pointer->flagA = true;
		Vx = this->EndNode->px - Pointer->px;
		Vy = this->EndNode->py - Pointer->py;

		//cout<<"Pointer ("<< Pointer->px<<","<< Pointer->py <<")"<<endl;

		if((Vx == 0)&&(Vy == 0))
		{	retVal = true;
			this->EndNode->Parent = Pointer->Parent;
			this->EndNode->cummulativeDistance = Pointer->cummulativeDistance;
			break;//arrived to destination
		}

		//create the available neighbours to Pointer and select the one in the direction

		ii_grow = this->exploreNeighbors(M, Pointer,Vx,Vy);
		M->setAsVisited(Pointer->px,Pointer->py);
		//cout<<"\t ii_grow = "<< ii_grow <<endl;

		if(ii_grow >= 0 )
		{
			//
			//if(Pointer->stateNeighbors[ii_grow]==EMPTY_SPACE || Pointer->stateNeighbors[ii_grow]== DESTINATION)
			if(Pointer->Connections[ii_grow]!=nullptr)
			{
				if((Pointer->stateNeighbors[ii_grow]==EMPTY_SPACE || Pointer->stateNeighbors[ii_grow]== DESTINATION)&&(Pointer->Connections[ii_grow]->flagA ==false))
				{
					if(Pointer->Connections[ii_grow]!=nullptr)
					{
						//cout<<"\t Putting ("<< Pointer->Connections[ii_grow]->px <<","<< Pointer->Connections[ii_grow]->py <<") into the bag..."<<endl;
						bag.push_front(Pointer->Connections[ii_grow]);
					}
				}
			}
			vectElem = this->mapConnInsOrder[ii_grow];

			//if(Pointer==nullptr) cout<<"Pointer = nullptr"<<endl;

			for(int kk=0;kk<N_CONN_S-1;kk++)
			{
				long long int el_ii = vectElem[kk];
				if(Pointer->Connections[el_ii]!=nullptr)
				{
					if((Pointer->Connections[el_ii]->flagB==false) && (Pointer->Connections[el_ii]->flagA==false))
					{
						/*
						cout<<"\t Putting ("<< Pointer->Connections[el_ii]->px <<","<< Pointer->Connections[el_ii]->py
											<<") into the bag LO, flagA = "<<Pointer->Connections[el_ii]->flagA
											<<", flagB = "<<Pointer->Connections[el_ii]->flagB << ", memory = "<< Pointer->Connections[el_ii] <<endl;
						*/
						Pointer->Connections[el_ii]->flagB=true;
						bag.push_back(Pointer->Connections[el_ii]);
					}
				}
			}

		}
		//cout<<"Bag size = "<<bag.size()<<endl;
	}

	//cout<<"Saliendo de la evolucion"<<endl;

	return retVal;
}

NodeS * A_Star::returnEndNode()
{
	return this->EndNode;
}

A_Star::~A_Star()
{
	delete this->StartNode;
	delete this->EndNode;
}

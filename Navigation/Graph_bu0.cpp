/*
 * Graph.cpp
 *
 *  Created on: Oct 10, 2020
 *      Author: luis
 */
#include "Graph.h"

Graph::Graph(long long int Ipx, long long int Ipy)
{
	this->StartPoint = new NodeG;

	this->initPosx = Ipx;
	this->initPosy = Ipy;

	this->StartPoint->px = 0;
	this->StartPoint->py = 0;

	this->StartPoint->vx = 0.0;
	this->StartPoint->vy = 0.0;

	this->StartPoint->NodeType = 8;

	this->StartPoint->Connections = std::vector<NodeG *>(N_CONN,nullptr);

	this->mapNodesGraph = { {0, {1, 2, 8, 6, 7}},
							{1, {2, 8, 0}},
							{2, {3, 4, 8, 0, 1}},
							{3, {4, 8, 2}},
							{4, {5, 6, 8, 2, 3}},
							{5, {6, 8, 4}},
							{6, {7, 0, 8, 4, 5}},
							{7, {0, 8, 6}}};

	this->mapPositionsGraph = { {0, {2, 3, 4, 5, 6}},
								{1, {4, 5, 6}},
								{2, {4, 5, 6, 7, 0}},
								{3, {6, 7, 0}},
								{4, {6, 7, 0, 1, 2}},
								{5, {0, 1, 2}},
								{6, {0, 1, 2, 3, 4,}},
								{7, {2, 3, 4}}};

	this->mapWhattoDiscoverGraph = { {0, {0, 1}},
									 {1, {1, 2}},
									 {2, {2, 3}},
									 {3, {3, 4}},
									 {4, {4, 5}},
									 {5, {5, 6}},
									 {6, {6, 7}},
									 {7, {7, 0}},
									 {8, {0, 1, 2, 3, 4, 5, 6, 7}}};

									//  L  R
	this->map_NodeCanProvideTo = { {0, {1, 7}},
			 	 	 	 	 	   {1, {2, 3}},
								   {2, {3}},
								   {3, {4, 5}},
								   {4, {5}},
								   {5, {6, 7}},
								   {6, {7}},
								   {7, {}}};


	this->mapWhattoShareGraphLeft = { {0, {0, 1}},
			 	 	 	 	 	  	  {1, {2, 3}},
									  {2, {3}},
									  {3, {4, 5}},
									  {4, {5}},
									  {5, {6, 7}},
									  {6, {7}},
									  {7, {}}};

	//The empty bracket means that it does not require information
	this->mapWhattoShareGraphRight = { {0, {7, 0}},
			 	 	 	 	 	  	   {1, {3}},
									   {2, {}},
									   {3, {5}},
									   {4, {}},
									   {5, {7}},
									   {6, {7}},
									   {7, {}}};

	this->mapArrivaltoShareGraphLeft = { {0, {7, 0}},
			 	 	 	 	 	  	     {1, {1, 2}},
										 {2, {2}},
										 {3, {3, 4}},
										 {4, {4}},
										 {5, {5, 6}},
										 {6, {6}},
										 {7, {}}};

	//The empty bracket means that it does not require information
	this->mapArrivaltoShareGraphRight = { {0, {0, 1}},
			 	 	 	 	 	  	   	  {1, {1}},
										  {2, {}},
										  {3, {3}},
										  {4, {}},
										  {5, {5}},
										  {6, {}},
										  {7, {}}};
}

NodeG * Graph::returnStartPoint()
{
	return this->StartPoint;
}

NodeG * Graph::createNode(NodeG * Pointer, long long int Ipx, long long int Ipy, int type)
{
	NodeG * Temp = new NodeG;

	Temp->px = Ipx;
	Temp->py = Ipy;

	Temp->vx = Pointer->vx + Ipx;
	Temp->vy = Pointer->vy + Ipy;

	Temp->NodeType = type;

	Temp->Connections = std::vector<NodeG *>(N_CONN,nullptr);

	return Temp;
}

NodeG * Graph::growGraphStepOne(Map * M, NodeG *ptr)
{
	std::list<NodeG *> bag;
	unsigned long long int upperBound = M->returnBoundPlaces();
	NodeG * Pointer;
	NodeG * retVal = nullptr;

	M->setAsVisited(this->initPosx + ptr->px,this->initPosy + ptr->py);
	bag.push_back(ptr);

	long long int Cx = 0;
	long long int Cy = 0;

	while(bag.size()>0)
	{
		Pointer = bag.front();
		Pointer->flagA = true;

		Cx = this->initPosx + Pointer->px;
		Cy = this->initPosy + Pointer->py;

		std::vector<long long int> NodesExplore = {0, 1, 2, 3, 4, 5, 6, 7};

		std::cout<<std::endl<<"Pointer ("<< Cx <<","<< Cy<<")..."<<std::endl;

		for(auto nn:NodesExplore)
		{
			char retValMap = M->returnStateMap(Cx + DeltaXG[nn], Cy+ DeltaYG[nn]);
			bool ocupied = M->returnVisitStatus(Cx + DeltaXG[nn], Cy+ DeltaYG[nn]);

			std::cout<<"Studying ("<< Cx + DeltaXG[nn] <<","<< Cy+ DeltaYG[nn] <<")"<<std::endl;

			if(( retValMap == EMPTY_SPACE || retValMap == DESTINATION )&& !ocupied)
			{
				NodeG * temp = this->createNode(Pointer,Pointer->px + DeltaXG[nn], Pointer->py + DeltaYG[nn],nn);
				M->setAsVisited(Cx + DeltaXG[nn], Cy+ DeltaYG[nn]);
				Pointer->Connections[nn] = temp;
				bag.push_back(temp);
			}else{
				std::cout<<"Node ("<< Cx + DeltaXG[nn] <<","<< Cy + DeltaYG[nn]
						 <<"), not created because retValMap = *"<<retValMap<<"*. ocupied = "<< ocupied <<std::endl<<std::endl;
			}
		}

		if(M->returnStateMap(Pointer->px + this->initPosx,Pointer->py + this->initPosy) == DESTINATION)
		{
			retVal = Pointer;
			//break;
		}

		if(bag.size() >= upperBound)
		{
			std::cout<<std::endl<<"bag is full, this is what is inside of bag"<<std::endl;
			for(auto Elem:bag) std::cout<<"("<<this->initPosx + Elem->px <<","<<this->initPosy + Elem->py<<") flagA="<<Elem->flagA<<std::endl;

			break;
		}

		bag.pop_front();
	}
	return retVal;
}

char Graph::computeConnections(Map * M, NodeG* Tile)
{
	int vcounter = 0;
	int wcounter = 0;
	char retVal = ' ';
	long long int cx = this->initPosx + Tile->px;
	long long int cy = this->initPosy + Tile->py;
	char option = ' ';

	for(int nn = 0;nn<N_CONN;nn++)
	{
		if(Tile->Connections[nn]!=nullptr)
		{
			vcounter++;
		}else{
			if(M->returnStateMap(cx + DeltaXG[nn],cy + DeltaYG[nn])== WALL) wcounter++;
		}
	}

	if(wcounter + vcounter == N_CONN)
	{
		Tile->NodeType != N_CONN? M->writeOnNConn(cx, cy,'C'): M->writeOnNConn(cx, cy,'@');
		retVal = 'C';
	}else{
		switch(vcounter)
		{
			case 0:
						option = '0';
						break;
			case 1:
						option = '1';
						break;
			case 2:
						option = '2';
						break;
			case 3:
						option = '3';
						break;
			case 4:
						option = '4';
						break;
			case 5:
						option = '5';
						break;
			case 6:
						option = '6';
						break;
			case 7:
						option = '7';
						break;
			default:
						option = 'i';
						break;
		}
		M->writeOnNConn(cx, cy,option);
		retVal = option;
	}
	return retVal;
}

void Graph::growGraphStepTwo(Map * M)
{
	NodeG * Pointer;
	std::list<NodeG *> bag;

	bag.push_back(this->StartPoint);

	while(bag.size()>0)
	{
		Pointer = bag.front();
		if(Pointer==nullptr) std::cout<<"Pointer = nullptr"<<std::endl;
		Pointer->flagB = true;

		std::cout<<"("<< this->initPosx + Pointer->px <<","<< this->initPosy + Pointer->py <<")"<<std::endl;

		char eval = this->computeConnections(M, Pointer );

		if((eval=='C'))
		{
			//Internal connections
			for(int nn = 0;nn<N_CONN;nn++)
			{
				NodeG* ptr = Pointer->Connections[nn];

				if(ptr==nullptr) continue;

				std::vector<long long int> NodesI = this->mapNodesGraph[nn];
				std::vector<long long int> NodesP = this->mapPositionsGraph[nn];

				int Size = NodesI.size();
				for(int mm = 0;mm<Size;mm++)
				{
					//ptr->Connections[NodesI[mm]] = Pointer->Connections[NodesP[mm]];//NO!!!
					if(NodesI[mm]==N_CONN)
					{
						ptr->Connections[NodesP[mm]] = Pointer;
					}else{
						ptr->Connections[NodesP[mm]] = Pointer->Connections[NodesI[mm]];
					}
				}
			}

			//connections among the new elements
			for(int nn = 0;nn<N_CONN;nn++)
			{
				std::vector<long long int> v_NodeCanProvideTo = this->map_NodeCanProvideTo[nn];
				std::vector<long long int> v_WhattoShareGraphLeft = this->mapWhattoShareGraphLeft[nn];
				std::vector<long long int> v_WhattoShareGraphRight = this->mapWhattoShareGraphRight[nn];
				std::vector<long long int> v_ArrivaltoShareGraphLeft = this->mapArrivaltoShareGraphLeft[nn];
				std::vector<long long int> v_ArrivaltoShareGraphRight = this->mapArrivaltoShareGraphRight[nn];

				NodeG* ptr = Pointer->Connections[nn];
				if(ptr!=nullptr)
				{
					NodeG* ptr_aux = nullptr;
					switch(v_NodeCanProvideTo.size())
					{
						case 2:		//both left and right
									ptr_aux = Pointer->Connections[v_NodeCanProvideTo[0]];
									for(unsigned int ii=0;ii<v_WhattoShareGraphLeft.size();ii++)
									{
										if(ptr_aux!=nullptr) ptr_aux->Connections[v_ArrivaltoShareGraphLeft[ii]] = ptr->Connections[v_WhattoShareGraphLeft[ii]];
									}

									ptr_aux = Pointer->Connections[v_NodeCanProvideTo[1]];
									for(unsigned int ii=0;ii<v_WhattoShareGraphRight.size();ii++)
									{
										if(ptr_aux!=nullptr) ptr_aux->Connections[v_ArrivaltoShareGraphRight[ii]] = ptr->Connections[v_WhattoShareGraphRight[ii]];
									}
									break;
						case 1:		//only left
									ptr_aux = Pointer->Connections[v_NodeCanProvideTo[0]];
									for(unsigned int ii=0;ii<v_WhattoShareGraphLeft.size();ii++)
									{
										if(ptr_aux!=nullptr) ptr_aux->Connections[v_ArrivaltoShareGraphLeft[ii]] = ptr->Connections[v_WhattoShareGraphLeft[ii]];
									}
									break;
						default:
									break;
					}
				}

			}

		}
		eval = this->computeConnections(M, Pointer );

		/*
		map_NodeCanProvideTo
		mapWhattoShareGraphLeft
		mapWhattoShareGraphRight
		mapArrivaltoShareGraphLeft
		mapArrivaltoShareGraphRight
		*/



		int pos_count = 0;
		for(auto &Elem: Pointer->Connections)
		{
			/*
			if(Elem==nullptr)
			{
				std::cout<<"\t pos: "<< pos_count <<", (null)"<<std::endl;
			}else{
				std::cout<<"\t pos: "<< pos_count <<", ("<< this->initPosx + Elem->px <<","<< this->initPosy + Elem->py << ")"<<std::endl;
			}
			*/

			if((Elem!=nullptr)&&(!Elem->flagB))
			{
				Elem->flagB = true;
				bag.push_back(Elem);
			}

			pos_count++;
		}
		bag.pop_front();
	}
}

Graph::~Graph()
{
	delete this->StartPoint;
}


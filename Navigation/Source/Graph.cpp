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
	this->StartPoint->cummulativeDistance = 0.0;
	this->StartPoint->Level = 0;

	this->StartPoint->Connections = std::vector<NodeG *>(N_CONN,nullptr);

	this->DestinationNode = nullptr;
	this->HeadList = nullptr;

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

	Temp->vx = Pointer->vx + (Ipx - Pointer->px);
	Temp->vy = Pointer->vy+ (Ipy - Pointer->py);

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

		//std::cout<<std::endl<<"Pointer ("<< Cx <<","<< Cy<<")..."<<std::endl;

		for(auto nn:NodesExplore)
		{
			char retValMap = M->returnStateMap(Cx + DeltaXG[nn], Cy+ DeltaYG[nn]);
			bool ocupied = M->returnVisitStatus(Cx + DeltaXG[nn], Cy+ DeltaYG[nn]);

			//std::cout<<"Studying ("<< Cx + DeltaXG[nn] <<","<< Cy+ DeltaYG[nn] <<")";

			if(( retValMap == EMPTY_SPACE || retValMap == DESTINATION )&& !ocupied)
			{
				NodeG * temp = this->createNode(Pointer,Pointer->px + DeltaXG[nn], Pointer->py + DeltaYG[nn],nn);
				M->setAsVisited(Cx + DeltaXG[nn], Cy+ DeltaYG[nn]);
				Pointer->Connections[nn] = temp;
				bag.push_back(temp);
				//std::cout<<std::endl;
			}else{
				//std::cout<<". Node ("<< Cx + DeltaXG[nn] <<","<< Cy + DeltaYG[nn]
				//		 <<"), not created because retValMap = *"<<retValMap<<"*. ocupied = "<< ocupied <<std::endl;
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

	/*
	//REMOVE LATER, ONLY FOR DISPLAY
	std::cout<<std::endl<<"REMOVE LATER, ONLY FOR DISPLAY"<<std::endl;
	NodeG* Apuntador;
	std::list<NodeG*> bagg;
	bagg.push_back(this->StartPoint);
	while(bagg.size()>0)
	{
		Apuntador = bagg.front();
		this->computeConnections(M, Apuntador );
		for(auto &el:Apuntador->Connections)
		{
			if((el!=nullptr)&&(!el->flagC))
			{
				el->flagC = true;
				bagg.push_back(el);
			}
		}
		bagg.pop_front();
	}
	std::cout<<"REMOVE LATER, ONLY FOR DISPLAY"<<std::endl;
	M->printNConn();
	//REMOVE LATER, ONLY FOR DISPLAY
	*/

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

		char eval = this->computeConnections(M, Pointer );
		/*
		std::cout<<std::endl<<"\nPointer = ("<< this->initPosx + Pointer->px <<","<< this->initPosy + Pointer->py <<"), connections = "<<eval<<std::endl;
		std::cout<<"its companions are"<<std::endl;
		*/
		if((eval=='C'))
		{
			//Internal connections
			//std::cout<<"After internal connection, we have"<<std::endl;
			for(int nn = 0;nn<N_CONN;nn++)
			{
				NodeG* ptr = Pointer->Connections[nn];

				if(ptr==nullptr) continue;
				if(this->computeConnections(M, ptr)=='C') continue;

				std::vector<long long int> NodesI = this->mapNodesGraph[nn];
				std::vector<long long int> NodesP = this->mapPositionsGraph[nn];

				int Size = NodesI.size();
				for(int mm = 0;mm<Size;mm++)
				{
					if(NodesI[mm]==N_CONN)
					{
						ptr->Connections[NodesP[mm]] = Pointer;

					}else{
						ptr->Connections[NodesP[mm]] = Pointer->Connections[NodesI[mm]];

					}
				}
				/*
				std::cout<<"nn = "<<nn<<", ptr = ("<<this->initPosy+ptr->px <<","<<this->initPosy+ptr->py<<")"<<std::endl<<"\t";
				for(auto el:ptr->Connections)
				{
					if(el==nullptr)
					{
						std::cout<<"null, ";
					}else{
						std::cout<<"("<<this->initPosy+ el->px<<","<<this->initPosy+ el->py<<") ";
					}
				}
				std::cout<<std::endl;
				*/
			}


			//connections among the new elements
			//std::cout<<"\nAfter External connection, we have"<<std::endl;
			for(int nn = 0;nn<N_CONN;nn++)
			{
				std::vector<long long int> v_NodeCanProvideTo = this->map_NodeCanProvideTo[nn];
				std::vector<long long int> v_WhattoShareGraphLeft = this->mapWhattoShareGraphLeft[nn];
				std::vector<long long int> v_WhattoShareGraphRight = this->mapWhattoShareGraphRight[nn];
				std::vector<long long int> v_ArrivaltoShareGraphLeft = this->mapArrivaltoShareGraphLeft[nn];
				std::vector<long long int> v_ArrivaltoShareGraphRight = this->mapArrivaltoShareGraphRight[nn];

				NodeG* ptr = Pointer->Connections[nn];

				if(ptr==nullptr) continue;
				//if(this->computeConnections(M, ptr)=='C') continue;

				if(ptr!=nullptr)
				{
					NodeG* ptr_aux = nullptr;


					switch(v_NodeCanProvideTo.size())
					{
						case 2:		//both left and right
									ptr_aux = Pointer->Connections[v_NodeCanProvideTo[0]];
									if(ptr_aux!=nullptr)
									{
										if(this->computeConnections(M, ptr_aux)!='C')
										{
											for(unsigned int ii=0;ii<v_WhattoShareGraphLeft.size();ii++)
											{
												if(ptr_aux->Connections[v_ArrivaltoShareGraphLeft[ii]] == nullptr)//last modification
												{
													ptr_aux->Connections[v_ArrivaltoShareGraphLeft[ii]] = ptr->Connections[v_WhattoShareGraphLeft[ii]];
												}
											}
										}
									}

									ptr_aux = Pointer->Connections[v_NodeCanProvideTo[1]];
									if(ptr_aux!=nullptr)
									{
										if(this->computeConnections(M, ptr_aux)!='C')
										{
											for(unsigned int ii=0;ii<v_WhattoShareGraphRight.size();ii++)
											{
												if(ptr_aux->Connections[v_ArrivaltoShareGraphRight[ii]] == nullptr)//last modification
												{
													ptr_aux->Connections[v_ArrivaltoShareGraphRight[ii]] = ptr->Connections[v_WhattoShareGraphRight[ii]];
												}
											}
										}
									}
									break;
						case 1:		//only left
									ptr_aux = Pointer->Connections[v_NodeCanProvideTo[0]];
									if(ptr_aux!=nullptr)
									{
										if(this->computeConnections(M, ptr_aux)!='C')
										{
											for(unsigned int ii=0;ii<v_WhattoShareGraphLeft.size();ii++)
											{
												if(ptr_aux->Connections[v_ArrivaltoShareGraphLeft[ii]] == nullptr)//last modification
												{
													ptr_aux->Connections[v_ArrivaltoShareGraphLeft[ii]] = ptr->Connections[v_WhattoShareGraphLeft[ii]];
												}
											}
										}
									}
									break;
						default:
									break;
					}
					/*
					std::cout<<"nn = "<<nn<<", ptr = ("<<this->initPosy+ptr->px <<","<<this->initPosy+ptr->py<<")"<<std::endl<<"\t";
					for(auto el:ptr->Connections)
					{
						if(el==nullptr)
						{
							std::cout<<"null, ";
						}else{
							std::cout<<"("<<this->initPosy+ el->px<<","<<this->initPosy+ el->py<<") ";
						}
					}
					std::cout<<std::endl;
					*/
				}

			}

		}
		eval = this->computeConnections(M, Pointer );

		//std::cout<<"\nputting connections of ("<< this->initPosx + Pointer->px <<","<< this->initPosy + Pointer->py <<") in bag"<<std::endl;
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
			}else{
				/*
				if(Elem==nullptr)
				{
					std::cout<<"\t pos: "<< pos_count <<" is nullptr"<<std::endl;
				}else{
					std::cout<<"\t pos: "<< pos_count <<", ("<< this->initPosx + Elem->px <<","<< this->initPosy + Elem->py << ") is flagB = "<<Elem->flagB<<std::endl;
				}
				*/
			}

			pos_count++;
		}
		bag.pop_front();
	}
}

void Graph::growGraphStepThree(Map * M)
{
	std::list<NodeG* > bag;
	std::multimap<int,NodeG*> MapaIncompletes;
	bag.push_back(this->StartPoint);
	char eval = ' ';
	std::vector<long long int> NodesExplore = {0, 1, 2, 3, 4, 5, 6, 7};

	while(bag.size()>0)
	{
		NodeG* Pointer = bag.front();
		Pointer->visited = true;

		eval = this->computeConnections(M, Pointer);
		//std::cout<<std::endl<<"\nPointer = ("<< this->initPosx + Pointer->px <<","<< this->initPosy + Pointer->py <<"), connections = "<<eval<<std::endl;

		if(eval!='C')
		{
			MapaIncompletes.insert (std::make_pair(N_CONN-(eval-48),Pointer));
		}

		for(auto &el:Pointer->Connections)
		{
			if(el!=nullptr)
			{
				//std::cout<<"el = ("<< this->initPosx + el->px <<","<< this->initPosy + el->py <<"), visited = "<<el->visited<<std::endl;
				if(el->visited==false)
				{
					el->visited = true;
					bag.push_back(el);
				}
			}
		}

		bag.pop_front();
	}

	//int SizeMMap = MapaIncompletes.size();
	//std::cout<<std::endl<<"Printing the multimap with size = "<<SizeMMap<<std::endl;

	for(auto Elem:MapaIncompletes)
	{
		//std::cout<<"Key: "<< Elem.first<<", Elem: ("<< this->initPosx+Elem.second->px<<","<< this->initPosx+Elem.second->py<<")"<<std::endl;

		for(auto nn:NodesExplore)
		{
			char retValMap = M->returnStateMap(this->initPosx + Elem.second->px+ DeltaXG[nn], this->initPosy + Elem.second->py+ DeltaYG[nn]);

			if((retValMap != WALL)&&(Elem.second->Connections[nn]==nullptr))
			{
				//std::cout<<"\tWe need the node ("<< this->initPosx + Elem.second->px+ DeltaXG[nn] <<","<< this->initPosy + Elem.second->py+ DeltaYG[nn]<<")"<<std::endl;

				for(auto missing:MapaIncompletes)
				{
					if((missing.second->px == Elem.second->px+ DeltaXG[nn])&&(missing.second->py == Elem.second->py+ DeltaYG[nn]))
					{
						Elem.second->Connections[nn] = missing.second;
					}
				}
				eval = this->computeConnections(M, Elem.second);
			}

		}
	}
}

void Graph::cleanAllFlags()
{
	std::list<NodeG* > bag;
	bag.push_back(this->StartPoint);

	while(bag.size()>0)
	{
		NodeG* Pointer = bag.front();

		Pointer->visited = false;
		Pointer->flagA = false;
		Pointer->flagB = false;
		Pointer->flagC = false;

		for(auto &el:Pointer->Connections)
		{
			if(el!=nullptr)
			{
				if( (el->visited ==true)||(el->flagA ==true)||(el->flagB ==true)||(el->flagC ==true) )
				{
					el->visited = false;
					el->flagA = false;
					el->flagB = false;
					el->flagC = false;

					bag.push_back(el);

				}
			}
		}

		bag.pop_front();
	}
}

NodeG * Graph::growGraph(Map * M)
{
	//std::cout<<std::endl<<"Results with Graph.\nDoing phase 1"<<std::endl;
	NodeG *retVal = this->growGraphStepOne(M,this->StartPoint);
	//retVal->cummulativeDistance = 0.0;

	//std::cout<<std::endl<<std::endl<<"DOING PHASE TWO... "<<std::endl;
	this->growGraphStepTwo(M);

	//std::cout<<std::endl<<std::endl<<"DOING PHASE THREE... "<<std::endl;
	this->growGraphStepThree(M);

	//NEED TO CLEAN ALL NODES' FLAGS
	this->cleanAllFlags();
	this->DestinationNode = retVal;

	return retVal;
}

void Graph::assignLevel(Map * M)
{
	NodeG* Pointer = nullptr;
	std::list<NodeG*> bag;

	bag.push_back(this->StartPoint);

	while(bag.size()>0)
	{
		Pointer = bag.front();

		M->writeoverMap(this->initPosx + Pointer->px,this->initPosy + Pointer->py,levelChar[Pointer->Level]);

		Pointer->flagA = true;
		//int pcounter = 0;
		for(auto &el:Pointer->Connections)
		{
			if(el!=nullptr)
			{
				if(el->flagA==false)
				{
					el->Level = Pointer->Level + 1;

					double RefMin = DBL_MAX;

					int pcounter = 0;
					int counterSelect = 0;
					NodeG * ptrSelect = nullptr;
					for(auto v:el->Connections)
					{
						if(v!=nullptr)
						{
							if((v->cummulativeDistance < RefMin)&&(v->Level < el->Level))
							{
								RefMin = v->cummulativeDistance;
								counterSelect = pcounter;
								ptrSelect = v;
							}
						}
						pcounter++;
					}
					el->cummulativeDistance = RefMin + DistanceGain[counterSelect];
					el->Parent = ptrSelect;
					el->flagA = true;
					bag.push_back(el);
				}
			}
		}

		bag.pop_front();
	}
}

double Graph::findPath(std::list<NodeG*> &Path,NodeG * Pointer)
{
	double retVal = 0.0;

	NodeG* ptr = Pointer;

	while(ptr->Parent != nullptr)
	{
		NodeG* candidate = nullptr;
		double Temp = DBL_MAX;

		//std::cout<<"contenders: \n\t";
		for(auto elem: ptr->Connections)
		{
			if(elem!=nullptr)
			{
				if ((elem->Level <= ptr->Level)&&(elem->cummulativeDistance < Temp))
				{
					Temp = elem->cummulativeDistance;
					candidate = elem;
				}
				//std::cout<<elem->cummulativeDistance<<" ";
			}//else{
			//	std::cout<<"null ";
			//}
		}
		if(candidate->cummulativeDistance < ptr->Parent->cummulativeDistance)
		{
			ptr = candidate;
		}else{
			ptr = ptr->Parent;
		}
		Path.push_front(ptr);
		//std::cout<<"\nptr = ("<< this->initPosx + ptr->px <<"," << this->initPosy + ptr->py <<"), L"<<ptr->Level<<std::endl;
	}
	Path.pop_front();


	return retVal;
}

Graph::~Graph()
{
	delete this->StartPoint;
}

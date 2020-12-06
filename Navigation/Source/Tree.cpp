/*
 * Tree.cpp
 *
 *  Created on: Oct 9, 2020
 *      Author: luis
 */
#include "Tree.h"
#include "Map.h"

Tree::Tree(long long int Ipx, long long int Ipy)
{
	this->Root = new Node;
	this->initPosx = Ipx;
	this->initPosy = Ipy;

	this->Root->px = 0;
	this->Root->py = 0;
}

Node * Tree::createNode(Node * Pointer, long long int Ipx, long long int Ipy)
{//Pointer is the father
	Node * Temp = new Node;

	Temp->px=Ipx;
	Temp->py=Ipy;
	Temp->Parent = Pointer;
	Temp->Level = Pointer->Level + 1;

	return Temp;
}

Node * Tree::exploreMapbyDepth(Map * M, Node *Ptr)
{
	long long int Cx = 0;
	long long int Cy = 0;
	long long int Ccx = 0;
	long long int Ccy = 0;

	Node * Pointer = Ptr;
	Node * retVal = nullptr;
	Node * AuxNode = nullptr;

	Cx = this->initPosx + Pointer->px;
	Cy = this->initPosy + Pointer->py;
	M->setAsVisited(Cx,Cy);

	//std::cout<<"Tree::exploreMap Pointer = ("<< Pointer->px <<", "<< Pointer->py <<")"<<std::endl;

	for(int nn = 0;nn < this->Nconst;nn++)
	{
		Cx = this->initPosx + Pointer->px + DeltaX[nn];
		Cy = this->initPosy + Pointer->py + DeltaY[nn];
		Ccx = Pointer->px + DeltaX[nn];
		Ccy = Pointer->py + DeltaY[nn];

		if(!M->returnVisitStatus(Cx,Cy))
		{
			AuxNode = this->createNode(Pointer,Ccx,Ccy);
			Pointer->Connections.push_back(AuxNode);
			if(M->returnStateMap(Cx,Cy) == DESTINATION)
			{
				retVal = AuxNode;

				std::cout<<"\nGoing up to the parent..."<<std::endl;
				Node *ppt = Pointer;
				Map auxMap = *M;

				long long int prev_px = retVal->px;
				long long int prev_py = retVal->py;
				double cumDist = 0.0;

				//std::cout<<"retVal ("<<this->initPosx + retVal->px <<","<<this->initPosy + retVal->py <<")"<<std::endl;
				//std::cout<<"Start ppt ("<<this->initPosx + ppt->px <<","<<this->initPosy + ppt->py <<")"<<std::endl;

				while(ppt->Parent!=nullptr)
				{
					auxMap.writeoverMap(this->initPosx + ppt->px,this->initPosy + ppt->py,PATH);

					//std::cout<<"("<< this->initPosx + ppt->px <<","<< this->initPosy + ppt->py <<") | ppt->px - prev_px ="<< ppt->px - prev_px <<", ppt->py - prev_py = "<< ppt->py - prev_py <<std::endl;
					cumDist += sqrt(pow(ppt->px - prev_px,2)+pow(ppt->py - prev_py,2));

					prev_px = ppt->px;
					prev_py = ppt->py;

					ppt = ppt->Parent;
				}
				//auxMap.writeoverMap(this->initPosx + ppt->px,this->initPosy + ppt->py,PATH);

				//std::cout<<"("<< this->initPosx + ppt->px <<","<< this->initPosy + ppt->py <<") | ppt->px - prev_px ="<< ppt->px - prev_px <<", ppt->py - prev_py = "<< ppt->py - prev_py <<std::endl;
				cumDist += sqrt(pow(ppt->px - prev_px,2)+pow(ppt->py - prev_py,2));

				std::cout<<"("<<this->initPosx + ppt->px <<","<<this->initPosy + ppt->py <<")"<<std::endl;

				auxMap.printMap();

				std::cout<< "Cost of path = "<<cumDist<<std::endl;

				break;
			}
			M->setAsVisited(Cx,Cy);
		}
	}

	for(auto Elem: Pointer->Connections)
	{
		retVal = this->exploreMapbyDepth(M,Elem);
		if(retVal!=nullptr) break;
	}

	return retVal;
}

Node * Tree::exploreMapbyBreadth(Map * M, Node *Ptr)
{
	long long int Cx = 0;
	long long int Cy = 0;
	long long int Ccx = 0;
	long long int Ccy = 0;

	Node * Pointer = nullptr;
	Node * retVal = nullptr;
	Node * AuxNode = nullptr;

	std::list<Node*> bag;
	bag.push_back(Ptr);

	while(bag.size()>0)
	{
		Pointer = bag.front();

		Cx = this->initPosx + Pointer->px;
		Cy = this->initPosy + Pointer->py;
		M->setAsVisited(Cx,Cy);

		//std::cout<<"Node considered ("<< Cx<<","<< Cy <<")"<<std::endl;

		for(int nn = 0;nn < this->Nconst;nn++)
		{
			Cx = this->initPosx + Pointer->px + DeltaXBA[nn];
			Cy = this->initPosy + Pointer->py + DeltaYBA[nn];
			Ccx = Pointer->px + DeltaXBA[nn];
			Ccy = Pointer->py + DeltaYBA[nn];

			//std::cout<<"M->returnVisitStatus("<<Cx<<","<<Cy<<") = "<<M->returnVisitStatus(Cx,Cy) <<std::endl;
			//std::cout<<"M->returnStateMap("<<Cx<<","<<Cy<<") = "<< M->returnStateMap(Cx,Cy) <<std::endl;

			if((!M->returnVisitStatus(Cx,Cy))&&(M->returnStateMap(Cx,Cy)!=WALL))
			{
				AuxNode = this->createNode(Pointer,Ccx,Ccy);
				Pointer->Connections.push_back(AuxNode);

				//std::cout<<"Node created ("<< this->initPosx + AuxNode->px<<","<< this->initPosy + AuxNode->py <<")"<<std::endl;

				//std::cout<<std::endl;
				//M->printVisits();
				//std::cout<<std::endl;

				if(M->returnStateMap(Cx,Cy) == DESTINATION)
				{
					retVal = AuxNode;
					this->setofRoads.push_back(AuxNode);

					std::cout<<"\nGoing up to the parent..."<<std::endl;
					Node *ppt = Pointer;
					Map auxMap = *M;

					long long int prev_px = retVal->px;
					long long int prev_py = retVal->py;
					double cumDist = 0.0;

					while(ppt->Parent!=nullptr)
					{
						auxMap.writeoverMap(this->initPosx + ppt->px,this->initPosy + ppt->py,PATH);
						cumDist += sqrt(pow(ppt->px - prev_px,2)+pow(ppt->py - prev_py,2));

						prev_px = ppt->px;
						prev_py = ppt->py;

						ppt = ppt->Parent;
					}
					std::cout<<"("<<this->initPosx + ppt->px <<","<<this->initPosy + ppt->py <<")"<<std::endl;
					cumDist += sqrt(pow(ppt->px - prev_px,2)+pow(ppt->py - prev_py,2));
					auxMap.printMap();

					std::cout<< "Cost of path = "<<cumDist<<std::endl;
					//break;
				}
				M->setAsVisited(Cx,Cy);
			}
		}

		for(auto &Elem: Pointer->Connections)
		{
			if((Elem!=nullptr)&&(!Elem->flagB))
			{
				Elem->flagB = true;
				bag.push_back(Elem);
			}
		}

		bag.pop_front();
		/*
		std::cout<<"bag.size() = "<<bag.size()<<std::endl;
		std::cout<<"Inside bag"<<std::endl;
		for(auto el:bag)
		{
			if(el==nullptr)
			{
				std::cout<<"nullptr"<<std::endl;
			}else{
			std::cout<<"("<< this->initPosx + el->px<<","<< this->initPosy + el->py <<"), ";
			}
		}
		std::cout<<std::endl<<std::endl;
		*/
	}
	return retVal;
}

void Tree::printTree()
{
	Node * Pointer = this->Root;
	long long int level = 0;

	std::list<Node *> bag;
	bag.push_back(Pointer);

	while(bag.size()>0)
	{
		Node * tp = bag.front();
		if(level < tp->Level) std::cout<<std::endl;
		std::cout<<"("<< tp->px <<","<< tp->py <<") ";
		level = tp->Level;

		for(auto el:tp->Connections) bag.push_back(el);

		bag.pop_front();
	}
}

Tree::~Tree()
{
	delete this->Root;
}


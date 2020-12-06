/*
 * Map.cpp
 *
 *  Created on: Oct 9, 2020
 *      Author: luis
 */
#include "Map.h"

Map::Map(long long int N,long long int M)
{
	std::vector<std::vector<char>> Temp(M);
	std::vector<char> temp(N);
	Temp[0] = temp;

	std::vector<std::vector<bool>> TempB(M);
	std::vector<bool> tempB(N);
	TempB[0] = tempB;

	this->Grid = Temp;
	this->visited = TempB;
	this->NConn = Temp;
	this->N = N;
	this->M = M;
}

Map::~Map()
{

}

void Map::addLine(const std::string &Str,long long int yy, long long int N)
{
	long long int counter = 0;
	std::vector<char> temp(N);
	std::vector<bool> tempB(N,false);
	std::vector<char> tempConn(N);

	for(auto ll:Str)
	{
		switch(ll)
		{
		case '0':
					temp[counter] = EMPTY_SPACE;
					tempConn[counter] = EMPTY_SPACE;
					break;
		case '1':
					temp[counter] = WALL;
					tempConn[counter] = WALL;
					tempB[counter] = true;
					break;
		case 'X':
					temp[counter] = DESTINATION;
					tempConn[counter] = DESTINATION;
					break;
		default:
					temp[counter] = '@';
					tempConn[counter] = '@';
					break;
		}
		counter++;
	}
	this->Grid[yy] = temp;
	this->visited[yy] = tempB;
	this->NConn[yy] = tempConn;
}

void Map::writeoverMap(long long int Ipx, long long int Ipy, char Sym)
{
	this->Grid[Ipy][Ipx] = Sym;
}

char Map::returnStateMap(long long int Ipx, long long int Ipy)
{
	char retVal =  this->Grid[Ipy][Ipx];
	return retVal;
}

bool Map::returnVisitStatus(long long int Ipx, long long int Ipy)
{
	return this->visited[Ipy][Ipx];
}

void Map::setAsVisited(long long int Ipx, long long int Ipy)
{
	this->visited[Ipy][Ipx] = true;
}

long long int Map::returnBoundPlaces()
{
	return ((this->M - 2)*(this->M - 2));
}

void Map::cleanVisits()
{
	for(long long int mm=0;mm<this->M;mm++)
	{
		for(long long int nn=0;nn<this->N;nn++)
		{
			if(this->Grid[mm][nn] != WALL) this->visited[mm][nn] = false;
		}
	}

}

void Map::writeOnNConn(long long int Ipx, long long int Ipy,char Letter)
{
	this->NConn[Ipy][Ipx] = Letter;
}

void Map::printMap()
{
	long long int counter = 0;



	std::cout<<"\t";
	for(long long int ii = 0;ii<this->N;ii++)
	{
		std::cout<<ii/10;
	}
	std::cout<<std::endl;

	std::cout<<"\t";
	for(long long int ii = 0;ii<this->N;ii++)
	{
		std::cout<<ii%10;
	}
	std::cout<<std::endl<<std::endl;


	for(auto Elem: this->Grid)
	{
		std::cout<<counter/10<<counter%10<<"\t";
		for(auto elem: Elem)
		{
			std::cout<<elem;
		}
		std::cout<<std::endl;
		counter++;
	}
}

void Map::printVisits()
{
	long long int counter = 0;

	std::cout<<"\t";
	for(long long int ii = 0;ii<this->N;ii++)
	{
		std::cout<<ii/10;
	}
	std::cout<<std::endl;

	std::cout<<"\t";
	for(long long int ii = 0;ii<this->N;ii++)
	{
		std::cout<<ii%10;
	}
	std::cout<<std::endl<<std::endl;

	for(auto Elem: this->visited)
	{
		std::cout<<counter/10<<counter%10<<"\t";
		for(auto elem: Elem)
		{
			elem? std::cout<<elem: std::cout<<" ";
		}
		std::cout<<std::endl;
		counter++;
	}
}
void Map::printNConn()
{
	long long int counter = 0;

	std::cout<<"\t";
	for(long long int ii = 0;ii<this->N;ii++)
	{
		std::cout<<ii/10;
	}
	std::cout<<std::endl;

	std::cout<<"\t";
	for(long long int ii = 0;ii<this->N;ii++)
	{
		std::cout<<ii%10;
	}
	std::cout<<std::endl<<std::endl;

	for(auto Elem: this->NConn)
	{
		std::cout<<counter/10<<counter%10<<"\t";
		for(auto elem: Elem)
		{
			std::cout<<elem;
		}
		std::cout<<std::endl;
		counter++;
	}
}

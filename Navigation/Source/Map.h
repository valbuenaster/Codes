/*
 * Map.h
 *
 *  Created on: Oct 9, 2020
 *      Author: luis
 */

#ifndef SOURCE_MAP_H_
#define SOURCE_MAP_H_

#include <iostream>
#include <vector>
#include <string>

#define WALL '#'
#define EMPTY_SPACE ' '
#define INITIAL_P '@'
#define PATH 'o'
#define DESTINATION 'X'

class Map
{
	std::vector<std::vector<char>> Grid;
	std::vector<std::vector<bool>> visited;
	std::vector<std::vector<char>> NConn;
	long long int N;
	long long int M;

public:
	Map(long long int N,long long int M);
	void addLine(const std::string &Str,long long int yy, long long int N);
	void writeoverMap(long long int Ipx, long long int Ipy, char Sym);
	char returnStateMap(long long int Ipx, long long int Ipy);
	bool returnVisitStatus(long long int Ipx, long long int Ipy);
	long long int returnBoundPlaces();
	void setAsVisited(long long int Ipx, long long int Ipy);
	void cleanVisits();
	void writeOnNConn(long long int Ipx, long long int Ipy,char Letter);

	void printMap();
	void printVisits();
	void printNConn();

	~Map();
};



#endif /* SOURCE_MAP_H_ */

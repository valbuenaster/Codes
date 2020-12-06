/*
 * main.cpp
 *
 *  Created on: Dec 5, 2020
 *      Author: luis
 */

#include <iostream>
#include <sstream>
#include <memory>
#include <string>

using namespace std;

/*
struct Nd
{
	double x = 0;
	double y = 0;

	struct Nd* pointer = nullptr;
}typedef Node;
*/

struct Nd
{
	double x = 0;
	double y = 0;

	shared_ptr<struct Nd> pointer = nullptr;
}typedef Node;

shared_ptr<Node> createNode(double xX, double yY, shared_ptr<Node> Pointer)
{
	auto Ptr = make_shared<Node>();
	Ptr->x = xX;
	Ptr->y = yY;
	Ptr->pointer = Pointer;

	return Ptr;
}


int main(int argc, char** argv)
{
	int cc = 3;
	int t1 = 0;
	int t2 = 0;

	auto Head = make_shared<Node>();

	if(argc < 2)
	{
		cout << "Need a bigger even number of arguments" << endl;
		return -1;
	}
	string TA = argv[1];
	string TB = argv[2];

	stringstream converterA(TA);
	converterA >> Head->x;

	stringstream converterB(TB);
	converterB >> Head->y;

	while(cc < argc)
	{
		string T1 = argv[cc];
		string T2 = argv[cc+1];
		stringstream converter(T1);
		stringstream converter2(T2);
		converter >> t1;
		converter2 >> t2;
		Head = createNode(t1,t2,Head);
		cc += 2;
	}

	auto Ptr = Head;

	while(Ptr->pointer!=nullptr)
	{
		cout << "("<< Ptr->x <<","<< Ptr->y <<")"<<endl;
		Ptr = Ptr->pointer;
	}
	cout << "("<< Ptr->x <<","<< Ptr->y <<")"<<endl;


	return 0;
}


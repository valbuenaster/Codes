#include <iostream>
#include <string>

using namespace std;

template <class T>
T addNumbers(T input1, T input2)
{
	return input1 + input2;
}


int main(int argc, char * argv[])
{
	//long long int A = 0;
	//long long int B = 0;

	double A = 0;
	double B = 0;

	cout<<"N = "<<argc<<endl;

	for(int n = 0;n<argc;n++)
	{
		cout<<"elem = "<<argv[n]<<endl;
	}


	cout<<"Type A and B"<<endl;
	cin >> A >> B;

	auto Resultado = addNumbers(A,B);

	cout <<"El resultado es "<< Resultado <<endl;

}

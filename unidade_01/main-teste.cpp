#include <iostream>
#include <cmath>
#include <cstring>

#include "Poly.hpp"

using namespace std;

string arq1 = "poly_P1.txt";
string arq2 = "poly_P2.txt";

int main(int argc, char *argv[]){
	Poly p(3);

	cin >> p;

	cout << p;
	cout << '\n';

	p.recriar(2);
	cout << p;
	cout <<'\n';

	

	return 0;
}
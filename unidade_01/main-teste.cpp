#include <iostream>
#include <cmath>
#include <cstring>

#include "Poly.h"

using namespace std;

string arq1 = "poly_P1.txt";
string arq2 = "poly_P2.txt";
string arq3 = "poly_result.txt";

string printGrau(const Poly& P)
{
  string prov;

  if (!P.empty()) prov=" ";
  prov += '[';
  if (P.empty()) prov += "Vazio";
  else if (P.isZero()) prov += "Nulo";
  else
  {
    prov += "Grau ";
    prov += to_string(P.getGrau());
  }
  prov += ']';
  return prov;
}

int main(int argc, char *argv[])
{
	Poly P1;
	Poly P2(0);
	Poly Result(1);

	P1.ler("poly_P1.txt");
	P2.ler("poly_P2.txt");
	Result.ler("poly_result.txt");

	cout << "P1:     " << P1 << printGrau(P1) << endl;
	cout << "P2:     " << P2 << printGrau(P2) << endl;
	cout << "Result: " << Result << printGrau(Result) << endl;

	Poly prov(Result);
	Result = P2;
	P2 = P1;
	P1 = prov;

	cout << endl;
	cout << "P1:     " << P1 << printGrau(P1) << endl;
	cout << "P2:     " << P2 << printGrau(P2) << endl;
	cout << "Result: " << Result << printGrau(Result) << endl;

	P1 = -P1;
	cout << endl;
	cout << "P1:     " << P1 << printGrau(P1) << endl;

	P1 = P1+P2;
	cout << endl;
	cout << "P1:     " << P1 << printGrau(P1) << endl;

	return 0;
}
#ifndef POLY_H
#define POLY_H

#include <iostream>
#include <cmath>

using std::pow;
using std::abs;

typedef unsigned int uint;

class Poly
{

	friend std::ostream &operator<<(std::ostream &out, Poly &pl);

public:
	Poly();
	Poly(uint grau);

	~Poly();

	bool empty() const;
	bool isZero() const;

	int getGrau() const;
	double getCoef(uint index) const;
	
	double getValor(double valor);

	double operator[](uint index);
	double operator()(double valor);

private:
	unsigned D; //Váriável que armazena a dimensão do poli.
	double *a;
};

#endif
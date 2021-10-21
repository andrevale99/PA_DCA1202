#ifndef POLY_H
#define POLY_H

#include <iostream>

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

private:
	unsigned D; //Váriável que armazena a dimensão do poli.
	double *a;
};

#endif
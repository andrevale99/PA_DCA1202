#ifndef POLY_H
#define POLY_H


typedef unsigned int uint;

class Poly
{

public:
	Poly();
	Poly(uint grau);

	bool empty() const;
	bool isZero() const;

	int getGrau() const;

private:
	unsigned D; //Váriável que armazena a dimensão do poli.
	double *a;
};

#endif
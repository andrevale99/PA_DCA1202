#ifndef POLY_H
#define POLY_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <cstring>

using std::pow;
using std::abs;

using std::string;

using std::ios;

typedef unsigned int uint;

class Poly
{

	friend std::ostream& operator<<(std::ostream &out, Poly &pl);
	friend std::istream& operator>>(std::istream &in, Poly &pl);

public:
	Poly();
	explicit Poly(uint grau);
	Poly(const Poly &poly);

	~Poly();

	void recriar(uint grau);

	bool empty() const;
	bool isZero() const;

	void setCoef(uint index, float num);

	int getGrau() const;
	double getCoef(uint index) const;
	
	double getValor(double valor) const;

	double operator[](uint index);
	double operator()(double valor);
	Poly operator+(const Poly &poly) const;
	Poly operator-(const Poly &poly) const;
	Poly operator-() const;
	void operator=(const Poly &poly);
	Poly operator*(const Poly &poly) const;

	bool salvar(string nome_arq);
	bool ler(string nome_arq);

private:
	unsigned D; //Váriável que armazena a dimensão do poli.
	double *a;
};

#endif
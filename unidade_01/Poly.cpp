#include "Poly.hpp"

/**
 *
 */
Poly::Poly()
{
	D = 0;
	a = nullptr;
}

/**
 *
 */
Poly::Poly(uint grau)
{
	D = grau + 1;
}

/**
 *
 */
bool Poly::empty() const
{
	if (D == 0)
		return true;
	else
		return false;
}

/**
 *
 */
bool Poly::isZero() const
{
	if (D == 1 && a[0] == 0)
		return true;
	else
		return false;
}

/**
 *
 */
int Poly::getGrau() const
{
	return int(D) - 1;
}
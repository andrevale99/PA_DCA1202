#include "Poly.hpp"

/**
 * @brief Construtor Padrão
 */
Poly::Poly()
{
	D = 0;
	a = nullptr;
}

/**
 * @brief Contrutor "overload" para já definir o graus do polinômio
 */
Poly::Poly(uint grau)
{
	D = grau + 1;
	a = new double[D];
}

/**
 * @brief Destrutor padrão
 */
Poly::~Poly()
{
	delete[] a;
}

/**
 * @brief Verifica se o vetor está vázio
 *
 * @return true--> Se for vázio
 * 		   false--> Caso contrário
 */
bool Poly::empty() const
{
	if (D == 0)
		return true;
	else
		return false;
}

/**
 * @brief Verifica se o polinômio é de Grau Zero
 *
 * @return true--> Caso seja somente 0.0
 * 		   false--> Caso tenha mais de uma dimensão (a + bx + cx^2 + ... + zx^n)
 */
bool Poly::isZero() const
{
	if (D == 1 && a[0] == 0)
		return true;
	else
		return false;
}

/**
 * @brief Adquire o grau dopolinômio
 *
 * @return grau do polinômio
 */
int Poly::getGrau() const
{
	return int(D) - 1;
}

//========================OVERLOADS====================================

/**
 *	@brief Mostra o polinômio da maneira padrão (std::cout << Poly)
 *
 * @return Mensagem padrão do polinômio
 */
std::ostream &operator<<(std::ostream &out, Poly &pl)
{
	for (int i = 0; i < pl.getGrau(); ++i)
		out << pl.a[i] << ' ';

	out << '\n';

	return out;
}
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

	a[0] = 6.2;
	a[1] = 5;
	a[2] = 3;
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

/**
 * @brief Retorna o valor do coeficiente do grau
 * 
 * @param index--> Índice do coeficiente
 * 
 * @return valor do coeficiente
*/
double Poly::getCoef(uint index) const
{
	if (index >= D)
		return 0;
	return a[index];
}

/**
 * 
*/
double Poly::getValor(double valor)
{
	double total = 0;
	for(int i=getGrau(); i>=0; --i){
		total += a[i]*pow(valor, i);
	}
	return total;
}

//========================OVERLOADS====================================

/**
 * @brief Overload para acessar o valor do coeficiente (mesmo que getCoef())
 * 
 * @param index--> Índice do coeficiente
 * 
 * @return valor do coeficiente
*/
double Poly::operator[](uint index)
{
	if (index >= D)
		return 0;
	return a[index];
}

/**
 * 
*/
double Poly::operator()(double valor)
{
	double total = 0;
	for(int i=getGrau(); i>=0; --i){
		total += a[i]*pow(valor, i);
	}
	return total;
}

/**
 *	@brief Mostra o polinômio da maneira padrão (std::cout << Poly)
 *
 * 	@param out--> ostream
 * 	@param pl--> Objeto criado
 * 
 *  @return Mensagem padrão do polinômio
 */
std::ostream &operator<<(std::ostream &out, Poly &pl)
{
	for (int i = pl.getGrau(); i >= 0; --i)
	{

		if (pl.a[i] == 0.0)
		{
			if (i == 0 && pl.getGrau() == 0)
			{
				out << pl.a[i];
			}
		}
		else
		{
			if (pl.a[i] < 0.0)
				out << '-';
			else if (i != pl.getGrau())
				out << '+';

			if (abs(pl.a[i]) != 1.0 || i == 0)
				out << abs(pl.a[i]);

			if (i != 0)
			{
				if (abs(pl.a[i]) != 1.0)
					out << '*';

				out << 'x';

				if (i > 1)
				{
					out << '^';
					out << i;
				}
			}
		}

	}//FIM LOOP FOR

	return out;
}
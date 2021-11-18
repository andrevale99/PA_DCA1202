#include "Poly.h"

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

	for (int i = 0; i < getGrau(); ++i)
	{
		a[i] = 0.;
	}
	a[getGrau()] = 1.;
}

Poly::Poly(const Poly &poly)
{
	D = poly.D;
	a = new double[D];

	for (uint i = 0; i < D; ++i)
		a[i] = poly.a[i];
}

/**
 * @brief Destrutor padrão
 */
Poly::~Poly()
{
	D = -1;
	//delete[] a;
}

/**
 * @brief Recria o polinômio para o grau desejado
 */
void Poly::recriar(uint grau)
{
	if (grau == 0)
	{
		delete[] a;
		a = nullptr;

		D = grau + 1;
		a = new double[grau + 1];

		a[0] = 0.0;
	}
	else
	{
		delete[] a;
		a = nullptr;

		D = grau + 1;
		a = new double[grau + 1];

		for (int i = 0; i < getGrau(); ++i)
		{
			a[i] = 0.;
		}
		a[getGrau()] = 1;
	}
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
 * @brief 
 * 
 */
void Poly::setCoef(uint index, float num)
{
	if (index < 0 || index > getGrau())
		std::cerr << "Indice Invalido\n";

	else if (index == getGrau() && num == 0.)
		std::cerr << "Coeficiente Invalido\n";

	else if (getGrau() == 0)
		a[0] = num;

	else
		a[index] = num;
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
double Poly::getValor(double valor) const
{
	double total = 0;
	if (empty())
		total = 0;

	else
	{
		for (int i = getGrau(); i >= 0; --i)
		{
			total += a[i] * pow(valor, i);
		}
	}

	return total;
}

/**
 * @brief Salva o polinômio em um arquivo .txt
 *
 * @return 1(true) --> Salvou nomrlamente
 * 		   0(false) --> Não salvou
 */
bool Poly::salvar(string nome_arq)
{
	std::ofstream save(nome_arq, std::ios::out);

	save << "POLY" << ' ' << D << '\n';

	for (uint i = 0; i < D; ++i)
	{
		save << a[i] << ' ';
	}

	save.close();

	std::ifstream save_test(nome_arq, std::ios::in);

	if (!save_test.good())
	{
		save_test.close();
		return false;
	}
	else
	{
		save_test.close();
		return true;
	}
}

/**
 * @brief Ler o arquivo .txt para criar o polinômio
 *
 * @return 1(true) --> Leu normalmente
 * 		   0(false) --> Não leu
 */
bool Poly::ler(string nome_arq)
{

	bool teste;

	std::ifstream arq_ler(nome_arq, ios::in);

	string poly;

	arq_ler >> poly;
	arq_ler >> D;

	if (poly == "POLY")
	{

		a = new double[D];

		for (uint i = 0; i < D; ++i)
		{
			if (arq_ler.eof())
			{
				arq_ler.close();

				teste = false;

				break;
			}
			arq_ler >> a[i];
		}

		teste = true;
	}
	else if (poly != "POLY" || D < 0)
	{
		arq_ler.close();

		teste = false;
	}

	return teste;
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
 * @brief Retorna o valor da função em x;
 *
 * @param valor --> Valor que deseja calcular
 *
 * @return total --> Resultado da equação em x
 */
double Poly::operator()(double valor)
{
	double total = 0;
	for (int i = getGrau(); i >= 0; --i)
	{
		total += a[i] * pow(valor, i);
	}
	return total;
}

/**
 * @brief Sobrecarrega o + para soma de polinômios
 * 
 * @param poly 
 * @return Poly 
 */
Poly Poly::operator+(const Poly &poly) const
{

	if (poly.empty() || poly.isZero())
		return *this;

	else if (this->empty() || this->isZero())
		return poly;

	else
	{
		if (getGrau() == poly.getGrau())
		{
			Poly prov(getGrau());
			for (uint i = 0; i < poly.D; ++i)
				prov.a[i] = this->a[i] + poly.a[i];

			return prov;
		}

		else if (getGrau() > poly.getGrau())
		{
			Poly prov;
			prov = (*this);

			for (uint i = 0; i < poly.D; ++i)
				prov.a[i] = this->a[i] + poly.a[i];

			return prov;
		}

		else
		{
			Poly prov;
			prov = poly;

			for (uint i = 0; i < this->D; ++i)
				prov.a[i] = this->a[i] + poly.a[i];

			return prov;
		}
	}
}

/**
 * @brief Sobrecarrega o - para subtração de polinômios
 * 
 * @param poly 
 * @return Poly 	
*/
Poly Poly::operator-(const Poly &poly) const
{

	Poly prov(poly.D);

	if (poly.empty() || poly.isZero())
		return *this;

	else if (this->empty() || this->isZero())
	{
		for (uint i = 0; i < poly.D; ++i)
		{
			prov.a[i] = -poly.a[i];
		}
		return prov;
	}

	else
	{
		if (getGrau() == poly.getGrau())
		{
			Poly prov(getGrau());
			for (uint i = 0; i < poly.D; ++i)
				prov.a[i] = this->a[i] - poly.a[i];

			return prov;
		}

		else if (getGrau() > poly.getGrau())
		{
			Poly prov;
			prov = (*this);

			for (uint i = 0; i < poly.D; ++i)
				prov.a[i] = this->a[i] - poly.a[i];

			return prov;
		}

		else
		{
			Poly prov;

			prov = poly;
			prov = -prov;

			for (uint i = 0; i < this->D; ++i)
				prov.a[i] = this->a[i] - poly.a[i];

			return prov;
		}
	}
}

Poly Poly::operator-() const
{

	if (this->empty())
		return Poly();

	else if (this->isZero())
		return Poly(0);

	Poly prov(this->getGrau());
	for (uint i = 0; i < prov.D; ++i)
	{
		prov.a[i] = -1 * (this->a[i]);
	}
	return prov;
}

void Poly::operator=(const Poly &poly)
{
	if (this != &poly)
	{
		if (this->D != poly.D)
		{
			delete[] a;

			D = poly.D;

			a = new double[D];

			for (uint i = 0; i < D; ++i)
				this->a[i] = poly.a[i];
		}

		else if (this->D == poly.D)
		{
			for (uint i = 0; i < D; ++i)
				this->a[i] = poly.a[i];
		}
	}
}

/**
 * @brief Multiplicação de Polinômios
 * 
 * @param poly 
 * @return Poly 
 */
Poly Poly::operator*(const Poly &poly) const
{
	if ((this->empty() && poly.isZero()) || (this->isZero() && poly.empty()))
		return Poly();

	else if ((this->empty() && poly.getGrau() > 0) || (this->getGrau() > 0 && poly.empty()))
		return Poly();

	else if ((this->getGrau() > 0 && poly.isZero()) || (this->isZero() && poly.getGrau() > 0))
		return Poly(0);

	else
	{
		// x^2 * x^2 = x^4

		uint grau = getGrau() + poly.getGrau();
		Poly prov(grau);

		for (uint i = 0; i < this->D; ++i)
		{
			for (uint j = 0; j < poly.D; ++j)
			{
				prov.a[i + j] += a[i] * poly.a[j];
				std::cout << prov.a[i] << '\t' << a[i] << ',' << poly.a[i] << '\n';
			}
		}

		return prov;
	}
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

	} // FIM LOOP FOR

	return out;
}

std::istream &operator>>(std::istream &in, Poly &pl)
{
	std::cout << pl.getGrau() << '\n';
	for (int i = pl.getGrau(); i >= 0; --i)
	{
		std::cout << "x^" << i << ": ";
		in >> pl.a[i];
	}

	return in;
}
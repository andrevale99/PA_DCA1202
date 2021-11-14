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
	D = -1;
	delete[] a;
}

/**
 * @brief Recria o polinômio para o grau desejado
 */
void Poly::recriar(uint grau)
{
	if (grau == 0)
	{
		delete[] a;

		D = grau + 1;
		a = new double[grau + 1];

		a[0] = 0.0;
	}
	else
	{
		D = grau + 1;
		double *prov = new double[grau + 1];

		for (int i = 0; i < getGrau()+1; ++i)
		{
			prov[i] = a[i];
		}

		delete[] a;

		a = new double[grau + 1];

		for (int i = 0; i < getGrau()+1; ++i)
		{
			a[i] = prov[i];
		}

		delete[] prov;
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
	for (int i = getGrau(); i >= 0; --i)
	{
		total += a[i] * pow(valor, i);
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

	save << "POLY" << ' ' << D << ' ';

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

Poly Poly::operator+(const Poly &poly) const
{
	Poly prov;

	if (poly.empty() || poly.isZero())
		return *this;
	else if (this->empty() || this->isZero())
		return poly;
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
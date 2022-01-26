#include <fstream>
#include "circuit.h"

///
/// As strings que definem os tipos de porta
///

// Funcao auxiliar que testa se uma string com nome de porta eh valida
// Caso necessario, converte os caracteres da string para maiusculas
bool validType(std::string &Tipo)
{
	if (Tipo.size() != 2)
		return false;
	Tipo.at(0) = toupper(Tipo.at(0));
	Tipo.at(1) = toupper(Tipo.at(1));
	if (Tipo == "NT" || Tipo == "AN" || Tipo == "NA" ||
		Tipo == "OR" || Tipo == "NO" ||
		Tipo == "XO" || Tipo == "NX")
		return true;
	return false;
}

// Funcao auxiliar que retorna um ponteiro que aponta para uma porta alocada dinamicamente
// O tipo da porta alocada depende do parametro string de entrada (AN, OR, etc.)
// Caso o tipo nao seja nenhum dos validos, retorna nullptr
// Pode ser utilizadas nas funcoes: Circuit::setPort, Circuit::digitar e Circuit::ler
ptr_Port allocPort(std::string &Tipo)
{
	if (!validType(Tipo))
		return nullptr;

	if (Tipo == "NT")
		return new Port_NOT;
	if (Tipo == "AN")
		return new Port_AND;
	if (Tipo == "NA")
		return new Port_NAND;
	if (Tipo == "OR")
		return new Port_OR;
	if (Tipo == "NO")
		return new Port_NOR;
	if (Tipo == "XO")
		return new Port_XOR;
	if (Tipo == "NX")
		return new Port_NXOR;

	// Nunca deve chegar aqui...
	return nullptr;
}

///
/// CLASSE CIRCUIT
///

/// ***********************
/// Inicializacao e finalizacao
/// ***********************

Circuit::Circuit()
{
    resize(0, 0, 0);
}

Circuit::Circuit(const Circuit &C)
{
	*this = C;
}

Circuit::~Circuit()
{
	clear();
}

void Circuit::clear()
{
	Nin = -1;

	id_out.erase(id_out.begin(), id_out.end());
	id_out.clear();

	out_circ.erase(out_circ.begin(), out_circ.end());
	out_circ.clear();

    ports.erase(ports.begin(), ports.end());
    ports.clear();
}

void Circuit::resize(unsigned NI, unsigned NO, unsigned NP)
{
	if (NI <= 0 || NO <= 0 || NP <= 0)
	{
		std::cerr << "ERRO dos valores de ENTRADA, SAIDAS e PORTAS\n";
		return;
	}
	else
	{
		clear();

		id_out.resize(NO);

		Nin = NI;

		out_circ.resize(NO);
		ports.resize(NP);

		for (auto &id : id_out)
		{
			id = 0;
		}

		for (auto &out : out_circ)
		{
			out = bool3S::UNDEF;
		}

		for (auto &p : ports)
		{
            p = nullptr;
		}
	}
}

void Circuit::operator=(const Circuit &C)
{
	Nin = C.Nin;
	id_out = C.id_out;
	out_circ = C.out_circ;
}

/// ***********************
/// Funcoes de testagem
/// ***********************

// Retorna true se IdInput eh uma id de entrada do circuito valida (entre -1 e -NInput)
bool Circuit::validIdInput(int IdInput) const
{
	return (IdInput <= -1 && IdInput >= -int(getNumInputs()));
}

// Retorna true se IdOutput eh uma id de saida do circuito valida (entre 1 e NOutput)
bool Circuit::validIdOutput(int IdOutput) const
{
	return (IdOutput >= 1 && IdOutput <= int(getNumOutputs()));
}

// Retorna true se IdPort eh uma id de porta do circuito valida (entre 1 e NPort)
bool Circuit::validIdPort(int IdPort) const
{
	return (IdPort >= 1 && IdPort <= int(getNumPorts()));
}

// Retorna true se IdOrig eh uma id valida para a origem do sinal de uma entrada de porta ou
// para a origem de uma saida do circuito (podem vir de uma entrada do circuito ou de uma porta)
// validIdOrig == validIdInput OR validIdPort
bool Circuit::validIdOrig(int IdOrig) const
{
	return validIdInput(IdOrig) || validIdPort(IdOrig);
}

// Retorna true se IdPort eh uma id de porta valida (validIdPort) e
// a porta estah definida (estah alocada, ou seja, != nullptr)
bool Circuit::definedPort(int IdPort) const
{
	if (!validIdPort(IdPort))
		return false;
	if (ports.at(IdPort - 1) == nullptr)
		return false;
	return true;
}

// Retorna true se IdPort eh uma porta existente (definedPort) e
// todas as entradas da porta com Id de origem valida (usa getId_inPort e validIdOrig)
bool Circuit::validPort(int IdPort) const
{
	if (!definedPort(IdPort))
		return false;
	for (unsigned j = 0; j < getNumInputsPort(IdPort); j++)
	{
		if (!validIdOrig(getId_inPort(IdPort, j)))
			return false;
	}
	return true;
}

// Retorna true se o circuito eh valido (estah com todos os dados corretos):
// - numero de entradas, saidas e portas valido (> 0)
// - todas as portas validas (usa validPort)
// - todas as saidas com Id de origem validas (usa getIdOutput e validIdOrig)
// Essa funcao deve ser usada antes de salvar ou simular um circuito
bool Circuit::valid() const
{
	if (getNumInputs() == 0)
		return false;
	if (getNumOutputs() == 0)
		return false;
	if (getNumPorts() == 0)
		return false;
	for (unsigned i = 0; i < getNumPorts(); i++)
	{
		if (!validPort(i + 1))
			return false;
	}
	for (unsigned i = 0; i < getNumOutputs(); i++)
	{
		if (!validIdOrig(getIdOutput(i + 1)))
			return false;
	}
	return true;
}

/// ***********************
/// Funcoes de consulta
/// ***********************

unsigned Circuit::getNumInputs() const
{
	return Nin;
}

unsigned Circuit::getNumOutputs() const
{
	return id_out.size();
}

unsigned Circuit::getNumPorts() const
{
	return ports.size();
}

int Circuit::getIdOutput(int IdOutput) const
{
	if (validIdOutput(IdOutput))
		return id_out[IdOutput - 1];

	else
		return 0;
}

bool3S Circuit::getOutput(int IdOutput) const
{
	if (validIdOutput(IdOutput))
		return out_circ[IdOutput - 1];

	else
		return bool3S::UNDEF;
}

std::string Circuit::getNamePort(int IdPort) const
{
	if (definedPort(IdPort))
		return ports[IdPort - 1]->getName();

	else
		return "??";
}

unsigned Circuit::getNumInputsPort(int IdPort) const
{
	if (definedPort(IdPort))
		return ports[IdPort - 1]->getNumInputs();

	else
		return 0;
}

int Circuit::getId_inPort(int IdPort, unsigned I) const
{
	if (definedPort(IdPort))
        return ports[IdPort - 1]->getId_in(I);

	else
		return 0;
}
/// ***********************
/// Funcoes de modificacao
/// ***********************

void Circuit::setIdOutput(int IdOut, int IdOrig)
{
	if (validIdOutput(IdOut) && validIdOrig(IdOrig))
		id_out[IdOut - 1] = IdOrig;

	else
		return;
}

void Circuit::setPort(int IdPort, std::string Tipo, unsigned NIn)
{
	if (validIdPort(IdPort) && NIn > 0)
	{
        if (validType(Tipo))
        {
            ports[IdPort-1] = allocPort(Tipo);
        }
        /*if (Tipo == "NT")
		{
			delete ports[IdPort - 1];
			ports[IdPort - 1] = new Port_NOT();
			ports[IdPort - 1]->setNumInputs(NIn);
		}

		else if (Tipo == "AN")
		{
			delete ports[IdPort - 1];
			ports[IdPort - 1] = new Port_AND();
			ports[IdPort - 1]->setNumInputs(NIn);
		}

		else if (Tipo == "NA")
		{
			delete ports[IdPort - 1];
			ports[IdPort - 1] = new Port_NAND();
			ports[IdPort - 1]->setNumInputs(NIn);
		}

		else if (Tipo == "OR")
		{
			delete ports[IdPort - 1];
			ports[IdPort - 1] = new Port_OR();
			ports[IdPort - 1]->setNumInputs(NIn);
		}

		else if (Tipo == "NO")
		{
			delete ports[IdPort - 1];
			ports[IdPort - 1] = new Port_NOR();
			ports[IdPort - 1]->setNumInputs(NIn);
		}

		else if (Tipo == "XO")
		{
			delete ports[IdPort - 1];
			ports[IdPort - 1] = new Port_XOR();
			ports[IdPort - 1]->setNumInputs(NIn);
		}

		else if (Tipo == "NX")
		{
			delete ports[IdPort - 1];
			ports[IdPort - 1] = new Port_NXOR();
			ports[IdPort - 1]->setNumInputs(NIn);
        }*/

		else
			return;
	}

	else
		return;
}

void Circuit::setId_inPort(int IdPort, unsigned I, int IdOrig) const
{
	if (definedPort(IdPort) && validIdOrig(IdOrig) && ports[IdPort - 1]->validIndex(I))
	{
		ports[IdPort - 1]->setId_in(I, IdOrig);
	}

	else
		return;
}

/// ***********************
/// E/S de dados
/// ***********************

void Circuit::digitar()
{

	std::cin.ignore();

	unsigned NIN, NOUT, NP;
	std::string tipo;

	do
	{

		std::cout << "Nums de Entradas: ";
		std::cin >> NIN;
		std::cin.ignore();

		std::cout << "Nums de Saidas: ";
		std::cin >> NOUT;
		std::cin.ignore();

		std::cout << "Nums de Portas: ";
		std::cin >> NP;
		std::cin.ignore();

	} while (NIN < 0 || NOUT < 0 || NP < 0);

	resize(NIN, NOUT, NP);

	bool repita = true;
	for (int i = 1; i <= getNumPorts(); ++i)
	{
		do
		{
			std::cout << "Digite o tipo (NT,AN,NA,OR,NO,XO,NX): ";
			std::cin >> tipo;
        } while (validType(tipo));

		std::cin.ignore();

        ports[i-1] = allocPort(tipo);

		do
		{
			ports[i - 1]->digitar();
		} while (!ports[i-1]->valid());
	}

	int id = 0;
	for (int i = 1; i <= NOUT; ++i)
	{
		do
		{
			std::cout << "Digite a ID (origem do sinal) da saida " << i << ": ";
			std::cin >> id;
			std::cin.ignore();
			id_out[i - 1] = id;

		} while (!validIdOrig(i));
	}
}

bool Circuit::ler(const std::string &arq)
{
	std::string palavra;
	std::string tipo;
	std::string ID;

	unsigned NIN;
	unsigned NOUT;
	unsigned NP;

	std::ifstream file(arq);

	if (!file.is_open())
		return false;

	file >> palavra;

	if (palavra == "CIRCUITO")
	{
		file >> palavra;
	}

	else if (palavra == "CIRCUITO:")
	{
		palavra.resize(palavra.size() - 1);
	}

	else if (palavra != "CIRCUITO")
	{
		std::cerr << "\n****Palavra \"CIRCUITO\" Nao encontrada****\n";
		clear();
		return false;
	}

	file >> NIN >> NOUT >> NP;

	if (NIN <= 0 || NOUT <= 0 || NP <= 0)
	{
		std::cerr << "\n****Valores \"Nin\" \"Nout\" e \"Nportas\" INVALIDOS****\n";
		clear();
		return false;
	}

	resize(NIN, NOUT, NP);

	file >> palavra;

	if (palavra == "PORTAS")
	{
		file >> palavra;
	}

	else if (palavra == "PORTAS:")
	{
		palavra.resize(palavra.size() - 1);
	}

	else if (palavra != "PORTAS")
	{
		std::cerr << "\n****Palavra \"PORTAS\" Nao encontrada****\n";
		clear();
		return false;
	}

	for (int i = 1; i <= NP; ++i)
	{
		file >> ID;

		file >> tipo;

		if (tipo == ")")
			file >> tipo;

        if (validType(tipo))
        {
            ports[i-1] = allocPort(tipo);
        }

		else
		{
			std::cerr << "\n****Tipo de Porta INVALIDA****\n";
			clear();
			return false;
        }

		ports[i - 1]->ler(file);

        if (!validPort(i))
        {
            std::cerr << "PORTA INVALIDA\n";
            return false;
        }
	}

	file >> palavra;

	if (palavra == "SAIDAS")
	{
		file >> palavra;
	}

	else if (palavra == "SAIDAS:")
	{
		palavra.resize(palavra.size() - 1);
	}

	else if (palavra != "SAIDAS")
	{
		std::cerr << "\n****Palavra \"SAIDAS\" Nao encontrada****\n";
		clear();
		return false;
	}

	int flag;
	for (int i = 1; i <= NOUT; ++i)
	{
		file >> ID;

		flag = std::atoi(&ID[0]);

		if (i == 1 && flag > i)
		{
			std::cerr << "\n****Saidas Fora de ordem****\n";
			clear();
			return false;
		}

		file >> ID;
		if (ID == ")")
			file >> ID;

		id_out[i - 1] = std::atoi(&ID[0]);

		if ((id_out[i - 1] > NP))
		{
			std::cerr << "\n****ID_OUT maior do que a quantidade de PORTAS****\n";
			clear();
			return false;
		}

		else if (id_out[i - 1] == 0)
		{
			std::cerr << "\n****ID_OUT igual a 0 (zero)****\n";
			clear();
			return false;
		}

		else if (i >= 2 && id_out[i - 2] == id_out[i - 1])
		{
			std::cerr << "\n****FALTANDO saidas****\n";
			clear();
			return false;
		}
	}

	file.close();

    if (!valid())
    {
        std::cerr << "CIRCUITO criado INVALIDO\n";
        return false;
    }

	return true;
}

std::ostream &Circuit::imprimir(std::ostream &O) const
{
	O << "CIRCUITO: ";
	O << Nin << ' ' << out_circ.size() << ' ' << ports.size() << '\n';
	O << "PORTAS: " << '\n';

	for (unsigned i = 1; i <= ports.size(); ++i)
	{
		O << i << ") ";
		ports[i - 1]->imprimir(O);
		O << '\n';

		if (i > ports.size())
			return O;
	}

	O << "SAIDAS: " << '\n';

	for (unsigned i = 1; i <= id_out.size(); ++i)
	{
		O << i << ") " << id_out[i - 1] << '\n';
	}

	return O;
}

bool Circuit::salvar(const std::string &arq) const
{
	std::ofstream file(arq, std::ios::out);

	if (!file.is_open())
		return false;

	imprimir(file);

	return true;
}

/// ***********************
/// SIMULACAO (funcao principal do circuito)
/// ***********************

bool Circuit::simular(const std::vector<bool3S> &in_circ)
{
	vector<int> id_in;

	bool tudo_def;
	bool alguma_def;
    vector<bool3S> in_port(10);

	int id;

	for (unsigned i = 1; i <= ports.size(); ++i)
		ports[i - 1]->setOutput(bool3S::UNDEF);

	do
	{
		tudo_def = true;
		alguma_def = false;

		for (unsigned i = 1; i <= ports.size(); ++i)
		{
            in_port.resize(ports[i-1]->getNumInputs());

			if (ports[i - 1]->getOutput() == bool3S::UNDEF)
			{
				for (int j = 0; j < ports[i - 1]->getNumInputs(); ++j)
				{
					id = ports[i - 1]->getId_in(j);

					if (id > 0)
					{
                        in_port[j] = ports[id-1]->getOutput();
                        //in_port.push_back(ports[id - 1]->getOutput());
					}
					else
					{
                        in_port[j] = in_circ[-id-1];
                        //in_port.push_back(in_circ[-id - 1]);
					}
				}
				ports[i - 1]->simular(in_port);

				if (ports[i - 1]->getOutput() == bool3S::UNDEF)
					tudo_def = false;

				else
					alguma_def = true;
			}
		}
	} while (!tudo_def && alguma_def);

	for (int j = 0; j < out_circ.size(); ++j)
	{
		id = id_out[j];

		if (id > 0)
		{
			out_circ[j] = ports[id - 1]->getOutput();
		}
		else
		{
			out_circ[j] = in_circ[-id - 1];
		}
	}

	return true;
}

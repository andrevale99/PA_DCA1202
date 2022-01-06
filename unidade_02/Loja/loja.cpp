#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>
#include "loja.h"

using namespace std;

/// Construtor
Produto::Produto(const string& N, unsigned P): nome(N), preco(P) {}

/// Funcoes de consulta
string Produto::getNome() const
{
  return nome;
}

double Produto::getPreco() const
{
  return preco/100.0;
}

/// Funcoes de entrada/saida
void Produto::digitar()
{
  // Leh o nomr do produto
  do
  {
    cout << "Nome: ";
    getline(cin,nome);
  }
  while (nome=="");
  // Leh o preco do produto
  double Pr;
  do
  {
    cout << "Preco (##.##): ";
    cin >> Pr;
  }
  while (Pr <= 0.0);
  cin.ignore();
  preco = (unsigned)round(100.0*Pr);
}

void Produto::imprimir() const
{
  salvar(cout);
}

bool Produto::ler(istream& I)
{
  // Formato da linha no arquivo-> "STRING_AUTOR";$FLOAT_PRECO
  double Pr;

  // Ignora ateh aspa inicial
  I.ignore(numeric_limits<streamsize>::max(),'"');
  // Leh ateh a aspa final do nome do produto
  getline(I,nome,'"');
  // Ignora o ";" e o "$"
  I.ignore(numeric_limits<streamsize>::max(),'$');
  // Leh o preco como numero real
  I >> Pr;
  // Converte o preco para centavos (inteiro)
  preco = (unsigned)round(100.0*Pr);
  // Testa a leitura
  if (nome=="" || preco==0)
  {
    nome = "";
    preco = 0;
    return false;
  }
  return true;
}

void Produto::salvar(ostream& O) const
{
  // Formato da linha-> "STRING_NOME";$FLOAT_PRECO;
  O << '"' << nome << '"' << ";$" << fixed << setprecision(2) << double(preco)/100.0;
}

istream& operator>>(istream& I, Produto& P)
{
  P.digitar();
  return I;
}

ostream& operator<<(ostream& O, const Produto& P)
{
  P.salvar(O);
  return O;
}

/// CLASSE LIVRO

Livro::Livro(const string& N, unsigned P, const string& A): Produto(N,P), autor(A) {}

/// Funcoes de consulta
string Livro::getAutor() const
{
  return autor;
}

/// Funcoes de entrada/saida
void Livro::digitar()
{
  Produto::digitar();
  do
  {
    cout << "Autor: ";
    getline(cin,autor);
  }
  while (autor=="");
}

void Livro::imprimir() const
{
  return salvar(cout);
}

bool Livro::ler(istream& I)
{
  // Formato da linha no arquivo->  L: <Conteudo do Produto>;"STRING_AUTOR"

  // Leh o caractere inicial
  char opcao;
  I >> opcao;
  if (opcao!='L')
  {
    *this = Livro();
    return false;
  }
  // Ignora o ":" apos a letra inicial
  I.ignore(numeric_limits<streamsize>::max(),':');
  // Comeca da aspa da string do nome do Produto
  if (!Produto::ler(I))
  {
    *this = Livro();
    return false;
  }
  // Ignora o ";" e a aspa inicial do nome do autor
  I.ignore(numeric_limits<streamsize>::max(),'"');
  // Leh o nome do autor
  getline(I,autor,'"');
  // Ignora o "\n" no fim da linha
  I.ignore();
  if (autor == "")
  {
    *this = Livro();
    return false;
  }
  return true;
}

void Livro::salvar(ostream& O) const
{
  // Formato da linha-> L: <Conteudo do Produto>;"STRING_AUTOR"
  O << "L: ";
  Produto::salvar(O);
  O << ';' << '"' << autor << '"';
}

istream& operator>>(istream& I, Livro& L)
{
  L.digitar();
  return I;
}

ostream& operator<<(ostream& O, const Livro& L)
{
  L.salvar(O);
  return O;
}

/// CLASSE CD

CD::CD(const string& N, unsigned P, unsigned NF): Produto(N,P), nfaixas(NF) {}

/// Funcoes de consulta
unsigned CD::getNumFaixas() const
{
  return nfaixas;
}

/// Funcoes de entrada/saida
void CD::digitar()
{
  Produto::digitar();
  do
  {
    cout << "Numero de faixas: ";
    cin >> nfaixas;
  }
  while (nfaixas == 0);
  cin.ignore();
}

void CD::imprimir() const
{
  salvar(cout);
}

bool CD::ler(istream& I)
{
  // Formato da linha no arquivo->  C: <Conteudo do Produto>;UNSIGNED_NFAIXAS

  // Leh o caractere inicial
  char opcao;
  I >> opcao;
  if (opcao!='C')
  {
    *this = CD();
    return false;
  }
  // Ignora o ":" apos a letra inicial
  I.ignore(numeric_limits<streamsize>::max(),':');
  // Comeca da aspa da string do nome do Produto
  if (!Produto::ler(I))
  {
    *this = CD();
    return false;
  }
  // Ignora o ";"
  I.ignore(numeric_limits<streamsize>::max(),';');
  // Leh o numero de faixa
  I >> nfaixas;
  // Ignora o "\n" no fim da linha
  I.ignore();
  if (nfaixas == 0)
  {
    *this = CD();
    return false;
  }
  return true;
}

void CD::salvar(ostream& O) const
{
  // Formato da linha-> C: <Conteudo do Produto>;UNSIGNED_NFAIXAS
  O << "C: ";
  Produto::salvar(O);
  O << ';' << nfaixas;
}

istream& operator>>(istream& I, CD& C)
{
  C.digitar();
  return I;
}

ostream& operator<<(ostream& O, const CD& C)
{
  C.salvar(O);
  return O;
}

/// CLASSE DVD

DVD::DVD(const string& N, unsigned P, unsigned D): Produto(N,P), duracao(D) {}

/// Funcoes de consulta
unsigned DVD::getDuracao() const
{
  return duracao;
}

/// Funcoes de entrada/saida
void DVD::digitar()
{
  Produto::digitar();
  do
  {
    cout << "Duracao: ";
    cin >> duracao;
  }
  while (duracao == 0);
  cin.ignore();
}

void DVD::imprimir() const
{
  salvar(cout);
}

bool DVD::ler(istream& I)
{
  // Formato da linha no arquivo-> D: <Conteudo do Produto>;UNSIGNED_DURACAO

  // Leh o caractere inicial
  char opcao;
  I >> opcao;
  if (opcao!='D')
  {
    *this = DVD();
    return false;
  }
  // Ignora o ":" apos a letra inicial
  I.ignore(numeric_limits<streamsize>::max(),':');
  // Comeca da aspa da string do nome do Produto
  if (!Produto::ler(I))
  {
    *this = DVD();
    return false;
  }
  // Ignora o ";"
  I.ignore(numeric_limits<streamsize>::max(),';');
  // Leh a duracao
  I >> duracao;
  // Ignora o "\n" no fim da linha
  I.ignore();
  if (duracao == 0)
  {
    *this = DVD();
    return false;
  }
  return true;
}

void DVD::salvar(ostream& O) const
{
  // Formato da linha-> D: <Conteudo do Produto>;UNSIGNED_DURACAO
  O << "D: ";
  Produto::salvar(O);
  O << ';' << duracao;
}

istream& operator>>(istream& I, DVD& D)
{
  D.digitar();
  return I;
}

ostream& operator<<(ostream& O, const DVD& D)
{
  D.salvar(O);
  return O;
}

/// CLASSE LOJA

/// Metodos basicos
Loja::Loja(): LL(), LC(), LD() {}

/// Funcoes de consulta
unsigned Loja::getNumLivro() const
{
  return LL.size();
}

unsigned Loja::getNumCD() const
{
  return LC.size();
}

unsigned Loja::getNumDVD() const
{
  return LD.size();
}

Livro Loja::getLivro(unsigned id) const
{
  if (LL.empty() || id>=LL.size()) return Livro();
  return LL[id];
}

CD Loja::getCD(unsigned id) const
{
  if (LC.empty() || id>=LC.size()) return CD();
  return LC[id];
}

DVD Loja::getDVD(unsigned id) const
{
  if (LD.empty() || id>=LD.size()) return DVD();
  return LD[id];
}

/// Funcoes de manipulacao (inclusao/exclusao) de itens do estoque

void Loja::incluirLivro(const Livro& X)
{
  LL.push_back(X);
}

bool Loja::excluirLivro(unsigned id)
{
  if (LL.empty() || id>=LL.size()) return false;
  LL.erase(LL.begin()+id);
  return true;
}

void Loja::incluirCD(const CD& X)
{
  LC.push_back(X);
}

bool Loja::excluirCD(unsigned id)
{
  if (LC.empty() || id>=LC.size()) return false;
  LC.erase(LC.begin()+id);
  return true;
}

void Loja::incluirDVD(const DVD& X)
{
  LD.push_back(X);
}

bool Loja::excluirDVD(unsigned id)
{
  if (LD.empty() || id>=LD.size()) return false;
  LD.erase(LD.begin()+id);
  return true;
}

/// Funcoes de entrada/saida

void Loja::imprimir() const
{
  cout << ">> LIVROS:" << endl;
  for (unsigned i=0; i<LL.size(); i++) cout << i << ") " << LL[i] << endl;

  cout << ">> CDS:" << endl;
  for (unsigned i=0; i<LC.size(); i++) cout << i << ") " << LC[i] << endl;

  cout << ">> DVDS:" << endl;
  for (unsigned i=0; i<LD.size(); i++) cout << i << ") " << LD[i] << endl;
}

bool Loja::ler(const string& arq)
{
  ifstream I(arq.c_str());
  if (!I.is_open()) return false;

  string pS;
  unsigned NN;
  Livro L;
  CD C;
  DVD D;

  // Leh os livros
  I >> pS >> NN;
  I.ignore();
  if (pS!="LISTALIVRO")
  {
    I.close();
    return false;
  }
  LL.clear();
  for (unsigned i=0; i<NN; i++)
  {
    if (!L.ler(I))
    {
      LL.clear();
      I.close();
      return false;
    }
    LL.push_back(L);
  }

  // Leh os CDs
  I >> pS >> NN;
  I.ignore();
  if (pS!="LISTACD")
  {
    I.close();
    return false;
  }
  LC.clear();
  for (unsigned i=0; i<NN; i++)
  {
    if (!C.ler(I))
    {
      LC.clear();
      I.close();
      return false;
    }
    LC.push_back(C);
  }

  // Leh os DVDs
  I >> pS >> NN;
  I.ignore();
  if (pS!="LISTADVD")
  {
    I.close();
    return false;
  }
  LD.clear();
  for (unsigned i=0; i<NN; i++)
  {
    if (!D.ler(I))
    {
      LD.clear();
      I.close();
      return false;
    }
    LD.push_back(D);
  }

  I.close();
  return true;
}

bool Loja::salvar(const string& arq) const
{
  ofstream O(arq.c_str());
  if (!O.is_open()) return false;

  O << "LISTALIVRO " << LL.size() << endl;
  for (unsigned i=0; i<LL.size(); i++) O << LL[i] << endl;

  O << "LISTACD " << LC.size() << endl;
  for (unsigned i=0; i<LC.size(); i++) O << LC[i] << endl;

  O << "LISTADVD " << LD.size() << endl;
  for (unsigned i=0; i<LD.size(); i++) O << LD[i] << endl;

  O.close();
  return true;
}

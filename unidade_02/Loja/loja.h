#ifndef LOJA_H
#define LOJA_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Produto
{
private:
  string nome;
  unsigned preco;
public:
  /// Construtor
  Produto(const string& N="", unsigned P=0);

  /// Funcoes de consulta
  string getNome() const;
  double getPreco() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(istream& I);
  void salvar(ostream& O) const;
};

istream& operator>>(istream& I, Produto& P);
ostream& operator<<(ostream& O, const Produto& P);

class Livro: public Produto
{
private:
  string autor;
public:
  /// Construtor
  Livro(const string& N="", unsigned P=0, const string& A="");

  /// Funcoes de consulta
  string getAutor() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(istream& I);
  void salvar(ostream& O) const;
};

istream& operator>>(istream& I, Livro& L);
ostream& operator<<(ostream& O, const Livro& L);

class CD: public Produto
{
private:
  unsigned nfaixas;
public:
  /// Construtor
  CD(const string& N="", unsigned P=0, unsigned NF=0);

  /// Funcoes de consulta
  unsigned getNumFaixas() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(istream& I);
  void salvar(ostream& O) const;
};

istream& operator>>(istream& I, CD& C);
ostream& operator<<(ostream& O, const CD& C);

class DVD: public Produto
{
private:
  unsigned duracao;
public:
  /// Construtor
  DVD(const string& N="", unsigned P=0, unsigned D=0);

  /// Funcoes de consulta
  unsigned getDuracao() const;

  /// Funcoes de entrada/saida
  void digitar();
  void imprimir() const;

  bool ler(istream& I);
  void salvar(ostream& O) const;
};

istream& operator>>(istream& I, DVD& D);
ostream& operator<<(ostream& O, const DVD& D);

class Loja
{
private:
  vector<Livro> LL;
  vector<CD>    LC;
  vector<DVD>   LD;
public:
  /// Construtor
  Loja();

  /// Funcoes de consulta
  unsigned getNumLivro() const;
  unsigned getNumCD() const;
  unsigned getNumDVD() const;
  Livro getLivro(unsigned id) const;
  CD getCD(unsigned id) const;
  DVD getDVD(unsigned id) const;

  /// Funcoes de manipulacao (inclusao/exclusao) de itens do estoque
  void incluirLivro(const Livro& X);
  bool excluirLivro(unsigned id);
  void incluirCD(const CD& X);
  bool excluirCD(unsigned id);
  void incluirDVD(const DVD& X);
  bool excluirDVD(unsigned id);

  /// Funcoes de entrada/saida
  bool ler(const string& arq);
  bool salvar(const string& arq) const;
  void imprimir() const;
};

#endif // LOJA_H

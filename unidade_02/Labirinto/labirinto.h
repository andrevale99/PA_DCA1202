#ifndef _LABIRINTO_H_
#define _LABIRINTO_H_

#include <vector>
#include "coord.h"

using namespace std;

#define LARGURA_MIN_MAPA 10
#define LARGURA_MED_MAPA 25
#define LARGURA_MAX_MAPA 50

#define ALTURA_MIN_MAPA 5
#define ALTURA_MED_MAPA 10
#define ALTURA_MAX_MAPA 20

#define PERC_MIN_OBST 0.05
#define PERC_MAX_OBST 0.50

/// Os possiveis estados de uma celula do mapa
enum class EstadoCel
{
  LIVRE,
  OBSTACULO,
  ORIGEM,
  DESTINO,
  CAMINHO
};

// Funcao para converter um estado de celula em uma string que o represente
string estadoCel2string(EstadoCel E);

/// A classe que armazena o mapa e os metodos de resolucao de labirintos
class Labirinto
{
private:
  /// Dimensoes do mapa
  /// NL = altura (numero de linhas)
  /// NC = largura (numero de colunas)
  unsigned int NLin, NCol;

  /// "Matriz" que contem os estados das casas do mapa.
  /// Na realidade, a "matriz" eh um vector de EstadoCel
  /// O acesso aos elementos da "matriz" se dah atraves dos metodos "set" e "at",
  /// que transformam os indices linha e coluna da matriz no indice do vetor:
  /// | 00 01 02 03 |
  /// | 10 11 12 13 |
  /// | 20 21 22 23 | -> 00 01 02 03 10 11 12 13 20 21 22 23
  vector<EstadoCel> mapa;

  /// A origem e o destino do caminho
  Coord orig, dest;

  /// Funcao set de alteracao de valor
  void set(unsigned i, unsigned j, EstadoCel valor);
  void set(const Coord& C, EstadoCel valor);

public:
  /// Cria um mapa vazio
  Labirinto();

  /// Cria um mapa com dimensoes dadas
  /// numL e numC sao as dimensoes do labirinto
  Labirinto(unsigned numL, unsigned numC);

  /// Cria um mapa com o conteudo do arquivo nome_arq
  /// Caso nao consiga ler do arquivo, cria mapa vazio
  Labirinto(const string& nome_arq);

  /// Destrutor (nao eh obrigatorio...)
  ~Labirinto();

  /// Torna o mapa vazio
  void clear();

  /// Limpa um eventual caminho anteriormente calculado
  void limpaCaminho();

  /// Funcoes de consulta
  unsigned getNumLin() const;
  unsigned getNumCol() const;
  Coord getOrig() const;
  Coord getDest() const;

  /// Funcao de consulta
  /// Retorna o estado da celula correspondente ao i-j-esimo elemento do mapa
  EstadoCel at(unsigned i, unsigned j) const;
  /// Retorna o estado da celula C
  EstadoCel at(const Coord& C) const;

  /// Operador() de consulta - usa o metodo "at"
  /// Retorna o estado da celula correspondente ao i-j-esimo elemento do mapa
  EstadoCel operator()(unsigned i, unsigned j) const;
  /// Retorna o estado da celula C
  EstadoCel operator()(const Coord& C) const;

  /// Testa se um mapa estah vazio
  bool empty() const;
  /// Testa se um mapa tem origem e destino definidos
  bool origDestDefinidos() const;

  /// Testa se uma celula eh valida dentro das dimensoes de um mapa
  bool coordValida(const Coord& C) const;
  /// Testa se uma celula estah livre (nao eh obstaculo) em um mapa
  bool celulaLivre(const Coord& C) const;
  /// Testa se um movimento Orig->Dest eh valido
  bool movimentoValido(const Coord& Orig, const Coord& Dest) const;

  /// Fixa a origem do caminho a ser encontrado
  bool setOrigem(const Coord& C);
  /// Fixa o destino do caminho a ser encontrado
  bool setDestino(const Coord& C);

  /// Imprime o mapa no console
  void imprimir() const;

  /// Leh um mapa do arquivo nome_arq
  /// Caso nao consiga ler do arquivo, cria mapa vazio
  /// Retorna true em caso de leitura bem sucedida
  bool ler(const string& nome_arq);
  /// Salva um mapa no arquivo nome_arq
  /// Retorna true em caso de escrita bem sucedida
  bool salvar(const string& nome_arq) const;

  /// Gera um novo mapa aleatorio
  /// numL e numC sao as dimensoes do labirinto
  /// perc_obst eh o percentual de casas ocupadas no mapa. Se <=0, assume um valor aleatorio
  /// entre PERC_MIN_OBST e PERC_MAX_OBST
  /// Se os parametros forem incorretos, gera um mapa vazio
  /// Retorna true em caso de geracao bem sucedida (parametros corretos)
  bool gerar(unsigned numL=ALTURA_MED_MAPA, unsigned numC=LARGURA_MED_MAPA,
             double perc_obst=0.0);

  /// Calcula o caminho entre a origem e o destino do labirinto usando o algoritmo A*
  ///
  /// Retorna o comprimento do caminho (<0 se nao existe)
  ///
  /// O parametro prof retorna o numero de nos (profundidade) do caminho encontrado
  /// ou <0 caso nao exista caminho.
  ///
  /// O parametro NA retorna o numero de nos em aberto ao termino do algoritmo A*
  /// O parametro NF retorna o numero de nos em fechado ao termino do algoritmo A*
  /// Mesmo quando nao existe caminho, esses parametros devem ser retornados
  double calculaCaminho(int& prof, int& NA, int& NF);
};

#endif // _LABIRINTO_H_

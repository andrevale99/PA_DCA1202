#ifndef _CIRCUIT_H_
#define _CIRCUIT_H_

/**
 * @Dupla
 * 		André Menezes de Freitas Vale
 * 		Lucas Emanuel Costa Moisés
*/

#include <iostream>
#include <string>
#include <vector>
#include "bool3S.h"
#include "port.h"

using std::vector;

/// ###########################################################################
/// ATENCAO PARA A CONVENCAO DOS NOMES E TIPOS PARA OS PARAMETROS DAS FUNCOES:
/// unsigned I: indice (de entrada de porta): de 0 a NInputs-1
/// int Id___: identificador (de entrada, saida ou porta):
///            de 1 a N ou de -1 a -N (nao 0)
/// - int IdInput: de -1 a -NumEntradasCircuito
/// - int IdOutput: de 1 a NumSaidasCircuito
/// - int IdPort: de 1 a NumPortasCircuito
/// - int IdOrig: de 1 a NumPortasCircuito ou de -1 a -NumEntradasCircuito
///       (id da origem de uma entrada de porta ou de uma saida do circuito)
/// ###########################################################################

///
/// CLASSE CIRCUIT
///

class Circuit {
private:
  /// ***********************
  /// Dados
  /// ***********************

  // Numero de entradas do circuito
  unsigned Nin;

  // Nao precisa manter variaveis para guardar o numero de saidas e ports.
  // Essas informacoes estao armazenadas nos tamanhos (size) dos vetores correspondentes:
  // id_out e ports, respectivamente
  // Os metodos de consulta getNumInputs, getNumOutputs e getNumPorts dao acesso a essas
  // informacoes de maneira eficiente

  // As saidas
  // As ids da origem dos sinais de saida do circuito
  std::vector<int> id_out;      // vetor a ser alocado com dimensao "Nout"
  // Os valores logicos das saidas do circuito
  std::vector<bool3S> out_circ; // vetor a ser alocado com dimensao "Nout"

  // As portas
  std::vector<ptr_Port> ports;  // vetor a ser alocado com dimensao "Nports"

public:

  /// ***********************
  /// Inicializacao e finalizacao
  /// ***********************

  // As variaveis do tipo Circuit sao sempre criadas sem nenhum dado
  // A definicao do numero de entradas, saidas e ports eh feita ao ler do teclado ou arquivo
  // ou ao executar o metodo resize
  Circuit();

  // Construtor por copia
  // Nin e os vetores id_out e out_circ serao copias dos equivalentes no Circuit C
  // O vetor ports terah a mesma dimensao do equivalente no Circuit C
  // Serah necessario utilizar a funcao virtual clone para criar copias das portas
  Circuit(const Circuit& C);
  // Destrutor: apenas chama a funcao clear()
  ~Circuit();

  // Limpa todo o conteudo do circuito. Faz Nin <- 0 e
  // utiliza o metodo STL clear para limpar os vetores id_out, out_circ e ports
  // ATENCAO: antes de dar um clear no vetor ports, tem que liberar (delete) as areas
  // de memoria para as quais cada ponteiro desse vetor aponta.
  void clear();

  // Operador de atribuicao 
  // Atribui (faz copia) de Nin e dos vetores id_out e out_circ
  // ATENCAO: antes de alterar o vetor ports, tem que liberar (delete) as areas
  // de memoria para as quais cada ponteiro desse vetor aponta.
  // O vetor ports terah a mesma dimensao do equivalente no Circuit C
  // Serah necessario utilizar a funcao virtual clone para criar copias das portas
  void operator=(const Circuit& C);

  // Redimensiona o circuito para passar a ter NI entradas, NO saidas e NP ports
  // Inicialmente checa os parametros. Caso sejam validos,
  // depois de limpar conteudo anterior (clear), altera Nin; os vetores tem as dimensoes
  // alteradas (resize) e sao inicializados com valores iniciais neutros ou invalidos:
  // id_out[i] <- 0
  // out_circ[i] <- UNDEF
  // ports[i] <- nullptr
  void resize(unsigned NI, unsigned NO, unsigned NP);

  /// ***********************
  /// Funcoes de testagem
  /// ***********************

  // Retorna true se IdInput eh uma id de entrada do circuito valida (entre -1 e -NInput)
  bool validIdInput(int IdInput) const;

  // Retorna true se IdOutput eh uma id de saida do circuito valida (entre 1 e NOutput)
  bool validIdOutput(int IdOutput) const;

  // Retorna true se IdPort eh uma id de porta do circuito valida (entre 1 e NPort)
  bool validIdPort(int IdPort) const;

  // Retorna true se IdOrig eh uma id valida para a origem do sinal de uma entrada de porta ou
  // para a origem de uma saida do circuito (podem vir de uma entrada do circuito ou de uma porta)
  // validIdOrig == validIdInput OR validIdPort
  bool validIdOrig(int IdOrig) const;

  // Retorna true se IdPort eh uma id de porta valida (validIdPort) e
  // a porta estah definida (estah alocada, ou seja, != nullptr)
  bool definedPort(int IdPort) const;

  // Retorna true se IdPort eh uma porta existente (definedPort) e
  // todas as entradas da porta com Id de origem valida (usa getId_inPort e validIdOrig)
  bool validPort(int IdPort) const;

  // Retorna true se o circuito eh valido (estah com todos os dados corretos):
  // - numero de entradas, saidas e portas valido (> 0)
  // - todas as portas validas (usa validPort)
  // - todas as saidas com Id de origem validas (usa getIdOutput e validIdOrig)
  // Essa funcao deve ser usada antes de salvar ou simular um circuito
  bool valid() const;

  /// ***********************
  /// Funcoes de consulta
  /// ***********************

  // Caracteristicas do circuito

  // Retorna o tamanho (size) dos vetores correspondentes:
  // inputs, id_out e ports, respectivamente
  unsigned getNumInputs() const;
  unsigned getNumOutputs() const;
  unsigned getNumPorts() const;

  // Caracteristicas das saidas do circuito

  // Retorna a origem (a id) do sinal de saida cuja id eh IdOutput
  // Depois de testar o parametro (validIdOutput), retorna id_out[IdOutput-1]
  // ou 0 se parametro invalido
  int getIdOutput(int IdOutput) const;

  // Retorna o valor logico atual da saida cuja id eh IdOutput
  // Depois de testar o parametro (validIdOutput), retorna out_circ[IdOutput-1]
  // ou bool3S::UNDEF se parametro invalido
  bool3S getOutput(int IdOutput) const;

  // Caracteristicas das portas

  // Retorna o nome da porta: AN, NX, etc
  // Depois de testar se a porta existe (definedPort),
  // retorna ports[IdPort-1]->getName()
  // ou "??" se parametro invalido
  std::string getNamePort(int IdPort) const;

  // Retorna o numero de entradas da porta
  // Depois de testar se a porta existe (definedPort),
  // retorna ports[IdPort-1]->getNumInputs()
  // ou 0 se parametro invalido
  unsigned getNumInputsPort(int IdPort) const;

  // Retorna a origem (a id) da I-esima entrada da porta cuja id eh IdPort
  // Depois de testar se a porta existe (definedPort) e o indice da entrada I,
  // retorna ports[IdPort-1]->getId_in(I)
  // ou 0 se parametro invalido
  int getId_inPort(int IdPort, unsigned I) const;

  /// ***********************
  /// Funcoes de modificacao
  /// ***********************

  // Caracteristicas das saidas

  // Altera a origem da saida de id "IdOut", que passa a ser "IdOrig"
  // Depois de testar os parametros (validIdOutput,validIdOrig),
  // faz: id_out[IdOut-1] <- IdOrig
  void setIdOutput(int IdOut, int IdOrig);

  // Caracteristicas das ports

  // A porta cuja id eh IdPort passa a ser do tipo Tipo (NT, AN, etc.), com NIn entradas
  // Depois de varios testes (Id, tipo, num de entradas), faz:
  // 1) Libera a antiga area de memoria: delete ports[IdPort-1]
  // 2) Cria a nova porta: ports[IdPort-1] <- new ... (de acordo com tipo)
  // 3) Fixa o numero de entrada: ports[IdPort-1]->setNumInputs(NIn)
  void setPort(int IdPort, std::string Tipo, unsigned NIn);

  // Altera a origem da I-esima entrada da porta cuja id eh IdPort, que passa a ser "IdOrig"
  // Depois de VARIOS testes (definedPort, validIndex, validIdOrig)
  // faz: ports[IdPort-1]->setId_in(I,Idorig)
  void setId_inPort(int IdPort, unsigned I, int IdOrig) const;

  /// ***********************
  /// E/S de dados
  /// ***********************

  // Entrada dos dados de um circuito via teclado
  // O usuario digita o numero de entradas, saidas e portas
  // apos o que, se os valores estiverem corretos (>0), redimensiona o circuito
  // Em seguida, para cada porta o usuario digita o tipo (NT,AN,NA,OR,NO,XO,NX) que eh conferido
  // Apos criada dinamicamente (new) a porta do tipo correto, chama a
  // funcao digitar na porta recem-criada. A porta digitada eh conferida (validPort).
  // Em seguida, o usuario digita as ids de todas as saidas, que sao conferidas (validIdOrig).
  // Se o usuario digitar um dado invalido, o metodo deve pedir que ele digite novamente
  // Deve utilizar o metodo digitar da classe Port
  void digitar();

  // Entrada dos dados de um circuito via arquivo
  // Leh do arquivo o cabecalho com o numero de entradas, saidas e portas
  // apos o que, se os valores estiverem corretos (>0), redimensiona o circuito
  // Em seguida, para cada porta leh e confere a id e o tipo (NT,AN,NA,OR,NO,XO,NX)
  // Apos criada dinamicamente (new) a porta do tipo correto, chama a
  // funcao ler na porta recem-criada. A porta lida eh conferida (validPort).
  // Em seguida, leh as ids de todas as saidas, que sao conferidas (validIdOrig).
  // Retorna true se deu tudo OK; false se deu erro.
  // Deve utilizar o metodo ler da classe Port
  bool ler(const std::string& arq);

  // Saida dos dados de um circuito (em tela ou arquivo, a mesma funcao serve para os dois)
  // Imprime os cabecalhos e os dados do circuito, caso o circuito seja valido
  // Deve utilizar os metodos de impressao da classe Port
  std::ostream& imprimir(std::ostream& O=std::cout) const;

  // Salvar circuito em arquivo, caso o circuito seja valido
  // Abre a stream, chama o metodo imprimir e depois fecha a stream
  // Retorna true se deu tudo OK; false se deu erro
  bool salvar(const std::string& arq) const;

  /// ***********************
  /// SIMULACAO (funcao principal do circuito)
  /// ***********************

  // Calcula a saida das portas do circuito para os valores de entrada
  // passados como parametro, caso o circuito e a dimensao da entrada sejam
  // validos (caso contrario retorna false)
  // A entrada eh um vetor de bool3S, com dimensao igual ao numero de entradas
  // do circuito.
  // Depois de simular todas as portas do circuito, calcula as saidas do
  // circuito (out_circ <- ...)
  // Retorna true se a simulacao foi OK; false caso deh erro
  bool simular(const std::vector<bool3S>& in_circ);

};

// Operador de impressao da classe Circuit
// Utiliza o metodo imprimir
inline std::ostream& operator<<(std::ostream& O, const Circuit& C) {return C.imprimir(O);}

#endif // _CIRCUIT_H_

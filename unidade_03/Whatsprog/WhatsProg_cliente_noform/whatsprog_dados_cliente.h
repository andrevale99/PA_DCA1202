#ifndef WHATSPROG_DADOS_CLIENTE_H
#define WHATSPROG_DADOS_CLIENTE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <stdint.h>
#include <mutex>
#include "whatsprog_dados.h"
#include "mysocket.h"

using namespace std;

// Classe utilizada para armazenar as conversas no cliente
class Conversa
{
private:
  /// Correspondente
  /// Deve ser o remetente ou destinatario de todas as mensagens da conversa.
  string correspondente;

  /// Todas as mensagens da conversa
  vector<Mensagem> LM;

private:
  /// Funcoes de alteracao do correspondente
  /// Retorna false em caso de erro, true se OK
  bool setCorrespondente(const string &U);

  /// Apaga todas as msgs da conversa
  inline void clearMessages() {LM.clear();}

  /// Insere nova msg no final da conversa
  inline void pushMessage(const Mensagem &M) {LM.push_back(M);}

  /// Remove ultima msg (do final da conversa)
  inline void popMessage() {LM.pop_back();}

  /// Remove a i-esima msg
  void eraseMessage(unsigned i);

  /// Ler/salvar em arquivo
  /// Recebe como entrada uma fstream jah aberta
  /// A fstream deve ser aberta pela funcao de ler/salvar de um nivel superior
  /// (da classe DadosCliente)
  bool ler(ifstream& arq);
  bool salvar(ofstream& arq) const;

public:
  /// Construtor default
  inline Conversa(): correspondente(""), LM() {}

  /// Retorna o numero de mensagens da conversa
  inline unsigned size() const {return LM.size();}

  /// Funcao de consulta do correspondente
  inline const string &getCorrespondente() const {return correspondente;}

  /// Funcao de consulta da i-esima mensagem da conversa (nao alteravel)
  const Mensagem& getMensagem(unsigned i) const;

  /// Teste de igualdade entre uma conversa e uma string
  /// Utilizado pelos algoritmos STL: find, count, etc
  inline bool operator==(const string &C) const {return correspondente==C;}

  /// Retorna o numero de msgs entregues mas ainda nao lidas de uma conversa
  unsigned getNumMsgsEntregues() const;

  /// Procura na conversa uma mensagem que tenha a id passada
  /// como parametro e que tenha o usuario como remetente.
  /// Retorna o indice da mensagem dentro dessa conversa.
  /// Caso nao exista, retorna um valor negativo.
  int findMensagem(int32_t id) const;

  friend class WhatsProgDadosCliente;
};

typedef list<Conversa> ListConversa;
typedef ListConversa::iterator IterConversa;

class WhatsProgDadosCliente
{
private:
  /// O computador (IP) onde roda o programa servidor
  string servidor;

  /// O nome do usuario do cliente
  string meuUsuario;

  /// O iterador para a conversa atualmente sendo visualizada
  /// Se end(), nenhuma conversa estah selecionada
  IterConversa convAtual;

  /// A ultima ID utilizada em msg enviada por mim
  /// Maior que zero; se igual a 0, nenhuma msg foi enviada ainda
  uint32_t idMensagem;

  /// Todas as conversas
  ListConversa LC;

  /// A variavel de exclusao mutua, que protege os dados do cliente de
  /// serem alterados simultaneamente por duas threads
  mutex mtx;
public:
  /// Construtor default
  WhatsProgDadosCliente();

  /// Destrutor
  inline ~WhatsProgDadosCliente() {}

  /// Iteradores de inicio e fim (depois do ultimo) da lista de conversa
  IterConversa begin() {return LC.begin();}
  IterConversa end() {return LC.end();}

  /// Iterador para a ultima conversa da lista de conversas
  IterConversa last();

  /// Retorna o numero de conversas
  inline unsigned size() const {return LC.size();}

  /// Testa se o servidor estah com os dados de conexao preenchidos
  inline bool connected() const {return servidor!="" && testarNomeUsuario(meuUsuario);}

  /// Funcoes de consulta dos dados principais (servidor, login do usuario)
  inline const string &getServidor() const {return servidor;}
  inline const string &getMeuUsuario() const {return meuUsuario;}

  /// Define os parametros principais da conexao: servidor e usuario (login)
  /// Retorna false em caso de erro, true se OK
  bool setServidorUsuario(const string &IP, const string &MU);

  /// Limpa o servidor e o nome do usuario (deslogar)
  void unsetServidorUsuario();

  /// Gera (incrementa) uma id valida para a proxima msg
  inline uint32_t getNovaIdMensagem() {return ++idMensagem;}

  /// Funcao de consulta da conversa atual
  inline IterConversa getConversaAtual() const {return convAtual;}

  /// Funcao de alteracao da conversa atual
  /// Recebe como parametro o nome do correspondente da conversa
  /// Retorna false em caso de erro, true se OK
  bool setConversaAtual(const string &U);

  /// Funcao de alteracao da conversa atual
  /// Recebe como parametro o iterador para a conversa
  /// Retorna false em caso de erro, true se OK
  bool setConversaAtual(IterConversa it);

  /// Limpa conversa atual
  inline void unsetConversaAtual() {convAtual=LC.end();}

  /// Inclui (cria) uma nova conversa com o correspondente passado como parametro
  /// Se for a unica conversa, torna-a a conversa ativa
  /// Retorna false em caso de erro, true se OK
  bool insertConversa(const string &U);

  /// Retorna o iterador para a conversa cujo correspondente seja igual ao parametro
  /// Retorna end() caso nao exista a conversa
  IterConversa findConversa(const string &U);

  /// Move uma conversa para a primeira posicao da lista (begin)
  bool moveConversaToBegin(IterConversa it);

  /// Exclui a conversa
  /// Retorna false em caso de erro, true se OK
  bool eraseConversa(IterConversa it);

  /// Apaga todas as conversas
  inline void clearConversas() {LC.clear();}

  /// Procura em todas as conversa uma mensagem que tenha a id igual ao parametro
  /// e que tenha o correspondente da conversa como destinatario
  /// (ou seja, tenha o usuario como remetente)
  /// Retorna o iterator para a conversa onde estah a mensagem que tem essa id
  /// e o indice da mensagem dentro dessa conversa
  /// Caso nao exista em nenhuma conversa uma msg com essa id,
  /// retorna o iterator igual a end() e o indice negativo
  void findMensagem(int32_t id, IterConversa& it, int& ind_msg);

  /// Altera o status da i-esima mensagem da conversa it
  void setStatus(IterConversa it, unsigned i, MsgStatus S);

  /// Insere nova msg M no final da conversa it
  void pushMessage(IterConversa it, const Mensagem &M);

  /// Remove ultima msg da conversa it
  void popMessage(IterConversa it);

  /// Apaga todas as msgs da conversa
  void clearMessages(IterConversa it);

  /// Apaga a i-esima mensagem da conversa it
  void eraseMessage(IterConversa it, unsigned i);

  /// Retorna o nome do arquivo para ler/salvar os dados
  string getNomeArq() const;

  /// Ler/salvar em arquivo
  /// O nome do arquivo eh padronizado (ver getNomeArq)
  bool ler();
  bool salvar();
};

#endif // WHATSPROG_DADOS_CLIENTE_H

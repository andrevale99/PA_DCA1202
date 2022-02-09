#ifndef _DADOS_SERVIDOR_
#define _DADOS_SERVIDOR_

#include <string>
#include <list>
#include "mysocket.h"
#include "whatsprog_dados.h"

using namespace std;

/// Os usuarios cadastrados no servidor
class Usuario
{
private:
  /// Identificacao do usuario
  string login;
  string senha;

  /// Socket de comunicacao (idle se nao conectado)
  tcp_mysocket s;

  /// Ultima id de mensagem recebida
  int32_t last_id;

public:
  /// Construtor default
  Usuario();

  /// Funcao de consulta ao valor para login
  const string& getLogin() const;

  /// Funcao de fixacao de valor para login e senha
  /// Retorna true em caso de sucesso, false se erro
  bool setUsuario(const string& L, const string& S);

  /// Testa se a senha eh valida
  bool validarSenha(const string& S) const;

  /// Funcoes de acesso ao socket do usuario
  const tcp_mysocket& getSocket() const;

  /// Alteracao do socket de um usuario
  void swapSocket(tcp_mysocket& S);

  /// Consulta do estado do socket
  bool connected() const;
  bool closed() const;

  /// Funcoes de envio de dados via socket
  mysocket_status read_int(int32_t& num, long milisec=-1) const;
  mysocket_status write_int(int32_t num) const;
  mysocket_status read_string(string& msg, long milisec=-1) const;
  mysocket_status write_string(const string& msg) const;

  /// Fechamento do socket
  void close();

  /// Consulta da ultima ID do usuario
  int32_t getLastId() const;

  /// Alteracao da ultima ID do usuario
  bool setLastId(int32_t ID);

  /// Reinicializa a ID do usuario
  void resetId() {last_id=0;}

  /// Teste de igualdade com uma string (testa se a string eh igual ao login)
  bool operator==(const string& L) const {return login==L;}
};

class WhatsProgDadosServidor
{
private:
  /// O socket de conexoes
  tcp_mysocket_server c;

  /* **************************************************************************************
  ATENCAO: a parte a seguir da implementacao do servidor FAKE ***NAO*** pode ser adaptada
  para o servidor real.
  A fonte que contem informacao correta sobre como implementar o servidor eh
  a especificacao. Alguns trechos de outros programas, como o servidor do mensageiro,
  podem servir de ideia ou inspiracao, embora ***CERTAMENTE*** precisem ser adaptados.
  ************************************************************************************** */
  /// O usuario
  /// No programa real, seriam varios usuarios
  Usuario user;

  /// Mensagens pendentes
  /// Este servidor fake nao tem um buffer de verdade
  /// Apenas guarda, para simulacao, a ultima msg recebida
  /// (do usuario para userfake1 e para userfake2)
  Mensagem doUsuario[2];
  /// e a ultima msg enviada (gerada aleatoriamente)
  /// (de userfake1 e userfake2 para usuario)
  Mensagem paraUsuario[2];
  /* Fim da parte que ***NAO*** pode ser adaptada para o servidor real ***************** */

  ///
  /// FUNCOES AUXILIARES PARA EVITAR DUPLICACAO DE CODIGO
  ///

  /* **************************************************************************************
  ATENCAO: a parte a seguir da implementacao do servidor FAKE ***NAO*** pode ser adaptada
  para o servidor real.
  A fonte que contem informacao correta sobre como implementar o servidor eh
  a especificacao. Alguns trechos de outros programas, como o servidor do mensageiro,
  podem servir de ideia ou inspiracao, embora ***CERTAMENTE*** precisem ser adaptados.
  ************************************************************************************** */
  /// Envia para o usuario as mensagens que estejam arquivadas (paraUsuario[])
  /// com status MSG_RECEBIDA e que seja destinada ao usuario.
  /// Apos o envio, altera o status da msg enviada para MSG_ENTREGUE
  void enviarMsgsParaUsuario();
  /// Envia para o usuario as confirmacoes de visualizacao das mensagens
  /// que estejam arquivadas (doUsuario[]) com status MSG_LIDA
  /// e que tenha sido enviada pelo usuario.
  /// Apos o envio da confirmacao, apaga a msg
  void enviarConfirmacoesParaUsuario();
  /* Fim da parte que ***NAO*** pode ser adaptada para o servidor real ***************** */

public:
  /// Funcoes de acesso aas funcionalidades basicas dos sockets
  mysocket_status listen(const char *port, int nconex=1);

  /// Fecha os sockets de conexao e de todos os usuarios conectados
  void closeSockets();

  /// Thread que efetivamente desempenha as tarefas do servidor
  int main_thread();
};

#endif //DADOS_SERVIDOR_

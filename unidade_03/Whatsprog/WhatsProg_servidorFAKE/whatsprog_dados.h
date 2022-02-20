#ifndef WHATSPROG_DADOS_H
#define WHATSPROG_DADOS_H

#include <iostream>
#include <fstream>
#include <string>
#include <stdint.h>
#include <list>

using namespace std;

// A porta de comunicacao cliente-servidor
#define PORTA_WHATSPROG "23456"

// Timeout (em segundos) para esperar o envio de um dado obrigatorio pelo socket
#define TIMEOUT_WHATSPROG 15
// Timeout (em segundos)  para o cliente enviar o nome de login apos conexao
#define TIMEOUT_LOGIN_WHATSPROG 60

// Numero de bytes das informacoes a serem enviadas/recebidas
#define TAM_MIN_NOME_USUARIO 6
#define TAM_MAX_NOME_USUARIO 12
#define TAM_MIN_SENHA 6
#define TAM_MAX_SENHA 12
#define TAM_MAX_MSG 255

/// Os possiveis estados de uma mensagem
enum class MsgStatus
{
  MSG_ENVIADA,
  MSG_RECEBIDA,
  MSG_ENTREGUE,
  MSG_LIDA,
  MSG_INVALIDA
};

/// Os tipos de comando
/// Declarados como um tipo enum normal (e nao enum class) porque
/// os comandos precisam ser tratados como inteiros em algumas situacoes
enum ComandoWhatsProg
{
  CMD_NEW_USER=1001,
  CMD_LOGIN_USER=1002,
  CMD_LOGIN_OK=1003,
  CMD_LOGIN_INVALIDO=1004,
  CMD_NOVA_MSG=1005,
  CMD_MSG_RECEBIDA=1006,
  CMD_MSG_ENTREGUE=1007,
  CMD_MSG_LIDA1=1008,
  CMD_MSG_LIDA2=1009,
  CMD_ID_INVALIDA=1010,
  CMD_USER_INVALIDO=1011,
  CMD_MSG_INVALIDA=1012,
  CMD_LOGOUT_USER=1013
};

/// Testa um possivel login: tamanho e soh contem caracteres de 33 a 126
bool testarNomeUsuario(const string& S);

/// Testa uma possivel senha: tamanho e soh contem caracteres de 33 a 126
bool testarSenha(const string& S);

/// Testa o texto de uma mensagem: tamanho
bool testarTextoMensagem(const string& S);

/// Testa um possivel status (todos os valores possiveis)
bool testarStatus(MsgStatus S);

/// Retorna uma string com o nome do comando
string nome_cmd(ComandoWhatsProg cmd);

/// Retorna uma string com o nome de um status
string nome_status(MsgStatus S);

/// Alguns tipos auxiliares que sao utilizados pelos algoritmos STL:
/// find, count, etc
/// Sao definidos diferentes tipos para que se possa fazer diferentes
/// buscas em uma lista de mensagens, sobrecarregando de formas diferentes
/// o operador == para cada um desses tipos

/// Testar mensagens com um dado remetente e status
class testRemetStatus
{
private:
  string remetente;
  MsgStatus status;
public:
  testRemetStatus(string R, MsgStatus S);
  friend class Mensagem;
};

/// Testar mensagens com um dado remetente e id
class testRemetId
{
private:
  string remetente;
  int32_t id;
public:
  testRemetId(string R, int32_t I);
  friend class Mensagem;
};

/// Testar mensagens com um dado destinatario e status
class testDestStatus
{
private:
  string destinatario;
  MsgStatus status;
public:
  testDestStatus(string D, MsgStatus S);
  friend class Mensagem;
};

/// Testar mensagens com um dado destinatario e id
class testDestId
{
private:
  string destinatario;
  int32_t id;
public:
  testDestId(string D, int32_t I);
  friend class Mensagem;
};

/// As mensagens armazenadas no cliente e no servidor
class Mensagem
{
private:

  /// Identificador da mensagem
  int32_t id;

  /// Estado (status) da mensagem
  MsgStatus status;

  /// Nomes do remetente e do destinatario
  string remetente;
  string destinatario;

  /// Texto da mensagem
  string texto;

public:

  /// Construtor default
  Mensagem();

  /// Funcoes de consulta/alteracao de parametros
  /// As funcoes de alteracao retornam false em caso de erro, true se OK

  int32_t getId() const;
  bool setId(int32_t I);

  const string& getRemetente() const;
  bool setRemetente(const string& R);

  const string& getDestinatario() const;
  bool setDestinatario(const string& D);

  const string& getTexto() const;
  bool setTexto(const string& T);

  MsgStatus getStatus() const;
  bool setStatus(MsgStatus S);

  /// Os testes de igualdade entre uma msg e os tipos auxiliares
  /// que sao utilizados pelos algoritmos STL: find, count, etc
  bool operator==(const testRemetStatus& T) const;
  bool operator==(const testRemetId& T) const;
  bool operator==(const testDestStatus& T) const;
  bool operator==(const testDestId& T) const;

  /// Ler/salvar em arquivo
  /// Recebe como entrada uma fstream jah aberta
  /// A fstream deve ser aberta pela funcao de ler/salvar de um nivel superior
  /// (da classe WhatsProgDadosCliente)
  bool ler(ifstream& arq);
  bool salvar(ofstream& arq) const;
};

#endif // WHATSPROG_DADOS_H

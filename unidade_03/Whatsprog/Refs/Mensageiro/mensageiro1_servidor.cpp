#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>
#include <thread>

#include "mysocket.h"
#include "mensageiro.h"

using namespace std;

/* ==================================================================

Este aplicativo cliente-servidor permite o envio de mensagens entre
os softwares clientes e o software servidor.
As mensagens trocadas sao formadas pelos seguintes campos:
1) Usuario - uma string
   Significa o destinatario, nas msgs do cliente para o servidor,
   ou o remetente, nas msgs do servidor para o cliente
2) Texto - uma string
   Contem a msg efetivamente enviada

A primeira msg a ser enviada pelo cliente para o servidor eh
diferente das demais. Deve conter apenas o nome do usuario. Esse
nome identificarah o cliente daih em diante: todas as msgs enviadas
para esse nome do usuario serao direcionadas pelo servidor para esse
cliente.
O login (nome do cliente) deve ter entre 4 e 16 caracteres.

================================================================== */

/// Classe que armazena os dados que definem um cliente: login e socket
struct Client
{
  string login;
  tcp_mysocket s;

  Client();
  bool operator==(const string& L);
};

Client::Client(): login(""), s() {}

bool Client::operator==(const string& L) {return login==L;}

/// Apelidos para uma lista de clientes e para o iterator correspondente
typedef list<Client> list_Client;
typedef list_Client::iterator iter_Client;

///
/// Variaveis globais das 2 threads
///

/// O socket de conexoes
tcp_mysocket_server c;
// A lista de clientes
list_Client LC;
// O flag que indica que o software deve encerrar todas as threads
bool fim = false;

enum class statusEnvio
{
  OK,
  REMETENTE_INVALIDO,
  DESTINATARIO_INVALIDO,
  DESTINATARIO_INEXISTENTE,
  DESTINATARIO_NAO_CONECTADO,
  ERRO_ESCRITA_SOCKET,
  ERRO_DESCONHECIDO
};

/// Funcao auxiliar para enviar mensagem para um cliente especifico
/// Testa se os nomes do remetente e do destinatario sao validos (4 a 16 caracteres)
/// Depois, testa se o destinatario existe
/// Finalmente, testa se o destinatario estah conectado
/// Valor de retorno: um codigo do tipo statusEnvio
statusEnvio enviar_msg(const string& remet, const string& dest, const string& msg)
{
  iter_Client iDest;
  mysocket_status iResult;

  // Testa os nomes do remetente e do destinatario
  if (remet.size()<4 || remet.size()>16)
  {
    return statusEnvio::REMETENTE_INVALIDO;
  }
  if (dest.size()<4 || dest.size()>16)
  {
    return statusEnvio::DESTINATARIO_INVALIDO;
  }
  // Testa se o destinatario existe, ou seja,
  // procura o destinatario dentro da lista de clientes
  iDest = find(LC.begin(), LC.end(), dest);
  if (iDest != LC.end())
  {
    // Testa se o destinatario estah conectado
    if (iDest->s.connected())
    {
      // Escreve o login do remetente no socket do destinatario
      iResult = iDest->s.write_string(remet);
      if ( iResult == mysocket_status::SOCK_ERROR )
      {
        iDest->s.close();
        return statusEnvio::ERRO_ESCRITA_SOCKET;
      }
      // Escreve o texto da msg
      iResult = iDest->s.write_string(msg);
      if ( iResult == mysocket_status::SOCK_ERROR )
      {
        iDest->s.close();
        return statusEnvio::ERRO_ESCRITA_SOCKET;
      }
    }
    else
    {
      // O destinatario existe mas nao estah conectado
      return statusEnvio::DESTINATARIO_NAO_CONECTADO;
    }
  }
  else
  {
    // O destinatario nao existe na lista de clientes
    return statusEnvio::DESTINATARIO_INEXISTENTE;
  }
  return statusEnvio::OK;
}

// Thread que efetivamente desempenha as tarefas do servidor
void servidor(void)
{
  tcp_mysocket t;
  mysocket_queue f;
  mysocket_status iResult;

  string usuario,msg;
  iter_Client i;

  while (!fim)
  {
    // Inclui na fila de sockets para o select todos os sockets que eu
    // quero monitorar para ver se houve chegada de dados
    f.clear();
    fim = !c.accepting();
    if (!fim)
    {
      // Monitora o socket de conexoes
      f.include(c);
      for (i=LC.begin(); i!=LC.end(); i++)
      {
    	  if (i->s.connected())
    	  {
    	    // Monitora todos os sockets de clientes conectados
	        f.include(i->s);
    	  }
      }
    }

    // Espera que chegue algum dado em qualquer dos sockets da fila
    iResult = f.wait_read(MENSAGEIRO_TIMEOUT*1000);
    if (iResult == mysocket_status::SOCK_ERROR)
    {
      if (!fim) cerr << "Erro na espera por alguma atividade\n";
      fim = true;
    }
    if (!fim)
    {
      if (iResult != mysocket_status::SOCK_TIMEOUT)
      {
        // Nao saiu por timeout: houve atividade em algum socket da fila
        // Testa em qual socket houve atividade.

        // Primeiro, testa os sockets dos clientes
        for (i=LC.begin(); i!=LC.end(); i++)
        {
          if (i->s.connected() && f.had_activity(i->s))
          {
            iResult = i->s.read_string(usuario);
            if (iResult != mysocket_status::SOCK_OK)
            {
              if (iResult==mysocket_status::SOCK_ERROR)
              {
                cerr << "Erro ao receber destinatario do cliente " << i->login
                     << ". Desconectando\n";
              }
              if (iResult==mysocket_status::SOCK_DISCONNECTED)
              {
                cout << "Cliente " << i->login << " desconectou\n";
              }
              if (iResult==mysocket_status::SOCK_TIMEOUT)
              {
                // Nunca deve acontecer, jah que a read_string nao tem timeout
                cerr << "MISTERIO! A leitura do destinatario do cliente " << i->login
                     << " deu um timeout nao previsto\n";
              }
              i->s.close();
            }
            if (usuario == "FIM")
            {
              cout << "Cliente " << i->login << " encerrou\n";
              i->s.close();
            }
            if (i->s.connected())
            {
              iResult = i->s.read_string(msg);
              if (iResult != mysocket_status::SOCK_OK)
              {
                if (iResult==mysocket_status::SOCK_ERROR)
                {
                  cerr << "Erro ao receber mensagem do cliente " << i->login
                       << ". Desconectando\n";
                }
                if (iResult==mysocket_status::SOCK_DISCONNECTED)
                {
                  cout << "Cliente " << i->login << " desconectou\n";
                }
                if (iResult==mysocket_status::SOCK_TIMEOUT)
                {
                  // Nunca deve acontecer, jah que a read_string nao tem timeout
                  cerr << "MISTERIO! A leitura da msg do cliente " << i->login
                       << " deu um timeout nao previsto\n";
                }
                i->s.close();
              }
            }
            if (i->s.connected())
            {
              statusEnvio result = enviar_msg(i->login,usuario,msg);
              if (result == statusEnvio::OK)
              {
                cout << i->login << "->" << usuario << ": " << msg << endl;
              }
              else
              {
                string msg_erro;
                switch(result)
                {
                case statusEnvio::REMETENTE_INVALIDO:
                  msg_erro = "REMETENTE "+i->login+" INVALIDO";
                  break;
                case statusEnvio::DESTINATARIO_INVALIDO:
                  msg_erro = "DESTINATARIO "+usuario+" INVALIDO";
                  break;
                case statusEnvio::ERRO_ESCRITA_SOCKET:
                  msg_erro = "PROBLEMA NO ENVIO PARA O DESTINATARIO "+usuario;
                  break;
                case statusEnvio::DESTINATARIO_NAO_CONECTADO:
                  msg_erro = "DESTINATARIO "+usuario+" NAO CONECTADO";
                  break;
                case statusEnvio::DESTINATARIO_INEXISTENTE:
                  msg_erro = "DESTINATARIO "+usuario+" NAO EXISTE";
                  break;
                case statusEnvio::ERRO_DESCONHECIDO:
                default:
                  msg_erro = "ERRO DESCONHECIDO (CONTATE O ADMNISTRADOR)";
                  break;
                }
                result = enviar_msg("SERVIDOR", i->login, msg_erro);
                if (result != statusEnvio::OK)
                {
                  i->s.close();
                }
              }
            }
          }
        }

        // Depois, testa se houve atividade no socket de conexao
        if (f.had_activity(c))
        {
          if (c.accept(t) != mysocket_status::SOCK_OK)
          {
            cerr << "Nao foi poss�vel estabelecer uma conexao\n";
            fim = true;
          }
          if (!fim)
          {
            // Leh o nome de usuario do cliente
            iResult = t.read_string(usuario,MENSAGEIRO_TIMEOUT*1000);
            if (iResult != mysocket_status::SOCK_OK)
            {
              cerr << "Nao foi possivel ler o login de um cliente que se conectou.\n";
              t.close();
            }
            else
            {
              if (usuario.size()<4 || usuario.size()>16)
              {
                t.write_string("SERVIDOR");
                t.write_string("LOGIN "+usuario+" INVALIDO");
                t.close();
              }
              else
              {
                // Procura se jah existe um cliente conectado com o mesmo login
                i = find(LC.begin(), LC.end(), usuario);
                if (i != LC.end())
                {
                  if (i->s.connected())
                  {
                    t.write_string("SERVIDOR");
                    t.write_string("USUARIO "+usuario+" JA CONECTADO");
                    t.close();
                  }
                  else
                  {
                    i->s.swap(t);
                    cout << "Cliente " << usuario << " reconectado.\n";
                  }
                }
                else
                {
                  Client novo;
                  novo.login = usuario;
                  novo.s.swap(t);
                  LC.push_back(novo);
                  cout << "Cliente " << usuario << " criado e conectado.\n";
                }
              }
            }
          }
        }
      }
      else
      {
        // Saiu por timeout: nao houve atividade em nenhum socket da fila
        if (LC.empty())
        {
          cout << "Servidor inativo hah " << MENSAGEIRO_TIMEOUT << " segundos...\n";
        }
      }
    }
  }
}

int main(void)
{
  thread thr;
  string msg;

  // Inicializa a biblioteca de sockets
  mysocket_status iResult = mysocket::init();
  if (iResult != mysocket_status::SOCK_OK) {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  if (c.listen(MENSAGEIRO_PORTA) != mysocket_status::SOCK_OK) {
    cerr << "Nao foi poss�vel abrir o socket de controle\n";
    exit(1);
  }

  // Cria a thread que recebe e reenvia as mensagens
  thr = thread(servidor);

  while (!fim)
  {
    do
    {
      cout << "FIM para terminar: ";
      cin >> ws;
      getline(cin,msg);
    } while (msg.size()==0);
    if (!fim) fim = (msg=="FIM");
  }

  // Encerra os sockets
  c.close();
  for (iter_Client i=LC.begin(); i!=LC.end(); i++) i->s.close();

  // Espera pelo fim da thread do servidor
  cout << "Esperando o fim da thread...\n";
  if (thr.joinable()) thr.join();

  // Encerra a biblioteca de sockets
  mysocket::end();
}

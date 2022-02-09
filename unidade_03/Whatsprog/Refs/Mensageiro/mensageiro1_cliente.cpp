#include <iostream>
#include <string.h>
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

/* ==================================================================

Este programa tem uma thread principal que espera que o usuário
digite alguma coisa e envia esta msg para um servidor via
socket. Uma outra thread imprime todas as msgs recebidas pelo
socket.

================================================================== */

// Aqui vao as variaveis globais das duas threads

tcp_mysocket s;
bool fim = false;

// Esta eh a thread que escreve em tela as mensagens recebidas

void le_msg(void)
{
  string remetente,msg;
  mysocket_status iResult;

  while (!fim) {
    iResult = s.read_string(remetente);
    if (iResult != mysocket_status::SOCK_OK)
    {
      if (iResult==mysocket_status::SOCK_ERROR)
      {
        cerr << "Problema na leitura do remetente via socket. Desconectando\n";
      }
      if (iResult==mysocket_status::SOCK_DISCONNECTED)
      {
        cout << "Servidor desconectou\n";
      }
      if (iResult==mysocket_status::SOCK_TIMEOUT)
      {
        // Nunca deve acontecer, jah que a read_int nao tem timeout
        cerr << "MISTERIO! A leitura do remetente deu um timeout nao previsto\n";
      }
      fim = true;
    }
    if (!fim)
    {
      iResult = s.read_string(msg);
      if (iResult != mysocket_status::SOCK_OK)
      {
        if (iResult==mysocket_status::SOCK_ERROR)
        {
          cerr << "Problema na leitura da msg via socket. Desconectando\n";
        }
        if (iResult==mysocket_status::SOCK_DISCONNECTED)
        {
          cout << "Servidor desconectou\n";
        }
        if (iResult==mysocket_status::SOCK_TIMEOUT)
        {
          // Nunca deve acontecer, jah que a read_string nao tem timeout
          cerr << "MISTERIO! A leitura da msg deu um timeout nao previsto\n";
        }
        fim = true;
      }
    }
    if (!fim)
    {
      cout << endl
           << "=========================================================\n"
           << "Mensagem recebida de " << remetente <<":\n"
           << msg << endl
    	     << "=========================================================\n";
    }
  }
}

// O programa principal contém o ciclo que envia as mensagens digitadas

int main(int argc, char **argv)
{
  thread thr;
  string usuario;
  string msg;

  // Inicializa a biblioteca de sockets
  mysocket_status iResult = mysocket::init();
  if (iResult != mysocket_status::SOCK_OK) {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  // Determina o numero IP do servidor (digitado ou lido do argumento de chamada)
  if( argc<2 )
  {
    cout << "Maquina onde estah rodando o servidor (IP): ";
    cin >> ws;
    getline(cin, msg);
  }
  else
  {
    msg = argv[1];
  }

  // Conecta com o servidor
  if (s.connect(msg.c_str(), MENSAGEIRO_PORTA) == mysocket_status::SOCK_OK)
  {
    cout << "Conectado ao servidor " << msg << " na porta " << MENSAGEIRO_PORTA << endl;
  }
  else
  {
    cerr << "Problema na conexao ao servidor " << msg << " na porta " << MENSAGEIRO_PORTA << endl;
    exit(1);
  }

  // Cria a thread que escreve as mensagens recebidas
  thr = thread(le_msg);

  // Envia para o servidor o nome de usuario que identificarah esse cliente
  do
  {
    cout << "Login deste cliente [4 a 16 caracteres]: ";
    cin >> ws;
    getline(cin, usuario);
  } while (usuario.size()<4 || usuario.size()>16);
  iResult = s.write_string(usuario);
  if ( iResult == mysocket_status::SOCK_ERROR )
  {
    cerr << "Problema na escrita do login no socket...\n";
    exit(1);
  }

  // Este é o ciclo que envia as mensagens digitadas
  while (!fim)
  {
    if (!fim)
    {
      do
      {
        cout << "Usuario a enviar msg [4 a 16 caracteres, FIM para terminar]: ";
        cin >> ws;
        getline(cin, usuario);
      } while (!fim && usuario!="FIM" && (usuario.size()<4 || usuario.size()>16));
      if (!fim) fim = (usuario=="FIM");
    }

    if (!fim)
    {
      do
      {
        cout << "Mensagem a enviar [FIM para terminar]: ";
        cin >> ws;
        getline(cin, msg);
      } while (!fim && msg.size()==0);
      if (!fim) fim = (msg=="FIM");
    }

    if (!fim || usuario=="FIM")
    {
      iResult = s.write_string(usuario);
      if ( iResult == mysocket_status::SOCK_ERROR )
      {
        cout << "Problema na escrita do usuario no socket...\n";
        fim = true;
      }
    }
    if (!fim)
    {
      iResult = s.write_string(msg);
      if ( iResult == mysocket_status::SOCK_ERROR )
      {
        cout << "Problema na escrita do texto no socket...\n";
        fim = true;
      }
    }
    // Espera 1 seg para as mensagens nao se sobreporem no terminal
    this_thread::sleep_for(chrono::seconds(1));
  }

  // Encerra o socket
  s.close();

  // Espera pelo fim da thread de recepcao
  cout << "Esperando o fim da thread...\n";
  if (thr.joinable()) thr.join();

  // Encerra a biblioteca de sockets
  mysocket::end();
}

#include <iostream>
#include <string.h>
#include <list>
#include <algorithm>

#include "winsocket.h"
#include "mensageiro.h"

using namespace std;

/* ==================================================================

Este aplicativo cliente-servidor permite o envio de mensagens entre
os softwares clientes e o software servidor.
As mensagens trocadas sao formadas pelos seguintes campos:
1) Usuario - uma string formada por dois campos:
   1.1) Tamanho da string: um inteiro [ 4 bytes ]
   1.2) Caracteres da string: varios chars [ tamanho do texto ]
   Significa o destinatario, nas msgs do cliente para o servidor,
   ou o remetente, nas msgs do servidor para o cliente
2) Texto - uma string formada por dois campos:
   2.1) Tamanho do texto: um inteiro [ sizeof(int) bytes ]
   2.2) Caracteres do texto: varios chars [ tamanho do texto ]
   Contem a msg efetivamente enviada

A primeira msg a ser enviada pelo cliente para o servidor eh
diferente das demais. Deve conter apenas o nome do usuario. Esse
nome identificarah o cliente daih em diante: todas as msgs enviadas
para esse nome do usuario serao direcionadas pelo servidor para esse
cliente.

================================================================== */

// Classe que armazena os dados que definem um cliente: login e socket
struct Client
{
  string login;
  tcp_winsocket s;
  inline Client(): login(""), s() {}
  inline bool operator==(const string &L) {return login==L;}
};

// Apelidos para uma lista de clientes e para o iterator correspondente
typedef list<Client> list_Client;
typedef list_Client::iterator iter_Client;

#define TEMPO_MAXIMO 60  // Tempo máximo de espera em segundos
#define TEMPO_ENVIO_LOGIN 60 // 60 segundos para o cliente enviar o nome de login apos conexao

//
// Variáveis globais das 2 threads
//
// O socket de conexoes
tcp_winsocket_server c;
// A lista de clientes
list_Client LC;
// O flag que indica que o software deve encerrar todas as threads
bool fim = false;

// Função auxiliar para enviar mensagem para um cliente especifico (dest)
// para o qual jah se tem um iterator que aponta para sua posicao dentro da
// lista de clientes
// Essa funcao deve ser chamada pela funcao envie_msg, que procurarah o
// destinatario especifico na lista e passarah o iterator
void envie_msg_um(const string &orig, iter_Client dest, const string &msg)
{
  WINSOCKET_STATUS iResult;

  // Escreve o login do remetente
  iResult = dest->s.write_string(orig);
  if ( iResult == SOCKET_ERROR )
  {
    cerr << "Problema na escrita do usuario no socket...\n";
    dest->s.shutdown();
  }
  if (dest->s.connected())
  {
    // Escreve o texto da msg
    iResult = dest->s.write_string(msg);
    if ( iResult == SOCKET_ERROR )
    {
      cerr << "Problema na escrita do texto no socket...\n";
      dest->s.shutdown();
    }
  }
}

// Funçoo auxiliar para enviar mensagem para um cliente especifico (dest != "ALL")
// ou para todos os clientes (dest == "ALL")
void envie_msg(const string &orig, const string &dest, const string &msg)
{
  iter_Client iDest;

  if (orig.size()<4 || orig.size()>16)
  {
    cerr << "Remetente (" << orig << ") invalido\n";
    return;
  }
  if (dest!="ALL" && (dest.size()<4 || dest.size()>16))
  {
    cerr << "Destinatario (" << dest << ") invalido\n";
    return;
  }
  if (dest=="ALL")
  {
    for (iDest=LC.begin(); iDest!=LC.end(); iDest++)
    {
      if (iDest->login != orig) envie_msg_um(orig, iDest, msg);
    }
  }
  else
  {
    // Procura o destinatario especificado dentro da lista de clientes
    iDest = find(LC.begin(), LC.end(), dest);
    if (iDest != LC.end())
    {
      envie_msg_um(orig, iDest, msg);
    }
    else
    {
      // O destinatario nao existe na lista atual de clientes
      cerr << "Usuario " << dest << " nao existe na lista de clientes\n";
    }
  }
}

// Thread que efetivamente desempenha as tarefas do servidor
DWORD WINAPI servidor(LPVOID lpParameter)
{
  tcp_winsocket t;
  winsocket_queue f;
  WINSOCKET_STATUS iResult;

  string usuario,msg;
  iter_Client i;

  while (!fim)
  {
    // Inclui na fila de sockets para o select todos os sockets que eu
    // quero monitorar para ver se houve chegada de dados
    f.clean();
    if (!(fim = !c.accepting()))
    {
      f.include(c);
      for (i=LC.begin(); i!=LC.end(); i++)
      {
    	  if (i->s.connected())
    	  {
	        f.include(i->s);
    	  }
      }
    }

    // Espera que chegue alguma dados em qualquer dos sockets da fila
    iResult = f.wait_read(TEMPO_MAXIMO*1000);
    if (iResult==SOCKET_ERROR)
    {
      if (!fim) cerr << "Erro na espera por alguma atividade\n";
      fim = true;
    }
    if (!fim)
    {
      if (iResult!=0)
      {
        // Não saiu por timeout: houve atividade em algum socket da fila
        // Testa em qual socket houve atividade.

        // Primeiro, testa os sockets dos clientes
        for (i=LC.begin(); i!=LC.end(); i++)
        {
          if (i->s.connected() && f.had_activity(i->s))
          {
            iResult = i->s.read_string(usuario);
            if (iResult == SOCKET_ERROR)
            {
              cerr << "Destinatario invalido (" << usuario << ") recebido do cliente " << i->login << ". Desconectando\n";
              i->s.shutdown();
            }
            if (i->s.connected())
            {
              iResult = i->s.read_string(msg);
              if (iResult == SOCKET_ERROR)
              {
                cerr << "Mensagem invalida recebida do cliente " << i->login << ". Desconectando\n";
                i->s.shutdown();
              }
            }
            if (i->s.connected())
            {
              cout << i->login << "->" << usuario << ": " << msg << endl;
              envie_msg(i->login,usuario,msg);
            }
          }
        }

        // Depois, esta se houve atividade no socket de conexao
        if (f.had_activity(c))
        {
          if (c.accept(t) != SOCKET_OK)
          {
            cerr << "Não foi possível estabelecer uma conexao\n";
            fim = true;
          }
          if (!fim)
          {
            // Leh o nome de usuario do cliente
            iResult = t.read_string(usuario,TEMPO_ENVIO_LOGIN*1000);
            if (iResult == SOCKET_ERROR)
            {
              cerr << "Erro na leitura do nome de login de um cliente que se conectou.\n";
              t.close();
            }
            else
            {
              if (usuario.size()<4 || usuario.size()>16)
              {
                cerr << "Nome de login (" << usuario << ") invalido.\n";
                t.close();
              }
              else
              {
                // Procura se jah existe um cliente conectado com o mesmo login
                i = find(LC.begin(), LC.end(), usuario);
                if (i != LC.end())
                {
                  cerr << "Nome de login (" << usuario << ") jah utilizado.\n";
                  t.close();
                }
                else
                {
                  Client novo;
                  novo.login = usuario;
                  novo.s = t;
                  LC.push_back(novo);
                  cout << "Cliente " << usuario << " conectado.\n";
                }
              }
            }
          }
        }
      }
      else
      {
        // Saiu poe timeout: não houve atividade em nenhum socket da fila
        if (LC.empty())
        {
          cout << "Servidor inativo hah " << TEMPO_MAXIMO << " segundos...\n";
        }
      }

      // Depois de testar a chegada de dados em todos os sockets,
      // elimina da lista de sockets as conexoes que foram fechadas porque houve
      // falha na comunicacao ou porque se desligaram
      for (i=LC.begin(); i!=LC.end(); i++)
      {
        while ( i!=LC.end() && !(i->s.connected()) )
        {
          i->s.close();
          i = LC.erase(i);
        }
      }
    }
  }
  return 0;
}

int main(void)
{
  WSADATA wsaData;
  string msg;

  // All processes that call Winsock functions must first initialize the use of the Windows Sockets DLL (WSAStartup)
  // before making other Winsock functions calls
  // The MAKEWORD(2,2) parameter of WSAStartup makes a request for version 2.2 of Winsock on the system
  WINSOCKET_STATUS iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != 0) {
    cerr << "WSAStartup failed: " << iResult << endl;
    exit(1);
  }

  if (c.listen(PORTA_TESTE) != SOCKET_OK) {
    cerr << "Não foi possível abrir o socket de controle\n";
    exit(1);
  }

  // Cria a thread que recebe e reenvia as mensagens
  HANDLE tHandle = CreateThread(NULL, 0, servidor, NULL , 0, NULL);
  if (tHandle == NULL)
  {
    cerr << "Problema na criação da thread: " << GetLastError() << endl;
    exit(1);
  }


  while (!fim)
  {
    do
    {
      cout << "Mensagem para todos os clientes [max " << TAM_MAX_MSG_STRING << " caracteres, FIM para terminar]: ";
      cin >> ws;
      getline(cin,msg);
    } while (msg.size()==0 || msg.size()>TAM_MAX_MSG_STRING);
    if (!fim) fim = (msg=="FIM");
    if (!fim)
    {
      envie_msg("SERVIDOR","ALL",msg);
    }
  }

  // Desliga os sockets
  cout << "Encerrando o socket de conexoes\n";
  c.shutdown();
  for (iter_Client i=LC.begin(); i!=LC.end(); i++)
  {
    cout << "Encerrando o socket do cliente " << i->login << endl;
    i->s.shutdown();
  }
  // Espera pelo fim da thread do servidor (máximo de 5 segundos)
  cout << "Aguardando o encerramento da outra thread...\n";
  WaitForSingleObject(tHandle, 5000);
  // Encerra na "força bruta" a thread do servidor caso ela não tenha terminado sozinha
  // (ou seja, a função WaitForSingleObject tenha saído por timeout)
  TerminateThread(tHandle,0);
  // Encerra o handle da thread
  CloseHandle(tHandle);
  // Enderra o socket
  c.close();
  for (iter_Client i=LC.begin(); i!=LC.end(); i++) i->s.close();

  /* call WSACleanup when done using the Winsock dll */
  WSACleanup();
}

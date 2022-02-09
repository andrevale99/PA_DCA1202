#include <iostream>
#include <string.h>
#include <vector>
#include <thread>

#include "mysocket.h"
#include "mensageiro.h"

using namespace std;

/// Numero maximo de clientes simultaneos que o servidor aceita
#define NUM_MAX_CONEX 30

/// Variáveis globais das 2 threads

/// O socket de conexoes
tcp_mysocket_server c;
/// Os sockets dos clientes
vector<tcp_mysocket> s(NUM_MAX_CONEX);
/// O numero de conexoes
int nconex=0;
/// O flag de termino da execucao
bool fim = false;

/// Funcao auxiliar para enviar mensagem para todos os clientes
void envie_msg(int32_t orig, const string& msg)
{
  mysocket_status iResult;

  for (int i=0; i<nconex; i++) if (i!=orig && s.at(i).connected()) {
    //cout << "Enviando para cliente " << i << endl;
    iResult = s.at(i).write_int(orig);
    if ( iResult != mysocket_status::SOCK_OK ) {
      cerr << "Problema ao enviar remetente para o cliente " << i << endl;
      s.at(i).close();
    }
    else {
      iResult = s.at(i).write_string(msg);
      if ( iResult != mysocket_status::SOCK_OK ) {
        cerr << "Problema ao enviar mensagem para o cliente " << i << endl;
        s.at(i).close();
      }
      else
      {
        //cout << "Mensagem enviada para o cliente " << i << endl;
      }
    }
  }
}

/// Thread que efetivamente desempenha as tarefas do servidor
void servidor(void)
{
  tcp_mysocket t;
  mysocket_queue f;
  mysocket_status iResult;

  string msg;
  int i;

  cout << "Iniciando a thread de leitura do socket!\n";
  while (!fim)
  {
    fim = !c.accepting();
    if (!fim)
    {
      f.clear();
      f.include(c);
      for (i=0; i<nconex; i++)
      {
	      if (s.at(i).connected())
        {
	        f.include(s.at(i));
	      }
      }
    }

    // Espera ateh que chegue dado em algum socket
    iResult = f.wait_read(MENSAGEIRO_TIMEOUT*1000);
    if (iResult == mysocket_status::SOCK_ERROR) {
      if (!fim) cerr << "Erro na espera por alguma atividade\n";
      fim = true;
    }
    if (!fim)
    {
      if (iResult != mysocket_status::SOCK_TIMEOUT)
      {
        // Nao saiu por timeout: houve atividade em algum socket da fila
        // Testa em qual socket houve atividade. Primeiro os sockets dos clientes
        for (i=0; i<nconex; i++)
        {
          if (s.at(i).connected() && f.had_activity(s.at(i)))
          {
            iResult = s.at(i).read_string(msg);
            if (iResult != mysocket_status::SOCK_OK)
            {
              if (iResult==mysocket_status::SOCK_ERROR)
              {
                cerr << "Mensagem invalida recebida do cliente " << i << ". Desconectando\n";
              }
              if (iResult==mysocket_status::SOCK_DISCONNECTED)
              {
                cout << "Cliente desconectou\n";
              }
              if (iResult==mysocket_status::SOCK_TIMEOUT)
              {
                // Nunca deve acontecer, jah que a read_string nao tem timeout
                cerr << "MISTERIO! A leitura da msg do cliente " << i << " deu um timeout nao previsto\n";
              }
              s.at(i).close();
            }
            else
            {
              cout << "Mensagem recebida do cliente " << i << ":\n";
              cout << msg << endl;
              if (msg == "FIM")
              {
                cout << "Cliente " << i << " encerrou\n";
                s.at(i).close();
              }
              else
              {
                envie_msg(i,msg);
              }
            }
          }
        }
        // Testa se houve atividade no socket de conexao
        if (f.had_activity(c))
        {
          // Estabelece nova conexao no socket temporario "t"
          if (c.accept(t) == mysocket_status::SOCK_OK)
          {
            cout << "Recebido novo pedido de conexao\n";
          }
          else
          {
            cerr << "Nao foi possivel estabelecer uma conexao\n";
            fim = true;
          }
          if (!fim)
          {
            if (nconex >= NUM_MAX_CONEX)
            {
              cout << "Numero máximo de conexoes atingido: conexao rejeitada\n";
              t.close();
            }
            else
            {
              // Aloca proximo socket livre para o novo cliente
              s.at(nconex).swap(t);
              cout << "Cliente " << nconex << " conectado no socket "
                   << s.at(nconex) << endl;
              nconex++;
            }
          }
        }
      }
      else
      {
        // Saiu por timeout: nao houve atividade em nenhum socket da fila
        cout << "Inatividade de " << MENSAGEIRO_TIMEOUT << " segundos...\n";
      }
      // Elimina da lista de sockets as conexoes que foram fechadas porque houve
      // falha na comunicacao ou porque se desligaram
      for (int i=nconex-1; i>=0; i--)
      {
        if (s.at(i).closed())
        {
          for (int j=i; j<nconex-1; j++)
          {
            s[j].swap(s[j+1]);
          }
          nconex--;
        }
      }
    }
  }
  cout << "Fim da thread de leitura do socket!\n";
}

/// O programa principal
/// Inicializa os sockets, envia msgs para todos os clientes (broadcast)
/// e encerra a execucao
int main(void)
{
  thread thr;
  string msg;

  // Inicializa a biblioteca de sockets (exigida no Windows)
  mysocket_status iResult = mysocket::init();
  if (iResult != mysocket_status::SOCK_OK) {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  if (c.listen(MENSAGEIRO_PORTA,NUM_MAX_CONEX) != mysocket_status::SOCK_OK)
  {
    cerr << "Nao foi possivel abrir o socket de controle\n";
    exit(1);
  }

  // Cria a thread que recebe e reenvia as mensagens
  thr = thread(servidor);

  while (!fim)
  {
    do
    {
      cout << "Mensagem para todos os clientes [FIM para terminar]: ";
      cin >> ws;
      getline(cin,msg);
    } while (msg.size()==0);
    if (!fim) fim = (msg=="FIM");
    envie_msg(-1,msg);
  }

  // Encerra os sockets
  cout << "Fechando os sockets...\n";
  c.close();
  for (int i=0; i<nconex; i++) s.at(i).close();

  // Espera pelo fim da thread do servidor
  cout << "Esperando o fim da thread...\n";
  if (thr.joinable()) thr.join();

  // Encerra a biblioteca de sockets
  mysocket::end();
}

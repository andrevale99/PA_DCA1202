#include <iostream>
#include <thread>

#include "whatsprog_dados.h"
#include "whatsprog_dados_servidorFAKE.h"

using namespace std;

/* ==================================================================

Servidor do aplicativo WhatsProg

================================================================== */

/// Variavel global (das duas threads)
/// O flag que indica que o software deve encerrar todas as threads
bool fim = false;

/// Funcao que efetivamente desempenha as tarefas do servidor
/// Chama o metodo main_thread da classe WhatsProgDadosServidor
void main_thread( WhatsProgDadosServidor* DS )
{
  DS->main_thread();
}

int main(void)
{
  /// Os dados do servidor
  WhatsProgDadosServidor DS;
  /// A thread do servidor
  thread thr;

  string msg;

  // Inicializa a biblioteca de sockets (exigida no Windows)
  if (mysocket::init() != mysocket_status::SOCK_OK)
  {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  cout << "WhatsProg - SERVIDOR FAKE\n";

  if (DS.listen(PORTA_WHATSPROG) != mysocket_status::SOCK_OK)
  {
    cerr << "Não foi possível abrir o socket de controle\n";
    exit(1);
  }

  // Lanca a thread que recebe e reenvia as mensagens
  thr = thread(main_thread, &DS);

  while (!fim)
  {
    do
    {
      cout << "Digite FIM para terminar:\n";
      cin >> ws;
      getline(cin,msg);
    } while (msg != "FIM");
    fim = true;
  }

  // Desliga os sockets
  cout << "Encerrando os sockets...\n";
  DS.closeSockets();

  // Espera pelo fim da thread de recepcao
  if (thr.joinable()) thr.join();

  /// Encerramento da biblioteca de sockets
  mysocket::end();
}

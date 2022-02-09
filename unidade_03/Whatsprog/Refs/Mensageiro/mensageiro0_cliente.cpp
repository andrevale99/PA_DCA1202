#include <iostream>
#include <string.h>
#include <thread>

#include "mysocket.h"
#include "mensageiro.h"

using namespace std;

/// Este programa tem uma thread principal que espera que o usuário
/// digite alguma coisa e envia esta string para um servidor via
/// socket. Uma outra thread imprime todas as strings recebidas pelo
/// socket.
///
/// PROTOCOLO DE COMUNICACAO
/// Cliente -> servidor:
/// 1) Envia a string com a mensagem
/// Servidor -> cliente:
/// 1) Envia um inteiro com o numero do remetente (cliente que enviou a mensagem)
///    1.1) Quando a msg vier do proprio servidor, o numero serah negativo
/// 2) Envia a string com a mensagem

/// Aqui vao as variáveis globais das duas threads

tcp_mysocket s;
bool fim = false;

/// Esta e a thread que escreve em tela as mensagens recebidas

void le_msg(void)
{
  bool msg_recebida;
  int32_t remetente;
  string msg;
  mysocket_status iResult;

  cout << "Iniciando a thread de leitura do socket!\n";
  while (!fim)
  {
    msg_recebida = true;
    // Leh o socket com timeout
    iResult = s.read_int(remetente, MENSAGEIRO_TIMEOUT*1000);
    if (iResult != mysocket_status::SOCK_OK)
    {
      msg_recebida = false;
      if (iResult==mysocket_status::SOCK_ERROR)
      {
        cerr << "Problema na leitura do remetente via socket. Desconectando.\n";
        fim = true;
      }
      if (iResult==mysocket_status::SOCK_DISCONNECTED)
      {
        cout << "Servidor desconectou.\n";
        fim = true;
      }
      if (iResult==mysocket_status::SOCK_TIMEOUT)
      {
        cout << "Inatividade de " << MENSAGEIRO_TIMEOUT << " segundos...\n";
      }
    }
    if (!fim && msg_recebida)
    {
      iResult = s.read_string(msg);
      if (iResult != mysocket_status::SOCK_OK)
      {
        msg_recebida = false;
        if (iResult==mysocket_status::SOCK_ERROR)
        {
          cerr << "Problema na leitura da msg via socket. Desconectando\n";
          fim = true;
        }
        if (iResult==mysocket_status::SOCK_DISCONNECTED)
        {
          cout << "Servidor desconectou\n";
          fim = true;
        }
        if (iResult==mysocket_status::SOCK_TIMEOUT)
        {
          cout << "Inatividade de " << MENSAGEIRO_TIMEOUT << " segundos...\n";
        }
      }
    }
    if (msg_recebida)
    {
      cout << endl
           << "=========================================================\n"
           << "Mensagem recebida de " << remetente <<":\n"
           << msg << endl
	         << "=========================================================\n";
      fim = (msg=="FIM");
    }
  }
  cout << "Fim da thread de leitura do socket!\n";
}

/// O programa principal contem o ciclo que envia as mensagens digitadas

int main(int argc, char **argv)
{
  mysocket_status iResult;
  thread thr;
  string msg;

  // Inicializa a biblioteca de sockets
  iResult = mysocket::init();
  if (iResult != mysocket_status::SOCK_OK) {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  if( argc<2 ) {
    cout << "Maquina onde esta rodando o servidor (IP): ";
    cin >> ws;
    getline(cin, msg);
  }
  else {
    msg = argv[1];
  }

  iResult = s.connect(msg.c_str(), MENSAGEIRO_PORTA);
  if (iResult == mysocket_status::SOCK_OK)
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

  // Este eh o ciclo que envia as mensagens digitadas
  while (!fim)
  {
    do
    {
      cout << "Mensagem a enviar [FIM para terminar]: ";
      cin >> ws;
      getline(cin, msg);
    } while (!fim && msg.size()==0);

    if (!fim)
    {
      iResult = s.write_string(msg);
      if ( iResult == mysocket_status::SOCK_ERROR )
      {
        cout << "Problema na escrita no socket...\n";
        fim = true;
      }
    }
    if (!fim) fim = (msg=="FIM");

    // Espera 1 seg para as mensagens nao se sobreporem no terminal
    this_thread::sleep_for(chrono::seconds(1));
  }

  // Enderra o socket
  cout << "Fechando o socket...\n";
  s.close();

  // Espera pelo fim da thread de recepcao
  cout << "Esperando o fim da thread...\n";
  if (thr.joinable()) thr.join();

  // Encerra a biblioteca de sockets
  mysocket::end();
}

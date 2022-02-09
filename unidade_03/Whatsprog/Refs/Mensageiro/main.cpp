#include <QApplication>

#include "mysocket.h"
#include "mensageiro2_cliente_main.h"

using namespace std;

// Variaveis globais
tcp_mysocket sock;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  Mensageiro2_Cliente_Main w;

  // Inicializa a API de sockets
  mysocket_status iResult = mysocket::init();
  if (iResult != mysocket_status::SOCK_OK) {
    cerr << "Biblioteca mysocket nao pode ser inicializada\n";
    exit(1);
  }

  // Exibe a janela principal do aplicativoi
  w.show();

  // Entra no laco sem fim de execucao
  int prov = a.exec();

  // Espera pelo fim da thread de recepcao
  w.desconectar();

  // Encerra a API de sockets
  mysocket::end();

  return prov;
}

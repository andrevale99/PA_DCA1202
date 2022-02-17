#include <QApplication>
#include <QMessageBox>

#include "whatsprog_main.h"
#include <thread>
#include "mysocket.h"
#include "whatsprog_dados.h"
#include "whatsprog_dados_cliente.h"

WhatsProgDadosCliente DC;

tcp_mysocket sock;

thread thr;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  WhatsProgMain w;

  a.setWindowIcon(QIcon("/home/andre/Downloads/WhatsProg_interface/whatsprog_icon.png"));

  mysocket_status iniciar_scks = mysocket::init();

  if(iniciar_scks != mysocket_status::SOCK_OK)
  {
      cerr << "[ERRO] A biblioteca nao conseguiu iniciar os sockets\n";
      exit(0);
  }

  w.show();
  int result = a.exec();

  if (thr.joinable())
      thr.join();

  mysocket::end();

  return result;
}

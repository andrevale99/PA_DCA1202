#ifndef MENSAGEIRO2_CLIENTE_MAIN_H
#define MENSAGEIRO2_CLIENTE_MAIN_H

#include <QMainWindow>
#include <QLabel>
#include <QString>

#include "mensageiro2_cliente_connect.h"
#include "mensageiro2_cliente_thread.h"

namespace Ui {
  class Mensageiro2_Cliente_Main;
}

class Mensageiro2_Cliente_Main: public QMainWindow
{
  Q_OBJECT

public:
  explicit Mensageiro2_Cliente_Main(QWidget *parent = 0);
  ~Mensageiro2_Cliente_Main();

  bool connected() const;

  void desconectar();

signals:
  void signExecutarThread();
  void signEncerrarThread();

public slots:
  void slotConectarServidor(QString server, QString usuario);

  void slotExibirInterfaceConectada();
  void slotExibirInterfaceDesconectada();
  void slotExibirMsgRecebida(QString remet, QString msg);
  void slotExibirErro(QString title, QString msg);

private slots:
  void on_actionSair_triggered();
  void on_actionConectar_triggered();
  void on_actionApagar_triggered();
  void on_actionDesconectar_triggered();
  void on_enviarButton_clicked();

private:
  Ui::Mensageiro2_Cliente_Main *ui;

  QString servidor;
  QString login;

  QLabel *labelServidor;
  QLabel *labelLogin;

  Mensageiro2_Cliente_Thread *cl_thread;
  Mensageiro2_Cliente_Connect *cl_connect;

  void exibirMsgEnviada(QString dest, QString msg);
};

#endif // MENSAGEIRO2_CLIENTE_MAIN_H

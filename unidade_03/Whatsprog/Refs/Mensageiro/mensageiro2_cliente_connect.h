#ifndef MENSAGEIRO2_CLIENTE_CONNECT_H
#define MENSAGEIRO2_CLIENTE_CONNECT_H

#include <QDialog>
#include <QString>

namespace Ui {
  class Mensageiro2_Cliente_Connect;
}

class Mensageiro2_Cliente_Connect : public QDialog
{
  Q_OBJECT

public:
  explicit Mensageiro2_Cliente_Connect(QWidget *parent = 0);
  ~Mensageiro2_Cliente_Connect();

signals:
  void signConectar(QString server, QString usuario);

private slots:
  void on_buttonBox_accepted();

private:
  Ui::Mensageiro2_Cliente_Connect *ui;
};

#endif // MENSAGEIRO2_CLIENTE_CONNECT_H

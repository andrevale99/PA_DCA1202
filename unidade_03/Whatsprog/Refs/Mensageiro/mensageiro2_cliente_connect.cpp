#include <string>
#include <QMessageBox>

#include "mensageiro2_cliente_connect.h"
#include "ui_mensageiro2_cliente_connect.h"

using namespace std;

Mensageiro2_Cliente_Connect::Mensageiro2_Cliente_Connect(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::Mensageiro2_Cliente_Connect)
{
  ui->setupUi(this);
}

Mensageiro2_Cliente_Connect::~Mensageiro2_Cliente_Connect()
{
  delete ui;
}

void Mensageiro2_Cliente_Connect::on_buttonBox_accepted()
{
  QString server = ui->editServidor->text();
  QString usuario = ui->editLogin->text();

  // O numero IP de um servidor deve ter no minimo 7 caracteres
  // O nome de login deve ter entre 4 e 16 caracteres
  if (server.size()>7 && usuario.size()>=4 && usuario.size()<=16)
  {
    emit signConectar(server,usuario);
  }
  else
  {
    // Emitir msg de erro: dado(s) invalido(s)
    QMessageBox msgBox;
    msgBox.setWindowTitle("Erro na conexao");
    msgBox.setText("Valor invalido de servidor ("+server+") e/ou login ("+usuario+")");
    msgBox.exec();
  }
}

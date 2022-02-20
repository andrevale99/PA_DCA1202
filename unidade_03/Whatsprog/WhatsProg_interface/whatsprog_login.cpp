#include "whatsprog_login.h"
#include "ui_whatsprog_login.h"
#include "whatsprog_main.h"
#include <qmessagebox.h>

/***
 *
 * DUPLA: Andre Menezes de Freitas Vale
 *        Luis Filipe Silva da Costa
 *
 * SISTEMA OPERACIONAL USADO: LINUX
 *
 * Caso queira roda no Windows
 * 1. Descomentar no ".pro" o link "-lWs2_32"
 * 2. Descomentar as partes para rodar no Windows nos arquivos "mysocket.h" e "mysocket.cpp"
 *
 */

WhatsProgLogin::WhatsProgLogin(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WhatsProgLogin)
{
  ui->setupUi(this);

  ui->lineEditSenhaUsuario->setEchoMode( QLineEdit::Password );
}

WhatsProgLogin::~WhatsProgLogin()
{
  delete ui;
}

void WhatsProgLogin::slotLogin(bool NovoUsuario)
{
  novoUsuario = NovoUsuario;
  if (novoUsuario) setWindowTitle("Usuario - Criar");
  else setWindowTitle("Usuario - Conectar");
  ui->lineEditIpServidor->clear();
  ui->lineEditNomeUsuario->clear();
  ui->lineEditSenhaUsuario->clear();
  show();
}

void WhatsProgLogin::on_buttonBox_accepted()
{
    QString ip,username,senha;

    ip = ui->lineEditIpServidor->text();
    username = ui->lineEditNomeUsuario->text();
    senha = ui->lineEditSenhaUsuario->text();

    if(testarNomeUsuario(username.toStdString()) && testarSenha(senha.toStdString()))
    {
        emit sign_newuser(ip,username,senha, novoUsuario);
        ui->lineEditIpServidor->clear();
        ui->lineEditNomeUsuario->clear();
        ui->lineEditSenhaUsuario->clear();
    }
    else
    {
        QMessageBox::critical(this, "Erro ao realizar o login", "Dados inválidos");
    }
}

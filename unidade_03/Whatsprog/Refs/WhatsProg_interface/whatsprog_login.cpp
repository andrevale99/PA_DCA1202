#include "whatsprog_login.h"
#include "ui_whatsprog_login.h"
#include "whatsprog_main.h"

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
}

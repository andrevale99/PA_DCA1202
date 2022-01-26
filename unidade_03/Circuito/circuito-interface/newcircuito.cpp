#include "newcircuito.h"
#include "ui_newcircuito.h"
#include "maincircuito.h"
#include <QMessageBox>

NewCircuito::NewCircuito(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewCircuito)
{
  ui->setupUi(this);
}

NewCircuito::~NewCircuito()
{
  delete ui;
}

// Limpa e depois exibe (show) a janela de novo circuito
void NewCircuito::slotShowNewCircuito()
{
  ui->spinNumIn->setValue(1);
  ui->spinNumOut->setValue(1);
  ui->spinNumPortas->setValue(1);
  show();
}

// Sinaliza a criacao de um novo circuito com as dimensoes especificadas pelo usuario
// Deve emitir um sinal para a interface principal com os parametros escolhidos
void NewCircuito::on_NewCircuito_accepted()
{
  // Recupera os parametros digitados
  int numInputs(ui->spinNumIn->value());
  int numOutputs(ui->spinNumOut->value());
  int numPortas(ui->spinNumPortas->value());

  // Sinaliza a necessidade de redimensionamento do circuito
  emit signNewCircuito(numInputs,numOutputs,numPortas);
}

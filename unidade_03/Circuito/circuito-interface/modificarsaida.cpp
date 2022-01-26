#include "modificarsaida.h"
#include "ui_modificarsaida.h"
#include "maincircuito.h"
#include <QPushButton>

ModificarSaida::ModificarSaida(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ModificarSaida)
{
  ui->setupUi(this);
}

ModificarSaida::~ModificarSaida()
{
  delete ui;
}

// Fixa o limite inferior (-numInputs) e superior (numPortas)
// do spinBox que eh usado para indicar a origem das saidas
void ModificarSaida::slotSetRangeInputs(int minimo, int maximo)
{
  ui->spinOrigemSaida->setRange(minimo,maximo);
  // Apos alterar os limites do spinBox, pode ser que o valor
  // seja alterado para se enquadrar no novo limite
  // Se isso acontecer, chama on_spinOrigemSaida_valueChanged, o que
  // vai testar se o botao OK deve ser habilitado ou nao
}

// Fixa a id e a origem da saida que estah sendo modificada
// Depois exibe (show) a janela de modificar saida
void ModificarSaida::slotShowModificarSaida(int IdSaida, int IdOrigemSaida)
{
  // Armazena a id da saida que estah sendo modificada
  idSaida = IdSaida;
  // Altera o label de exibicao da janela
  ui->labelIdSaida->setNum(idSaida);

  // Ajusta o box de entrada de dados
  // Origem da saida
  ui->spinOrigemSaida->setValue(IdOrigemSaida);

  // Testa se o botao OK deve ser habilitado ou nao
  on_spinOrigemSaida_valueChanged(IdOrigemSaida);

  // Exibe a janela
  show();
}

// Quando modifica o valor da origem de uma saida, testa se o valor eh
// invalido (zero); se for o caso, desabilita o botao OK
void ModificarSaida::on_spinOrigemSaida_valueChanged(int arg1)
{
  bool saida_valida = (arg1 != 0);
  // Recupera um ponteiro para o botao OK
  QPushButton *botao_ok = ui->buttonBox->button(QDialogButtonBox::Ok);
  // Habilita ou nao o botao OK
  botao_ok->setEnabled(saida_valida);
}

// Sinaliza a alteracao da origem da saida "idSaida"
// de acordo com o valor especificado pelo usuario
void ModificarSaida::on_buttonBox_accepted()
{
  // Recupera o valor escolhido pelo usuario
  int idOrigemSaida = ui->spinOrigemSaida->value();
  // Emite sinal com o parametro
  emit signModificarSaida(idSaida, idOrigemSaida);
}

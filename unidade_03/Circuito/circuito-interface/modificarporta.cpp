#include "maincircuito.h"
#include "modificarporta.h"
#include "ui_modificarporta.h"
#include <QPushButton>
#include <string>

ModificarPorta::ModificarPorta(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::ModificarPorta),
  idPort(0)
{
  ui->setupUi(this);

  // Inclui os tipos de portas
  ui->comboTipoPorta->addItems(QStringList() << "NT" << "AN" << "OR" << "XO" << "NA" << "NO" << "NX");
  // Seleciona o primeiro tipo de porta (NT)
  ui->comboTipoPorta->setCurrentText("NT");
  // Como o index foi alterado via programa, chama on_comboTipoPorta_currentIndexChanged
  // Isso altera os limites do spinBox do numero de entradas (de 1 a 1 para NT)

  // Desabilita os widgets das entradas 2, 3 e 4, jah que portas NOT soh teem uma entrada
  ui->labelInput2->setEnabled(false);
  ui->spinInput2->setEnabled(false);
  ui->labelInput3->setEnabled(false);
  ui->spinInput3->setEnabled(false);
  ui->labelInput4->setEnabled(false);
  ui->spinInput4->setEnabled(false);

  testa_entradas_validas();
}

ModificarPorta::~ModificarPorta()
{
  delete ui;
}

// Fixa o limite inferior (-numInputs) e superior (numPortas)
// dos spinBoxs que sao usados para indicar a origem das entradas das portas
void ModificarPorta::slotSetRangeInputs(int minimo, int maximo)
{
  ui->spinInput1->setRange(minimo,maximo);
  ui->spinInput2->setRange(minimo,maximo);
  ui->spinInput3->setRange(minimo,maximo);
  ui->spinInput4->setRange(minimo,maximo);
  // Apos alterar os limites dos spinBox, pode ser que o valor atual de algum deles
  // seja alterado para se enquadrar no novo limite
  // Se isso acontecer, chama on_spinInput#_valueChanged, o que
  // vai testar se o botao OK deve ser habilitado ou nao
}

// Fixa as caracteristas da porta que estah sendo modificada
// Ajusta o comboBox e os spinBox para refletirem o estado atual da porta
// Depois exibe (show) a janela de modificar porta
void ModificarPorta::slotShowModificarPorta(int IdPort, QString TipoPort, int NumInputsPort,
                                            int IdInput0, int IdInput1, int IdInput2, int IdInput3)
{
  // Armazena a id da porta que estah sendo modificada
  idPort = IdPort;
  // Altera o label de exibicao da janela
  ui->labelIdPorta->setNum(idPort);

  // Ajusta os boxs de entrada de dados aos parametros da porta

  // Tipo de porta
  if (TipoPort!="AN" && TipoPort!="NA" && TipoPort!="OR" &&
      TipoPort!="NO" && TipoPort!="XO" && TipoPort!="NX") TipoPort="NT";
  ui->comboTipoPorta->setCurrentText(TipoPort);
  // Como a escolha do combo foi alterado via programa, chama on_comboTipoPorta_currentIndexChanged
  // Isso, por sua vez, altera os limites do spinBox do numero de entradas (1 a 1 p/ NT, 2 a 4 p/ demais)

  // Numero de entradas
  ui->spinNumInputs->setValue(NumInputsPort);
  // Como o numero do spin foi alterado, chama on_spinNumInputs_valueChanged
  // Isso habilita os spinBoxs que correspondem a entradas existentes, desabilita os demais

  // Entrada da porta 1
  ui->spinInput1->setValue(IdInput0);
  // Como a origem (id) da entrada foi alterada, chama on_spinInput1_valueChanged
  // Isso habilita/desabilita os spinBoxs das entradas que existem ou nao na porta

  // Entrada da porta 2
  ui->spinInput2->setValue(IdInput1);
  // Como a origem (id) da entrada foi alterada, chama on_spinInput2_valueChanged
  // Isso habilita/desabilita os spinBoxs das entradas que existem ou nao na porta

  // Entrada da porta 3
  ui->spinInput3->setValue(IdInput2);
  // Como a origem (id) da entrada foi alterada, chama on_spinInput3_valueChanged
  // Isso habilita/desabilita os spinBoxs das entradas que existem ou nao na porta

  // Entrada da porta 4
  ui->spinInput4->setValue(IdInput3);
  // Como a origem (id) da entrada foi alterada, chama on_spinInput4_valueChanged
  // Isso habilita/desabilita os spinBoxs das entradas que existem ou nao na porta

  // Exibe a janela
  show();
}

// Sempre que modificar o tipo de porta, modifica os limites do spinBox que eh utilizado
// para escolher o numero de entradas daquela porta:
// NT: de 1 a 1
// Demais: de 2 a 4
void ModificarPorta::on_comboTipoPorta_currentIndexChanged(const QString &arg1)
{
  // Fixa os limites para o numero de entradas: de 1 a 1 se for NT, 2 a 4 para outros tipos
  if (arg1=="NT") ui->spinNumInputs->setRange(1,1);
  else ui->spinNumInputs->setRange(2,4);
  // Apos fixar os limites do spinBox do numero de entradas da porta, pode ser que o valor dele
  // seja alterado para se enquadrar no novo limite
  // Se isso acontecer, chama on_spinNumInputs_valueChanged, o que vai habilitar/desabilitar
  // entradas e vai testar se o botao OK deve ser habilitado ou nao
}

// Quando modifica o numero de entradas da porta, habilita apenas os spinBoxs que sao utilizados
// para indicar a origem do sinal de entradas que existem na porta em questao
// Em seguida, verifica se o botao OK deve ser habilitado/desabilitado
void ModificarPorta::on_spinNumInputs_valueChanged(int arg1)
{
  ui->labelInput2->setEnabled(arg1 >= 2);
  ui->spinInput2->setEnabled(arg1 >= 2);
  ui->labelInput3->setEnabled(arg1 >= 3);
  ui->spinInput3->setEnabled(arg1 >= 3);
  ui->labelInput4->setEnabled(arg1 >= 4);
  ui->spinInput4->setEnabled(arg1 >= 4);
  testa_entradas_validas();
}

// Testa se a origem (id) do sinal de alguma das entradas da porta tem valor invalido (zero)
// e se a entrada estah habilitada; se for o caso, desabilita o botao OK
void ModificarPorta::testa_entradas_validas(void)
{
  bool entradas_validas = (ui->spinInput1->value() != 0);
  if (entradas_validas)
  {
    entradas_validas = (ui->spinInput2->value()!=0 || !ui->spinInput2->isEnabled());
  }
  if (entradas_validas)
  {
    entradas_validas = (ui->spinInput3->value()!=0 || !ui->spinInput3->isEnabled());
  }
  if (entradas_validas)
  {
    entradas_validas = (ui->spinInput4->value()!=0 || !ui->spinInput4->isEnabled());
  }
  // Recupera um ponteiro para o botao OK
  QPushButton *botao_ok = ui->buttonBox->button(QDialogButtonBox::Ok);
  // Habilita/desabilita o botao OK
  botao_ok->setEnabled(entradas_validas);
}

// Quando modifica o valor da entrada de alguma das portas, verifica se a configuracao
// com as entradas da porta eh valida ou nao para habilitar ou nao o botao OK
void ModificarPorta::on_spinInput1_valueChanged(int arg1)
{
  // A linha a seguir nao tem nenhuma utilidade... Foi incluida apenas
  // para o compilador deixar de emitir advertencia de que arg1 nao estah sendo utilizado
  arg1++;

  testa_entradas_validas();
}

void ModificarPorta::on_spinInput2_valueChanged(int arg1)
{
  // A linha a seguir nao tem nenhuma utilidade... Foi incluida apenas
  // para o compilador deixar de emitir advertencia de que arg1 nao estah sendo utilizado
  arg1++;

  testa_entradas_validas();
}

void ModificarPorta::on_spinInput3_valueChanged(int arg1)
{
  // A linha a seguir nao tem nenhuma utilidade... Foi incluida apenas
  // para o compilador deixar de emitir advertencia de que arg1 nao estah sendo utilizado
  arg1++;

  testa_entradas_validas();
}

void ModificarPorta::on_spinInput4_valueChanged(int arg1)
{
  // A linha a seguir nao tem nenhuma utilidade... Foi incluida apenas
  // para o compilador deixar de emitir advertencia de que arg1 nao estah sendo utilizado
  arg1++;

  testa_entradas_validas();
}

// Sinaliza a alteracao das caracteristas da porta "idPorta"
// de acordo com os valores especificados pelo usuario
void ModificarPorta::on_buttonBox_accepted()
{
  // Recupera os valores escolhidos pelo usuario
  QString tipoPort = ui->comboTipoPorta->currentText();
  int numInputsPort = ui->spinNumInputs->value();
  int idInputPort[] = {ui->spinInput1->value(),
                       ui->spinInput2->value(),
                       ui->spinInput3->value(),
                       ui->spinInput4->value()};
  // Emite sinal com os parametros
  emit signModificarPorta(idPort, tipoPort, numInputsPort,
                          idInputPort[0], idInputPort[1], idInputPort[2], idInputPort[3]);
}

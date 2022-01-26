#ifndef MODIFICARPORTA_H
#define MODIFICARPORTA_H

#include <QDialog>

/* ======================================================================== *
 * ESSA EH A CLASSE QUE REPRESENTA A CAIXA DE DIALOGO PARA ALTERAR PORTAS   *
 * ======================================================================== */

namespace Ui {
class ModificarPorta;
}

class ModificarPorta : public QDialog
{
  Q_OBJECT

public:
  explicit ModificarPorta(QWidget *parent = 0);
  ~ModificarPorta();

public slots:
  // Fixa o limite inferior (-numInputs) e superior (numPortas)
  // dos spinBoxs que sao usados para indicar a origem das entradas das portas
  void slotSetRangeInputs(int minimo, int maximo);

  // Fixa as caracteristas da porta que estah sendo modificada
  // Depois exibe (show) a janela de modificar porta
  void slotShowModificarPorta(int IdPort, QString TipoPort, int NumInputsPort,
                              int IdInput0, int IdInput1, int IdInput2, int IdInput3);

private slots:
  // Sempre que modificar o tipo de porta, modifica os limites do spinBox que eh utilizado
  // para escolher o numero de entradas daquela porta:
  // NT: de 1 a 1
  // Demais: de 2 a 4
  void on_comboTipoPorta_currentIndexChanged(const QString &arg1);

  // Quando modifica o numero de entradas da porta, habilita apenas os spinBoxs que sao utilizados
  // para indicar a origem do sinal de entradas que existem na porta em questao
  void on_spinNumInputs_valueChanged(int arg1);

  // Quando modifica o valor da entrada de alguma das portas, testa se alguma delas tem valor
  // invalido (zero) e estah habilitada; se for o caso, desabilita o botao OK
  void on_spinInput1_valueChanged(int arg1);
  void on_spinInput2_valueChanged(int arg1);
  void on_spinInput3_valueChanged(int arg1);
  void on_spinInput4_valueChanged(int arg1);

  // Sinaliza a alteracao das caracteristas da porta "idPorta"
  // de acordo com os valores especificados pelo usuario
  void on_buttonBox_accepted();

private:
  Ui::ModificarPorta *ui;

  // Qual porta estah sendo modificada
  int idPort;

  // Testa se a origem (id) do sinal de alguma das entradas da porta tem valor invalido (zero)
  // e se a entrada estah habilitada; se for o caso, desabilita o botao OK
  void testa_entradas_validas();

signals:
  void signModificarPorta(int IdPort, QString TipoPort, int NumInputsPort,
                          int IdInput0, int IdInput1, int IdInput2, int IdInput3);
};

#endif // MODIFICARPORTA_H

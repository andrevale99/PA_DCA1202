#ifndef MODIFICARSAIDA_H
#define MODIFICARSAIDA_H

#include <QDialog>

/* ======================================================================== *
 * ESSA EH A CLASSE QUE REPRESENTA A CAIXA DE DIALOGO PARA ALTERAR SAIDAS   *
 * ======================================================================== */

namespace Ui {
class ModificarSaida;
}

class ModificarSaida : public QDialog
{
  Q_OBJECT

public:
  explicit ModificarSaida(QWidget *parent = 0);
  ~ModificarSaida();

public slots:
  // Fixa o limite inferior (-numInputs) e superior (numPortas)
  // do spinBox que eh usado para indicar a origem das saidas
  void slotSetRangeInputs(int minimo, int maximo);

  // Fixa a id e a origem da saida que estah sendo modificada
  // Depois exibe (show) a janela de modificar saida
  void slotShowModificarSaida(int IdSaida, int IdOrigemSaida);

private slots:
  // Quando modifica o valor da origem de uma saida, testa se o valor eh
  // invalido (zero); se for o caso, desabilita o botao OK
  void on_spinOrigemSaida_valueChanged(int arg1);

  // Sinaliza a alteracao da origem da saida "idSaida"
  // de acordo com o valor especificado pelo usuario
  void on_buttonBox_accepted();

private:
  Ui::ModificarSaida *ui;

  // Qual saida estah sendo modificada
  int idSaida;

signals:
  void signModificarSaida(int idSaida, int idOrigemSaida);
};

#endif // MODIFICARSAIDA_H

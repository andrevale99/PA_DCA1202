#ifndef NEWCIRCUITO_H
#define NEWCIRCUITO_H

#include <QDialog>

/* =============================================================================== *
 * ESSA EH A CLASSE QUE REPRESENTA A CAIXA DE DIALOGO PARA CRIAR NOVOS CIRCUITOS   *
 * =============================================================================== */

namespace Ui {
class NewCircuito;
}

class NewCircuito : public QDialog
{
  Q_OBJECT

public:
  explicit NewCircuito(QWidget *parent = 0);
  ~NewCircuito();

public slots:
  // Limpa e depois exibe (show) a janela de novo circuito
  void slotShowNewCircuito();

private slots:
  // Sinaliza a criacao de um novo circuito com as dimensoes especificadas pelo usuario
  // Deve emitir um sinal para a interface principal com os parametros escolhidos
  void on_NewCircuito_accepted();

private:
  Ui::NewCircuito *ui;

signals:
    void signNewCircuito(int, int, int);
};

#endif // NEWCIRCUITO_H

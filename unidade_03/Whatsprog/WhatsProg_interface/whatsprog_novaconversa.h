#ifndef NOVACONVERSA_H
#define NOVACONVERSA_H

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


#include <QDialog>

namespace Ui {
class WhatsProgNovaConversa;
}

class WhatsProgNovaConversa : public QDialog
{
  Q_OBJECT

public:
  explicit WhatsProgNovaConversa(QWidget *parent = 0);
  ~WhatsProgNovaConversa();

private slots:
  void on_buttonBox_accepted();
  signals:
  void signNova_Conversa(QString user);

private:
  Ui::WhatsProgNovaConversa *ui;
};

#endif // NOVACONVERSA_H

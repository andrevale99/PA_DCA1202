#ifndef NOVACONVERSA_H
#define NOVACONVERSA_H

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

private:
  Ui::WhatsProgNovaConversa *ui;
};

#endif // NOVACONVERSA_H

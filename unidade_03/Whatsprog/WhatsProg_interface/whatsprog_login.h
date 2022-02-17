#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

using namespace std;

namespace Ui {
class WhatsProgLogin;
}

class WhatsProgLogin : public QDialog
{
  Q_OBJECT

public:
  explicit WhatsProgLogin(QWidget *parent = 0);
  ~WhatsProgLogin();

public slots:
  void slotLogin(bool NovoUsuario);

private slots:
  void on_buttonBox_accepted();
    signals:
  void sign_newuser(QString ip, QString username, QString senha, bool eh_novo);

private:
  Ui::WhatsProgLogin *ui;

  /// Indica se a tentativa de login eh de um novo usuario (novoUsuario = true)
  /// ou de um usuario existente (novoUsuario = false)
  bool novoUsuario;
};

#endif // LOGINDIALOG_H

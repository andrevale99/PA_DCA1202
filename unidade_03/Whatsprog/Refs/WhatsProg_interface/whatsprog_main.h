#ifndef WHATSPROGMAIN_H
#define WHATSPROGMAIN_H

#include <QMainWindow>
#include <QLabel>
#include "whatsprog_login.h"
#include "whatsprog_novaconversa.h"

namespace Ui {
class WhatsProgMain;
}

class WhatsProgMain : public QMainWindow
{
  Q_OBJECT

private:
  Ui::WhatsProgMain *ui;

  /// Os icones do status das mensagens
  QPixmap pixEnviada;
  QPixmap pixRecebida;
  QPixmap pixEntregue;
  QPixmap pixLida;
  QPixmap pixOther;

  /// O texto da barra de status
  QLabel *msgStatus;

  /// As caixas de dialogo de login e de nova conversa
  WhatsProgLogin *loginDialog;
  WhatsProgNovaConversa *novaConversa;

public:
  explicit WhatsProgMain(QWidget *parent = 0);
  ~WhatsProgMain();

signals:
  /// Abrir a janela de login (novo usuario/usuario existente)
  void signLogin(bool NovoUsuario);

  /// Exibe a janela de nova conversa
  void signShowNovaConversa();

private slots:
  void on_actionNovo_usuario_triggered();
  void on_actionUsuario_existente_triggered();
  void on_actionDesconectar_triggered();
  void on_actionSair_triggered();
  void on_tableConversas_activated(const QModelIndex &index);
  void on_tableConversas_clicked(const QModelIndex &index);
  void on_lineEditMensagem_returnPressed();
  void on_actionNova_conversa_triggered();
  void on_actionRemover_conversa_triggered();
  void on_actionApagar_mensagens_triggered();
};

#endif // WHATSPROGMAIN_H

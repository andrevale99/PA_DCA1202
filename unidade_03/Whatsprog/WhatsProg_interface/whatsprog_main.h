#ifndef WHATSPROGMAIN_H
#define WHATSPROGMAIN_H

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


#include <QMainWindow>
#include <QLabel>

#include "whatsprog_login.h"
#include "whatsprog_novaconversa.h"
#include "whatsprog_login.h"
#include "WhatsProgThread.h"

#include "whatsprog_dados_cliente.h"
#include "whatsprog_dados.h"

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
  WhatsProgThread *thr_class;

public:
  explicit WhatsProgMain(QWidget *parent = 0);
  ~WhatsProgMain();

  bool aceita_User(const string &IP, const string &login, const string &senha, bool novosuario);
  void interface_Conectada(string user, string servidor);

  //void listar_Conversas();

signals:
  /// Abrir a janela de login (novo usuario/usuario existente)
  void signLogin(bool NovoUsuario);

  /// Exibe a janela de nova conversa
  void signShowNovaConversa();

  void lancar_thread();
  void terminar_thread();

private slots:
  void listar_Mensagens(IterConversa itr);
  void interface_Desconectada();
  void listar_Conversas();

  void slot_CriaLogar_Usuario(QString ip, QString usuario, QString senha, bool eh_novo);
  void slotNova_Conversa(QString usuario);
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

#include <QWindow>
#include <iostream>
#include "whatsprog_main.h"
#include "ui_whatsprog_main.h"

using namespace std;

/// Construtor da janela principal da interface
WhatsProgMain::WhatsProgMain(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::WhatsProgMain)
{
  ui->setupUi(this);

  // Cria caixas de dialogos de login e de nova conversa
  loginDialog = new WhatsProgLogin(this);
  novaConversa = new WhatsProgNovaConversa(this);

  // A lista da esquerda (conversas)
  ui->tableConversas->setColumnCount(2);
  ui->tableConversas->setShowGrid(false);
  ui->tableConversas->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableConversas->setSelectionMode(QAbstractItemView::SingleSelection);
  ui->tableConversas->setTabKeyNavigation(false);
  ui->tableConversas->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
  ui->tableConversas->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
  ui->tableConversas->horizontalHeader()->setSectionsClickable(false);
  // O cabecalho
  ui->tableConversas->setStyleSheet("QHeaderView::section { text-align:center }"
                                    "QHeaderView::section { font: bold }"
                                    "QHeaderView::section { background-color:lightgray }");
  ui->tableConversas->setHorizontalHeaderLabels(QStringList() << "N Msg" <<  "Usuario");

  // A lista da direita (mensagens)
  ui->tableMensagens->setColumnCount(3);
  ui->tableMensagens->setShowGrid(true);
  ui->tableMensagens->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->tableMensagens->setSelectionMode(QAbstractItemView::NoSelection);
  ui->tableMensagens->setTabKeyNavigation(false);
  ui->tableMensagens->horizontalHeader()->setSectionResizeMode(0,QHeaderView::ResizeToContents);
  ui->tableMensagens->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
  ui->tableMensagens->horizontalHeader()->setSectionResizeMode(2,QHeaderView::ResizeToContents);
  ui->tableMensagens->horizontalHeader()->setSectionsClickable(false);
  // O cabecalho
  ui->tableMensagens->setStyleSheet("QHeaderView::section { text-align:center }"
                                    "QHeaderView::section { font: bold }"
                                    "QHeaderView::section { background-color:lightgray }");
  ui->tableMensagens->setHorizontalHeaderLabels(QStringList() << "Id" << "Mensagem" <<  "St");

  // Os icones do status das mensagens
  QString dir = ".\\";
  QString pixfile;

  pixfile = dir+"status0.png";
  if (!pixEnviada.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  pixfile = dir+"status1.png";
  if (!pixRecebida.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  pixfile = dir+"status2.png";
  if (!pixEntregue.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  pixfile = dir+"status3.png";
  if (!pixLida.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  pixfile = dir+"status_other.png";
  if (!pixOther.load(pixfile,"PNG"))
  {
      cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }

  // A conexao dos sinais e slots

  // Os sinais da WhatsProgMain
  connect(this, &WhatsProgMain::signLogin,
          loginDialog, &WhatsProgLogin::slotLogin);
  connect(this, &WhatsProgMain::signShowNovaConversa,
          novaConversa, &WhatsProgNovaConversa::show);

  // O icone da aplicacao
  QPixmap pixIcon;
  pixfile = dir+"whatsprog_icon.png";
  if (!pixIcon.load(pixfile,"PNG"))
  {
    cerr << "Erro na leitura do pixmap " << pixfile.toStdString() << endl;
  }
  else
  {
    setWindowIcon(QIcon(pixIcon));
  }

  // A barra de status
  msgStatus = new QLabel("?");
  statusBar()->addWidget(msgStatus);
}

WhatsProgMain::~WhatsProgMain()
{
  delete ui;
}

void WhatsProgMain::on_actionNova_conversa_triggered()
{
  emit signShowNovaConversa();
}

void WhatsProgMain::on_actionRemover_conversa_triggered()
{

}

void WhatsProgMain::on_actionApagar_mensagens_triggered()
{

}

void WhatsProgMain::on_actionNovo_usuario_triggered()
{
  /// Exibe a janela de login para novo usuario (true)
  emit signLogin(true);
}

void WhatsProgMain::on_actionUsuario_existente_triggered()
{
  /// Exibe a janela de login para usuario existente (false)
  emit signLogin(false);
}

void WhatsProgMain::on_actionDesconectar_triggered()
{

}

void WhatsProgMain::on_actionSair_triggered()
{
  QCoreApplication::quit();
}

void WhatsProgMain::on_tableConversas_activated(const QModelIndex &index)
{
  on_tableConversas_clicked(index);
}

void WhatsProgMain::on_tableConversas_clicked(const QModelIndex &index)
{

}

void WhatsProgMain::on_lineEditMensagem_returnPressed()
{

}

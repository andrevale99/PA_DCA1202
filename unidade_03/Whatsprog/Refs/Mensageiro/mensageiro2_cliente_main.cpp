#include <QMessageBox>

#include "mysocket.h"
#include "mensageiro.h"
#include "mensageiro2_cliente_main.h"
#include "ui_mensageiro2_cliente_main.h"

// Variaveis globais
extern tcp_mysocket sock;

Mensageiro2_Cliente_Main::Mensageiro2_Cliente_Main(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::Mensageiro2_Cliente_Main),
  servidor(""),
  login(""),
  labelServidor(nullptr),
  labelLogin(nullptr),
  cl_thread(nullptr),
  cl_connect(nullptr)
{
  ui->setupUi(this);

  // Cria novos labels a serem inseridos na barra de status
  labelServidor = new QLabel(this);
  labelLogin = new QLabel(this);

  // Cria novo objeto gerenciador da thread
  cl_thread = new Mensageiro2_Cliente_Thread(this);

  // Cria caixas de dialogos de conexao
  cl_connect = new Mensageiro2_Cliente_Connect(this);

  // Largura das colunas da tabela de msg
  ui->msgTable->setColumnWidth(0,140);
  ui->msgTable->setColumnWidth(1,502);
  ui->msgTable->setColumnWidth(2,140);

  // Cabecalhos da tabela de msg
  ui->msgTable->setStyleSheet("QHeaderView::section { background-color:lightgray }");
  ui->msgTable->setHorizontalHeaderLabels(QStringList() << "REMETENTE" << "MENSAGEM" << "DESTINATARIO");

  // Widgets da barra de status
  statusBar()->insertWidget(0,new QLabel("Servidor: ",this));
  statusBar()->insertWidget(1,labelServidor);
  statusBar()->insertWidget(2,new QLabel("   Login: ",this));
  statusBar()->insertWidget(3,labelLogin);

  // Conexao dos sinais e slots

  // Sinais da classe Mensageiro2_Cliente_Connect
  // Avisa aa interface principal que ela deve tentar se conectar com servidor
  connect(cl_connect, &Mensageiro2_Cliente_Connect::signConectar,
          this, &Mensageiro2_Cliente_Main::slotConectarServidor);

  // Sinais da classe Mensageiro2_Cliente_Main
  // Avisa ao controlador de thread que a thread deve iniciar
  connect(this, &Mensageiro2_Cliente_Main::signExecutarThread,
          cl_thread, &Mensageiro2_Cliente_Thread::slotIniciar);
  // Avisa ao controlador de thread que a thread deve encerrar
  connect(this, &Mensageiro2_Cliente_Main::signEncerrarThread,
          cl_thread, &Mensageiro2_Cliente_Thread::slotEncerrar);

  // Sinais da classe Mensageiro2_Cliente_Thread
  // O controlador de thread avisa que a thread iniciou a execucao
  connect(cl_thread, &Mensageiro2_Cliente_Thread::signExibirThreadIniciada,
          this, &Mensageiro2_Cliente_Main::slotExibirInterfaceConectada);
  // O controlador de thread avisa que chegou mensagem
  connect(cl_thread, &Mensageiro2_Cliente_Thread::signExibirMsgRecebida,
          this, &Mensageiro2_Cliente_Main::slotExibirMsgRecebida);
  // O controlador de thread avisa que houve erro na recepcao
  connect(cl_thread, &Mensageiro2_Cliente_Thread::signExibirErro,
          this, &Mensageiro2_Cliente_Main::slotExibirErro);
  // O controlador de thread avisa que a thread se encerrou
  // Acao 1: colocar a interface em modo desconectado
  connect(cl_thread, &Mensageiro2_Cliente_Thread::signExibirThreadEncerrada,
          this, &Mensageiro2_Cliente_Main::slotExibirInterfaceDesconectada);
  // Acao 2: dar o join na thread que se encerrou
  connect(cl_thread, &Mensageiro2_Cliente_Thread::signExibirThreadEncerrada,
          cl_thread, &Mensageiro2_Cliente_Thread::slotEncerrar);

  // Inicia colocando a interface no modo desconectado
  slotExibirInterfaceDesconectada();
}

Mensageiro2_Cliente_Main::~Mensageiro2_Cliente_Main()
{
  delete ui;
}

bool Mensageiro2_Cliente_Main::connected() const
{
  return cl_thread->executing();
}

void Mensageiro2_Cliente_Main::desconectar()
{
  // Envia FIM para o servidor
  if (sock.connected()) sock.write_string("FIM");

  // Encerra a thread
  emit signEncerrarThread();

  // Limpa os dados da conexao
  servidor = login = "";
}

void Mensageiro2_Cliente_Main::slotConectarServidor(QString server, QString usuario)
{
  if (sock.connect(server.toStdString(), MENSAGEIRO_PORTA) == mysocket_status::SOCK_OK)
  {
    // Socket estah conectado
    if ( sock.write_string(usuario.toStdString()) == mysocket_status::SOCK_OK )
    {
      servidor = server;
      login = usuario;
      emit signExecutarThread();
    }
    else
    {
      // Emitir msg de erro: nao foi possivel enviar nome de login
      QMessageBox msgBox;
      msgBox.setWindowTitle("Erro na conexao");
      msgBox.setText("Nao foi possivel enviar nome de login "+usuario);
      msgBox.exec();
      servidor = login = "";
    }
  }
  else
  {
    // Emitir msg de erro: nao foi possivel conectar
    QMessageBox msgBox;
    msgBox.setWindowTitle("Erro na conexao");
    msgBox.setText("Nao foi possivel conectar com o servidor "+server);
    msgBox.exec();
    servidor = login = "";
  }
}

// Coloca a interface no modo conectado
void Mensageiro2_Cliente_Main::slotExibirInterfaceConectada()
{
  if (connected())
  {
    // Habilita o envio de msgs
    ui->userEdit->setEnabled(true);
    ui->msgEdit->setEnabled(true);
    ui->enviarButton->setEnabled(true);

    // Desabilita a acao de conectar
    ui->actionConectar->setEnabled(false);
    // Habilita a acao de desconectar
    ui->actionDesconectar->setEnabled(true);

    //Barra de status
    labelServidor->setText(servidor);
    labelLogin->setText(login);
  }
}

// Coloca a interface no modo desconectado
void Mensageiro2_Cliente_Main::slotExibirInterfaceDesconectada()
{
  if (!connected())
  {
    // Desabilita o envio de msgs
    ui->userEdit->setEnabled(false);
    ui->msgEdit->setEnabled(false);
    ui->enviarButton->setEnabled(false);

    // Habilita a acao de conectar
    ui->actionConectar->setEnabled(true);
    // Desabilita a acao de desconectar
    ui->actionDesconectar->setEnabled(false);

    //Barra de status
    labelServidor->setText("NAO CONECTADO");
    labelLogin->setText("NAO DEFINIDO");
  }
}

void Mensageiro2_Cliente_Main::exibirMsgEnviada(QString dest, QString msg)
{
  if (connected())
  {
    // Incrementa o numero de linhas da tabela de msgs
    int nMsg = ui->msgTable->rowCount();
    nMsg++;
    ui->msgTable->setRowCount(nMsg);

    // Cria os conteudos da nova linha
    QLabel *prov;
    // O remetente
    prov = new QLabel(login,this);
    prov->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    prov->setStyleSheet("background: C8FFC8"); // 200,255,200
    ui->msgTable->setCellWidget(nMsg-1,0,prov);
    // A mensagem
    prov = new QLabel(msg,this);
    prov->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    prov->setStyleSheet("background: C8FFC8"); // 200,255,200
    ui->msgTable->setCellWidget(nMsg-1,1,prov);
    // O destinatario
    prov = new QLabel(dest,this);
    prov->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    prov->setStyleSheet("background: C8FFC8"); // 200,255,200
    ui->msgTable->setCellWidget(nMsg-1,2,prov);
  }
}

void Mensageiro2_Cliente_Main::slotExibirMsgRecebida(QString remet, QString msg)
{
  if (connected())
  {
    // Incrementa o numero de linhas da tabela de msgs
    int nMsg = ui->msgTable->rowCount();
    nMsg++;
    ui->msgTable->setRowCount(nMsg);

    // Cria os conteudos da nova linha
    QTableWidgetItem *prov;
    // O remetente
    prov = new QTableWidgetItem(remet);
    prov->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    prov->setBackground(QBrush(QColor(255,255,255)));
    ui->msgTable->setItem(nMsg-1,0,prov);
    // A mensagem
    prov = new QTableWidgetItem(msg);
    prov->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    prov->setBackground(QBrush(QColor(255,255,255)));
    ui->msgTable->setItem(nMsg-1,1,prov);
    // O destinatario
    prov = new QTableWidgetItem(login);
    prov->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    prov->setBackground(QBrush(QColor(255,255,255)));
    ui->msgTable->setItem(nMsg-1,2,prov);
  }
}

void Mensageiro2_Cliente_Main::slotExibirErro(QString title, QString msg)
{
  QMessageBox msgBox;
  msgBox.setWindowTitle(title);
  msgBox.setText(msg);
  msgBox.exec();
}

void Mensageiro2_Cliente_Main::on_actionSair_triggered()
{
  desconectar();
  QCoreApplication::quit();
}

void Mensageiro2_Cliente_Main::on_actionConectar_triggered()
{
  if (!connected())
  {
    cl_connect->show();
  }
}

void Mensageiro2_Cliente_Main::on_actionApagar_triggered()
{
   ui->msgTable->clearContents();
   ui->msgTable->setRowCount(0);
}

void Mensageiro2_Cliente_Main::on_actionDesconectar_triggered()
{
  if (connected())
  {
    desconectar();
  }
}

void Mensageiro2_Cliente_Main::on_enviarButton_clicked()
{
  QString destinatario = ui->userEdit->text();
  QString msg = ui->msgEdit->text();
  mysocket_status iResult;

  if (connected())
  {
    if (msg.size()>0 && destinatario.size()>=4 && destinatario.size()<=16)
    {
      iResult = sock.write_string(destinatario.toStdString());
      if ( iResult == mysocket_status::SOCK_OK )
      {
        iResult = sock.write_string(msg.toStdString());
        if ( iResult == mysocket_status::SOCK_OK )
        {
          exibirMsgEnviada(destinatario,msg);
        }
        else
        {
          desconectar();

          // Emitir msg de erro: nao foi possivel enviar texto
          QMessageBox msgBox;
          msgBox.setWindowTitle("Erro no envio");
          msgBox.setText("Nao foi possivel enviar texto "+msg);
          msgBox.exec();
        }
      }
      else
      {
        desconectar();

        // Emitir msg de erro: nao foi possivel enviar nome de destinatario
        QMessageBox msgBox;
        msgBox.setWindowTitle("Erro no envio");
        msgBox.setText("Nao foi possivel enviar nome de destinatario "+destinatario);
        msgBox.exec();
      }
    }
    else
    {
      // Emitir msg de erro: dado(s) invalido(s)
      QMessageBox msgBox;
      msgBox.setWindowTitle("Erro no envio");
      msgBox.setText("Valor invalido de mensagem ("+msg+") e/ou destinatario ("+destinatario+")");
      msgBox.exec();
    }
  }
  ui->userEdit->clear();
  ui->msgEdit->clear();
}

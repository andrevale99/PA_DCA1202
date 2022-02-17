#include <QWindow>
#include <iostream>
#include "whatsprog_main.h"
#include "ui_whatsprog_main.h"
#include "mysocket.h"
#include <thread>
#include <QMessageBox>
#include <cstdio>
//#include "whatsprog_dados_cliente.h"

using namespace std;

extern WhatsProgDadosCliente DC;
extern std::thread thr;
extern tcp_mysocket sock;

/// Construtor da janela principal da interface
WhatsProgMain::WhatsProgMain(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::WhatsProgMain),
  loginDialog(nullptr),
  novaConversa(nullptr),
  msgStatus(nullptr),
  thr_class(nullptr)
{
  ui->setupUi(this);

  // Cria caixas de dialogos de login e de nova conversa
  loginDialog = new WhatsProgLogin(this);
  novaConversa = new WhatsProgNovaConversa(this);
  thr_class = new WhatsProgThread(this);

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
  //QString dir = ".\\"; //No windows
  QString dir = "/home/andre/Downloads/WhatsProg_interface/";
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

  connect(novaConversa, SIGNAL(signNova_Conversa(QString)),
          this, SLOT(slotNova_Conversa(QString)));

  connect(loginDialog, SIGNAL(sign_newuser(QString, QString, QString, bool)),
          this, SLOT(slot_CriaLogar_Usuario(QString,QString,QString,bool)));

  //Nova msg vinda da thread
  connect(thr_class, SIGNAL(conf_leitura(IterConversa)),
          this, SLOT(listar_Mensagens(IterConversa)));

  //Visualizar conversas
  connect(thr_class, SIGNAL(nova_msg(IterConversa)),
          this, SLOT(listar_Conversas()));

  //Lanca a thread
  connect(this, SIGNAL(lancar_thread()),
          thr_class, SLOT(conf_lancar_thread()));

  //Finalizar thread cliente
  connect(this, SIGNAL(terminar_thread()),
          thr_class, SLOT(matar_thread()));

  connect(thr_class, SIGNAL(desconectado()),
          this, SLOT(interface_Desconectada()));

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
    IterConversa it = DC.getConversaAtual();
    DC.eraseConversa(it);
    //listar_Conversas();
    on_actionApagar_mensagens_triggered();
    ui->labelMensagens->setText("NAO HA MENSAGEM SELECIONADA");
}

void WhatsProgMain::on_actionApagar_mensagens_triggered()
{
    IterConversa it = DC.getConversaAtual();
    DC.clearMessages(it);
    ui->tableMensagens->clearContents();
    ui->tableMensagens->setRowCount(0);
    //listar_Conversas();
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

   interface_Desconectada();
   ui->tableMensagens->setRowCount(0);

    emit terminar_thread();
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
    string destinatario = ui->tableConversas->cellWidget(index.row(), 1)->property("text").toString().toStdString();

    if(!DC.setConversaAtual(destinatario)) QMessageBox::critical(this, "ERRO", "ERRO AO SETAR CONVERSA");
    if(!DC.moveConversaToBegin(DC.getConversaAtual())) QMessageBox::critical(this, "ERRO", "ERRO AO MANDAR PARA O INICIO");

    ui->labelMensagens->setText("CONVERSA: " + QString(destinatario.c_str()));
    listar_Conversas();

    ui->tableConversas->cellWidget(0, 0)->setStyleSheet("font-weight: bold");
    ui->tableConversas->cellWidget(0, 1)->setStyleSheet("font-weight: bold");

    //Exibi as mensagens ao clicar
    listar_Mensagens(DC.getConversaAtual());
}

void WhatsProgMain::on_lineEditMensagem_returnPressed()
{
    IterConversa it = DC.getConversaAtual();
    Mensagem M;
    string mensagem = ui->lineEditMensagem->text().toStdString();

    if(!testarTextoMensagem(mensagem)) QMessageBox::critical(this, "ERRO AO ENVIAR MENSAGEM", "O texto seguinte é muito longo");
        if (!M.setId(DC.getNovaIdMensagem()) ||
            !M.setRemetente(DC.getMeuUsuario()) ||
            !M.setDestinatario(it->getCorrespondente()) ||
            !M.setTexto(mensagem) ||
            !M.setStatus(MsgStatus::MSG_ENVIADA))
        {
          sock.close();
          QMessageBox::critical(this, "ERRO MENSAGEM", "Mensagem inválida");
        }
        DC.pushMessage(it, M);
            bool envioOK = (sock.write_int(CMD_NOVA_MSG) != mysocket_status::SOCK_ERROR);
            if (envioOK) envioOK = (sock.write_int(M.getId()) != mysocket_status::SOCK_ERROR);
            if (envioOK) envioOK = (sock.write_string(M.getDestinatario()) != mysocket_status::SOCK_ERROR);
            if (envioOK) envioOK = (sock.write_string(M.getTexto()) != mysocket_status::SOCK_ERROR);
            if (envioOK)
            {
              DC.moveConversaToBegin(it);
            }
            else
            {
              DC.popMessage(it);
              sock.close();
              QMessageBox::critical(this, "ERRO", "Erro no envio da mensagem");
            }
        ui->tableMensagens->clearContents();
        ui->lineEditMensagem->clear();
        listar_Mensagens(it);
}

void WhatsProgMain::slot_CriaLogar_Usuario(QString ip, QString usuario, QString senha, bool eh_novo)
{
    string newIp = ip.toStdString();
    string newUsuario = usuario.toStdString();
    string newSenha = senha.toStdString();
    if(!aceita_User(newIp, newUsuario, newSenha, eh_novo))
    {
        if(eh_novo)
            QMessageBox::critical(this, "Erro ao criar usuário", "Houve algum problema");

        else
            QMessageBox::critical(this, "Erro ao logar usuário", "Houve algum problema");
    }

    //qDebug() << "ANTES\n";
    emit lancar_thread();
    //qDebug() << "DEPOIS\n";
}

void WhatsProgMain::slotNova_Conversa(QString usuario)
{
    string destinatario = usuario.toStdString();
    if(!testarNomeUsuario(destinatario)) QMessageBox::critical(this, "ERRO", "Problema com o nome do destinatário");
    if(DC.insertConversa(destinatario))
        QMessageBox::critical(this, "ERRO", "Nova conversa com " + QString(destinatario.c_str()) + " é invalida.");

    //qDebug() << "TESTE: " << DC.insertConversa(destinatario) << '\n';
    ui->menuMensagens->setEnabled(1);
    ui->lineEditMensagem->setEnabled(1);
    DC.setConversaAtual(destinatario);
    //qDebug() << "TESTE 02: " << DC.setConversaAtual(destinatario) << '\n';
    //listar_Conversas();
}

bool WhatsProgMain::aceita_User(const string &IP, const string &login, const string &senha, bool novoUsuario)
{
    if (sock.connected())
      {
        QMessageBox::critical(this, "ERRO", "Esta funcao soh deve ser chamada quando o cliente estah desconectado");
        return false;
      }
      bool conexaoOK(true);
      int32_t cmd;
      if (sock.connect(IP.c_str(), PORTA_WHATSPROG) != mysocket_status::SOCK_OK)
      {
        sock.close();
        return false;
      }
      if (novoUsuario)
      {
        if (conexaoOK) conexaoOK = (sock.write_int(CMD_NEW_USER) != mysocket_status::SOCK_ERROR);
      }
      else
      {
        if (conexaoOK) conexaoOK = (sock.write_int(CMD_LOGIN_USER) != mysocket_status::SOCK_ERROR);
        //listar_Conversas();
      }
      if (!conexaoOK) return false;
      if (conexaoOK) conexaoOK = (sock.write_string(login) != mysocket_status::SOCK_ERROR);
      if (conexaoOK) conexaoOK = (sock.write_string(senha) != mysocket_status::SOCK_ERROR);
      if (!conexaoOK)
      {
        sock.close();
        return false;
      }
      conexaoOK = (sock.read_int(cmd,1000*TIMEOUT_WHATSPROG) == mysocket_status::SOCK_OK);
      if (conexaoOK) conexaoOK = (cmd == CMD_LOGIN_OK);
      if (!conexaoOK)
      {
        sock.close();
        return false;
      }
      DC.setServidorUsuario(IP,login);
      DC.ler();
      if (DC.size() == 1)
      {
        DC.setConversaAtual(DC.begin());
      }
      interface_Conectada(DC.getMeuUsuario(), DC.getServidor());
      return true;
}

void WhatsProgMain::interface_Desconectada()
{
    ui->tableConversas->clearContents();
    ui->tableMensagens->clearContents();

    msgStatus->setText("NAO CONECTADO");
    statusBar()->insertWidget(0, msgStatus);

    ui->lineEditMensagem->setEnabled(0);
    ui->menuMensagens->setEnabled(0);
    ui->menuConversa->setEnabled(0);
}

void WhatsProgMain::interface_Conectada(string user, string servidor)
{
    string user_servidor = "CONECTADO: " + user + "@" + servidor;
    msgStatus->setText(user_servidor.c_str());
    statusBar()->insertWidget(0, msgStatus);
    ui->menuConversa->setEnabled(1);
}

void WhatsProgMain::listar_Mensagens(IterConversa it)
{
    bool enviado = true;
    QLabel *prov;

    ui->tableMensagens->clearContents();
    ui->tableMensagens->setRowCount(it->size());
    for(unsigned i=0; i<it->size(); i++)
    {
        int id = it->getMensagem(i).getId();
        QString idString = QString::number(id);
        prov = new QLabel(idString.toStdString().c_str());
        prov->setAlignment(Qt::AlignCenter);
        if(it->getMensagem(i).getRemetente() == DC.getMeuUsuario()) prov->setStyleSheet("background: lightgreen");
        ui->tableMensagens->setCellWidget(i, 0, prov);

        if(it->getMensagem(i).getRemetente() == DC.getMeuUsuario()){
            string mensagem = it->getMensagem(i).getTexto();
            prov = new QLabel(mensagem.c_str());
            prov->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
            prov->setStyleSheet("background: lightgreen");
            ui->tableMensagens->setCellWidget(i, 1, prov);
            if (it->getMensagem(i).getStatus() == MsgStatus::MSG_ENVIADA){
                prov = new QLabel();
                prov->setPixmap(pixEnviada);
            }
            if (it->getMensagem(i).getStatus() == MsgStatus::MSG_RECEBIDA){
                prov = new QLabel();
                prov->setPixmap(pixRecebida);
            }
            if (it->getMensagem(i).getStatus() == MsgStatus::MSG_ENTREGUE){
                prov = new QLabel();
                prov->setPixmap(pixEntregue);
            }
            if (it->getMensagem(i).getStatus() == MsgStatus::MSG_LIDA){
                prov = new QLabel();
                prov->setPixmap(pixLida);
            }
            prov->setScaledContents(true);
            ui->tableMensagens->setCellWidget(i, 2, prov);
        }

        else
        {
            string mensagem = it->getMensagem(i).getTexto();
            prov = new QLabel(mensagem.c_str());
            prov->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
            ui->tableMensagens->setCellWidget(i, 1, prov);

            prov = new QLabel();
            prov->setPixmap(pixOther);
            prov->adjustSize();
            ui->tableMensagens->setCellWidget(i, 2, prov);
            if(it->getMensagem(i).getStatus() == MsgStatus::MSG_ENTREGUE){
                if(enviado) enviado = (sock.write_int(CMD_MSG_LIDA1) != mysocket_status::SOCK_ERROR);
                if(enviado) enviado = (sock.write_int(it->getMensagem(i).getId()) != mysocket_status::SOCK_ERROR);
                if(enviado) enviado = (sock.write_string(it->getMensagem(i).getRemetente()) != mysocket_status::SOCK_ERROR);
                if(enviado){
                    DC.setStatus(it,i,MsgStatus::MSG_LIDA);
                }
            }
        }
        ui->tableMensagens->viewport()->update();
    }

    ui->tableConversas->viewport()->update();

    if (!enviado){
        sock.close();
        QMessageBox::critical(this, "ERRO", "Falha no envio das confimacoes de visualizacao");
    }

}

void WhatsProgMain::listar_Conversas()
{
    ui->tableConversas->clearContents();
    ui->tableConversas->setRowCount(DC.size());

    QLabel *prov;
    unsigned cont(0);

    for(IterConversa it=DC.begin(); it!=DC.end(); it++)
    {
        QString nMensagens = QString::number(it->getNumMsgsEntregues()) + "/" + QString::number(it->size());

        prov = new QLabel(nMensagens);
        prov->setAlignment(Qt::AlignCenter);

        ui->tableConversas->setCellWidget(cont, 0, prov);
        QString destinatario = it->getCorrespondente().c_str();

        prov = new QLabel(destinatario);
        prov->setAlignment(Qt::AlignCenter);

        ui->tableConversas->setCellWidget(cont, 1, prov);
        if(it->getNumMsgsEntregues() > 0)
        {
            ui->tableConversas->cellWidget(cont, 0)->setStyleSheet("background-color:lightgreen");
            ui->tableConversas->cellWidget(cont, 1)->setStyleSheet("background-color:lightgreen");
        }
        else
        {
            ui->tableConversas->cellWidget(cont, 0)->setStyleSheet("background-color:white");
            ui->tableConversas->cellWidget(cont, 1)->setStyleSheet("background-color:white");
        }

        cont++;
    }
    ui->tableConversas->viewport()->update();
}

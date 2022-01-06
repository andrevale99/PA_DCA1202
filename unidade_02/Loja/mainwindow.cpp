#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
*   @brief Construtor para inicializar a janela
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Init_Janelas();

    total_itens = new QLabel("-");

    statusBar()->insertWidget(0, new QLabel("Total de Itens: "));
    statusBar()->insertWidget(1, total_itens);

    connect(
            inclLivro,
            SIGNAL(signIncluirLivro(QString,QString,QString)),
            this,
            SLOT(slotIncluirLivro(QString,QString,QString))
            );

    connect(
             inclCD,
                SIGNAL(signIncluirCD(QString,QString,QString)),
                this,
                SLOT(slotIncluirCD(QString,QString,QString))
           );

    connect(
             inclDVD,
                SIGNAL(signIncluirDVD(QString,QString,QString)),
                this,
                SLOT(slotIncluirDVD(QString,QString,QString))
           );

    setupConfirmacao();
}

/**
*   @brief Destrutor
*/
MainWindow::~MainWindow()
{
    delete inclLivro;
    delete inclCD;
    delete inclDVD;

    delete total_itens;

    delete confirmacao;

    delete ui;
}

/**
*   @brief Mostra Todos os itens presentes na Loja
*          (serve para atualização)
*/
void MainWindow::show_Loja()
{
    show_livros();
    show_CDs();
    show_DVDs();

    refresh_totalItens();
}

/**
*   @brief Mostra os itens presentes do tipo LIVRO no QTableWidget
*/
void MainWindow::show_livros()
{
    ui->table_livros->clearContents();

    ui->table_livros->setRowCount(x.getNumLivro());

    for(int i=0; i<ui->table_livros->rowCount(); ++i)
    {

        for(int j=0; j<=2; ++j)
        {
            prov = new QLabel();

            Livro L = x.getLivro(i);

            if(j==0)
            {
                info = QString::fromStdString(L.getNome());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->table_livros->setCellWidget(i, j, prov);
            }

            else if(j==1)
            {
                info = QString::number(L.getPreco(), 'f', 2);
                prov->setText(info);
                prov->setAlignment(Qt::AlignRight | Qt::AlignCenter);
                ui->table_livros->setCellWidget(i, j, prov);
            }

            else
            {
                info = QString::fromStdString(L.getAutor());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->table_livros->setCellWidget(i, j, prov);
            }

        }
    }
}

/**
*   @brief Mostra os itens presentes do tipo CD no QTableWidget
*/
void MainWindow::show_CDs()
{
    ui->table_cds->clearContents();

    ui->table_cds->setRowCount(x.getNumCD());

    for(int i=0; i<ui->table_cds->rowCount(); ++i)
    {

        for(int j=0; j<=2; ++j)
        {
            prov = new QLabel();
            CD cd = x.getCD(i);

            if(j==0)
            {
                info = QString::fromStdString(cd.getNome());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->table_cds->setCellWidget(i, j, prov);
            }

            else if(j==1)
            {
                info = QString::number(cd.getPreco(), 'f', 2);
                prov->setText(info);
                prov->setAlignment(Qt::AlignRight | Qt::AlignCenter);
                ui->table_cds->setCellWidget(i, j, prov);
            }

            else
            {
                info = QString::number(cd.getNumFaixas());
                prov->setText(info);
                prov->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
                ui->table_cds->setCellWidget(i, j, prov);
            }

        }
    }
}

/**
*   @brief Mostra os itens presentes do tipo DVD no QTableWidget
*/
void MainWindow::show_DVDs()
{
    ui->table_dvds->clearContents();

    ui->table_dvds->setRowCount(x.getNumDVD());

    for(int i=0; i<ui->table_dvds->rowCount(); ++i)
    {

        for(int j=0; j<=2; ++j)
        {
            prov = new QLabel();
            DVD dvd = x.getDVD(i);

            if(j==0)
            {
                info = QString::fromStdString(dvd.getNome());
                prov->setText(info);
                prov->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
                ui->table_dvds->setCellWidget(i, j, prov);
            }

            else if(j==1)
            {
                info = QString::number(dvd.getPreco(), 'f', 2);
                prov->setText(info);
                prov->setAlignment(Qt::AlignRight | Qt::AlignCenter);
                ui->table_dvds->setCellWidget(i, j, prov);
            }

            else
            {
                info = QString::number(dvd.getDuracao());
                prov->setText(info);
                prov->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
                ui->table_dvds->setCellWidget(i, j, prov);
            }

        }
    }
}

//=========================================================================================
//              ACTIONS
//====================================================================================================

/**
*   @brief Salva o arquivo Loja.txt
*/
void MainWindow::on_actionSalvar_triggered()
{
    QString fileName = QFileDialog::
          getSaveFileName(this,
                          "Save File",
                          QString(),
                          "Text Files (*.txt);;C++ Files (*.cpp *.h)");

      if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
          // error message
          QMessageBox::critical(this, tr("Error"), tr("O arquivo nao pode ser salvo"));
          return;
        }
        x.salvar(fileName.toStdString());
        file.close();
      }
}

/**
*   @brief Ler o arquivo Loja.txt
*/
void MainWindow::on_actionLer_triggered()
{
    QString fileName = QFileDialog::
         getOpenFileName(this,
                         "Open File",
                         QString(),
                         "Text Files (*.txt);;C++ Files (*.cpp *.h)");

     if (!fileName.isEmpty()) {
       QFile file(fileName);
       if (!file.open(QIODevice::ReadOnly)) {
         // error message
         QMessageBox::critical(this, tr("Error"), tr("O arquivo nao pode ser aberto"));
         return;
       }
     }

     x.ler(fileName.toStdString());

     show_Loja();
}

/**
*   @brief Desliga o programa quando apertar em Arquivo->Sair
*/
void MainWindow::on_actionSair_triggered()
{
    QCoreApplication::exit();
}

/**
*   @brief Abre a janela de dialogo para inserir um Livro
*/
void MainWindow::on_actionIncluir_Livro_triggered()
{
    inclLivro->exec();
    show_Loja();
}

/**
*   @brief Abre a janela de dialogo para inserir um CD
*/
void MainWindow::on_actionIncluir_CD_triggered()
{
    inclCD->exec();
    show_Loja();
}

/**
*   @brief Abre a janela de dialogo para inserir um DVD
*/
void MainWindow::on_actionIncluir_DVD_triggered()
{
    inclDVD->exec();
    show_Loja();
}

/**
*   @brief Exclui LIVRO quando da click duplo no item e atualiza a lista
*/
void MainWindow::on_table_livros_cellDoubleClicked(int row, int column)
{
    butao_pressionado = confirmacao->exec();

    switch (butao_pressionado) {
        case QMessageBox::Ok:
        {
            if(row < 0 || static_cast<unsigned>(row) > x.getNumLivro())
                return;

            x.excluirLivro(row);
            show_Loja();
        }
        break;

    case QMessageBox::Cancel:
        butao_pressionado = -1;
        break; //faz nada
    }


}

/**
*   @brief Exclui CD quando da click duplo no item e atualiza a lista
*/
void MainWindow::on_table_cds_cellDoubleClicked(int row, int column)
{
    butao_pressionado = confirmacao->exec();

    switch (butao_pressionado) {
        case QMessageBox::Ok:
        {
            if(row < 0 || static_cast<unsigned>(row) > x.getNumCD())
                return;

            x.excluirCD(row);
            show_Loja();
        }
        break;

        case QMessageBox::Cancel:
            butao_pressionado = -1;
            break; //faz nada
    }


}

/**
*   @brief Exclui DVD quando da click duplo no item e atualiza a lista
*/
void MainWindow::on_table_dvds_cellDoubleClicked(int row, int column)
{
    butao_pressionado = confirmacao->exec();

    switch (butao_pressionado) {
        case QMessageBox::Ok:
        {
            if(row < 0 || static_cast<unsigned>(row) > x.getNumDVD())
                return;

            x.excluirDVD(row);
            show_Loja();
        }
        case QMessageBox::Cancel:
            butao_pressionado = -1;
            break; //faz nada
    }
}

//=========================================================================================
//              SLOTS
//====================================================================================================

/**
*   @brief Slot para colocar um novo Livro
*/
void MainWindow::slotIncluirLivro(QString nome, QString preco, QString autor)
{
    Livro temp(nome.toStdString(), preco.toFloat()*100., autor.toStdString());
    x.incluirLivro(temp);
    show_Loja();
}

/**
*   @brief Slot para colocar um novo CD
*/
void MainWindow::slotIncluirCD(QString nome, QString preco, QString numfaixas)
{
    //qDebug() << "PASSOU AQUI CDS\n";
    CD temp(nome.toStdString(), preco.toFloat()*100, numfaixas.toInt());
    x.incluirCD(temp);
    show_Loja();
}

/**
*   @brief Slot para colocar um novo DVD
*/
void MainWindow::slotIncluirDVD(QString nome, QString preco, QString duracao)
{
    //qDebug() << "PASSOU AQUI DVDS\n";
    DVD temp(nome.toStdString(), preco.toFloat()*100, duracao.toFloat());
    x.incluirDVD(temp);
    show_Loja();
}

//=========================================================================================
//              MEMBROS PRIVADOS
//====================================================================================================

/**
 * @brief Atualiza a qauntidade de itens da loja no Status Bar
*/
void MainWindow::refresh_totalItens()
{
    QString ti = QString::number(x.getNumCD() + x.getNumDVD() + x.getNumLivro());
    total_itens->setText(ti);
}

/**
 * @brief Realiza as configurações iniciais
*/
void MainWindow::setupMain()
{
    setupLabels();
}

//=========================================================================================
//              CONFIGURAÇÕES INICIAIS
//====================================================================================================

/**
*   @brief Inicializa todas as janelas (aloca espaço para a memória
*          para poderem aparecer na tela)
*/
void MainWindow::Init_Janelas()
{
    inclLivro = new IncluirLivro();
    inclCD = new IncluirCD();
    inclDVD = new IncluirDVD();

    confirmacao = new QMessageBox();
}

/**
*   @brief Realiza as configurações iniciais
*          como colocar os butões e o Icone
*/
void MainWindow::setupConfirmacao()
{
    confirmacao->setWindowTitle("Exclusao de item");
    confirmacao->setIcon(QMessageBox::Warning);

    confirmacao->addButton(QMessageBox::Ok);
    confirmacao->addButton(QMessageBox::Cancel);

    confirmacao->setText("Realmente deseja excluir o ITEM ?");

    confirmacao->setDefaultButton(QMessageBox::Cancel);

    //qDebug() << "TESTE: " << teste << '\n';
}


/**
 * @brief Função para configurar as Labels
*/
void MainWindow::setupLabels()
{
    ui->label_livros->setStyleSheet("lightgray");
    ui->label_cds->setStyleSheet("lightgray");
    ui->label_dvds->setStyleSheet("lightgray");
}




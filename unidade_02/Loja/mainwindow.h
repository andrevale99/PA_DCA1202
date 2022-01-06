#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "loja.h"
#include "incluirlivro.h"
#include "incluircd.h"
#include "incluirdvd.h"

#include <QMainWindow>
#include <QFileDialog>
#include <QDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QTableWidgetItem>
#include <QDebug>
#include <QStatusBar>
#include <QLabel>
#include <QVBoxLayout>

using std::string;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void show_Loja();

    void show_livros();
    void show_CDs();
    void show_DVDs();

private slots:
    void on_actionSair_triggered();

    void on_actionSalvar_triggered();
    void on_actionLer_triggered();

    void on_actionIncluir_Livro_triggered();
    void on_actionIncluir_CD_triggered();
    void on_actionIncluir_DVD_triggered();

    void on_table_livros_cellDoubleClicked(int row, int column);
    void on_table_cds_cellDoubleClicked(int row, int column);
    void on_table_dvds_cellDoubleClicked(int row, int column);

    void slotIncluirLivro(QString nome, QString preco, QString autor);
    void slotIncluirCD(QString nome, QString preco, QString numfaixas);
    void slotIncluirDVD(QString nome, QString preco, QString duracao);

private:
    void Init_Janelas();

    void refresh_totalItens();

    void setupMain();
    void setupLabels();
    void setupDialog();
    void setupConfirmacao();

    Ui::MainWindow *ui;

    QMessageBox *confirmacao;
    int butao_pressionado; //  Variável para pegar o valor de qual butão
                           //da janela de confirmação foi pressionado

    Loja x;

    IncluirLivro *inclLivro;
    IncluirCD *inclCD;
    IncluirDVD *inclDVD;

    QLabel *total_itens;
    QLabel *prov;

    QString info;

};
#endif // MAINWINDOW_H

#include "incluirdvd.h"
#include "ui_incluirdvd.h"

IncluirDVD::IncluirDVD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirDVD)
{
    ui->setupUi(this);

    erro_box = new QMessageBox();
    erro_box->setIcon(QMessageBox::Critical);
    erro_box->setWindowTitle("DVD Invalido");
}

IncluirDVD::~IncluirDVD()
{
    delete ui;
}

/**
 * @brief Limpa os lineEdits anteriores
*/
void IncluirDVD::clear()
{
    ui->lineEdit_duracao->setText("");
    ui->lineEdit_nome->setText("");
    ui->lineEdit_preco->setText("");
}

/**
 * @brief Quando apertar em "OK" emite os sinais com as variáveis
*/
void IncluirDVD::on_buttonBox_accepted()
{
    nome = ui->lineEdit_nome->text();
    preco = ui->lineEdit_preco->text();
    duracao = ui->lineEdit_duracao->text();

    if(preco.toFloat() < 0 || nome.isEmpty() || duracao.toFloat() <= 0)
    {
        erro_string = "Nao foi possivel incluir o LIVRO:\n";
        erro_string += "Nome = " + nome + "\n";
        erro_string += "Preco = " + preco +"\n";
        erro_string += "Duracao = " + duracao;

        erro_box->setText(erro_string);
        erro_box->exec();

        return;
    }

    clear();

    emit signIncluirDVD(nome, preco, duracao);
}


#include "incluircd.h"
#include "ui_incluircd.h"

IncluirCD::IncluirCD(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirCD)
{
    ui->setupUi(this);

    erro_box = new QMessageBox();
    erro_box->setIcon(QMessageBox::Critical);
    erro_box->setWindowTitle("CD Invalido");
}

IncluirCD::~IncluirCD()
{
    delete erro_box;
    delete ui;
}

/**
 * @brief Limpa os lineEdits anteriores
*/
void IncluirCD::clear()
{
    ui->lineEdit_faixas->setText("");
    ui->lineEdit_nome->setText("");
    ui->lineEdit_preco->setText("");
}

/**
 * @brief Quando apertar em "OK" emite os sinais com as variáveis
*/
void IncluirCD::on_buttonBox_accepted()
{
    nome = ui->lineEdit_nome->text();
    preco = ui->lineEdit_preco->text();
    numfaixas = ui->lineEdit_faixas->text();

    if(preco.toFloat() < 0 || nome.isEmpty() || numfaixas.toInt() <= 0)
    {
        erro_string = "Nao foi possivel incluir o CD\n";
        erro_string += "Nome = " + nome + "\n";
        erro_string += "Preco = " + preco + "\n";
        erro_string += "NumFaixas = " + numfaixas;

        erro_box->setText(erro_string);
        erro_box->exec();

        return;
    }

    clear();

    emit signIncluirCD(nome, preco, numfaixas);
}


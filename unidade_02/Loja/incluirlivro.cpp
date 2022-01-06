#include "incluirlivro.h"
#include "ui_incluirlivro.h"

IncluirLivro::IncluirLivro(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IncluirLivro)
{
    ui->setupUi(this);

    erro_box = new QMessageBox();
    erro_box->setIcon(QMessageBox::Critical);
    erro_box->setWindowTitle("LIVRO Invalido");
}

IncluirLivro::~IncluirLivro()
{
    delete erro_box;
    delete ui;
}

/**
 * @brief Limpa os lineEdits anteriores
*/
void IncluirLivro::clear()
{
    ui->lineEdit_autor->setText("");
    ui->lineEdit_nome->setText("");
    ui->lineEdit_preco->setText("");
}

/**
 * @brief Quando apertar em "OK" emite os sinais com as variáveis
*/
void IncluirLivro::on_buttonBox_accepted()
{
    nome = ui->lineEdit_nome->text();
    preco = ui->lineEdit_preco->text();
    autor = ui->lineEdit_autor->text();

    if(preco.toFloat() < 0 || nome.isEmpty() || autor.isEmpty())
    {
        erro_string = "Nao foi possivel incluir o LIVRO:\n";
        erro_string += "Nome = " + nome + "\n";
        erro_string += "Preco = " + preco +"\n";
        erro_string += "Autor = " + autor;

        erro_box->setText(erro_string);
        erro_box->exec();

        return;
    }

    clear();

    emit signIncluirLivro(nome, preco, autor);
}



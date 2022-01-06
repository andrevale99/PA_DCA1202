#ifndef INCLUIRLIVRO_H
#define INCLUIRLIVRO_H

#include <QDialog>
#include <QString>
#include <QDebug>
#include <QMessageBox>

namespace Ui {
class IncluirLivro;
}

class IncluirLivro : public QDialog
{
    Q_OBJECT

public:
    explicit IncluirLivro(QWidget *parent = nullptr);
    ~IncluirLivro();

    void clear();

private slots:
    void on_buttonBox_accepted();

signals:
    void signIncluirLivro(QString nome, QString preco, QString autor);

private:
    Ui::IncluirLivro *ui;

    QMessageBox *erro_box;

    QString erro_string;

    QString nome;
    QString preco;
    QString autor;

};

#endif // INCLUIRLIVRO_H

#ifndef INCLUIRDVD_H
#define INCLUIRDVD_H

#include <QDialog>
#include <QString>
#include <QMessageBox>

namespace Ui {
class IncluirDVD;
}

class IncluirDVD : public QDialog
{
    Q_OBJECT

public:
    explicit IncluirDVD(QWidget *parent = nullptr);
    ~IncluirDVD();

    void clear();

private slots:
    void on_buttonBox_accepted();

signals:
    void signIncluirDVD(QString nome, QString preco, QString duracao);

private:
    Ui::IncluirDVD *ui;

    QMessageBox *erro_box;

    QString erro_string;

    QString nome;
    QString preco;
    QString duracao;
};

#endif // INCLUIRDVD_H

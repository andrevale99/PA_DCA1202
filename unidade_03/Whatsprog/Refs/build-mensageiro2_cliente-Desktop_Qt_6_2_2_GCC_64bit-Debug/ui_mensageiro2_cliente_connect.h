/********************************************************************************
** Form generated from reading UI file 'mensageiro2_cliente_connect.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENSAGEIRO2_CLIENTE_CONNECT_H
#define UI_MENSAGEIRO2_CLIENTE_CONNECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_Mensageiro2_Cliente_Connect
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *labelServidor;
    QLabel *labelLogin;
    QLineEdit *editServidor;
    QLineEdit *editLogin;

    void setupUi(QDialog *Mensageiro2_Cliente_Connect)
    {
        if (Mensageiro2_Cliente_Connect->objectName().isEmpty())
            Mensageiro2_Cliente_Connect->setObjectName(QString::fromUtf8("Mensageiro2_Cliente_Connect"));
        Mensageiro2_Cliente_Connect->resize(235, 125);
        buttonBox = new QDialogButtonBox(Mensageiro2_Cliente_Connect);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(20, 80, 200, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(false);
        labelServidor = new QLabel(Mensageiro2_Cliente_Connect);
        labelServidor->setObjectName(QString::fromUtf8("labelServidor"));
        labelServidor->setGeometry(QRect(10, 10, 60, 16));
        labelLogin = new QLabel(Mensageiro2_Cliente_Connect);
        labelLogin->setObjectName(QString::fromUtf8("labelLogin"));
        labelLogin->setGeometry(QRect(10, 40, 60, 16));
        editServidor = new QLineEdit(Mensageiro2_Cliente_Connect);
        editServidor->setObjectName(QString::fromUtf8("editServidor"));
        editServidor->setGeometry(QRect(80, 10, 140, 20));
        editLogin = new QLineEdit(Mensageiro2_Cliente_Connect);
        editLogin->setObjectName(QString::fromUtf8("editLogin"));
        editLogin->setGeometry(QRect(80, 40, 140, 20));

        retranslateUi(Mensageiro2_Cliente_Connect);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, Mensageiro2_Cliente_Connect, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, Mensageiro2_Cliente_Connect, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(Mensageiro2_Cliente_Connect);
    } // setupUi

    void retranslateUi(QDialog *Mensageiro2_Cliente_Connect)
    {
        Mensageiro2_Cliente_Connect->setWindowTitle(QCoreApplication::translate("Mensageiro2_Cliente_Connect", "Conex\303\243o ao servidor", nullptr));
        labelServidor->setText(QCoreApplication::translate("Mensageiro2_Cliente_Connect", "SERVIDOR:", nullptr));
        labelLogin->setText(QCoreApplication::translate("Mensageiro2_Cliente_Connect", "LOGIN:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Mensageiro2_Cliente_Connect: public Ui_Mensageiro2_Cliente_Connect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENSAGEIRO2_CLIENTE_CONNECT_H

/********************************************************************************
** Form generated from reading UI file 'whatsprog_login.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WHATSPROG_LOGIN_H
#define UI_WHATSPROG_LOGIN_H

#include <QtCore/QVariant>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_WhatsProgLogin
{
public:
    QGridLayout *gridLayout;
    QLabel *labelIpServidor;
    QLineEdit *lineEditIpServidor;
    QLabel *labelNomeUsuario;
    QLineEdit *lineEditNomeUsuario;
    QLabel *labelSenhaUsuario;
    QLineEdit *lineEditSenhaUsuario;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *WhatsProgLogin)
    {
        if (WhatsProgLogin->objectName().isEmpty())
            WhatsProgLogin->setObjectName(QString::fromUtf8("WhatsProgLogin"));
        WhatsProgLogin->resize(248, 125);
        gridLayout = new QGridLayout(WhatsProgLogin);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        labelIpServidor = new QLabel(WhatsProgLogin);
        labelIpServidor->setObjectName(QString::fromUtf8("labelIpServidor"));

        gridLayout->addWidget(labelIpServidor, 0, 0, 1, 1);

        lineEditIpServidor = new QLineEdit(WhatsProgLogin);
        lineEditIpServidor->setObjectName(QString::fromUtf8("lineEditIpServidor"));

        gridLayout->addWidget(lineEditIpServidor, 0, 1, 1, 1);

        labelNomeUsuario = new QLabel(WhatsProgLogin);
        labelNomeUsuario->setObjectName(QString::fromUtf8("labelNomeUsuario"));

        gridLayout->addWidget(labelNomeUsuario, 1, 0, 1, 1);

        lineEditNomeUsuario = new QLineEdit(WhatsProgLogin);
        lineEditNomeUsuario->setObjectName(QString::fromUtf8("lineEditNomeUsuario"));

        gridLayout->addWidget(lineEditNomeUsuario, 1, 1, 1, 1);

        labelSenhaUsuario = new QLabel(WhatsProgLogin);
        labelSenhaUsuario->setObjectName(QString::fromUtf8("labelSenhaUsuario"));

        gridLayout->addWidget(labelSenhaUsuario, 2, 0, 1, 1);

        lineEditSenhaUsuario = new QLineEdit(WhatsProgLogin);
        lineEditSenhaUsuario->setObjectName(QString::fromUtf8("lineEditSenhaUsuario"));

        gridLayout->addWidget(lineEditSenhaUsuario, 2, 1, 1, 1);

        buttonBox = new QDialogButtonBox(WhatsProgLogin);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 3, 0, 1, 2);


        retranslateUi(WhatsProgLogin);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, WhatsProgLogin, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, WhatsProgLogin, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(WhatsProgLogin);
    } // setupUi

    void retranslateUi(QDialog *WhatsProgLogin)
    {
        WhatsProgLogin->setWindowTitle(QCoreApplication::translate("WhatsProgLogin", "MUDE TITULO - Criar ou conectar?", nullptr));
        labelIpServidor->setText(QCoreApplication::translate("WhatsProgLogin", "Servidor (IP):", nullptr));
        labelNomeUsuario->setText(QCoreApplication::translate("WhatsProgLogin", "Usuario", nullptr));
        labelSenhaUsuario->setText(QCoreApplication::translate("WhatsProgLogin", "Senha", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WhatsProgLogin: public Ui_WhatsProgLogin {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WHATSPROG_LOGIN_H

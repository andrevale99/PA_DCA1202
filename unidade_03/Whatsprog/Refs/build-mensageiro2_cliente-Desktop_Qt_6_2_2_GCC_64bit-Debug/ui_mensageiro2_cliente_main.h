/********************************************************************************
** Form generated from reading UI file 'mensageiro2_cliente_main.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MENSAGEIRO2_CLIENTE_MAIN_H
#define UI_MENSAGEIRO2_CLIENTE_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Mensageiro2_Cliente_Main
{
public:
    QAction *actionConectar;
    QAction *actionDesconectar;
    QAction *actionApagar;
    QAction *actionSair;
    QWidget *centralWidget;
    QLabel *userLabel;
    QLineEdit *userEdit;
    QLabel *msgLabel;
    QLineEdit *msgEdit;
    QPushButton *enviarButton;
    QTableWidget *msgTable;
    QMenuBar *menuBar;
    QMenu *menuServidor;
    QMenu *menuMensagens;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Mensageiro2_Cliente_Main)
    {
        if (Mensageiro2_Cliente_Main->objectName().isEmpty())
            Mensageiro2_Cliente_Main->setObjectName(QString::fromUtf8("Mensageiro2_Cliente_Main"));
        Mensageiro2_Cliente_Main->resize(800, 600);
        actionConectar = new QAction(Mensageiro2_Cliente_Main);
        actionConectar->setObjectName(QString::fromUtf8("actionConectar"));
        actionDesconectar = new QAction(Mensageiro2_Cliente_Main);
        actionDesconectar->setObjectName(QString::fromUtf8("actionDesconectar"));
        actionApagar = new QAction(Mensageiro2_Cliente_Main);
        actionApagar->setObjectName(QString::fromUtf8("actionApagar"));
        actionSair = new QAction(Mensageiro2_Cliente_Main);
        actionSair->setObjectName(QString::fromUtf8("actionSair"));
        centralWidget = new QWidget(Mensageiro2_Cliente_Main);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        userLabel = new QLabel(centralWidget);
        userLabel->setObjectName(QString::fromUtf8("userLabel"));
        userLabel->setGeometry(QRect(10, 530, 51, 16));
        userEdit = new QLineEdit(centralWidget);
        userEdit->setObjectName(QString::fromUtf8("userEdit"));
        userEdit->setGeometry(QRect(60, 530, 100, 20));
        msgLabel = new QLabel(centralWidget);
        msgLabel->setObjectName(QString::fromUtf8("msgLabel"));
        msgLabel->setGeometry(QRect(170, 530, 61, 16));
        msgEdit = new QLineEdit(centralWidget);
        msgEdit->setObjectName(QString::fromUtf8("msgEdit"));
        msgEdit->setGeometry(QRect(230, 530, 491, 20));
        enviarButton = new QPushButton(centralWidget);
        enviarButton->setObjectName(QString::fromUtf8("enviarButton"));
        enviarButton->setGeometry(QRect(734, 530, 61, 23));
        msgTable = new QTableWidget(centralWidget);
        if (msgTable->columnCount() < 3)
            msgTable->setColumnCount(3);
        msgTable->setObjectName(QString::fromUtf8("msgTable"));
        msgTable->setGeometry(QRect(0, 0, 800, 525));
        msgTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        msgTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        msgTable->setTabKeyNavigation(false);
        msgTable->setSelectionMode(QAbstractItemView::NoSelection);
        msgTable->setRowCount(0);
        msgTable->setColumnCount(3);
        msgTable->verticalHeader()->setVisible(false);
        Mensageiro2_Cliente_Main->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Mensageiro2_Cliente_Main);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 21));
        menuServidor = new QMenu(menuBar);
        menuServidor->setObjectName(QString::fromUtf8("menuServidor"));
        menuMensagens = new QMenu(menuBar);
        menuMensagens->setObjectName(QString::fromUtf8("menuMensagens"));
        Mensageiro2_Cliente_Main->setMenuBar(menuBar);
        statusBar = new QStatusBar(Mensageiro2_Cliente_Main);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Mensageiro2_Cliente_Main->setStatusBar(statusBar);

        menuBar->addAction(menuServidor->menuAction());
        menuBar->addAction(menuMensagens->menuAction());
        menuServidor->addAction(actionConectar);
        menuServidor->addAction(actionDesconectar);
        menuServidor->addSeparator();
        menuServidor->addAction(actionSair);
        menuMensagens->addAction(actionApagar);

        retranslateUi(Mensageiro2_Cliente_Main);

        QMetaObject::connectSlotsByName(Mensageiro2_Cliente_Main);
    } // setupUi

    void retranslateUi(QMainWindow *Mensageiro2_Cliente_Main)
    {
        Mensageiro2_Cliente_Main->setWindowTitle(QCoreApplication::translate("Mensageiro2_Cliente_Main", "Mensageiro - Trocador de mensagens via rede", nullptr));
        actionConectar->setText(QCoreApplication::translate("Mensageiro2_Cliente_Main", "Conectar...", nullptr));
        actionDesconectar->setText(QCoreApplication::translate("Mensageiro2_Cliente_Main", "Desconectar", nullptr));
        actionApagar->setText(QCoreApplication::translate("Mensageiro2_Cliente_Main", "Apagar", nullptr));
        actionSair->setText(QCoreApplication::translate("Mensageiro2_Cliente_Main", "Sair", nullptr));
        userLabel->setText(QCoreApplication::translate("Mensageiro2_Cliente_Main", "USU\303\201RIO:", nullptr));
        msgLabel->setText(QCoreApplication::translate("Mensageiro2_Cliente_Main", "MENSAGEM:", nullptr));
        enviarButton->setText(QCoreApplication::translate("Mensageiro2_Cliente_Main", "ENVIAR", nullptr));
        menuServidor->setTitle(QCoreApplication::translate("Mensageiro2_Cliente_Main", "Servidor", nullptr));
        menuMensagens->setTitle(QCoreApplication::translate("Mensageiro2_Cliente_Main", "Mensagens", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Mensageiro2_Cliente_Main: public Ui_Mensageiro2_Cliente_Main {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MENSAGEIRO2_CLIENTE_MAIN_H

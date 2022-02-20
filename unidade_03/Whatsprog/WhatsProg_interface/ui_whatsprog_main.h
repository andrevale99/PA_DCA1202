/********************************************************************************
** Form generated from reading UI file 'whatsprog_main.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WHATSPROG_MAIN_H
#define UI_WHATSPROG_MAIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WhatsProgMain
{
public:
    QAction *actionNova_conversa;
    QAction *actionRemover_conversa;
    QAction *actionApagar_mensagens;
    QAction *actionNovo_usuario;
    QAction *actionUsuario_existente;
    QAction *actionDesconectar;
    QAction *actionSair;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QLabel *labelConversas;
    QTableWidget *tableConversas;
    QVBoxLayout *verticalLayout_2;
    QLabel *labelMensagens;
    QTableWidget *tableMensagens;
    QLabel *labelMensagem;
    QLineEdit *lineEditMensagem;
    QMenuBar *menuBar;
    QMenu *menuConversa;
    QMenu *menuMensagens;
    QMenu *menuConexao;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *WhatsProgMain)
    {
        if (WhatsProgMain->objectName().isEmpty())
            WhatsProgMain->setObjectName(QString::fromUtf8("WhatsProgMain"));
        WhatsProgMain->resize(900, 700);
        actionNova_conversa = new QAction(WhatsProgMain);
        actionNova_conversa->setObjectName(QString::fromUtf8("actionNova_conversa"));
        actionRemover_conversa = new QAction(WhatsProgMain);
        actionRemover_conversa->setObjectName(QString::fromUtf8("actionRemover_conversa"));
        actionApagar_mensagens = new QAction(WhatsProgMain);
        actionApagar_mensagens->setObjectName(QString::fromUtf8("actionApagar_mensagens"));
        actionNovo_usuario = new QAction(WhatsProgMain);
        actionNovo_usuario->setObjectName(QString::fromUtf8("actionNovo_usuario"));
        actionUsuario_existente = new QAction(WhatsProgMain);
        actionUsuario_existente->setObjectName(QString::fromUtf8("actionUsuario_existente"));
        actionDesconectar = new QAction(WhatsProgMain);
        actionDesconectar->setObjectName(QString::fromUtf8("actionDesconectar"));
        actionSair = new QAction(WhatsProgMain);
        actionSair->setObjectName(QString::fromUtf8("actionSair"));
        centralWidget = new QWidget(WhatsProgMain);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        labelConversas = new QLabel(centralWidget);
        labelConversas->setObjectName(QString::fromUtf8("labelConversas"));
        labelConversas->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(labelConversas);

        tableConversas = new QTableWidget(centralWidget);
        if (tableConversas->columnCount() < 2)
            tableConversas->setColumnCount(2);
        tableConversas->setObjectName(QString::fromUtf8("tableConversas"));
        tableConversas->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableConversas->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableConversas->setTabKeyNavigation(false);
        tableConversas->setSelectionMode(QAbstractItemView::SingleSelection);
        tableConversas->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableConversas->setShowGrid(false);
        tableConversas->setColumnCount(2);
        tableConversas->verticalHeader()->setVisible(false);

        verticalLayout->addWidget(tableConversas);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        labelMensagens = new QLabel(centralWidget);
        labelMensagens->setObjectName(QString::fromUtf8("labelMensagens"));
        labelMensagens->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(labelMensagens);

        tableMensagens = new QTableWidget(centralWidget);
        if (tableMensagens->columnCount() < 3)
            tableMensagens->setColumnCount(3);
        tableMensagens->setObjectName(QString::fromUtf8("tableMensagens"));
        tableMensagens->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        tableMensagens->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        tableMensagens->setTabKeyNavigation(false);
        tableMensagens->setSelectionMode(QAbstractItemView::NoSelection);
        tableMensagens->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableMensagens->setColumnCount(3);
        tableMensagens->verticalHeader()->setVisible(false);

        verticalLayout_2->addWidget(tableMensagens);


        gridLayout->addLayout(verticalLayout_2, 0, 2, 1, 1);

        labelMensagem = new QLabel(centralWidget);
        labelMensagem->setObjectName(QString::fromUtf8("labelMensagem"));

        gridLayout->addWidget(labelMensagem, 1, 0, 1, 1);

        lineEditMensagem = new QLineEdit(centralWidget);
        lineEditMensagem->setObjectName(QString::fromUtf8("lineEditMensagem"));

        gridLayout->addWidget(lineEditMensagem, 1, 1, 1, 2);

        WhatsProgMain->setCentralWidget(centralWidget);
        labelMensagem->raise();
        lineEditMensagem->raise();
        menuBar = new QMenuBar(WhatsProgMain);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 900, 21));
        menuConversa = new QMenu(menuBar);
        menuConversa->setObjectName(QString::fromUtf8("menuConversa"));
        menuMensagens = new QMenu(menuBar);
        menuMensagens->setObjectName(QString::fromUtf8("menuMensagens"));
        menuConexao = new QMenu(menuBar);
        menuConexao->setObjectName(QString::fromUtf8("menuConexao"));
        WhatsProgMain->setMenuBar(menuBar);
        statusBar = new QStatusBar(WhatsProgMain);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        WhatsProgMain->setStatusBar(statusBar);

        menuBar->addAction(menuConexao->menuAction());
        menuBar->addAction(menuConversa->menuAction());
        menuBar->addAction(menuMensagens->menuAction());
        menuConversa->addAction(actionNova_conversa);
        menuConversa->addAction(actionRemover_conversa);
        menuMensagens->addAction(actionApagar_mensagens);
        menuConexao->addAction(actionNovo_usuario);
        menuConexao->addAction(actionUsuario_existente);
        menuConexao->addAction(actionDesconectar);
        menuConexao->addSeparator();
        menuConexao->addAction(actionSair);

        retranslateUi(WhatsProgMain);

        QMetaObject::connectSlotsByName(WhatsProgMain);
    } // setupUi

    void retranslateUi(QMainWindow *WhatsProgMain)
    {
        WhatsProgMain->setWindowTitle(QCoreApplication::translate("WhatsProgMain", "WhatsProg", nullptr));
        actionNova_conversa->setText(QCoreApplication::translate("WhatsProgMain", "Nova conversa ...", nullptr));
        actionRemover_conversa->setText(QCoreApplication::translate("WhatsProgMain", "Remover conversa", nullptr));
        actionApagar_mensagens->setText(QCoreApplication::translate("WhatsProgMain", "Apagar mensagens", nullptr));
        actionNovo_usuario->setText(QCoreApplication::translate("WhatsProgMain", "Novo usu\303\241rio...", nullptr));
        actionUsuario_existente->setText(QCoreApplication::translate("WhatsProgMain", "Usu\303\241rio existente...", nullptr));
        actionDesconectar->setText(QCoreApplication::translate("WhatsProgMain", "Desconectar", nullptr));
        actionSair->setText(QCoreApplication::translate("WhatsProgMain", "Sair", nullptr));
        labelConversas->setText(QCoreApplication::translate("WhatsProgMain", "CONVERSAS", nullptr));
        labelMensagens->setText(QCoreApplication::translate("WhatsProgMain", "NENHUMA CONVERSA SELECIONADA", nullptr));
        labelMensagem->setText(QCoreApplication::translate("WhatsProgMain", "Mensagem:", nullptr));
        menuConversa->setTitle(QCoreApplication::translate("WhatsProgMain", "Conversas", nullptr));
        menuMensagens->setTitle(QCoreApplication::translate("WhatsProgMain", "Mensagens", nullptr));
        menuConexao->setTitle(QCoreApplication::translate("WhatsProgMain", "Conex\303\243o", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WhatsProgMain: public Ui_WhatsProgMain {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WHATSPROG_MAIN_H

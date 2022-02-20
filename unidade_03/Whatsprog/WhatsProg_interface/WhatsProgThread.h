#ifndef WHATSPROGTHREAD_H
#define WHATSPROGTHREAD_H

/***
 *
 * DUPLA: Andre Menezes de Freitas Vale
 *        Luis Filipe Silva da Costa
 *
 * SISTEMA OPERACIONAL USADO: LINUX
 *
 * Caso queira roda no Windows
 * 1. Descomentar no ".pro" o link "-lWs2_32"
 * 2. Descomentar as partes para rodar no Windows nos arquivos "mysocket.h" e "mysocket.cpp"
 *
 */


#include <QObject>
#include <thread>

#include "whatsprog_dados_cliente.h"
#include "whatsprog_dados.h"

class WhatsProgThread : public QObject
{
    Q_OBJECT
public:
    explicit WhatsProgThread(QObject *parent = nullptr);

signals:

   // void nova_conversa();
    void nova_msg(IterConversa itr);
    void conf_leitura(IterConversa itr);

    //void conversa_atual();

    //void erro_msg();
    void matou_thread();
    void desconectado();

private:
    std::thread thr;

    void main_thread(void);

    friend void main_thread(WhatsProgThread *_thr);

private slots:
    void conf_lancar_thread();
    void matar_thread();
};

#endif // WHATSPROGTHREAD_H

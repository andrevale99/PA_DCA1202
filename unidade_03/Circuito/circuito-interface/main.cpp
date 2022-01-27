#include "maincircuito.h"
#include <QApplication>

/**
 * @Dupla
 * 		André Menezes de Freitas Vale
 * 		Lucas Emanuel Costa Moisés
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainCircuito w;
    w.show();

    return a.exec();
}

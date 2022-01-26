#include "maincircuito.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainCircuito w;
    w.show();

    return a.exec();
}

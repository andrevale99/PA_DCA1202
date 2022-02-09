#include <QApplication>
#include <QMessageBox>

#include "whatsprog_main.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  WhatsProgMain w;

  w.show();
  int result = a.exec();

  return result;
}

#include <string>
#include <QMessageBox>
#include "whatsprog_novaconversa.h"
#include "ui_whatsprog_novaconversa.h"
#include "whatsprog_main.h"

using namespace std;

WhatsProgNovaConversa::WhatsProgNovaConversa(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::WhatsProgNovaConversa)
{
  ui->setupUi(this);
}

WhatsProgNovaConversa::~WhatsProgNovaConversa()
{
  delete ui;
}

void WhatsProgNovaConversa::on_buttonBox_accepted()
{
}

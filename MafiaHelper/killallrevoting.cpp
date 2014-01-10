#include "killallrevoting.h"
#include "ui_killallrevoting.h"

KillAllRevoting::KillAllRevoting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KillAllRevoting)
{
    ui->setupUi(this);
    this->setModal(true);
}

KillAllRevoting::~KillAllRevoting()
{
    delete ui;
}

#include "nightdialog.h"
#include "ui_nightdialog.h"

NightDialog::NightDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightDialog)
{
    ui->setupUi(this);

    ui->comboBox->addItem("Milk");
    ui->comboBox->setFont(QFont("Times",22));
    for (int i=1;i<11;i++)
        ui->comboBox->addItem(QString("%1").arg(i));
    ui->comboBox->setCurrentIndex(0);

    this->setModal(true);
}

NightDialog::~NightDialog()
{
    delete ui;
}

void NightDialog::on_pushButton_clicked()
{
    QString player = ui->comboBox->currentText();
    if (player!= "Milk") emit killed(player.toInt());
    else emit killed(-1);
    this->accept();
}

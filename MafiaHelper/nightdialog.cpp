#include "nightdialog.h"
#include "ui_nightdialog.h"
#include "player.h"
NightDialog::NightDialog(QList<Player*> playersList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightDialog)
{
    ui->setupUi(this);

    players = playersList;
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

    for(int i = 0; i < players.size(); i++)
        if (players.at(i)->getNumber() == ui->comboBox->currentText().toInt()) players.at(i)->die();
    this->accept();
}

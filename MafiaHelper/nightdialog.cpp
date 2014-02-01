#include "nightdialog.h"
#include "ui_nightdialog.h"
#include "player.h"
NightDialog::NightDialog(QList<Player*> playersList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightDialog)
{
    ui->setupUi(this);

    players = playersList;
    ui->comboBox->addItem("Nobody");
    ui->comboBox->setFont(QFont("Times",22));
    QList<int> dead;
    for (int i = 0; i < 10; i++)
         if (!players[i]->isAlive) dead.push_back(players[i]->getNumber());
    for (int i = 1; i <= 10; i++)
        if (!dead.contains(i)) ui->comboBox->addItem(QString("%1").arg(i));

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
        if (players.at(i)->getNumber() == ui->comboBox->currentText().toInt())
        {
            players.at(i)->die();
            emit killed(players.at(i)->getNumber());
            this->accept();
            return;
        }
    emit killed(-1);
    this->accept();
}

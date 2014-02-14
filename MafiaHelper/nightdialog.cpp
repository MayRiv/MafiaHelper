#include "nightdialog.h"
#include "ui_nightdialog.h"
#include "player.h"
#include "Constants.h"
NightDialog::NightDialog(QList<Player*> playersList, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NightDialog)
{
    ui->setupUi(this);

    ui->comboBox->setMinimumHeight(MIN_HEIGHT);
    ui->comboBox_2->setMinimumHeight(MIN_HEIGHT);
    ui->comboBox_3->setMinimumHeight(MIN_HEIGHT);
    players = playersList;
    ui->comboBox->addItem("Nobody");
    ui->comboBox->setFont(QFont("Times",22));
    ui->comboBox_2->addItem("Nobody");
    ui->comboBox_2->setFont(QFont("Times",22));
    ui->comboBox_3->addItem("Nobody");
    ui->comboBox_3->setFont(QFont("Times",22));
    QList<int> dead;
    for (int i = 0; i < 10; i++)
         if (!players[i]->isAlive) dead.push_back(players[i]->getNumber());
    for (int i = 1; i <= 10; i++)
        if (!dead.contains(i))
        {
            ui->comboBox->addItem(QString("%1").arg(i));
            ui->comboBox_2->addItem(QString("%1").arg(i));
            ui->comboBox_3->addItem(QString("%1").arg(i));
        }
    ui->comboBox->setCurrentIndex(0);

    this->setModal(true);

}

NightDialog::~NightDialog()
{
    delete ui;
}

void NightDialog::on_pushButton_clicked()
{
    int donCheck = -1;
    int sherifCheck = -1;
    if (ui->comboBox_2->currentIndex() != 0) donCheck = ui->comboBox_2->currentText().toInt();
    if (ui->comboBox_3->currentIndex() != 0) sherifCheck = ui->comboBox_3->currentText().toInt();
    for(int i = 0; i < players.size(); i++)
        if (players.at(i)->getNumber() == ui->comboBox->currentText().toInt())
        {
            players.at(i)->die();
            emit nightEnded(players[i]->getNumber(),donCheck,sherifCheck);
            this->accept();
            return;
        }
    emit nightEnded(-1,donCheck,sherifCheck);
    this->accept();
}

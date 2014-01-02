#include "votedialog.h"
#include "ui_votedialog.h"
#include <QComboBox>
#include <QLayout>
#include <QDebug>
#include <QPair>
#include "player.h"
VoteDialog::VoteDialog(QList<Player *> *allPlayers, QList<int> ePlayers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoteDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    exposedPlayers = ePlayers;
    players = allPlayers;

    for (int i=0; i < players->size();i++)
        if (players->at(i)->isAlive)
            numberOfPlayers.push_back(QString("%1").arg(i));
    numberOfPlayers.push_back(QString("%1").arg(numberOfPlayers.size()));
    for (int i = 0; i < exposedPlayers.size(); i++)
    {

        QLabel* label = new QLabel(this);
        label->setText(QString("Voting for %1 player").arg(exposedPlayers[i]));
        comboBoxes.push_back(new QComboBox(this));
        comboBoxes.back()->setMinimumHeight(100);
        comboBoxes.back()->setMinimumWidth(100);
        comboBoxes.back()->addItems(numberOfPlayers);
        comboBoxes.back()->setFont(QFont("Times",22));

        ui->votesLayout->addWidget(comboBoxes.back());
        ui->labelsLayout->addWidget(label);
    }


    connect(comboBoxes.back(),SIGNAL(currentIndexChanged(QString)),this,SLOT(calculate()));

}

VoteDialog::~VoteDialog()
{
    delete ui;
}
void VoteDialog::calculate()
{

    condemned.first = exposedPlayers[0];
    condemned.second = comboBoxes[0]->currentText().toInt();
    for(int i = 1; i < comboBoxes.size(); i++)
        if (comboBoxes[i]->currentText().toInt() > condemned.second) {
            condemned.first = exposedPlayers[i];
            condemned.second = comboBoxes[i]->currentText().toInt();
        }
    ui->labelResult->setText(QString("%1 player has been condemned with %2 votes").arg(condemned.first).arg(condemned.second));
    ui->pushButtonAccept->setEnabled(true);
}

void VoteDialog::on_pushButtonAccept_clicked()
{

    for (int i = 0; i < players->size(); i++)
        if (players->at(i)->getNumber() == condemned.first) players->at(i)->die();
    this->accept();
}

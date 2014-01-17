#include "votedialog.h"
#include "ui_votedialog.h"
#include <QComboBox>
#include <QLayout>
#include <QDebug>
#include <QPair>
#include "player.h"
#include "killallrevoting.h"
#include <QScroller>
#include <qalgorithms.h>
VoteDialog::VoteDialog(QList<Player *> allPlayers, QList<int> ePlayers, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoteDialog)
{

    ui->setupUi(this);
    this->setModal(true);
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture );

    needRevoting = false;
    condemned.resize(ePlayers.size());
    exposedPlayers = ePlayers;
    players = allPlayers;

    int maxVotes = 0;
    for (int i = 0; i <  players.size(); i++)
        if (players[i]->isAlive) maxVotes++;

    for (int i=0; i <= maxVotes;i++)
       numberOfPlayers.push_back(QString("%1").arg(i));


    for (int i = 0; i < exposedPlayers.size(); i++)
    {

        QLabel* label = new QLabel(this);
        label->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">For %1 player</span></p></body></html>").arg(exposedPlayers[i]));
        comboBoxes.push_back(new QComboBox(this));
        comboBoxes.back()->setMinimumHeight(100);
        comboBoxes.back()->setMinimumWidth(100);
        comboBoxes.back()->addItems(numberOfPlayers);
        comboBoxes.back()->setFont(QFont("Times",22));


        connect(comboBoxes[i],SIGNAL(currentTextChanged(QString)),this, SLOT(voting(QString)));
        ui->votesLayout->addWidget(comboBoxes.back());
        ui->labelsLayout->addWidget(label);
    }

    for (int i =0; i < comboBoxes.size() - 1; i++)
    {
        connect(comboBoxes[i],SIGNAL(currentIndexChanged(int)),this,SLOT(changeEnabled()));
        comboBoxes[i + 1]->setEnabled(false);
    }
    for (int i = 0; i < comboBoxes.size(); i++)
        connect(comboBoxes[i],SIGNAL(currentIndexChanged(QString)),this,SLOT(calculate()));

}

VoteDialog::~VoteDialog()
{
    delete ui;
}

void VoteDialog::voting(QString number)
{
    int maxVotes = 0;
    for (int i = 0; i <  players.size(); i++)
        if (players[i]->isAlive) maxVotes++;

    for (int i = 0; i < comboBoxes.size(); i++)
        maxVotes-= comboBoxes[i]->currentText().toInt();
    numberOfPlayers.clear();
    for (int i = 0; i <= maxVotes; i++)
        numberOfPlayers.push_back(QString("%1").arg(i));

    for (int i=0; i < comboBoxes.size(); i++)
    {
        comboBoxes[i]->blockSignals(true);
        QString currentNumber = comboBoxes[i]->currentText();
        comboBoxes[i]->clear();
        if (currentNumber.toInt() > maxVotes)
        {
            QStringList temp;
            for (int i = 0; i <= currentNumber.toInt(); i++)
                temp.push_back(QString("%1").arg(i));

            comboBoxes[i]->addItems(temp);
        }
        else comboBoxes[i]->addItems(numberOfPlayers);
        comboBoxes[i]->setCurrentText(currentNumber);
        comboBoxes[i]->blockSignals(false);

    }
}
void VoteDialog::calculate()
{
    for (int i = 0; i < comboBoxes.size(); i++)
    {
        condemned[i].first = exposedPlayers[i];
        condemned[i].second = comboBoxes[i]->currentText().toInt();
    }

    for (int i = 0; i < condemned.size() - 1; i++)
        for (int j = 1; j < condemned.size(); j++)
            if (condemned[i].second < condemned[j].second)
            {
               QPair<int,int> temp = condemned[i];
               condemned[i] = condemned[j];
               condemned[j] = temp;
            }

    int playersWithEqualNumberOfVotes = 1;
    int higthScoreOfVoting = condemned[0].second;
    for (int i = 1; i < condemned.size() && condemned[i].second == higthScoreOfVoting; i++, playersWithEqualNumberOfVotes++)
    {}
    if (playersWithEqualNumberOfVotes == 1)
        ui->labelResult->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player has been condemned with %2 votes</span></p></body></html>").arg(condemned[0].first).arg(condemned[0].second));
    else
    {
        ui->labelResult->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Several players have got the same votes. The city should revote.</span></p></body></html>"));
        needRevoting = true;
    }
    ui->pushButtonAccept->setEnabled(true);

}

void VoteDialog::on_pushButtonAccept_clicked()
{
    static bool wasRevoting = false;
    if (needRevoting)
    {

        if (!wasRevoting)
        {
            wasRevoting = true;
            QList<int> playersForRevoting;
            playersForRevoting.push_back(condemned[0].first);
            for (int i = 1; i < condemned.size() && condemned[i].second == condemned[0].second; i++)
                playersForRevoting.push_back(condemned[i].first);
            emit revoting(playersForRevoting);
        }
        else
        {
            wasRevoting = false;
            KillAllRevoting* d = new KillAllRevoting();
            connect(d,SIGNAL(accepted()),this,SLOT(killThemAll()));
            connect(d,SIGNAL(rejected()),this,SLOT(leaveThemAlive()));
            d->showFullScreen();
        }
    }
    else
    {
        wasRevoting = false;
        for (int i = 0; i < players.size(); i++)
            if (players.at(i)->getNumber() == condemned[0].first)
            {
                players.at(i)->die();
                emit killed(players.at(i)->getNumber());
            }
    }
    this->accept();
}

void VoteDialog::on_pushButtonClear_clicked()
{
    for (int i = 0; i < comboBoxes.size(); i++)
        comboBoxes[i]->setCurrentText(QString("%1").arg(0));
    ui->labelResult->clear();
}

void VoteDialog::killThemAll()
{
    QList<int> playersForRevoting;
    playersForRevoting.push_back(condemned[0].first);
    for (int i = 1; i < condemned.size() && condemned[i].second == condemned[0].second; i++)
        playersForRevoting.push_back(condemned[i].first);

    for (int i = 0; i < players.size(); i++)
        if (playersForRevoting.contains(players[i]->getNumber())) players[i]->die();
    emit killed(-1);
    this->close();
}

void VoteDialog::leaveThemAlive()
{
    emit killed(-1);
    this->close();
}

void VoteDialog::changeEnabled()
{
    for (int i = 0; i < comboBoxes.size() - 1; i++)
        if (!comboBoxes[i]->currentText().isEmpty()) comboBoxes[i + 1]->setEnabled(true);
}

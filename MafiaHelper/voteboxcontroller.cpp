#include "voteboxcontroller.h"
#include <QComboBox>
#include <QStringList>
#include "player.h"
extern QString NOBODY;
VoteBoxController::VoteBoxController(QList<QComboBox *> comboBoxes, QList<Player *> players, QObject *parent) :
    QObject(parent)
{
    this->comboBoxes = comboBoxes;
    //this->players    = players;
    for (QList<Player*>::iterator i = players.begin(); i != players.end(); i++)
        this->players.push_back(*i);
    for(QList<QComboBox*>::iterator i = comboBoxes.begin(); i != comboBoxes.end(); i++)
        connect(*i,SIGNAL(currentTextChanged(QString)),this,SLOT(on_votebox_item_change(QString)));
}

void VoteBoxController::setEnabledForAlivePlayers()
{
    for (int i = 0; i < players.size(); i++)
        if (players[i]->isAlive) comboBoxes[players[i]->getNumber() - 1]->setEnabled(true);
}

void VoteBoxController::setNobodyToAll()
{
    for(QList<QComboBox*>::iterator i = comboBoxes.begin(); i != comboBoxes.end(); i++)
        (*i)->setCurrentText(NOBODY);
}

void VoteBoxController::setNominations(QList<int> revotingPlayers)
{
    this->setNobodyToAll();
    for (int i = 0; i < revotingPlayers.size() - 1; i++)
        this->comboBoxes[revotingPlayers[i] - 1]->setCurrentText(QString("%1").arg(revotingPlayers[i + 1]));
    this->comboBoxes[revotingPlayers.back() - 1]->setCurrentText(QString("%1").arg(revotingPlayers.first()));
}



void VoteBoxController::on_votebox_item_change(QString item)
{
    QStringList avaibleForVote;
    avaibleForVote.push_back(NOBODY);

    for (int i = 0; i < players.size(); i++)
        if (players[i]->isAlive) avaibleForVote.push_back(QString("%1").arg(players[i]->getNumber()));

    for (int i = 0; i < comboBoxes.size(); i++)
        if (comboBoxes[i]->currentText() != NOBODY)
            avaibleForVote.removeOne(comboBoxes[i]->currentText());

    for (int i = 0; i < comboBoxes.size(); i++)
    {
            comboBoxes[i]->blockSignals(true);
            QString currentText = comboBoxes[i]->currentText();
            comboBoxes[i]->clear();
            comboBoxes[i]->addItems(avaibleForVote);
            if (currentText != NOBODY) comboBoxes[i]->addItem(currentText); //This also could be insterted in the right spot.
            comboBoxes[i]->setCurrentText(currentText);
            comboBoxes[i]->blockSignals(false);
    }

}

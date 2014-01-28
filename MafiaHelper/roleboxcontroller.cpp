#include "roleboxcontroller.h"
#include <QComboBox>
#include <QStringList>
#include "player.h"
RoleBoxController::RoleBoxController(QList<QComboBox *> roleBoxes, QList<Player *> players, QObject *parent) :
    QObject(parent)
{
    //this->playersList    = players;
    for (QList<Player*>::iterator i = players.begin(); i != players.end(); i++)
        this->players.push_back(*i);
    this->comboBoxes = roleBoxes;
    for(QList<QComboBox*>::iterator i = comboBoxes.begin(); i != comboBoxes.end(); i++)
        connect(*i,SIGNAL(currentTextChanged(QString)),this,SLOT(on_votebox_item_change(QString)));
}

void RoleBoxController::setVisibleRoleComboBoxes(bool visible)
{
    for (int i = 0; i < comboBoxes.size(); i++)
        comboBoxes[i]->setVisible(visible);
}

void RoleBoxController::setEnableRoleComboBoxes(bool enabled)
{
    for (int i = 0; i < comboBoxes.size(); i++)
        comboBoxes[i]->setEnabled(enabled);
}

void RoleBoxController::on_votebox_item_change(QString item)
{
    int mafiaCount   = 2;
    int citizenCount = 6;
    int sherifCount  = 1;
    int donCount     = 1;
    QStringList avaibleRoles;
    avaibleRoles.push_back("");
    avaibleRoles.push_back("Citizen");
    avaibleRoles.push_back("Sherif");
    avaibleRoles.push_back("Don");
    avaibleRoles.push_back("Mafia");

    for (int i = 0; i < comboBoxes.size(); i++)
    {
        if (comboBoxes[i]->currentText() == "Mafia") mafiaCount--;
        if (comboBoxes[i]->currentText() == "Citizen") citizenCount--;
        if (comboBoxes[i]->currentText() == "Don") donCount--;
        if (comboBoxes[i]->currentText() == "Sherif") sherifCount--;
    }
    if (!mafiaCount)   avaibleRoles.removeOne("Mafia");
    if (!citizenCount) avaibleRoles.removeOne("Citizen");
    if (!donCount)     avaibleRoles.removeOne("Don");
    if (!sherifCount)  avaibleRoles.removeOne("Sherif");




    for (int i = 0; i < comboBoxes.size(); i++)
    {
        comboBoxes[i]->blockSignals(true);
        QString currentText = comboBoxes[i]->currentText();

        comboBoxes[i]->clear();
        comboBoxes[i]->addItems(avaibleRoles);
        if (!currentText.isEmpty() && !avaibleRoles.contains(currentText)) comboBoxes[i]->addItem(currentText);
        comboBoxes[i]->setCurrentText(currentText);
        comboBoxes[i]->blockSignals(false);
    }

    if (!mafiaCount && !sherifCount && !donCount)
        for (int i = 0; i < comboBoxes.size(); i++)
        {
            comboBoxes[i]->blockSignals(true);
            if (comboBoxes[i]->currentText().isEmpty()) comboBoxes[i]->setCurrentText("Citizen");
            comboBoxes[i]->blockSignals(false);
        }

}

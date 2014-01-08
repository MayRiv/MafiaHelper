#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QScroller>
#include <QTimer>
#include <QDebug>
#include <QStringList>
#include <QFont>
#include <QLineEdit>
#include "nightdialog.h"
#include "votedialog.h"
#include "warningbutton.h"
#include "player.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),font("Times",22),pause(true),secondsLeft(60)
{
    ui->setupUi(this);
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture );

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(minusSecond()));
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));

    avaibleRoles.push_back("");
    avaibleRoles.push_back("Citizen");
    avaibleRoles.push_back("Sherif");
    avaibleRoles.push_back("Don");
    avaibleRoles.push_back("Mafia");

    avaibleForVote.push_back("Nobody");
    for (int i=1;i<=10;i++)
        avaibleForVote.push_back(QString("%1").arg(i));

    for (int i=0;i<10;i++)
    {
        rolesComboBoxes.push_back(new QComboBox(this));
        rolesComboBoxes.back()->setMinimumHeight(100);
        rolesComboBoxes.back()->setMinimumWidth(100);
        rolesComboBoxes.back()->addItems(avaibleRoles);
        rolesComboBoxes.back()->setFont(font);
        connect(rolesComboBoxes.back(),SIGNAL(currentTextChanged(QString)),this,SLOT(on_rolebox_item_change(QString)));

        votesComboBoxes.push_back(new QComboBox(this));
        votesComboBoxes.back()->setMinimumHeight(100);
        votesComboBoxes.back()->setMinimumWidth(100);
        votesComboBoxes.back()->addItems(avaibleForVote);
        votesComboBoxes.back()->setFont(font);
        votesComboBoxes.back()->setStyleSheet("alignment: right;");
        for (int j=0;j<10;j++)
            votesComboBoxes.back()->setItemData(j,Qt::AlignHCenter, Qt::TextAlignmentRole);

        connect(votesComboBoxes.back(),SIGNAL(currentTextChanged(QString)),this,SLOT(on_votebox_item_change(QString)));

        warningButtons.push_back(new WarningButton);
        warningButtons.back()->setMinimumHeight(100);
        warningButtons.back()->setMinimumWidth(100);
        warningButtons.back()->setFont(font);

        names.push_back(new QLineEdit(this));
        names.back()->setMinimumHeight(100);
        names.back()->setMinimumWidth(100);
        names.back()->setFont(font);

        players.push_back(new Player(rolesComboBoxes.back(),votesComboBoxes.back(),names.back(),warningButtons.back(), i + 1,this));

        ui->verticalLayout->addWidget(names.back());
        ui->verticalLayout_2->addWidget(rolesComboBoxes.back());
        ui->verticalLayout_3->addWidget(votesComboBoxes.back());
        ui->verticalLayout_4->addWidget(warningButtons.back());
    }
    currentSpeaker = players.begin();


    for (int i = 0; i < players.size(); i++)
    {
        connect(warningButtons[i],SIGNAL(scored4warnings()),players[i],SLOT(die()));
    }

    for (int i = 0; i < names.size() - 1; i++)
        connect(names[i],SIGNAL(returnPressed()),names[i+1],SLOT(setFocus()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


QList<Player*> MainWindow::shift(QList<Player*> l)
{
    QList<Player*> temp = l;
    Player* t = temp.first();
    temp.removeFirst();
    temp.push_back(t);
    return temp;
}

void MainWindow::on_votebox_item_change(QString item)
{
   avaibleForVote.clear();
    avaibleForVote.push_back("Nobody");
    for (int i=1;i<=10;i++)
        avaibleForVote.push_back(QString("%1").arg(i));

    for (int i = 0; i < votesComboBoxes.size(); i++)
        if (votesComboBoxes[i]->currentText() != "Nobody")
            avaibleForVote.removeOne(votesComboBoxes[i]->currentText());

    for (int i = 0; i < votesComboBoxes.size(); i++)
    {
            votesComboBoxes[i]->blockSignals(true);
            QString currentText = votesComboBoxes[i]->currentText();
            votesComboBoxes[i]->clear();
            votesComboBoxes[i]->addItems(avaibleForVote);
            if (currentText != "Nobody") votesComboBoxes[i]->addItem(currentText);
            votesComboBoxes[i]->setCurrentText(currentText);
            votesComboBoxes[i]->blockSignals(false);
    }

}


void MainWindow::on_rolebox_item_change(QString item)
{
    int mafiaCount   = 2;
    int citizenCount = 6;
    int sherifCount  = 1;
    int donCount     = 1;
    avaibleRoles.clear();
    avaibleRoles.push_back("");
    avaibleRoles.push_back("Citizen");
    avaibleRoles.push_back("Sherif");
    avaibleRoles.push_back("Don");
    avaibleRoles.push_back("Mafia");

    for (int i = 0; i < rolesComboBoxes.size(); i++)
    {
        if (rolesComboBoxes[i]->currentText() == "Mafia") mafiaCount--;
        if (rolesComboBoxes[i]->currentText() == "Citizen") citizenCount--;
        if (rolesComboBoxes[i]->currentText() == "Don") donCount--;
        if (rolesComboBoxes[i]->currentText() == "Sherif") sherifCount--;
    }
    if (!mafiaCount)   avaibleRoles.removeOne("Mafia");
    if (!citizenCount) avaibleRoles.removeOne("Citizen");
    if (!donCount)     avaibleRoles.removeOne("Don");
    if (!sherifCount)  avaibleRoles.removeOne("Sherif");

    for (int i = 0; i < rolesComboBoxes.size(); i++)
        {
            rolesComboBoxes[i]->blockSignals(true);
            QString currentText = rolesComboBoxes[i]->currentText();

            rolesComboBoxes[i]->clear();
            rolesComboBoxes[i]->addItems(avaibleRoles);
            if (!currentText.isEmpty()) rolesComboBoxes[i]->addItem(currentText);
            rolesComboBoxes[i]->setCurrentText(currentText);
            rolesComboBoxes[i]->blockSignals(false);
        }
}

void MainWindow::changeSpeaker()
{
    timer->stop();
    pause = true;
    ui->pushButton_11->setText("Start");
    secondsLeft = 59;
    ui->label_5->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Time left: %1</span></p>").arg(secondsLeft));

    currentSpeaker++;
    while(currentSpeaker != players.end()  && !(*currentSpeaker)->isAlive)
        currentSpeaker++;

    if (currentSpeaker != players.end())
    {
        ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));
    }
    else
    {
        //emit lastPlayerEnded();
        afterDay();
    }
}


void MainWindow::on_pushButton_11_clicked()
{
    pause = !pause;
    if (!pause)
    {
        ui->pushButton_11->setText("Pause");
        timer->start();
    }
    else
    {
        ui->pushButton_11->setText("Start");
        timer->stop();
    }
}


void MainWindow::afterDay()
{
    QList<int> votes;
    for (int i=0;i<votesComboBoxes.size();i++)
        if (votesComboBoxes[i]->currentText() != "Nobody" )
        {
            votes.push_back(votesComboBoxes[i]->currentText().toInt());
        }

    if (!votes.empty() && votes.size() != 1)
    {
        VoteDialog* d = new VoteDialog(players,votes,this);
        connect(d,SIGNAL(killed(int)),this,SLOT(lastWordAfterDay(int)));
        connect(d,SIGNAL(revoting(QList<int>)),this,SLOT(revote(QList<int>)));
        d->showFullScreen();
    }
    else
    {
        if (votes.size() == 1)
        {
            for (int i = 0; i < players.size(); i++)
                if (players.at(i)->getNumber() == votes.first())
                {
                    players.at(i)->die();
                    lastWordAfterDay(players.at(i)->getNumber());
                }
        }
        else
        {
            disconnect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
            night();

        }
    }
}


void MainWindow::night()
{
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
    disconnect(this,SIGNAL(timeIsLeft()),this,SLOT(night()));

    NightDialog *d = new NightDialog(players,this);
    connect(d, SIGNAL(killed(int)),this,SLOT(lastWordAfterNight(int)));
    d->showFullScreen();
}


void MainWindow::afterNight()
{
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
    disconnect(this,SIGNAL(timeIsLeft()),this,SLOT(afterNight()));

    players = shift(players);

    currentSpeaker=players.begin();
    while(currentSpeaker != players.end()  && !(*currentSpeaker)->isAlive )
        currentSpeaker++;
    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));

    avaibleForVote.clear();
    avaibleForVote.push_back("Nobody");
    for (int i=1;i<=10;i++)
        if (players[i-1]->isAlive) avaibleForVote.push_back(QString("%1").arg(i));



    for (int i=0;i<votesComboBoxes.size();i++)
    {
        votesComboBoxes[i]->blockSignals(true);
        votesComboBoxes[i]->clear();
        votesComboBoxes[i]->addItems(avaibleForVote);
        votesComboBoxes[i]->setCurrentIndex(0);
        votesComboBoxes[i]->blockSignals(false);
    }
}


void MainWindow::minusSecond()
{
    secondsLeft--;
    ui->label_5->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Time left: %1</span></p>").arg(secondsLeft));
    if (secondsLeft == 0)
       // changeSpeaker();
        emit timeIsLeft();
}



void MainWindow::on_actionChange_Names_triggered()
{
    bool enabled;
    if (ui->actionChange_Names->text() == "Enable Names")
    {
        enabled = true;
        ui->actionChange_Names->setText("Disable Names");
    }
    else
    {
        enabled = false;
        ui->actionChange_Names->setText("Enable Names");
    }

    for(int i=0; i < names.size(); i++)
        names[i]->setEnabled(enabled);

}


void MainWindow::on_actionHide_Show_Roles_triggered()
{
    bool visible;
    if (ui->actionHide_Show_Roles->text() == "Hide Roles")
    {
        visible = false;
        ui->actionHide_Show_Roles->setText("Show Roles");
    }
    else
    {
        visible = true;
        ui->actionHide_Show_Roles->setText("Hide Roles");
    }
    for (int i = 0; i < rolesComboBoxes.size(); i++)
        rolesComboBoxes[i]->setVisible(visible);
    ui->rolesLabel->setVisible(visible);
}


void MainWindow::on_actionRestart_triggered()
{
    players.empty();
    for (int i = 0; i < 10; i++)
    {
        votesComboBoxes[i]->setEnabled(true);
        votesComboBoxes[i]->setCurrentIndex(0);
        rolesComboBoxes[i]->setEnabled(true);
        names[i]->setEnabled(true);
        names[i]->clear();
        warningButtons[i]->setEnabled(true);
        warningButtons[i]->setText(QString("%1").arg(0));
        players.push_back(new Player(rolesComboBoxes[i],votesComboBoxes[i],names[i],warningButtons[i], i + 1,this));
    }
    currentSpeaker = players.begin();

    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));

}

void MainWindow::switch_revotinglist_and_players()
{
    QList<Player*> temp = players;
    players = revotingPlayers;
    revotingPlayers = temp;
    disconnect(this,SIGNAL(lastPlayerEnded()),this,SLOT(switch_revotinglist_and_players()));
}

void MainWindow::revote(QList<int> rList)
{
    for (int i = 0; i < rList.size(); i++)
        revotingPlayers.push_back(getPlayerByNumber(rList[i]));
    switch_revotinglist_and_players();

    for( int i = 0; i < votesComboBoxes.size(); i++)
        votesComboBoxes[i]->setEnabled(false);

    //TO DO set comboboxes of condemned to each other pointer.
    currentSpeaker = players.begin();
    connect(this,SIGNAL(lastPlayerEnded()),this,SLOT(switch_revotinglist_and_players()));

    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));

}

Player *MainWindow::getPlayerByNumber(int number)
{
    for( int i = 0; i < votesComboBoxes.size(); i++)
        votesComboBoxes[i]->setEnabled(true);
    for (int i = 0; i < players.size(); i++)
        if (players[i]->getNumber() == number) return players[i];
}

void MainWindow::lastWordAfterDay(int player)
{
    disconnect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(night()));

    secondsLeft = 59;
    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg(player));

}

void MainWindow::lastWordAfterNight(int player)
{
    if (player != -1)
    {
        disconnect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
        connect(this,SIGNAL(timeIsLeft()),this,SLOT(afterNight()));
        secondsLeft = 59;
        ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg(player));
    }
    else
    {
        afterNight();
        disconnect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
        connect(this,SIGNAL(timeIsLeft()),this,SLOT(afterNight()));
    }
}


void MainWindow::on_pushButton_15_clicked()
{
    emit timeIsLeft();
}

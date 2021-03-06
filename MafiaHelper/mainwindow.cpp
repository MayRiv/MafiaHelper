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
#include "Constants.h"
#include "roleboxcontroller.h"
#include "voteboxcontroller.h"
#include "logger.h"
#include <QTime>
#include <QDate>
QString NOBODY = "";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),font("Times",22),pause(true),secondsLeft(60)
{
    ui->setupUi(this);
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture );

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(minusSecond()));
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(handleMafiaAgreement()));
    QStringList avaibleRoles;
    avaibleRoles.push_back("");
    avaibleRoles.push_back("Citizen");
    avaibleRoles.push_back("Sherif");
    avaibleRoles.push_back("Don");
    avaibleRoles.push_back("Mafia");

    QStringList avaibleForVote;
    avaibleForVote.push_back(NOBODY);
    for (int i=1;i<=10;i++)
        avaibleForVote.push_back(QString("%1").arg(i));

    QList<QComboBox*> rolesComboBoxes;
    QList<QComboBox*> votesComboBoxes;
    for (int i=0;i<10;i++)
    {
        rolesComboBoxes.push_back(new QComboBox(this));
        rolesComboBoxes.back()->setMinimumHeight(MIN_HEIGHT);
        rolesComboBoxes.back()->setMinimumWidth(MIN_COMBOBOX_WIDTH);
        rolesComboBoxes.back()->addItems(avaibleRoles);
        rolesComboBoxes.back()->setFont(font);

        votesComboBoxes.push_back(new QComboBox(this));
        votesComboBoxes.back()->setMinimumHeight(MIN_HEIGHT);
        votesComboBoxes.back()->setMinimumWidth(MIN_COMBOBOX_WIDTH);
        votesComboBoxes.back()->addItems(avaibleForVote);
        votesComboBoxes.back()->setFont(font);
        votesComboBoxes.back()->setStyleSheet("alignment: right;");
        for (int j=0;j<10;j++)
            votesComboBoxes.back()->setItemData(j,Qt::AlignHCenter, Qt::TextAlignmentRole);


        warningButtons.push_back(new WarningButton);
        warningButtons.back()->setMinimumHeight(MIN_HEIGHT);
        warningButtons.back()->setMinimumWidth(MIN_WARNING_BUTTON_WIDTH);
        warningButtons.back()->setFont(font);

        names.push_back(new QLineEdit(this));
        names.back()->setMinimumHeight(MIN_HEIGHT);
        names.back()->setFont(font);

        players.push_back(new Player(rolesComboBoxes.back(),votesComboBoxes.back(),names.back(),warningButtons.back(), i + 1));

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

    wasRevoting = false;
    roleBoxController = new RoleBoxController(rolesComboBoxes,players,this);
    voteBoxController = new VoteBoxController(votesComboBoxes,players,this);
    QTime Ti; Ti=QTime::currentTime();
    QDate D; D=QDate::currentDate();
    //QString path = D.toString().append("_").append(Ti.toString()).append(".txt");
    QString path = "log.txt";
    logger            = new Logger(path,players,this);
    for (int i = 1; i <= 10; i++)
    {
        QLabel* label = new QLabel(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1</span></p>").arg(i));
        ui->verticalLayout_6->addWidget(label);
        label->setMinimumHeight(MIN_HEIGHT);
    }
    ui->pushButton_11->setMinimumHeight(MIN_HEIGHT);
    ui->pushButton_15->setMinimumHeight(MIN_HEIGHT);

    connect(roleBoxController,SIGNAL(rolesAreDefined()),this,SLOT(rolesAreDefined()));
    voteBoxController->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    ui->pushButton_11->setEnabled(false);
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

    voteBoxController->setEnabled(false);


    if (currentSpeaker != players.end())
    {
        voteBoxController->setEnabledVoteBox((*currentSpeaker)->getNumber());
        ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));
    }
    else
    {
        emit lastPlayerEnded();
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

    QList<int> votes = voteBoxController->getNominations();
    logger->addNominations(voteBoxController->getPairNominationsAndNominators());
    if (!votes.empty() && votes.size() != 1)
    {
        VoteDialog* d = new VoteDialog(players,votes,this);
        connect(d,SIGNAL(revoting(QList<int>)),this,SLOT(revote(QList<int>)));
        connect(d,SIGNAL(playersWereCondemned(QList<int>)),this,SLOT(lastWordAfterDay(QList<int>)));
        d->showFullScreen();
    }
    else
    {
        if (votes.size() == 1 && players[0]->getNumber() != 1) //not first circle
        {
            for (int i = 0; i < players.size(); i++)
                if (players.at(i)->getNumber() == votes.first())
                {
                    players.at(i)->die();
                    QList<int> l;
                    l.push_back(players[i]->getNumber());
                    lastWordAfterDay(l);
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
    connect(d,SIGNAL(nightEnded(int,int,int)),this,SLOT(lastWordAfterNight(int,int,int)));
    d->showFullScreen();
}

void MainWindow::afterNight()
{
    if (isEndOfTheGame(players))
    {

        bool mafiaWon = false;
        for(QList<Player*>::iterator i = players.begin(); i != players.end(); i++)
        {
            if ((*i)->isAlive &&
                    ((*i)->getRole() == "Mafia" || (*i)->getRole() == "Don"))
                mafiaWon = true;
        }
        logger->setMafiaWon(mafiaWon);
        logger->writeLog();
        on_actionRestart_triggered();
        return;

    }
    disconnect(this,SIGNAL(timeIsLeft()),0,0);
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));

    players = shift(players);

    currentSpeaker = players.begin();
    while(currentSpeaker != players.end()  && !(*currentSpeaker)->isAlive )
        currentSpeaker++;

    voteBoxController->setEnabledVoteBox((*currentSpeaker)->getNumber());
    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));
}

void MainWindow::minusSecond()
{
    secondsLeft--;
    ui->label_5->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Time left: %1</span></p>").arg(secondsLeft));
    if (secondsLeft == 0)
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

    roleBoxController->setVisibleRoleComboBoxes(visible);
    ui->rolesLabel->setVisible(visible);
}

void MainWindow::on_actionRestart_triggered()
{
    for (QList<Player*>::iterator i = players.begin(); i != players.end(); i++)
        delete (*i);
    players.clear();

    QStringList avaibleForVote;
    avaibleForVote.push_back(NOBODY);

    for (int i=1;i<=10;i++)
        avaibleForVote.push_back(QString("%1").arg(i));

    roleBoxController->setEnableRoleComboBoxes(true);
    roleBoxController->clearAll();
    voteBoxController->setNobodyToAll();
    for (int i = 0; i < 10; i++)
    {
        names[i]->setEnabled(true);
        names[i]->clear();

        warningButtons[i]->setEnabled(true);
        warningButtons[i]->removeAllWarnings();

        players.push_back(new Player(roleBoxController->getRoleComboBoxes()[i],this->voteBoxController->getVoteComboBoxes()[i],names[i],warningButtons[i], i + 1));
    }
    currentSpeaker = players.begin();

    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Enter roles.</span></p></body></html>"));
    ui->pushButton_11->setEnabled(false);
    ui->pushButton_15->setEnabled(false);
    voteBoxController->setEnabled(false);
    disconnect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(handleMafiaAgreement()));
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
    revotingPlayers.clear();
    for (int i = 0; i < rList.size(); i++)
        revotingPlayers.push_back(getPlayerByNumber(rList[i]));
    switch_revotinglist_and_players();

    voteBoxController->setNominations(rList);

    currentSpeaker = players.begin();
    connect(this,SIGNAL(lastPlayerEnded()),this,SLOT(switch_revotinglist_and_players()));

    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));

}

Player *MainWindow::getPlayerByNumber(int number)
{
    for (int i = 0; i < players.size(); i++)
        if (players[i]->getNumber() == number) return players[i];
    return NULL;
}

void MainWindow::lastWordAfterDay(QList<int> condemned)
{
    for(int i = 0; i < condemned.size(); i++)
        qDebug() << condemned[i] << " was condemned.";
    qDebug() << "At this day.";

    logger->addCondemned(condemned);
    if (condemned.size() == 1)
    {
        disconnect(this,SIGNAL(timeIsLeft()),0,0);
        connect(this,SIGNAL(timeIsLeft()),this,SLOT(night()));

        secondsLeft = 59;
        ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg(condemned.first()));
    }
    else night();
}

void MainWindow::on_pushButton_15_clicked()
{
    emit timeIsLeft();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionPrevious_Speaker_triggered()
{
    voteBoxController->setEnabled(false);
    if (currentSpeaker != players.begin()) currentSpeaker--;
    secondsLeft = 59;
    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));
    voteBoxController->setEnabledVoteBox((*currentSpeaker)->getNumber());
}

void MainWindow::handleMafiaAgreement()
{
    secondsLeft = 60;
    ui->label_5->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Time left: %1</span></p>").arg(secondsLeft));
    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg((*currentSpeaker)->getNumber()));
    timer->stop();
    pause = true;
    ui->pushButton_11->setText("Start");
    disconnect(this,SIGNAL(timeIsLeft()),0,0);
    connect(this,SIGNAL(timeIsLeft()),this,SLOT(changeSpeaker()));
    voteBoxController->setEnabledVoteBox(1);
}

void MainWindow::rolesAreDefined()
{
    ui->pushButton_15->setEnabled(true);
    ui->pushButton_11->setEnabled(true);
    for (int i = 0; i < players.size(); i++)
        players[i]->setRole(roleBoxController->getRoleComboBoxes()[i]->currentText());
    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Mafia's negotiating.</span></p></body></html>"));

}

bool MainWindow::isEndOfTheGame(QList<Player *> l)
{
    int playersCount = 0;
    int mafiaCount   = 0;
    for(QList<Player*>::iterator i = l.begin(); i != l.end(); i++)
    {
        if ((*i)->isAlive) playersCount++;
        if ((*i)->isAlive &&
                ((*i)->getRole() == "Mafia" || (*i)->getRole() == "Don")) mafiaCount++;
    }
    qDebug() << "Player's amount " << playersCount << " and mafia's amount " << mafiaCount;
    if (!mafiaCount || mafiaCount >= playersCount  - mafiaCount) return true;
    else return false;
}

void MainWindow::lastWordAfterNight(int victim, int donCheck, int sherifCheck)
{
    voteBoxController->setNobodyToAll();
    logger->addKill(victim);
    logger->addDonCheck(donCheck);
    logger->addSherifCheck(sherifCheck);
    if (victim != -1)
    {
        disconnect(this,SIGNAL(timeIsLeft()),0,0);
        connect(this,SIGNAL(timeIsLeft()),this,SLOT(afterNight()));
        secondsLeft = 59;
        ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg(victim));
    }
    else
    {
        afterNight();
    }
}


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QScrollBar>
#include <QScroller>
#include <QTimer>
#include <QDebug>
#include <QStringList>
#include <QFont>
#include "nightdialog.h"
#include "votedialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),font("Times",22),pause(true),secondsLeft(60)
{
    ui->setupUi(this);
    QScroller::grabGesture(ui->scrollArea->viewport(), QScroller::LeftMouseButtonGesture );

    timer = new QTimer(this);
    timer->setInterval(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(minusSecond()));

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
        rolesComboBoxes.back()->setStyleSheet("color: red;");

        votesComboBoxes.push_back(new QComboBox(this));
        votesComboBoxes.back()->setMinimumHeight(100);
        votesComboBoxes.back()->setMinimumWidth(100);
        votesComboBoxes.back()->addItems(avaibleForVote);
        votesComboBoxes.back()->setFont(font);
        votesComboBoxes.back()->setStyleSheet("alignment: right;");
        for (int j=0;j<10;j++)
            votesComboBoxes.back()->setItemData(j,Qt::AlignHCenter, Qt::TextAlignmentRole);

        ui->verticalLayout_2->addWidget(rolesComboBoxes[i]);
        ui->verticalLayout_3->addWidget(votesComboBoxes[i]);
    }

    for (int i=1;i<=10;i++)
        speakers.push_back(i);
    futureSpeakers = shift(speakers);
    currentSpeaker=speakers.begin();

    connect(ui->pushButton_15,SIGNAL(clicked()),this,SLOT(changeSpeaker()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::kill(int playerNumber)
{
    for(int i=0;i<futureSpeakers.size();i++)
        if (futureSpeakers[i]==playerNumber)
        {
            futureSpeakers.removeAt(i);
            break;
        }
}

QList<int> MainWindow::shift(QList<int> l)
{
    QList<int> temp = l;
    int t = temp.first();
    temp.removeFirst();
    temp.push_back(t);
    return temp;
}
void MainWindow::changeSpeaker()
{

    timer->stop();
    pause = true;
    ui->pushButton_11->setText("Start");
    secondsLeft = 60;
    ui->label_5->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Time left: %1</span></p>").arg(secondsLeft));

    currentSpeaker++;
    if (currentSpeaker != speakers.end())
    {
        ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg(*currentSpeaker));
    }
    else
    {
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
        VoteDialog* d = new VoteDialog(votes,speakers.size(),this);
        connect(d,SIGNAL(killed(int)),this,SLOT(kill(int)));
        connect(d,SIGNAL(accepted()),this,SLOT(night()));
        d->show();
    }
    else
    {
        if (votes.size() == 1) kill(votes.back());
        night();
    }
}

void MainWindow::night()
{
    NightDialog *d = new NightDialog(this);
    connect(d,SIGNAL(killed(int)),this,SLOT(kill(int)));
    connect(d,SIGNAL(accepted()),this,SLOT(afterNight()));
    d->show();
}

void MainWindow::afterNight()
{

    speakers = futureSpeakers;
    futureSpeakers = shift(speakers);

    currentSpeaker=speakers.begin();
    ui->label_6->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">%1 player is speaking</span></p></body></html>").arg(*currentSpeaker));
    for (int i=0;i<votesComboBoxes.size();i++)
        votesComboBoxes[i]->setCurrentIndex(0);
}

void MainWindow::minusSecond()
{
    secondsLeft--;
    ui->label_5->setText(QString("<html><head/><body><p><span style=\" font-size:22pt;\">Time left: %1</span></p>").arg(secondsLeft));
    if (secondsLeft == 0)
        changeSpeaker();
}


void MainWindow::on_pushButton_clicked()
{
    int numberOfFalls = ui->pushButton->text().toInt();
    numberOfFalls++;
    ui->pushButton->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton->setEnabled(false);
        ui->lineEdit->setEnabled(false);
        votesComboBoxes[0]->setEnabled(false);
        rolesComboBoxes[0]->setEnabled(false);
        kill(1);
        if (*currentSpeaker == 1) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==1)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    int numberOfFalls = ui->pushButton_2->text().toInt();
    numberOfFalls++;
    ui->pushButton_2->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_2->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        votesComboBoxes[1]->setEnabled(false);
        rolesComboBoxes[1]->setEnabled(false);
        kill(2);
        if (*currentSpeaker == 2) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==2)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    int numberOfFalls = ui->pushButton_3->text().toInt();
    numberOfFalls++;
    ui->pushButton_3->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_3->setEnabled(false);
        ui->lineEdit_3->setEnabled(false);
        votesComboBoxes[2]->setEnabled(false);
        rolesComboBoxes[2]->setEnabled(false);
        kill(3);
        if (*currentSpeaker == 3) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==3)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    int numberOfFalls = ui->pushButton_4->text().toInt();
    numberOfFalls++;
    ui->pushButton_4->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_4->setEnabled(false);
        ui->lineEdit_4->setEnabled(false);
        votesComboBoxes[3]->setEnabled(false);
        rolesComboBoxes[3]->setEnabled(false);
        kill(4);
        if (*currentSpeaker == 4) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==4)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    int numberOfFalls = ui->pushButton_5->text().toInt();
    numberOfFalls++;
    ui->pushButton_5->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_5->setEnabled(false);
        ui->lineEdit_5->setEnabled(false);
        votesComboBoxes[4]->setEnabled(false);
        rolesComboBoxes[4]->setEnabled(false);
        kill(5);
        if (*currentSpeaker == 5) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==5)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    int numberOfFalls = ui->pushButton_6->text().toInt();
    numberOfFalls++;
    ui->pushButton_6->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_6->setEnabled(false);
        ui->lineEdit_6->setEnabled(false);
        votesComboBoxes[5]->setEnabled(false);
        rolesComboBoxes[5]->setEnabled(false);
        kill(6);
        if (*currentSpeaker == 6) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==6)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_7_clicked()
{
    int numberOfFalls = ui->pushButton_7->text().toInt();
    numberOfFalls++;
    ui->pushButton_7->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_7->setEnabled(false);
        ui->lineEdit_7->setEnabled(false);
        votesComboBoxes[6]->setEnabled(false);
        rolesComboBoxes[6]->setEnabled(false);
        kill(7);
        if (*currentSpeaker == 7) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==7)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    int numberOfFalls = ui->pushButton_8->text().toInt();
    numberOfFalls++;
    ui->pushButton_8->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_8->setEnabled(false);
        ui->lineEdit_8->setEnabled(false);
        votesComboBoxes[7]->setEnabled(false);
        rolesComboBoxes[7]->setEnabled(false);
        kill(8);
        if (*currentSpeaker == 8) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==8)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_9_clicked()
{
    int numberOfFalls = ui->pushButton_9->text().toInt();
    numberOfFalls++;
    ui->pushButton_9->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_9->setEnabled(false);
        ui->lineEdit_9->setEnabled(false);
        votesComboBoxes[8]->setEnabled(false);
        rolesComboBoxes[8]->setEnabled(false);
        kill(9);
        if (*currentSpeaker == 9) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==9)
            {
                speakers.removeAt(i);
                break;
            }
    }
}

void MainWindow::on_pushButton_10_clicked()
{
    int numberOfFalls = ui->pushButton_10->text().toInt();
    numberOfFalls++;
    ui->pushButton_10->setText(QString("%1").arg(numberOfFalls));
    if (numberOfFalls == 4)
    {
        ui->pushButton_10->setEnabled(false);
        ui->lineEdit_10->setEnabled(false);
        votesComboBoxes[9]->setEnabled(false);
        rolesComboBoxes[9]->setEnabled(false);
        kill(10);
        if (*currentSpeaker == 10) changeSpeaker();

        for(int i=0;i<speakers.size();i++)
            if (speakers[i]==10)
            {
                speakers.removeAt(i);
                break;
            }
    }
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
    ui->lineEdit->setEnabled(enabled);
    ui->lineEdit_2->setEnabled(enabled);
    ui->lineEdit_3->setEnabled(enabled);
    ui->lineEdit_4->setEnabled(enabled);
    ui->lineEdit_5->setEnabled(enabled);
    ui->lineEdit_6->setEnabled(enabled);
    ui->lineEdit_7->setEnabled(enabled);
    ui->lineEdit_8->setEnabled(enabled);
    ui->lineEdit_9->setEnabled(enabled);
    ui->lineEdit_10->setEnabled(enabled);
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

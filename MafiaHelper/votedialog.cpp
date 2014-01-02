#include "votedialog.h"
#include "ui_votedialog.h"
#include <QComboBox>
#include <QLayout>
#include <QDebug>
#include <QPair>
VoteDialog::VoteDialog(QList<int> votes, int playerNumber, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoteDialog)
{
    ui->setupUi(this);
    this->setModal(true);

    votesList = votes;

    for (int i = 0; i <= playerNumber; i++)
        players.push_back(QString("%1").arg(i));
    for (int i = 0; i < votesList.size(); i++)
    {

        QLabel* label = new QLabel(this);
        label->setText(QString("Voting for %1 player").arg(votesList[i]));
        comboBoxes.push_back(new QComboBox(this));
        comboBoxes.back()->setMinimumHeight(100);
        comboBoxes.back()->setMinimumWidth(100);
        comboBoxes.back()->addItems(players);
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

    condemned.first = votesList[0];
    condemned.second = comboBoxes[0]->currentText().toInt();
    for(int i = 1; i < comboBoxes.size(); i++)
        if (comboBoxes[i]->currentText().toInt() > condemned.second) {
            condemned.first = votesList[i];
            condemned.second = comboBoxes[i]->currentText().toInt();
        }
    //qDebug() << "condemned is " << condemned.first << " with " << condemned.second <<" votes";
    ui->labelResult->setText(QString("%1 player has been condemned with %2 votes").arg(condemned.first).arg(condemned.second));
}

void VoteDialog::on_pushButtonAccept_clicked()
{
    emit killed(condemned.first);
    this->accept();
    qDebug() << condemned.first;
}

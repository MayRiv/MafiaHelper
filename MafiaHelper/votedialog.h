#ifndef VOTEDIALOG_H
#define VOTEDIALOG_H

#include <QDialog>

namespace Ui {
class VoteDialog;
}
class QComboBox;
class Player;
class VoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VoteDialog(QList<Player *> allPlayers, QList<int> ePlayers, QWidget *parent = 0);
    ~VoteDialog();

private:
    Ui::VoteDialog *ui;
    QList<QComboBox*> comboBoxes;
    QVector<QPair<int,int> > condemned;
    QList<Player*> players;
    QStringList numberOfPlayers; //rename, please, I want to sleep.
    QList<int> exposedPlayers;
    bool needRevoting;
signals:
    void killed(int);
    void revoting(QList<int>);
private slots:
    void voting(QString number);
    void calculate();
    void on_pushButtonAccept_clicked();
    void on_pushButtonClear_clicked();
};

#endif // VOTEDIALOG_H

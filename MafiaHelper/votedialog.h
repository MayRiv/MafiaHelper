#ifndef VOTEDIALOG_H
#define VOTEDIALOG_H

#include <QDialog>

namespace Ui {
class VoteDialog;
}
class QComboBox;
class VoteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VoteDialog(QList<int> votes,int playerNumber,QWidget *parent = 0);
    ~VoteDialog();

private:
    Ui::VoteDialog *ui;
    QList<int> votesList;
    QStringList players;
    QList<QComboBox*> comboBoxes;
    QPair<int,int> condemned;
signals:
    void killed(int);
    void revoting(QList<int>);
private slots:
    void calculate();
    void on_pushButtonAccept_clicked();
};

#endif // VOTEDIALOG_H

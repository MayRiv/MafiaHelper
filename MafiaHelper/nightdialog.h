#ifndef NIGHTDIALOG_H
#define NIGHTDIALOG_H
#include <QList>
#include <QDialog>

namespace Ui {
class NightDialog;
}
class Player;
class NightDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NightDialog(QList<Player *> playersList, QWidget *parent = 0);
    ~NightDialog();
signals:
    void killed(int);
    void nightEnded(int kiled, int donChecked, int sherifChecked);
private slots:
    void on_pushButton_clicked();

private:
    QList<Player*> players;
    Ui::NightDialog *ui;
};

#endif // NIGHTDIALOG_H

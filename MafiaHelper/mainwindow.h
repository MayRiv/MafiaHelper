#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QComboBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QList<int> speakers;
    QList<int> futureSpeakers;
    QStringList avaibleRoles;
    QStringList avaibleForVote;
    QList<QComboBox*> rolesComboBoxes;
    QList<QComboBox*> votesComboBoxes;
    QList<int>::iterator currentSpeaker;
    Ui::MainWindow *ui;
    QTimer* timer;
    QFont font;
    int secondsLeft;
    bool pause;
private slots:
    void kill(int playerNumber); //kills from futureSpeakers-list.
    QList<int> shift(QList<int> l);
    void changeSpeaker();
    void on_pushButton_11_clicked();
    void afterDay();
    void night();
    void afterNight();
    void minusSecond();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_actionChange_Names_triggered();
    void on_actionHide_Show_Roles_triggered();
};

#endif // MAINWINDOW_H

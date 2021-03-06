#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QComboBox>

namespace Ui {
class MainWindow;
}
class WarningButton;
class Player;
class VoteBoxController;
class RoleBoxController;
class Logger;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool wasRevoting;
    void setTime(int seconds);
signals:
    void lastPlayerEnded();
    void timeIsLeft();
private:
    QList<WarningButton*> warningButtons;
    QList<Player*> players;
    QList<Player*> revotingPlayers;
    QList<Player*>::iterator currentSpeaker;
    QList<QLineEdit*> names;
    Ui::MainWindow *ui;
    QTimer* timer;
    QFont font;
    int secondsLeft;
    bool pause;
    VoteBoxController* voteBoxController;
    RoleBoxController* roleBoxController;
    Logger* logger;
    void setSeconds(int seconds);

private slots:
    QList<Player*> shift(QList<Player*> l);
    void changeSpeaker();
    void on_pushButton_11_clicked();
    void afterDay();
    void night();
    void afterNight();
    void minusSecond();
    void on_actionChange_Names_triggered();
    void on_actionHide_Show_Roles_triggered();
    void on_actionRestart_triggered();
    void switch_revotinglist_and_players();
    void revote(QList<int>);
    Player* getPlayerByNumber(int number);
    void on_pushButton_15_clicked();
    void lastWordAfterDay(QList<int> condemned);
    void on_actionExit_triggered();
    void on_actionPrevious_Speaker_triggered();
    void handleMafiaAgreement();
    void rolesAreDefined();
    bool isEndOfTheGame(QList<Player*> l);
    void lastWordAfterNight(int killed,int donCheck,int sherifCheck);

};

#endif // MAINWINDOW_H

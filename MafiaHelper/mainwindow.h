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
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    QStringList avaibleRoles;
    QStringList avaibleForVote;
    QList<QComboBox*> rolesComboBoxes;
    QList<QComboBox*> votesComboBoxes;
    QList<WarningButton*> warningButtons;
    QList<Player*> players;
    QList<Player*>::iterator currentSpeaker;
    QList<QLineEdit*> names;
    Ui::MainWindow *ui;
    QTimer* timer;
    QFont font;
    int secondsLeft;
    bool pause;
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
};

#endif // MAINWINDOW_H

#ifndef VOTEBOXCONTROLLER_H
#define VOTEBOXCONTROLLER_H
#include <QObject>
class QComboBox;
class Player;
class VoteBoxController : public QObject
{
    Q_OBJECT
public:
    explicit VoteBoxController(QList<QComboBox*> comboBoxes, QList<Player*> players,QObject *parent = 0);
    QList<QComboBox*> getVoteComboBoxes(){return comboBoxes;}
    void setEnabledForAlivePlayers();
    void setNobodyToAll();
signals:

public slots:
private:
    QList<QComboBox*> comboBoxes;
    QList<Player*> players;
private slots:
    void on_votebox_item_change(QString item);
};

#endif // VOTEBOXCONTROLLER_H

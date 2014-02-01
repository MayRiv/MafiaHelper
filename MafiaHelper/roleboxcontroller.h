#ifndef ROLEBOXCONTROLLER_H
#define ROLEBOXCONTROLLER_H

#include <QObject>
class QComboBox;
class Player;
class RoleBoxController : public QObject
{
    Q_OBJECT
public:
    explicit RoleBoxController(QList<QComboBox*> roleBoxes, QList<Player*> players,QObject *parent = 0);
    void setVisibleRoleComboBoxes(bool visible);
    void setEnableRoleComboBoxes(bool enabled);
    QList<QComboBox*> getRoleComboBoxes(){return comboBoxes;}
signals:
    void rolesAreDefined();
public slots:
private:
    QList<QComboBox*> comboBoxes;
    QList<Player*> players;
private slots:
    void on_votebox_item_change(QString item);
};

#endif // ROLEBOXCONTROLLER_H

#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QPair>
class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);
    void addNominations(QList<QPair<int,int> > nominationsForDay);
    void addCondemned(QList<int> condemned);
    void addDonCheck(int);
    void addSherifCheck(int);
    void addKill(int);
    void setMafiaWon(bool won);
signals:

public slots:
private:
    QList<QList<QPair<int,int> > > nominations;
    QList<QList<int> >             condemned;
    QList<int>                     nightVictims;
    QList<int>                     donChecks;
    QList<int>                     sherifChecks;
    bool                           mafiaWon;
};

#endif // LOGGER_H

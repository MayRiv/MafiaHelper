#include "logger.h"

Logger::Logger(QObject *parent) :
    QObject(parent)
{
}

void Logger::addNominations(QList<QPair<int, int> > nominationsForDay)
{
    nominations.push_back(nominationsForDay);
}

void Logger::addCondemned(QList<int> condemned)
{
    this->condemned.push_back(condemned);
}

void Logger::addDonCheck(int check)
{
    donChecks.push_back(check);
}

void Logger::addSherifCheck(int check)
{
    sherifChecks.push_back(check);
}

void Logger::addKill(int victim)
{
    nightVictims.push_back(victim);
}

void Logger::setMafiaWon(bool won)
{
    this->mafiaWon = won;
}


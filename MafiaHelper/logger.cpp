#include "logger.h"
#include <QFile>
#include <QTextStream>
Logger::Logger(QString pathToLogFile,QList<Player*> players, QObject *parent) :
    QObject(parent),logFile(pathToLogFile)
{
    for (QList<Player*>::iterator i = players.begin(); i != players.end(); i++)
        this->players.push_back(*i);
}

void Logger::writeLog()
{
    logFile.open(QIODevice::WriteOnly);
    QTextStream stream(&logFile);
    using namespace std;
    stream  << "Players:\r\n";
    for (int i = 0; i < players.size(); i++)
        stream << QString("%1").arg(players[i]->getNumber()) << " "
               << players[i]->getName() << " "
               << players[i]->getRole() << "\r\n";
//TODO
    if (mafiaWon) stream << "Mafia won\r\n";
    else stream << "City won\r\n";

    for (int i = 0; i < this->condemned.size(); i++)
    {
        stream << "Day " << QString("%1").arg(i+1) << "\r\n";
        stream << "There were nominated:\r\n";
        for (int j = 0; j < nominations[i].size(); j++)
            stream << QString("%1").arg(nominations[i][j].first) << " by player "
                   << QString("%1").arg(nominations[i][j].second) << "\r\n";
        stream << "Condemned:\r\n";
        for (int j = 0; j < condemned[i].size(); j++)
            stream << QString("%1").arg(condemned[i][j]) << " ";
        stream << "\r\n There was killed " << QString("%1").arg(nightVictims[i]) << "\r\n";
        stream << "\r\n\r\n";
    }
    logFile.close();
}

void Logger::setPlayers(QList<Player *> l)
{
    players = l;
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


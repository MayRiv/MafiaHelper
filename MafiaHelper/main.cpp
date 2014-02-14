#include "mainwindow.h"
#include <QApplication>
#include <QtPlatformSupport/QtPlatformSupport>
#include <QFile>
#include <QDataStream>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.addLibraryPath("plugins");
    MainWindow w;
    //w.showFullScreen();
    w.show();
#ifdef Q_OS_WIN
    QFile file("1.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QDataStream stream(&file);
    stream << QString("%1").arg(3);
    file.close();

#endif
    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
#include <QtPlatformSupport/QtPlatformSupport>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.addLibraryPath("plugins");
    MainWindow w;
    //w.showFullScreen();
    w.show();
#ifdef Q_OS_WIN
    qDebug() << "Hello";
#endif
    return a.exec();
}

#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.addLibraryPath("plugins");
    MainWindow w;
    w.showFullScreen();

    return a.exec();
}

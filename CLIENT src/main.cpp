#include "mainwindow.h"
#include "application.h"
#include <QtDebug>

int main(int argc, char *argv[])
{
    Application a(argc, argv);
    MainWindow w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();

    return a.exec();
}

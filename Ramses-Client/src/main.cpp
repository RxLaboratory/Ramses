#include "config.h"

#include "mainwindow.h"

#include <QApplication>
#include <QSettings>
#include <QTcpSocket>

#ifdef Q_OS_WIN
#include "windows.h"
#endif

#include "duqf-app/app-utils.h"

int main(int argc, char *argv[])
{
    DuApplication a(argc, argv);

    // process CLI arguments
    if ( a.processArgs() ) return 0;

    // show splashscreen
    a.showSplashScreen();
    DuSplashScreen *s = a.splashScreen();
    s->newMessage("Checking if Ramses is already running...");

    // Single instance, check if the daemon is already running.
    QTcpSocket *tcpSocket = new QTcpSocket();
    tcpSocket->connectToHost(
                "127.0.01",
                QSettings().value("server/port", 18185).toInt(),
                QIODevice::ReadWrite,
                QAbstractSocket::IPv4Protocol);
    tcpSocket->waitForConnected(1000);
    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
    {

        tcpSocket->write("ping");
        tcpSocket->waitForReadyRead(1000);
        QJsonDocument reply = QJsonDocument::fromJson( tcpSocket->readAll() );
        QJsonObject obj = reply.object();
        QJsonObject content = obj.value("content").toObject();
        if (content.value("ramses").toString() == STR_INTERNALNAME && content.value("version").toString() == STR_VERSION)
        {
            tcpSocket->write("raise");
            tcpSocket->waitForReadyRead(1000);
            qDebug("Ramses is already running.");
            return 0;
        }
    }
    else
    {
        qDebug() << tcpSocket->errorString();
        qDebug() << tcpSocket->error();
    }

    // build and show UI
    s->newMessage("Building UI");
    MainWindow *w = new MainWindow( a.arguments() );
#ifndef Q_OS_LINUX
    //FrameLessWindow f(w);
#endif
    w->show();

    // hide splash when finished
    s->newMessage("Ready!");
    s->finish(w);//*/

    return a.exec();
}

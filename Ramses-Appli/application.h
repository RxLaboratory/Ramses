#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QTimer>

class Application : public QApplication
{
    Q_OBJECT
public:
    explicit Application(int& argc, char** argv);
    void setTimeOut(int to);
signals:
    void idle();
public slots:
    void idleTimeOut();
protected:
    virtual bool notify(QObject *receiver, QEvent *ev);
private:
    QTimer *timer;
    int timeout;
};

#endif // APPLICATION_H

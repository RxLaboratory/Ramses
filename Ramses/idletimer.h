#ifndef IDLETIMER_H
#define IDLETIMER_H

#include <QObject>
#include <QTimer>
#include <QEvent>

class IdleTimer : public QObject
{
    Q_OBJECT
public:
    explicit IdleTimer(QTimer *t,QObject *parent = 0);
protected:
    bool eventFilter(QObject *obj, QEvent *ev);
private:
    QTimer *timer;
};

#endif // IDLETIMER_H

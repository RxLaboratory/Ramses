#include "idletimer.h"
#include <QtDebug>

IdleTimer::IdleTimer(QTimer *t, QObject *parent) : QObject(parent)
{
    timer = t;
}

bool IdleTimer::eventFilter(QObject *obj, QEvent *ev)
{
    if(ev->type() == QEvent::KeyPress || ev->type() == QEvent::MouseButtonPress)
    {
         //reset timer
        qDebug() <<"event";
        timer->start(30000); //30s
    }
    else if (ev->type() == QEvent::ChildAdded)
    {
        QChildEvent* ce = (QChildEvent*)ev;
        // Install the filter to each new child object created
        ce->child()->installEventFilter(this);
    }
    else if (ev->type() == QEvent::ChildRemoved)
    {
        QChildEvent* ce = (QChildEvent*)ev;
        // Remove the the filter from each new child object removed
        ce->child()->removeEventFilter(this);
    }

    return QObject::eventFilter(obj, ev);
}

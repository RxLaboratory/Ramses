#ifndef MAINWINDOWDRAGEVENT_H
#define MAINWINDOWDRAGEVENT_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>
#include <QWidget>

class mainWindowDragEvent : public QObject
{
    Q_OBJECT
public:
    explicit mainWindowDragEvent(QObject *parent = 0);
private:
    QPoint dragPosition;
protected:
    bool eventFilter(QWidget *obj, QEvent *event);
};

#endif // MAINWINDOWDRAGEVENT_H

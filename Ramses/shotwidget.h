#ifndef SHOTWIDGET_H
#define SHOTWIDGET_H

#include "ui_shotwidget.h"
#include "ramshot.h"

class ShotWidget : public QWidget, private Ui::ShotWidget
{
    Q_OBJECT

public:
    explicit ShotWidget(RAMShot *s,QWidget *parent = 0);
    RAMShot *getShot();

private slots:
    void nameChanged(QString name);
    void durationChanged(double duration);

private:
    RAMShot *shot;
};

#endif // SHOTWIDGET_H

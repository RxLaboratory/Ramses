#ifndef RAMASSIGNMENT_H
#define RAMASSIGNMENT_H

#include <QObject>
#include "ramstage.h"
#include "ramshot.h"

class RAMAssignment : public QObject
{
    Q_OBJECT
public:
    explicit RAMAssignment(RAMStage *s,RAMShot *sh,QObject *parent = 0);
    RAMStage *getStage();
    RAMShot *getShot();

signals:

public slots:

private:
    RAMStage *stage;
    RAMShot *shot;

};

#endif // RAMASSIGNMENT_H

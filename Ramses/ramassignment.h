#ifndef RAMASSIGNMENT_H
#define RAMASSIGNMENT_H

#include "ramstage.h"
#include "ramshot.h"

class RAMAssignment
{
public:
    explicit RAMAssignment(RAMStage *s,RAMShot *sh);
    RAMStage *getStage();
    RAMShot *getShot();

signals:

public slots:

private:
    RAMStage *stage;
    RAMShot *shot;

};

#endif // RAMASSIGNMENT_H

#include "ramassignment.h"

RAMAssignment::RAMAssignment(RAMStage *s, RAMShot *sh, QObject *parent) : QObject(parent)
{
    stage = s;
    shot = sh;
}

RAMStage *RAMAssignment::getStage()
{
    return stage;
}

RAMShot *RAMAssignment::getShot()
{
    return shot;
}


#include "ramassignment.h"

RAMAssignment::RAMAssignment(RAMStage *s, RAMShot *sh)
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


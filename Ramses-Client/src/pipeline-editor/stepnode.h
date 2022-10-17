#ifndef STEPNODE_H
#define STEPNODE_H

#include "objectnode.h"

#include "ramstep.h"

class StepNode : public ObjectNode
{
public:
    StepNode(RamStep *step);
    RamStep *step() const;
private slots:
    void stepChanged();
    void removeStep();
private:
    RamStep *_step;
};

#endif // STEPNODE_H

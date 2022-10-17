#include "stepnode.h"

#include "ramproject.h"

StepNode::StepNode(RamStep *step): ObjectNode(step)
{
    _step = step;

    stepChanged();

    connect( step, &RamStep::dataChanged, this, &StepNode::stepChanged);
    connect( this, &StepNode::removed, this, &StepNode::removeStep);
}

RamStep *StepNode::step() const
{
    return _step;
}

void StepNode::stepChanged()
{
    this->setIcon(_step->iconName());
    this->setTitleColor(_step->color());
}

void StepNode::removeStep()
{
    if (!_step) return;
    RamProject *p = _step->project();
    if (p) p->steps()->removeObjects(QStringList(_step->uuid()));
    _step->remove();

}

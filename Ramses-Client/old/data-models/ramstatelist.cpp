#include "ramstatelist.h"

#include "ramstate.h"

RamStateList::RamStateList(QObject *parent):
    RamObjectList("RamState", "States", State, Table, parent)
{
    this->setObjectName( "RamStateList" );
}

bool stateSorter(RamObject *a, RamObject *b)
{
    RamState *as = RamState::c(a);
    RamState *bs = RamState::c(b);
    if (as->completionRatio() != bs->completionRatio()) return as->completionRatio() < bs->completionRatio();
    else return a->shortName() < b->shortName();
}

void RamStateList::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column)
    Q_UNUSED(order)

    std::sort(m_objectList.begin(), m_objectList.end(), stateSorter);
}

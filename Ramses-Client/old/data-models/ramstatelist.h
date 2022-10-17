#ifndef RAMSTATELIST_H
#define RAMSTATELIST_H

#include "ramobjectlist.h"

/**
 * @brief The RamStateList class stores the list of available states in Ramses.
 * It is just a simple heritance of RamObjectList used to sort the states according to their completion ratio.
 */
class RamStateList : public RamObjectList
{
public:
    explicit RamStateList(QObject *parent = nullptr);

    virtual void sort(int column = 0, Qt::SortOrder order = Qt::AscendingOrder) override;
};

bool stateSorter(RamObject *a, RamObject *b);

#endif // RAMSTATELIST_H

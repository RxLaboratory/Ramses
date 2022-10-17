#ifndef RAMITEMVIEW_H
#define RAMITEMVIEW_H

#include "ramobjectview.h"
#include "ramitemsortfilterproxymodel.h"

class RamItemView : public RamObjectView
{
    Q_OBJECT
public:
    RamItemView(QWidget *parent = nullptr);
    RamItemSortFilterProxyModel *filteredModel();
};

#endif // RAMITEMVIEW_H

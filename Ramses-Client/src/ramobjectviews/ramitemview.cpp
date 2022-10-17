#include "ramitemview.h"

RamItemView::RamItemView(QWidget *parent): RamObjectView(Table, parent)
{
    m_objectModel = new RamItemSortFilterProxyModel(this);
    this->setModel(m_objectModel);
}

RamItemSortFilterProxyModel *RamItemView::filteredModel()
{
    return qobject_cast<RamItemSortFilterProxyModel*>( m_objectModel );
}

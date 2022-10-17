#include "ramfilterlistproxymodel.h"

RamFilterListProxyModel::RamFilterListProxyModel(QString listName, QObject *parent): QSortFilterProxyModel(parent)
{
    m_listName = listName;
}

int RamFilterListProxyModel::rowCount(const QModelIndex &parent) const
{
    if (!this->sourceModel()) return 1;
    return sourceModel()->rowCount(parent) + 1;
}

QVariant RamFilterListProxyModel::data(const QModelIndex &index, int role) const
{
    if (columnCount() == 0) return QVariant();

    int row = index.row();
    int col = index.column();
    if (row < 0 || col < 0) return QVariant();
    if (row >= rowCount() || col >= columnCount() ) return QVariant();

    // return ALL
    if (index.row() == 0)
    {
        if (role == Qt::DisplayRole) return "All " + m_listName;
        if (role == Qt::StatusTipRole) return m_listName;
        if (role == Qt::ToolTipRole) return "Do not filter " + m_listName;
        return QVariant();
    }

    return QSortFilterProxyModel::data( createIndex(index.row(),index.column()), role);
}

QModelIndex RamFilterListProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid())
            return QModelIndex();

    if (sourceIndex.parent().isValid())
        return QModelIndex();

    return createIndex(sourceIndex.row()+1, sourceIndex.column());
}

QModelIndex RamFilterListProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!sourceModel()) return QModelIndex();

    if (!proxyIndex.isValid())
        return QModelIndex();

    if (proxyIndex.row() == 0)
        return QModelIndex();

    return sourceModel()->index(proxyIndex.row() - 1, proxyIndex.column());
}

Qt::ItemFlags RamFilterListProxyModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
         return Qt::NoItemFlags;
     if (index.row() == 0)
         return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
     return QSortFilterProxyModel::flags(createIndex(index.row(),index.column()));
}

QModelIndex RamFilterListProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    if (row > rowCount())
            return QModelIndex();
    return createIndex(row, column);
}

QModelIndex RamFilterListProxyModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child)

    return QModelIndex();
}

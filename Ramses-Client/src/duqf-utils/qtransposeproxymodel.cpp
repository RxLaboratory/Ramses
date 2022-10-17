#if (QT_VERSION < QT_VERSION_CHECK(5, 13, 0))

#include "qtransposeproxymodel.h"


void QTransposeProxyModel::onLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint)
{
    QModelIndexList toList;
    toList.reserve(layoutChangePersistentIndexes.size());
    for (const QPersistentModelIndex &persistIdx : qAsConst(layoutChangePersistentIndexes))
        toList << mapFromSource(persistIdx);
    changePersistentIndexList(layoutChangeProxyIndexes, toList);
    layoutChangeProxyIndexes.clear();
    layoutChangePersistentIndexes.clear();
    QList<QPersistentModelIndex> proxyParents;
    proxyParents.reserve(parents.size());
    for (const QPersistentModelIndex &srcParent : parents)
        proxyParents << mapFromSource(srcParent);
    QAbstractItemModel::LayoutChangeHint proxyHint = QAbstractItemModel::NoLayoutChangeHint;
    if (hint == QAbstractItemModel::VerticalSortHint)
        proxyHint = QAbstractItemModel::HorizontalSortHint;
    else if (hint == QAbstractItemModel::HorizontalSortHint)
        proxyHint = QAbstractItemModel::VerticalSortHint;
    emit layoutChanged(proxyParents, proxyHint);
}

void QTransposeProxyModel::onLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &sourceParents, QAbstractItemModel::LayoutChangeHint hint)
{
    QList<QPersistentModelIndex> proxyParents;
    proxyParents.reserve(sourceParents.size());
    for (const QPersistentModelIndex &parent : sourceParents) {
        if (!parent.isValid()) {
            proxyParents << QPersistentModelIndex();
            continue;
        }
        const QModelIndex mappedParent = mapFromSource(parent);
        proxyParents << mappedParent;
    }
    QAbstractItemModel::LayoutChangeHint proxyHint = QAbstractItemModel::NoLayoutChangeHint;
    if (hint == QAbstractItemModel::VerticalSortHint)
        proxyHint = QAbstractItemModel::HorizontalSortHint;
    else if (hint == QAbstractItemModel::HorizontalSortHint)
        proxyHint = QAbstractItemModel::VerticalSortHint;
    emit layoutAboutToBeChanged(proxyParents, proxyHint);
    const QModelIndexList proxyPersistentIndexes = persistentIndexList();
    layoutChangeProxyIndexes.clear();
    layoutChangePersistentIndexes.clear();
    layoutChangeProxyIndexes.reserve(proxyPersistentIndexes.size());
    layoutChangePersistentIndexes.reserve(proxyPersistentIndexes.size());
    for (const QModelIndex &proxyPersistentIndex : proxyPersistentIndexes) {
        layoutChangeProxyIndexes << proxyPersistentIndex;
        const QPersistentModelIndex srcPersistentIndex = mapToSource(proxyPersistentIndex);
        layoutChangePersistentIndexes << srcPersistentIndex;
    }
}

void QTransposeProxyModel::onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                                const QVector<int> &roles)
{
    emit dataChanged(mapFromSource(topLeft), mapFromSource(bottomRight), roles);
}

void QTransposeProxyModel::onHeaderDataChanged(Qt::Orientation orientation, int first, int last)
{
    emit headerDataChanged(orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal, first, last);
}

void QTransposeProxyModel::onColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    beginInsertRows(mapFromSource(parent), first, last);
}

void QTransposeProxyModel::onColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    beginRemoveRows(mapFromSource(parent), first, last);
}

void QTransposeProxyModel::onColumnsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn)
{
    beginMoveRows(mapFromSource(sourceParent), sourceStart, sourceEnd, mapFromSource(destinationParent), destinationColumn);
}

void QTransposeProxyModel::onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last)
{
    beginInsertColumns(mapFromSource(parent), first, last);
}

void QTransposeProxyModel::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    beginRemoveColumns(mapFromSource(parent), first, last);
}

void QTransposeProxyModel::onRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow)
{
    beginMoveColumns(mapFromSource(sourceParent), sourceStart, sourceEnd, mapFromSource(destinationParent), destinationRow);
}

QTransposeProxyModel::QTransposeProxyModel(QObject* parent)
    :  QIdentityProxyModel(parent)
{}

void QTransposeProxyModel::setSourceModel(QAbstractItemModel* newSourceModel)
{
    if (newSourceModel == sourceModel())
         return;
     beginResetModel();
     if (sourceModel()) {
         for (const QMetaObject::Connection& discIter : qAsConst(sourceConnections))
             disconnect(discIter);
     }
     sourceConnections.clear();
     QAbstractProxyModel::setSourceModel(newSourceModel);
     if (sourceModel()) {
         sourceConnections = QList<QMetaObject::Connection>{
             connect(sourceModel(), &QAbstractItemModel::modelAboutToBeReset, this, &QTransposeProxyModel::beginResetModel),
             connect(sourceModel(), &QAbstractItemModel::modelReset, this, &QTransposeProxyModel::endResetModel),
             connect(sourceModel(), &QAbstractItemModel::dataChanged, this, &QTransposeProxyModel::onDataChanged),
             connect(sourceModel(), &QAbstractItemModel::headerDataChanged, this, &QTransposeProxyModel::onHeaderDataChanged),
             connect(sourceModel(), &QAbstractItemModel::columnsAboutToBeInserted, this, &QTransposeProxyModel::onColumnsAboutToBeInserted),
             connect(sourceModel(), &QAbstractItemModel::columnsAboutToBeMoved, this, &QTransposeProxyModel::onColumnsAboutToBeMoved),
             connect(sourceModel(), &QAbstractItemModel::columnsAboutToBeRemoved, this, &QTransposeProxyModel::onColumnsAboutToBeRemoved),
             connect(sourceModel(), &QAbstractItemModel::columnsInserted, this, &QTransposeProxyModel::endInsertRows),
             connect(sourceModel(), &QAbstractItemModel::columnsRemoved, this, &QTransposeProxyModel::endRemoveRows),
             connect(sourceModel(), &QAbstractItemModel::columnsMoved, this, &QTransposeProxyModel::endMoveRows),
             connect(sourceModel(), &QAbstractItemModel::rowsAboutToBeInserted, this, &QTransposeProxyModel::onRowsAboutToBeInserted),
             connect(sourceModel(), &QAbstractItemModel::rowsAboutToBeMoved, this, &QTransposeProxyModel::onRowsAboutToBeMoved),
             connect(sourceModel(), &QAbstractItemModel::rowsAboutToBeRemoved, this, &QTransposeProxyModel::onRowsAboutToBeRemoved),
             connect(sourceModel(), &QAbstractItemModel::rowsInserted, this, &QTransposeProxyModel::endInsertColumns),
             connect(sourceModel(), &QAbstractItemModel::rowsRemoved, this, &QTransposeProxyModel::endRemoveColumns),
             connect(sourceModel(), &QAbstractItemModel::rowsMoved, this, &QTransposeProxyModel::endMoveColumns),
             connect(sourceModel(), &QAbstractItemModel::layoutAboutToBeChanged, this, &QTransposeProxyModel::onLayoutAboutToBeChanged),
             connect(sourceModel(), &QAbstractItemModel::layoutChanged, this, &QTransposeProxyModel::onLayoutChanged)
         };
     }
     endResetModel();
}


QModelIndex QTransposeProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!this->sourceModel() || !sourceIndex.isValid())
        return QModelIndex();
    return this->sourceModel()->index(sourceIndex.column(), sourceIndex.row(), sourceIndex.parent());
}

QModelIndex QTransposeProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!this->sourceModel() || !proxyIndex.isValid())
            return QModelIndex();
    return this->sourceModel()->index(proxyIndex.column(), proxyIndex.row(), proxyIndex.parent());
}

QItemSelection QTransposeProxyModel::mapSelectionToSource(const QItemSelection &selection) const
{
    if (!this->sourceModel())
        return QItemSelection();

    QItemSelection sourceSelection;
    foreach(QModelIndex index, selection.indexes())
    {
        sourceSelection.select( mapToSource(index), mapToSource(index) );
    }

    return sourceSelection;
}

QItemSelection QTransposeProxyModel::mapSelectionFromSource(const QItemSelection &selection) const
{
    if (!this->sourceModel())
        return QItemSelection();

    QItemSelection proxySelection;
    foreach(QModelIndex index, selection.indexes())
    {
        proxySelection.select( mapFromSource(index), mapFromSource(index) );
    }

    return proxySelection;
}

QVariant QTransposeProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!sourceModel()) return QVariant();
    return sourceModel()->headerData(section, orientation == Qt::Vertical ? Qt::Horizontal : Qt::Vertical, role);
}

QVariant QTransposeProxyModel::data(const QModelIndex &index, int role) const
{
    QModelIndex sourceIndex = sourceModel()->index( index.column(), 0 );
    return sourceModel()->data(sourceIndex, role);
}

QMap<int, QVariant> QTransposeProxyModel::itemData(const QModelIndex &proxyIndex) const
{
    return sourceModel()->itemData( mapToSource( proxyIndex ) );
}

int QTransposeProxyModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if (!sourceModel()) return 0;
    return sourceModel()->rowCount();
}

int QTransposeProxyModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
    if (!sourceModel()) return 0;
    return sourceModel()->columnCount();
}

QModelIndex QTransposeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return sourceModel()->index(column, row);
}


/*!
    \reimp
*/
bool QTransposeProxyModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if (!this->sourceModel())
        return false;
    return this->sourceModel()->insertColumns(row, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool QTransposeProxyModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (!this->sourceModel())
        return false;
    return this->sourceModel()->removeColumns(row, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool QTransposeProxyModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (!this->sourceModel())
        return false;
    return this->sourceModel()->moveColumns(mapToSource(sourceParent), sourceRow, count, mapToSource(destinationParent), destinationChild);
}

/*!
    \reimp
*/
bool QTransposeProxyModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    if (!this->sourceModel())
        return false;
    return this->sourceModel()->insertRows(column, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool QTransposeProxyModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    if (!this->sourceModel())
        return false;
    return this->sourceModel()->removeRows(column, count, mapToSource(parent));
}

/*!
    \reimp
*/
bool QTransposeProxyModel::moveColumns(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (!this->sourceModel())
        return false;
    return this->sourceModel()->moveRows(mapToSource(sourceParent), sourceRow, count, mapToSource(destinationParent), destinationChild);
}

QSize QTransposeProxyModel::span(const QModelIndex &index) const
{
    if (!this->sourceModel() || !index.isValid())
        return QSize();
    return this->sourceModel()->span(mapToSource(index)).transposed();
}


#endif

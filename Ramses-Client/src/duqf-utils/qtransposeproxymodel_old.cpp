/****************************************************************************
**
** Copyright (C) 2018 Luca Beldi <v.ronin@yahoo.it>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


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

/*!
    \class QTransposeProxyModel
    \inmodule QtCore
    \brief This proxy transposes the source model.

    This model will make the rows of the source model become columns of the proxy model and vice-versa.

    If the model is a tree, the parents will be transposed as well. For example, if an index in the source model had parent `index(2,0)`, it will have parent `index(0,2)` in the proxy.
*/

/*!
    Constructs a new proxy model with the given \a parent.
*/
QTransposeProxyModel::QTransposeProxyModel(QObject* parent)
    : QIdentityProxyModel(parent)
{}

QTransposeProxyModel::~QTransposeProxyModel() = default;

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
         using namespace std::placeholders;
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

/*!
    \reimp
*/
int QTransposeProxyModel::rowCount(const QModelIndex &parent) const
{
    if (!this->sourceModel())
        return 0;
    return this->sourceModel()->columnCount(mapToSource(parent));
}

/*!
    \reimp
*/
int QTransposeProxyModel::columnCount(const QModelIndex &parent) const
{
    if (!this->sourceModel())
        return 0;
    return this->sourceModel()->rowCount(mapToSource(parent));
}

/*!
    \reimp
*/
QVariant QTransposeProxyModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (!this->sourceModel())
        return QVariant();
    return this->sourceModel()->headerData(section, orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal, role);
}

/*!
    \reimp
*/
bool QTransposeProxyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (!this->sourceModel())
        return false;
    return this->sourceModel()->setHeaderData(section, orientation == Qt::Horizontal ? Qt::Vertical : Qt::Horizontal, value, role);
}

/*!
    \reimp
*/
bool QTransposeProxyModel::setItemData(const QModelIndex &index, const QMap<int, QVariant> &roles)
{
    if (!this->sourceModel() || !index.isValid())
        return false;
    return this->sourceModel()->setItemData(mapToSource(index), roles);
}

/*!
    \reimp
*/
QSize QTransposeProxyModel::span(const QModelIndex &index) const
{
    if (!this->sourceModel() || !index.isValid())
        return QSize();
    return this->sourceModel()->span(mapToSource(index)).transposed();
}

/*!
    \reimp
*/
QMap<int, QVariant> QTransposeProxyModel::itemData(const QModelIndex &index) const
{
    if (!this->sourceModel())
        return QMap<int, QVariant>();
    return this->sourceModel()->itemData(mapToSource(index));
}

/*!
    \reimp
*/
QModelIndex QTransposeProxyModel::mapFromSource(const QModelIndex &sourceIndex) const
{
    if (!this->sourceModel() || !sourceIndex.isValid())
        return QModelIndex();
    return this->sourceModel()->index(sourceIndex.column(), sourceIndex.row(), sourceIndex.parent());
}

/*!
    \reimp
*/
QModelIndex QTransposeProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
    if (!this->sourceModel() || !proxyIndex.isValid())
        return QModelIndex();
    qDebug() << "map";
    qDebug() << checkIndex(proxyIndex);
    return this->sourceModel()->index(proxyIndex.column(), proxyIndex.row(), proxyIndex.parent());
}

/*!
    \reimp
*/
QModelIndex QTransposeProxyModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!this->sourceModel())
        return QModelIndex();
    return mapFromSource(this->sourceModel()->index(column, row, mapToSource(parent)));
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

/*!
    \reimp
    This method will perform no action. Use a QSortFilterProxyModel on top of this one if you require sorting.
*/
void QTransposeProxyModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column);
    Q_UNUSED(order);
    return;
}

#endif

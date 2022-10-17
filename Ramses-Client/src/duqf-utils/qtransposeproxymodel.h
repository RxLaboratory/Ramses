#ifndef DUQFTRANSPOSEPROXYMODEL_H
#define DUQFTRANSPOSEPROXYMODEL_H

#if (QT_VERSION < QT_VERSION_CHECK(5, 13, 0))

#include <QIdentityProxyModel>

class QTransposeProxyModel : public  QIdentityProxyModel
{
public:
    explicit QTransposeProxyModel(QObject* parent = nullptr);
    void setSourceModel(QAbstractItemModel* newSourceModel) override;
    virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    virtual QItemSelection mapSelectionToSource(const QItemSelection &selection) const override;
    virtual QItemSelection mapSelectionFromSource(const QItemSelection &selection) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QMap<int, QVariant> itemData(const QModelIndex &proxyIndex) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    bool moveColumns(const QModelIndex &sourceParent, int sourceColumn, int count, const QModelIndex &destinationParent, int destinationChild) override;
    QSize span(const QModelIndex &index) const override;
private:
    QList<QMetaObject::Connection> sourceConnections;
    QList<QPersistentModelIndex> layoutChangePersistentIndexes;
    QModelIndexList layoutChangeProxyIndexes;
    void onLayoutChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint);
    void onLayoutAboutToBeChanged(const QList<QPersistentModelIndex> &parents, QAbstractItemModel::LayoutChangeHint hint);
    void onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles);
    void onHeaderDataChanged(Qt::Orientation orientation, int first, int last);
    void onColumnsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void onColumnsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onColumnsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationColumn);
    void onRowsAboutToBeInserted(const QModelIndex &parent, int first, int last);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);
    void onRowsAboutToBeMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd, const QModelIndex &destinationParent, int destinationRow);
};

#else

#include <QTransposeProxyModel>

#endif // QT VERSION
#endif // QTRANSPOSEPROXYMODEL_H

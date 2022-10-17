#ifndef RAMFILTERLISTPROXYMODEL_H
#define RAMFILTERLISTPROXYMODEL_H

#include <QSortFilterProxyModel>


/**
 * @brief The RamObjectFilterList class is used to add an "ALL" item in the top of a list,
 * Used by QComboBox when displaying a list of filters.
 */
class RamFilterListProxyModel : public QSortFilterProxyModel
{
public:
    RamFilterListProxyModel(QString listName, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndex mapFromSource(const QModelIndex &sourceIndex) const override;
    QModelIndex mapToSource(const QModelIndex &proxyIndex) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;

private:
    QString m_listName;
};

#endif // RAMFILTERLISTPROXYMODEL_H

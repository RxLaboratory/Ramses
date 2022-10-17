#ifndef RAMOBJECTSORTFILTERPROXYMODEL_H
#define RAMOBJECTSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>

#include "ramobject.h"

/**
 * @brief The RamObjectSortFilterProxyModel class is a proxy used to filter and sort RamObjectModel.
 * Filters:
 *      by RamObject.filterUuid: single uuid used as a filter (e.g. containing group/object filter))
 *      search, by name / shortname: a string contained in the name or shortName
 *      by RamObject.filterListUuids: a list of uuids (e.g. assigned users)
 */
class RamObjectSortFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    // Construct and prepare
    explicit RamObjectSortFilterProxyModel(QObject *parent = nullptr);
    explicit RamObjectSortFilterProxyModel(QString listName, QObject *parent = nullptr);
    void setSingleColumn(bool singleColumn = true);

    // Model reimplementation
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Convenience access
    // An object by its row
    RamObject *get(int row) const;
    // An object by index
    RamObject *get(QModelIndex index) const;
    // An object by its shortname, or name
    RamObject *searchObject(QString searchString) const;

    RamObject::ObjectType type() const;

    // Set filters
    void setFilterUuid(const QString &filterUuid);
    void search(const QString &searchStr);
    void addFilterUuid(const QString &uuid);
    void removeFilterUuid(const QString &uuid);
    void clearFilterListUuids();

signals:
    void aboutToFilter();

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    void prepareFilter();

private:
    // Config
    QString m_listName;
    bool m_isSingleColumn = false;

    // Filters
    QString m_searchString;
    QString m_currentFilterUuid;
    QStringList m_filterListUuids;
};

#endif // RAMOBJECTSORTFILTERPROXYMODEL_H

#ifndef RAMITEMTABLELISTPROXY_H
#define RAMITEMTABLELISTPROXY_H

#include <QIdentityProxyModel>

/**
 * @brief The RamItemTableListProxy class is used to display a table in a single column list.
 * It just overrides the column count to a fixed value: 1.
 * This is necessary as in Ramses, lists are actually QTableViews with a single column.
 */
class RamItemTableListProxy : public QIdentityProxyModel
{
public:
    RamItemTableListProxy(QObject *parent = nullptr);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
};

#endif // RAMITEMTABLELISTPROXY_H

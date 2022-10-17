#ifndef DBTABLEMODEL_H
#define DBTABLEMODEL_H

#include "ramobjectmodel.h"

/**
 * @brief The DBTableModel class handles a list of objects taken from a complete table in the DB
 */
class DBTableModel: public RamObjectModel
{
    Q_OBJECT
public:
    DBTableModel(RamAbstractObject::ObjectType type, QObject *parent = nullptr);

private slots:
    void insertObject(QString uuid, QString table);
    void removeObject(QString uuid, QString table);
    void reload();
    void saveOrder(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
};

bool objSorter(const QStringList a, const QStringList b);

#endif // DBTABLEMODEL_H

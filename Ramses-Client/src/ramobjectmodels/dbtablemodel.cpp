#include "dbtablemodel.h"
#include "localdatainterface.h"

DBTableModel::DBTableModel(RamAbstractObject::ObjectType type, QObject *parent):
    RamObjectModel(type, parent)
{
    // Initial loading
    reload();

    // Monitor the DB for changes
    LocalDataInterface *ldi = LocalDataInterface::instance();
    connect(ldi, &LocalDataInterface::inserted, this, &DBTableModel::insertObject);
    connect(ldi, &LocalDataInterface::removed, this, &DBTableModel::removeObject);
    connect(ldi, &LocalDataInterface::dataReset, this, &DBTableModel::reload);

    // Monitor order changes to save the new orders
    connect(this, &RamObjectModel::rowsMoved, this, &DBTableModel::saveOrder);
}

void DBTableModel::insertObject(QString uuid, QString table)
{
    RamObject::ObjectType t = RamObject::objectTypeFromName( table );
    // Not for us
    if (t != m_type) return;

    // Insert
    insertObjects( rowCount(), QVector<QString>() << uuid );
}

void DBTableModel::removeObject(QString uuid, QString table)
{
    RamObject::ObjectType t = RamObject::objectTypeFromName( table );
    // Not for us
    if (t != m_type) return;

    // Remove
    removeObjects( QStringList(uuid) );
}

void DBTableModel::reload()
{
    clear();
    QVector<QStringList> objs = LocalDataInterface::instance()->tableData( RamObject::objectTypeName( type() ));

    // Sort
    std::sort(objs.begin(), objs.end(), objSorter);

    QVector<QString> uuids;
    for (int i = 0; i < objs.count(); i++)
    {
        uuids << objs[i][0];
    }

    insertObjects(0, uuids);
}

void DBTableModel::saveOrder(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{
    Q_UNUSED(parent);
    Q_UNUSED(destination);

    // Modified rows only
    int movedCount = end - start;
    int first = std::min(start, row);
    int last = std::max(end, row + movedCount);

    for (int i = first; i <= last; i++)
    {
        RamObject *o = this->get(i);
        o->setOrder(i);
    }
}

bool objSorter(const QStringList a, const QStringList b)
{
    QString dataA = a[1];
    QJsonDocument docA = QJsonDocument::fromJson(dataA.toUtf8());
    QJsonObject objA = docA.object();

    QString dataB = b[1];
    QJsonDocument docB = QJsonDocument::fromJson(dataB.toUtf8());
    QJsonObject objB = docB.object();

    int orderA = objA.value("order").toInt();
    int orderB = objB.value("order").toInt();

    if (orderA != orderB) return orderA < orderB;

    QString shortNameA = objA.value("shortName").toString();
    QString shortNameB = objB.value("shortName").toString();

    if (shortNameA != shortNameB) return shortNameA < shortNameB;

    QString nameA = objA.value("name").toString();
    QString nameB = objB.value("name").toString();

    if (nameA != nameB) return nameA < nameB;

    return false;
}

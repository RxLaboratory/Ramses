#ifndef RAMOBJECTMODEL_H
#define RAMOBJECTMODEL_H

#include <QAbstractTableModel>

#include "ramobject.h"
#include "ramobjectsortfilterproxymodel.h"

/**
 * @brief The RamObjectModel class represents a list of RamObjects
 */
class RamObjectModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    static RamObjectModel *emptyModel();

    explicit RamObjectModel(RamAbstractObject::ObjectType type, QObject *parent = nullptr);

    // Data Access
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Edit structure
    virtual void insertObjects(int row, QVector<QString> uuids);
    virtual void removeObjects(QStringList uuids);

    // Columns
    void setColumnModel(RamObjectModel *model);
    RamObjectSortFilterProxyModel *columnModel() const;

    // Support move rows and columns
    virtual bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;

    // Convenience access

    // Clear the model
    void clear();

    // Append object
    void appendObject(QString uuid);

    // An object by its row
    RamObject *get(int row);
    // An object by index
    static RamObject *get(const QModelIndex &index);
    // An object by its shortname, or name
    RamObject *search(QString searchString) const;

    // Check if contains
    bool contains(QString uuid) const;

    // The type of objects contained in this model
    RamObject::ObjectType type() const;

    // All the uuids
    QVector<QString> toVector() const;
    QStringList toStringList() const;

protected:
    static RamObjectModel *m_emptyModel;

    RamObject::ObjectType m_type;

private slots:
    void objectDataChanged(RamObject *obj);
    void columnDataChanged(RamObject *obj);

    // Column changes
    void insertModelColumns(const QModelIndex &parent, int first, int last);
    void removeModelColumns(const QModelIndex &parent, int first, int last);
    void moveModelColumns(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);

private:
    RamObject *getObject(QString uuid) const;
    void connectObject(QString uuid);
    void connectObject(RamObject *o);
    void disconnectObject(QString uuid);

    QVector<QString> m_objectsUuids;
    RamObjectSortFilterProxyModel *m_columnObjects;
};

#endif // RAMOBJECTMODEL_H

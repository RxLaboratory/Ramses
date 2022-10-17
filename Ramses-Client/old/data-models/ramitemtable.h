#ifndef RAMITEMTABLE_H
#define RAMITEMTABLE_H

#include "ramobjectlist.h"
#include "ramabstractitem.h"

/**
 * @brief The RamItemTable class is the model used to associate shots/assets with their status.
 * It keeps the latest status for each step associated to the corresponding shot/asset
 */
class RamItemTable : public RamObjectList
{
    Q_OBJECT
public:
    static RamItemTable *get(QString uuid);
    static RamItemTable *c(RamObjectList *o);

    // METHODS //

    RamItemTable(QString shortName, QString name, ObjectType type, QObject *parent = nullptr, DataListMode mode = ListObject);

    // MODEL REIMPLEMENTATION
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

public slots:
    // Used by RamProject to warn us the list of steps have changed
    void newSteps(const QModelIndex &parent, int first, int last);
    void removeSteps(const QModelIndex &parent, int first, int last);
    void moveSteps(const QModelIndex &parent, int first, int last, const QModelIndex &dest, int destRow);

protected:
    static QMap<QString, RamItemTable*> m_existingObjects;
    RamItemTable(QString uuid, QObject *parent = nullptr);

private slots:
    void removeItem(const QModelIndex &parent, int first, int last);
    void statusChanged(RamAbstractItem *item, RamStep *step);

    // Used to update the list of steps
    // when the first item is inserted.
    void inserted(const QModelIndex &parent, int first, int last);

private:

    // METHODS //
    void connectEvents();
    void updateStepList() const;
    // Utils
    RamAbstractItem *itemAt(int row) const;
    RamProject *project() const;
    RamStep *stepAt(int col) const;
    int stepCol(RamStep *step) const;
};

#endif // RAMITEMTABLE_H

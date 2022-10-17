#ifndef RAMOBJECTMENU_H
#define RAMOBJECTMENU_H

#include <QMenu>

#include "ramobjectmodel.h"
#include "ramobjectsortfilterproxymodel.h"

/**
 * @brief The RamObjectListMenu class is a QMenu showing a RamObjectModel.
 * The items can be checkable to be used as a selection list, and in this case the menu contains a 'Select all' and a 'Select none' QAction.
 * A 'Create new...' QAction can also be added to the top of the list.
 */
class RamObjectMenu : public QMenu
{
    Q_OBJECT
public:
    RamObjectMenu(bool checkable = false, QWidget *parent = nullptr);
    void setObjectModel(RamObjectModel *list);
    RamObjectSortFilterProxyModel *model();

    void setSortMode(RamObject::DataRole mode);

    void addCreateButton();

    void setObjectVisible(RamObject *obj, bool visible = true);
    void showAll();

    RamObject *objectAt(int i);

    bool isAllChecked() const;

public slots:
    void selectAll();
    void selectNone();
    void setFilterObject(RamObject *o);
    void select(RamObject *o);

    void saveState(QSettings *settings, QString group) const;
    void restoreState(QSettings *settings, QString group);

signals:
    void createTriggered();
    void assignmentChanged(RamObject*,bool);
    void assigned(RamObject*);

private slots:
    void newObject(const QModelIndex &parent, int first, int last);
    void removeObject(const QModelIndex &parent, int first, int last);
    void objectChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());

    void actionAssign(bool checked);
    void actionAssign();
    void actionCreate();

    void clear();
    void reset();

private:
    // UTILS //
    RamObject *object(QAction *a) const;

    RamObjectSortFilterProxyModel *m_objects;
    bool m_checkable = false;
};

#endif // RAMOBJECTMENU_H

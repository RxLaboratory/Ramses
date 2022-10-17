#ifndef OBJECTLISTEDITWIDGET_H
#define OBJECTLISTEDITWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMessageBox>
#include <QScrollBar>
#include <QMenu>
#include <QShortcut>

#include "duqf-widgets/duqfsearchedit.h"
#include "ramobjectlistcombobox.h"
#include "ramobjectlistmenu.h"
#include "ramuser.h"
#include "ramobjectview.h"

/**
 * @brief The ObjectListEditWidget class displays and edits a RamObjectList
 * It does not handle object creation:
 * Connect to the "add" signal to create a new object in the list
 * The add QToolButton is available with addButton() to be able to add a submenu to it for example.
 * It includes some filters
 * And a searh field
 */
class ObjectListEditWidget : public QWidget
{
    Q_OBJECT
public:
    enum EditMode { UnassignObjects, RemoveObjects };
    Q_ENUM( EditMode )

    explicit ObjectListEditWidget(bool editableObjects = false, RamUser::UserRole editRole = RamUser::Admin, QWidget *parent = nullptr);
    explicit ObjectListEditWidget(RamObjectList *objectList, bool editableObjects = false, RamUser::UserRole editRole = RamUser::Admin, QWidget *parent = nullptr);
    void setList(QAbstractItemModel *objectList);
    void setFilterList(RamObjectList *filterList);
    void setAssignList(RamObjectList *assignList);
    void setDontRemoveShortNameList(QStringList dontRemove);
    void clear();
    void setEditMode(ObjectListEditWidget::EditMode editMode);
    void setEditable(bool editable = true);
    void setSearchable(bool searchable = true);
    void setSortable(bool sortable = true);
    void setTitle(QString title);
    void select(RamObject *o);
    QToolButton *addButton() const;
    QString currentFilterUuid() const;
    RamObject *currentFilter() const;
    void setSortMode(RamObjectList::DataRole mode);
    void sort();

    RamObjectListView *listWidget();

public slots:
    void setFilter(RamObject *o);
    void removeSelectedObjects();

signals:
    void objectSelected(RamObject*);
    void add();

private slots:
    void assign(RamObject *obj);

    void objectAssigned(const QModelIndex &parent,int first,int last);
    void objectUnassigned(const QModelIndex &parent,int first,int last);

    void setSearchFocus();

private:
    void setupUi(bool editableObjects = false, RamUser::UserRole editRole = RamUser::Admin);
    void connectEvents();

    // UI Controls
    QToolButton *ui_addButton;
    QToolButton *ui_removeButton;
    QLabel *ui_title;
    RamObjectListComboBox *ui_filterBox;
    DuQFSearchEdit *ui_searchEdit;
    RamObjectListView *ui_listWidget;
    RamObjectListMenu *ui_assignMenu = nullptr;

    // Settings
    EditMode m_editMode = UnassignObjects;
    bool m_useAssignList = false;
    QStringList m_dontRemove;

    // Current List
    RamObject::ObjectType m_type = RamObject::State;
    QAbstractItemModel *m_objectList = nullptr;
    RamObjectList *m_assignList = nullptr;
    RamObjectList *m_filterList = nullptr;

    QList<QMetaObject::Connection> m_listConnections;
};

#endif // OBJECTLISTEDITWIDGET_H

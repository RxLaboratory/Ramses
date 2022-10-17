#ifndef OBJECTLISTWIDGET_H
#define OBJECTLISTWIDGET_H

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
#include "ramobjectcombobox.h"
#include "ramobjectmenu.h"
#include "ramobjectmodel.h"
#include "ramuser.h"
#include "ramobjectview.h"

/**
 * @brief The ObjectListWidget class displays and edits a RamObjectList
 * It does not handle object creation:
 * Connect to the "add" signal to create a new object in the list
 * The add QToolButton is available with addButton() to be able to add a submenu to it for example.
 * It includes some filters
 * And a searh field
 */
class ObjectListWidget : public QWidget
{
    Q_OBJECT
public:
    enum EditMode { UnassignObjects, RemoveObjects };
    Q_ENUM( EditMode )

    explicit ObjectListWidget(bool editableObjects = false, RamUser::UserRole editRole = RamUser::Admin, QWidget *parent = nullptr);
    explicit ObjectListWidget(RamObjectModel *objectList, bool editableObjects = false, RamUser::UserRole editRole = RamUser::Admin, QWidget *parent = nullptr);
    void setObjectModel(RamObjectModel *objectModel);
    void setFilterList(RamObjectModel *filterList, QString filterListName = "");
    void setAssignList(RamObjectModel *assignList);
    void setDontRemoveShortNameList(QStringList dontRemove);
    void clear();
    void setEditMode(ObjectListWidget::EditMode editMode);
    void setEditable(bool editable = true);
    void setSearchable(bool searchable = true);
    void setSortable(bool sortable = true);
    void setTitle(QString title);
    void select(RamObject *o);
    QToolButton *addButton() const;
    QString currentFilterUuid() const;
    RamObject *currentFilter() const;
    void setSortMode(RamObject::DataRole mode);
    void sort();

    RamObjectView *listWidget();

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
    RamObjectComboBox *ui_filterBox;
    DuQFSearchEdit *ui_searchEdit;
    RamObjectView *ui_objectView;
    RamObjectMenu *ui_assignMenu = nullptr;

    // Settings
    EditMode m_editMode = UnassignObjects;
    bool m_useAssignList = false;
    QStringList m_dontRemove;

    // Current List
    RamObjectModel *m_objectModel = nullptr;

    QList<QMetaObject::Connection> m_listConnections;
};

#endif // OBJECTLISTWIDGET_H

#ifndef RAMOBJECTLISTVIEW_H
#define RAMOBJECTLISTVIEW_H

#include <QTableView>
#include <QHeaderView>
#include <QScrollBar>
#include <QSortFilterProxyModel>

#include "ramitemsortfilterproxymodel.h"
#include "ramobjectdelegateold.h"
#include "ramuser.h"

/**
 * @brief The RamObjectListView class is the base class displaying for all lists in Ramses, displaying RamObject
 * It displays a RamObjectList using RamObjectDelegate for the painging.
 * It used mainly in ObjectListEditWidget (in order to manage the list)
 */
class RamObjectListView : public QTableView
{
    Q_OBJECT
public:
    enum EditMode { UnassignObjects, RemoveObjects };
    Q_ENUM( EditMode )
    enum DisplayMode { List, Table };
    Q_ENUM( DisplayMode )

    explicit RamObjectListView(DisplayMode mode = List, QWidget *parent = nullptr);
    explicit RamObjectListView(RamObjectList *list, DisplayMode mode = List, QWidget *parent = nullptr);
    explicit RamObjectListView(QAbstractItemModel *list, bool editableObjects, RamUser::UserRole editRole = RamUser::Admin, DisplayMode mode = List, QWidget *parent = nullptr);
    // Content
    void setList(QAbstractItemModel *list);
    RamItemFilterModel *filteredList();
    // Settings
    void setEditableObjects(bool editableObjects, RamUser::UserRole editRole = RamUser::Admin);
    void setSortable(bool sortable = true);
    void setSortMode(RamObjectList::DataRole mode);
    void sort();

signals:
    void objectSelected(RamObject*);

public slots:
    void search(QString s);
    void select(RamObject *o);
    void filter(RamObject *o);
    void setTimeTracking(bool trackTime);
    void setCompletionRatio(bool showCompletion);
    void showDetails(bool s);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

protected slots:
    // Moved
    void rowMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex);

private slots:
    void select(const QModelIndex &index);

private:
    void setupUi();
    void connectEvents();

    RamItemFilterModel *m_objectList = nullptr;
    DisplayMode m_displayMode;

    // Delegate
    RamObjectDelegateOld *m_delegate;

    // Filters
    QString m_currentFilterUuid;

    // UI Events
    QPoint m_initialDragPos;
    bool m_dragging = false;
    bool m_layout = false;
    QModelIndex m_clicking = QModelIndex();
};

#endif // RAMOBJECTLISTVIEW_H

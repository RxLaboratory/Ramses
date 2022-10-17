#ifndef RAMOBJECTVIEW_H
#define RAMOBJECTVIEW_H

#include <QTableView>
#include <QHeaderView>
#include <QScrollBar>
#include <QSortFilterProxyModel>

#include "ramobjectdelegate.h"
#include "ramobjectmodel.h"
#include "ramobjectsortfilterproxymodel.h"
#include "ramuser.h"

/**
 * @brief The RamObjectView class is the base class displaying for all lists in Ramses, displaying RamObject
 * It displays a RamObjectList using RamObjectDelegate for the painging.
 * It used mainly in ObjectListEditWidget (in order to manage the list)
 */
class RamObjectView : public QTableView
{
    Q_OBJECT
public:
    enum EditMode { UnassignObjects, RemoveObjects };
    Q_ENUM( EditMode )
    enum DisplayMode { List, Table };
    Q_ENUM( DisplayMode )

    explicit RamObjectView(DisplayMode mode = List, QWidget *parent = nullptr);
    // Settings
    void setEditableObjects(bool editableObjects, RamUser::UserRole editRole = RamUser::Admin);
    void setSortable(bool sortable = true);
    void setSortRole(RamObject::DataRole role);
    void sort();
   // Content
    void setObjectModel(RamObjectModel *model);

signals:
    void objectSelected(RamObject*);

public slots:
    void search(QString s);
    void select(RamObject *o);
    void filter(RamObject *o);
    void setCompletionRatio(bool showCompletion);
    void showDetails(bool s);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;

    // Model
    RamObjectSortFilterProxyModel *m_objectModel;

protected slots:
    // Moved
    void rowMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex);

private slots:
    void select(const QModelIndex &index);

private:
    void setupUi();
    void connectEvents();

    DisplayMode m_displayMode;

    // Delegate
    RamObjectDelegate *m_delegate;

    // Filters
    QString m_currentFilterUuid;

    // UI Events
    QPoint m_initialDragPos;
    bool m_dragging = false;
    bool m_layout = false;
    QModelIndex m_clicking = QModelIndex();
};

#endif // RAMOBJECTVIEW_H

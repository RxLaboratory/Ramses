#ifndef TIMELINEVIEW_H
#define TIMELINEVIEW_H

#include <QTableView>

#include "timelineproxy.h"
#include "ramshot.h"
#include "timelinedelegate.h"

/**
 * @brief The TimelineView class displays shots on a horizontal line,
 * derived from a QTableView, using the RamObjectList of the shots, displayed with the TimelineDelegate.
 */
class TimelineView : public QTableView
{
    Q_OBJECT
public:
    enum Transformation { Zoom, Pan, None };
    Q_ENUM(Transformation)

    TimelineView(QWidget *parent = nullptr);
    // Content
    void setObjectModel(RamObjectModel *shots);
    void zoom(double amount);
    double currentZoom() const;

public slots:
    void select(RamObject *o);
    void setZoom(double zoom);
    void resetZoom();

signals:
    void objectSelected(RamShot*);
    void zoomed(double);

protected:
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

protected slots:
    // Moved
    void columnMoved( int logicalIndex, int oldVisualIndex, int newVisualIndex);

private slots:
    void changeProject(RamProject*project);
    void select(const QModelIndex &index);
    void selectShot(RamShot *shot);

private:
    void setupUi();
    void connectEvents();
    // Delegate
    TimelineDelegate *m_delegate;
    // List
    TimeLineProxy *m_objectList;

    // Settings
    double m_zoom = 1.0;
    double m_zoomSensitivity = 0.2;

    // UI Events
    QPoint m_initialDragPos;
    bool m_layout = false;
    Transformation m_mouseTransformation = None;
    QModelIndex m_clicking = QModelIndex();
};

#endif // TIMELINEVIEW_H

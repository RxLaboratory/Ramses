#ifndef RAMSTEPHEADERVIEW_H
#define RAMSTEPHEADERVIEW_H

#include <QHeaderView>
#include <QtDebug>
#include <QPainter>
#include <QPainterPath>

#include "ramstep.h"

class RamStepHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    RamStepHeaderView(QWidget *parent = nullptr);

public slots:
    void setTimeTracking(bool newTimeTracking);
    void setCompletionRatio(bool newCompletionRatio);

    void setSortable( bool sortable = true );

signals:
    void sort(int col, Qt::SortOrder);
    void unsort();

protected:
    virtual void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

private:
    // Style
    QColor m_abyss;
    QColor m_dark;
    QColor m_medium;
    QColor m_lessLight;
    QColor m_light;
    QFont m_textFont;
    QFont m_detailsFont;
    int m_padding;

    // Buttons
    QPixmap m_editIcon;
    QPixmap m_folderIcon;
    QPixmap m_sortIcon;
    QPixmap m_sortUpIcon;
    QPixmap m_sortDownIcon;

    // Settings
    bool m_timeTracking = true;
    bool m_completionRatio = true;
    bool m_sortable = false;

    // Events
    int m_editButtonPressed = -1;
    int m_editButtonHover = -1;
    int m_folderButtonHover = -1;
    int m_folderButtonPressed = -1;
    int m_sortButtonHover = -1;
    int m_sortButtonPressed = -1;

    // Current sorting
    int m_sortColumn = -1;
    Qt::SortOrder m_sortOrder = Qt::DescendingOrder;

    // Utils
    bool canEdit() const;
    RamStep *getStep(int section) const;

    // drawing specific items
    void drawButton(QPainter *painter, QRect rect, QPixmap icon, bool hover = false, bool checked = false) const;
};

#endif // RAMSTEPHEADERVIEW_H

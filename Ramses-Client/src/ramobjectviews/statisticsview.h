#ifndef STATISTICSIEW_H
#define STATISTICSIEW_H

#include <QTableView>
#include <QHeaderView>
#include <QScrollBar>
#include <QMouseEvent>

#include "statisticsmodel.h"

class StatisticsView : public QTableView
{
public:
    StatisticsView(QWidget *parent = nullptr);
    void setUser(RamUser *user);

protected:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void showEvent(QShowEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void setupUi();
    void connectEvents();

    // Models
    StatisticsModel *m_statsModel;

    // UI Events
    QPoint m_initialDragPos;
    bool m_dragging = false;
};

#endif // STATISTICSIEW_H

#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>

#include "duqf-app/app-style.h"

class ProgressWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProgressWidget(QWidget *parent = nullptr);

signals:

public slots:
    void setCompletionRatio(int completionRatio);
    void setLatenessRatio(float latenessRatio);
    void setTimeSpent(qint64 timeSpent);
    void setEstimation(float newEstimation);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_completionRatio = 0;
    float m_latenessRatio = 1;
    qint64 m_timeSpent = 0;
    float m_estimation = 0;

    // Style
    QColor m_abyss;
    QColor m_dark;
    QColor m_medium;
    QColor m_lessLight;
    QColor m_light;
    int m_padding;

    // Settings
    bool m_timeTracking = true;
    bool m_showCompletionRatio = true;
};

#endif // PROGRESSWIDGET_H

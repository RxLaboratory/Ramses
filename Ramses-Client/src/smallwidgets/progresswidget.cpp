#include "progresswidget.h"

#include "ramstatus.h"

ProgressWidget::ProgressWidget(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum );


    m_abyss = DuUI::getColor("abyss-grey");
    m_dark = DuUI::getColor("dark-grey");
    m_medium = DuUI::getColor("medium-grey");
    m_lessLight = DuUI::getColor("less-light-grey");
    m_light = DuUI::getColor("light-grey");

    m_padding = 10;

    this->setMinimumHeight( 8 );
}

void ProgressWidget::setCompletionRatio(int completionRatio)
{
    m_completionRatio = completionRatio;
    update();
}

void ProgressWidget::setLatenessRatio(float latenessRatio)
{
    m_latenessRatio = latenessRatio;
    update();
}

void ProgressWidget::setTimeSpent(qint64 timeSpent)
{
    m_timeSpent = timeSpent;
    update();
}

void ProgressWidget::setEstimation(float newEstimation)
{
    m_estimation = newEstimation;
    update();
}

void ProgressWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // BG Bar
    QBrush statusBrush( m_abyss );
    int statusWidth = this->width() - 20 ;
    QRect statusRect( m_padding, 0, statusWidth, this->height() );
    QPainterPath path;
    path.addRoundedRect(statusRect, 5, 5);
    painter.fillPath(path, statusBrush);

    if (!m_timeTracking && !m_showCompletionRatio) return;

    // Draw a timebar first

    if ( m_timeTracking && m_latenessRatio > 0 )
    {
        QColor timeColor;
        if ( m_latenessRatio < 1.1 ) timeColor = QColor(32,62,32);
        else if ( m_latenessRatio < 1.2 ) timeColor = QColor(76,30,0);
        else if ( m_latenessRatio < 1.3 ) timeColor = QColor(93,31,0);
        else if ( m_latenessRatio < 1.4 ) timeColor = QColor(118,27,0);
        else if ( m_latenessRatio < 1.5 ) timeColor = QColor(140,23,0);
        else if ( m_latenessRatio < 1.6 ) timeColor = QColor(168,14,0);
        else if ( m_latenessRatio < 1.7 ) timeColor = QColor(188,0,0);
        else if ( m_latenessRatio < 1.8 ) timeColor = QColor(214,0,0);
        else if ( m_latenessRatio < 1.9 ) timeColor = QColor(236,0,0);
        else timeColor = QColor(255,0,0);
        statusBrush.setColor( timeColor );

        float timeSpentDays = RamStatus::hoursToDays( m_timeSpent/3600 );
        float ratio = timeSpentDays / m_estimation;

        statusRect.setWidth( statusWidth * ratio );
        if (statusRect.right() > this->width() - 10) statusRect.setRight( this->width() - 10);
        QPainterPath timePath;
        timePath.addRoundedRect(statusRect, 3, 3);
        painter.fillPath(timePath, statusBrush);
    }

    if (m_showCompletionRatio)
    {
        // Set a color according to the completion
        float completionRatio = m_completionRatio / 100.0;
        QColor completionColor;
        if (completionRatio < 0.12) completionColor = QColor( 197, 0, 0);
        else if (completionRatio < 0.25) completionColor = QColor( 197, 98, 17);
        else if (completionRatio < 0.5) completionColor = QColor( 197, 179, 40);
        else if (completionRatio < 0.75) completionColor = QColor( 128, 197, 37);
        else if (completionRatio < 0.88) completionColor = QColor( 100, 172, 69);
        else if (completionRatio < 0.98) completionColor = QColor( 55, 172, 23);
        else completionColor = QColor( 6, 116, 24);

        // Draw completion ratio
        statusBrush.setColor( completionColor );
        statusRect.setWidth(statusWidth * completionRatio);
        QPainterPath completionPath;
        completionPath.addRoundedRect(statusRect, 5, 5);
        painter.fillPath(completionPath, statusBrush);
    }//*/
}



#include "statisticsdelegate.h"

#include "ramstep.h"
#include "duqf-app/app-style.h"

StatisticsDelegate::StatisticsDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
    m_abyss = DuUI::getColor("abyss-grey");
    m_dark = DuUI::getColor("dark-grey");
    m_medium = DuUI::getColor("medium-grey");
    m_lessLight = DuUI::getColor("less-light-grey");
    m_light = DuUI::getColor("light-grey");
    m_textFont = qApp->font();
    m_textFont.setPixelSize( DuUI::getSize("font", "size-medium"));
    m_detailsFont = m_textFont;
    m_detailsFont.setItalic(true);
    m_padding = 10;

}

void StatisticsDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    int col = index.column();

    if (col != 0) return QStyledItemDelegate::paint(painter, option, index);

    // Base Settings
    const QRect rect = option.rect;
    painter->setRenderHint(QPainter::Antialiasing);

    // bg
    const QRect bgRect = rect.adjusted(m_padding,2,-m_padding,-2);

    // Select the bg Color
    QColor bgColor = m_dark;
    QColor textColor = m_lessLight;
    QColor detailsColor = m_medium;

    if (option.state & QStyle::State_MouseOver)
    {
        bgColor = bgColor.lighter();
        detailsColor = detailsColor.darker();
    }

    QBrush bgBrush(bgColor);
    QPen textPen(textColor);

    // Background
    QPainterPath path;
    path.addRoundedRect(bgRect, 5, 5);
    painter->fillPath(path, bgBrush);

    // Completion bar
    int statusWidth = bgRect.width() - m_padding ;
    QRect statusRect( bgRect.left() + m_padding / 2, bgRect.top() + m_padding, statusWidth, 0 );

    int estimation = index.data(Qt::UserRole +2).toInt();

    RamStep::Type stepType = static_cast<RamStep::Type>( index.data(Qt::UserRole +6).toInt() );
    if ( (stepType == RamStep::AssetProduction || stepType == RamStep::ShotProduction) && estimation > 0)
    {
        statusRect.setHeight(5);
        QBrush statusBrush( m_abyss );
        QPainterPath statusPath;
        statusPath.addRoundedRect(statusRect, 5, 5);
        painter->fillPath(statusPath, statusBrush);

        if (m_completionRatio)
        {
            float completionRatio = index.data(Qt::UserRole ).toInt() / 100.0;
            float latenessRatio = index.data(Qt::UserRole +1).toFloat();
            float missingDays = index.data(Qt::UserRole+7).toInt();

            // Add a warning
            if ((missingDays > 0.4 || missingDays < -0.4) && bgRect.height() > 35 && completionRatio < 0.99)
            {
                if (latenessRatio > 0.01 || latenessRatio < -0.01)
                {
                    QRect warnRect(statusRect.left(), bgRect.top() + bgRect.height() / 2 - m_padding/2, m_padding, m_padding);
                    statusBrush.setColor( index.data(Qt::ForegroundRole).value<QColor>());
                    QPainterPath warnPath;
                    warnPath.addEllipse(warnRect);
                    painter->fillPath(warnPath, statusBrush);
                }
            }

            if (m_completionRatio)
            {
                // Set a color according to the completion

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
                painter->fillPath(completionPath, statusBrush);


            }

        }
    }

    // Too Small !
    if (bgRect.height() < 35 ) return;

    // Details
    QRect detailsRect(statusRect.left()+2*m_padding, statusRect.bottom() + 5, statusWidth-+2*m_padding, bgRect.height() - 30);
    painter->setPen(textPen);
    painter->setFont(m_detailsFont);
    painter->drawText(detailsRect, Qt::AlignLeft | Qt::AlignTop, index.data(Qt::DisplayRole).toString(), &detailsRect);
    if (detailsRect.bottom() > bgRect.bottom() - 5)
    {
        drawMore(painter, bgRect, textPen);
    }//*/
}

QSize StatisticsDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.column() != 0) return QStyledItemDelegate::sizeHint(option, index);

    if (index.data(Qt::UserRole +4).toInt() <= 0) return QSize(200,20);

    RamStep::Type stepType = static_cast<RamStep::Type>( index.data(Qt::UserRole +6).toInt() );
    if (stepType == RamStep::AssetProduction || stepType == RamStep::ShotProduction )
    {
        float completionRatio = index.data(Qt::UserRole ).toInt();
        if (completionRatio > 99) return QSize(200,50);
        return QSize(200,105);
    } 

    return QSize(200,50);
}

void StatisticsDelegate::drawMore(QPainter *painter, QRect rect, QPen pen) const
{
    painter->save();
    painter->setPen( pen );
    painter->setFont(m_textFont);
    painter->drawText(
                QRect( rect.left(), rect.top(), rect.width() - 5, rect.height() - 5),
                "...",
                QTextOption(Qt::AlignRight | Qt::AlignBottom));
    painter->restore();
}

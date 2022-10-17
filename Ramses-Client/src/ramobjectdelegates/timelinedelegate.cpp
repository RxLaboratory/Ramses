#include "timelinedelegate.h"

#include "ramshot.h"

TimelineDelegate::TimelineDelegate(QObject *parent)
    : RamObjectDelegate{parent}
{

}

void TimelineDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QString shotUuid = index.data(RamObject::UUID).toString();
    if (shotUuid == "") return RamObjectDelegate::paint(painter, option, index);
    RamShot *shot = RamShot::get( shotUuid );
    if (!shot) return RamObjectDelegate::paint(painter, option, index);

    // Base Settings
    QRect rect = option.rect.adjusted(0,0,-3,0);
    painter->setRenderHint(QPainter::Antialiasing);

    // Select the bg Color
    QColor bgColor;
    bgColor = shot->color();
    if (!bgColor.isValid()) bgColor = m_dark;
    QColor textColor = m_lessLight;
    QColor detailsColor = m_medium;

    // Adjust according to bg
    if (bgColor.lightness() > 100)
    {
        textColor = m_abyss;
        detailsColor = m_dark;
    }

    // State mouseover
    if (option.state & QStyle::State_MouseOver)
    {
        bgColor = bgColor.lighter(120);
        detailsColor = detailsColor.lighter(120);
        textColor = textColor.lighter(120);
    }

    // Selected
    if (option.state & QStyle::State_Selected)
    {
        bgColor = bgColor.darker();
        textColor = textColor.lighter(150);
    }

    QBrush bgBrush(bgColor);

    // Background
    QPainterPath path;
    path.addRect(rect);
    painter->fillPath(path, bgBrush);

    // Draw either the preview, either the text
    QString previewImagePath = shot->previewImagePath();
    if (previewImagePath != "")
    {
        QRect imageRect = rect;
        QPixmap pix(previewImagePath);
        float pixRatio = pix.width() / float(pix.height());
        // Adjust image rect height to fit ratio
        float rectRatio = rect.width() / float(rect.height());
        if (rectRatio < pixRatio)
            imageRect.setHeight( rect.width() / pixRatio );
        else
            imageRect.setWidth( rect.height() * pixRatio );
        // Center image
        imageRect.moveCenter(rect.center());

        painter->drawPixmap( imageRect, QPixmap(previewImagePath));
    }
    else
    {
        QPen textPen(textColor);
        QPen commentPen(textColor);
        QPen detailsPen(detailsColor);

        QRect titleRect( rect.left() + 5, rect.top() + 5, rect.width() - 32, rect.height() - 10 );
        painter->setPen( textPen );
        painter->setFont( m_textFont );
        painter->drawText( titleRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, shot->name(), &titleRect);

        // Get details
        if (titleRect.bottom() + 5 < rect.bottom())
        {
            QString details = shot->details();

            // Draw details
            QRect detailsRect( titleRect.left() + 5, titleRect.bottom() + 3, rect.width() -5, rect.height() - titleRect.height() - 15 );
            if (detailsRect.height() > 15 )
            {
                painter->setPen( detailsPen );
                painter->setFont( m_detailsFont );
                painter->drawText( detailsRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, details, &detailsRect);
            }

            // Draw Comment
            QRect commentRect( titleRect.left() + 5, detailsRect.bottom() + 5, rect.width() - 5, rect.bottom() - detailsRect.bottom() - 5);
            if (detailsRect.bottom() + 20 < rect.bottom() && shot->comment() != "")
            {
                painter->setPen( commentPen );
                painter->setFont(m_textFont);
                painter->drawText( commentRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, shot->comment(), &commentRect);
            }
            else commentRect.setHeight(0);
        }
    }

    // Draw Folder button
    // Folder button
    if ( shot->path() != "" )
    {
         const QRect folderButtonRect( rect.right() - 20, rect.top() +7, 12, 12 );
         drawButton(painter, folderButtonRect, m_folderIcon, m_folderButtonHover == index);
    }
}

QSize TimelineDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    QString shotUuid = index.data(RamObject::UUID).toString();
    if (shotUuid == "") return RamObjectDelegate::sizeHint(option, index);
    RamShot *shot = RamShot::get( shotUuid );
    if (!shot) return RamObjectDelegate::sizeHint(option, index);

    return QSize(shot->duration()*50, 30);
}

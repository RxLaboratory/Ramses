#include "ramstepheaderview.h"

#include "ramstatus.h"
#include "duqf-app/app-style.h"
#include "ramstep.h"
#include "ramses.h"

RamStepHeaderView::RamStepHeaderView(QWidget *parent):
    QHeaderView(Qt::Horizontal, parent)
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

    m_editIcon = QIcon(":/icons/edit").pixmap(QSize(12,12));
    m_folderIcon = QIcon(":/icons/folder").pixmap(QSize(12,12));
    m_sortIcon = QIcon(":/icons/sort-no").pixmap(QSize(12,12));
    m_sortUpIcon = QIcon(":/icons/sort-up").pixmap(QSize(12,12));
    m_sortDownIcon = QIcon(":/icons/sort-down").pixmap(QSize(12,12));

    this->setMinimumHeight( 42 );
}

void RamStepHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    if (!painter) return;

    if (logicalIndex == 0) return QHeaderView::paintSection(painter,rect,logicalIndex);

    painter->setRenderHint(QPainter::Antialiasing);

    // Get the step
    RamStep *step = getStep(logicalIndex);
    if (!step) return QHeaderView::paintSection(painter,rect,logicalIndex);

    // Draw title
    QRect titleRect( rect.left(), rect.top(), rect.width(), rect.height() - 16 );
    QPen textPen( step->color() );
    painter->setPen( textPen );
    painter->setFont( m_textFont );
    painter->drawText( titleRect, Qt::AlignCenter | Qt::AlignVCenter | Qt::TextWordWrap, step->name(), &titleRect);

    // Draw buttons
    int xpos = rect.right() - 20;

    // Draw editbutton
    // Edit button
    if (canEdit())
    {
        const QRect editButtonRect( xpos, rect.top() +7, 12, 12 );
        xpos -= 22;
        drawButton(painter, editButtonRect, m_editIcon, m_editButtonHover == logicalIndex);
    }

    // Draw Folder button
    // Folder button
    const QRect folderButtonRect( xpos, rect.top() +7, 12, 12 );
    drawButton(painter, folderButtonRect, m_folderIcon, m_folderButtonHover == logicalIndex);

    // Draw Sort button
    if (m_sortable)
    {
        const QRect sortButtonRect( rect.left() + 10, rect.top( ) + 7, 12, 12 );
        if (m_sortColumn != logicalIndex) drawButton(painter, sortButtonRect, m_sortIcon, m_sortButtonHover == logicalIndex);
        else if (m_sortOrder == Qt::AscendingOrder) drawButton(painter, sortButtonRect, m_sortUpIcon, m_sortButtonHover == logicalIndex, true);
        else drawButton(painter, sortButtonRect, m_sortDownIcon, m_sortButtonHover == logicalIndex, true);
    }

    // Draw status

    // BG Bar
    QBrush statusBrush( m_abyss );
    int statusWidth = rect.width() - 20 ;
    QRect statusRect( rect.left() + 10, titleRect.bottom() + 5, statusWidth, 6 );
    QPainterPath path;
    path.addRoundedRect(statusRect, 5, 5);
    painter->fillPath(path, statusBrush);

    if (m_timeTracking || m_completionRatio)
    {
        // Draw a timebar first
        float latenessRatio = step->latenessRatio();
        if (m_timeTracking && latenessRatio > 0 && step->estimation() > 0)
        {
            QColor timeColor;
            if ( latenessRatio < 1.1 ) timeColor = QColor(32,62,32);
            else if ( latenessRatio < 1.2 ) timeColor = QColor(76,30,0);
            else if ( latenessRatio < 1.3 ) timeColor = QColor(93,31,0);
            else if ( latenessRatio < 1.4 ) timeColor = QColor(118,27,0);
            else if ( latenessRatio < 1.5 ) timeColor = QColor(140,23,0);
            else if ( latenessRatio < 1.6 ) timeColor = QColor(168,14,0);
            else if ( latenessRatio < 1.7 ) timeColor = QColor(188,0,0);
            else if ( latenessRatio < 1.8 ) timeColor = QColor(214,0,0);
            else if ( latenessRatio < 1.9 ) timeColor = QColor(236,0,0);
            else timeColor = QColor(255,0,0);
            statusBrush.setColor( timeColor );

            float timeSpentDays = RamStatus::hoursToDays( step->timeSpent()/3600 );
            float ratio = timeSpentDays / step->estimation();

            statusRect.setWidth( statusWidth * ratio );
            if (statusRect.right() > rect.right() - 10) statusRect.setRight( rect.right() - 10);
            QPainterPath timePath;
            timePath.addRoundedRect(statusRect, 3, 3);
            painter->fillPath(timePath, statusBrush);
        }

        if (m_completionRatio)
        {
            // Set a color according to the completion
            float completionRatio = step->completionRatio() / 100.0;
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
    }//*/
}

void RamStepHeaderView::mousePressEvent(QMouseEvent *event)
{
    const QPoint pos = event->pos();
    const int sectionIndex = this->logicalIndexAt( pos );
    const int sectionWidth = this->sectionSize( sectionIndex );
    const int sectionLeft = this->sectionViewportPosition( sectionIndex );

    const bool edit = canEdit();

    int xpos = sectionLeft + sectionWidth - 22;
    const QRect editButtonRect = QRect( xpos, 7, 20, 20 );
    if (edit) xpos -= 22;
    const QRect folderButtonRect( xpos, 7, 20, 20 );

    const QRect sortButtonRect( sectionLeft + 10, 7, 20, 20);

    if (editButtonRect.contains(pos) && edit)
    {
        m_editButtonPressed = sectionIndex;
        return;
    }

    if (folderButtonRect.contains(pos))
    {
        m_folderButtonPressed = sectionIndex;
        return;
    }

    if (m_sortable && sortButtonRect.contains(pos))
    {
        m_sortButtonPressed = sectionIndex;
    }

    QHeaderView::mousePressEvent(event);
}

void RamStepHeaderView::mouseReleaseEvent(QMouseEvent *event)
{
    const QPoint pos = event->pos();
    const int sectionIndex = this->logicalIndexAt( pos );
    const int sectionWidth = this->sectionSize( sectionIndex );
    const int sectionLeft = this->sectionViewportPosition( sectionIndex );

    const bool edit = canEdit();

    int xpos = sectionLeft + sectionWidth - 22;
    const QRect editButtonRect = QRect( xpos, 7, 20, 20 );

    if (edit) xpos -= 22;
    const QRect folderButtonRect( xpos, 7, 20, 20 );

    const QRect sortButtonRect( sectionLeft + 10, 7, 20, 20);

    if (m_editButtonPressed == sectionIndex)
    {
        if (editButtonRect.contains(pos))
        {
            // Get the step
            RamStep *step = getStep(sectionIndex);
            if (step) step->edit();
        }
        m_editButtonPressed = -1;
        return;
    }

    if (m_folderButtonPressed == sectionIndex)
    {
        if (folderButtonRect.contains(pos))
        {
            // Get the step
            RamStep *step = getStep(sectionIndex);
            if(step) step->revealFolder();
        }
        m_folderButtonPressed = -1;
        return;
    }

    if (m_sortable && m_sortButtonPressed == sectionIndex)
    {
        if (sortButtonRect.contains(pos))
        {
            // New sorting
            if (sectionIndex != m_sortColumn)
            {
                m_sortOrder = Qt::AscendingOrder;
                m_sortColumn = sectionIndex;
                emit sort( sectionIndex, m_sortOrder );
            }
            else if (m_sortOrder == Qt::AscendingOrder)
            {
                m_sortOrder = Qt::DescendingOrder;
                emit sort( sectionIndex, m_sortOrder );
            }
            else {
                m_sortColumn = -1;
                emit unsort();
            }
        }
    }

    QHeaderView::mouseReleaseEvent(event);
}

void RamStepHeaderView::mouseMoveEvent(QMouseEvent *event)
{
    const QPoint pos = event->pos();
    const int sectionIndex = this->logicalIndexAt( pos );
    const int sectionWidth = this->sectionSize( sectionIndex );
    const int sectionLeft = this->sectionViewportPosition( sectionIndex );

    const bool edit = canEdit();

    int xpos = sectionLeft + sectionWidth - 22;
    const QRect editButtonRect = QRect( xpos, 7, 20, 20 );

    if (edit) xpos -= 22;
    const QRect folderButtonRect( xpos, 7, 20, 20 );

    const QRect sortButtonRect( sectionLeft + 10, 7, 20, 20);

    if (editButtonRect.contains(pos) && edit)
    {
        m_editButtonHover = sectionIndex;
        this->update();
    }
    else if (m_editButtonHover == sectionIndex)
    {
        m_editButtonHover = -1;
        this->update();
    }

    if (folderButtonRect.contains(pos))
    {
        m_folderButtonHover = sectionIndex;
        this->update();
    }
    else if (m_folderButtonHover == sectionIndex)
    {
        m_folderButtonHover = -1;
        this->update();
    }

    if (m_sortable && sortButtonRect.contains(pos))
    {
        m_sortButtonHover = sectionIndex;
        this->update();
    }
    else if (m_sortButtonHover == sectionIndex)
    {
        m_sortButtonHover = -1;
        this->update();
    }

    QHeaderView::mouseMoveEvent(event);
}

bool RamStepHeaderView::canEdit() const
{
    return Ramses::instance()->isProjectAdmin();
}

RamStep *RamStepHeaderView::getStep(int section) const
{
    QString stepUuid = this->model()->headerData( section, Qt::Horizontal, RamObject::UUID).toString();
    if (stepUuid == "") return nullptr;
    return RamStep::get( stepUuid );
}

void RamStepHeaderView::drawButton(QPainter *painter, QRect rect, QPixmap icon, bool hover, bool checked) const
{
    if (hover || checked)
    {
        QPainterPath path;
        path.addRoundedRect(rect.adjusted(-5, -5, 5, 5), 3, 3);
        if (hover && checked) painter->fillPath(path, QBrush(m_abyss));
        painter->fillPath(path, QBrush(m_abyss));
    }
    painter->drawPixmap( rect, icon );
}

void RamStepHeaderView::setCompletionRatio(bool newCompletionRatio)
{
    m_completionRatio = newCompletionRatio;
}

void RamStepHeaderView::setSortable(bool sortable)
{
    m_sortable = sortable;
}

void RamStepHeaderView::setTimeTracking(bool newTimeTracking)
{
    m_timeTracking = newTimeTracking;
}

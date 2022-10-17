#include "ramscheduledelegate.h"

#include "ramobjectcombobox.h"
#include "ramses.h"
#include "ramscheduleentry.h"
#include "duqf-utils/guiutils.h"
#include "duqf-app/app-style.h"

RamScheduleDelegate::RamScheduleDelegate(QObject *parent) : QStyledItemDelegate(parent)
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

void RamScheduleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
     // Base Settings
    const QRect rect = option.rect;
    painter->setRenderHint(QPainter::Antialiasing);

    // PAINT BG

    // bg
    const QRect bgRect = rect.adjusted(m_padding/2,2,-m_padding/2,-2);

    // Select the bg Color
    QBrush bgBrush = index.data(Qt::BackgroundRole).value<QBrush>();

    // State
    QColor bgColor = bgBrush.color();
    if (option.state & QStyle::State_Selected) bgColor = bgColor.darker();
    else if (option.state & QStyle::State_MouseOver) bgColor = bgColor.lighter();

    // before today -> a bit darker
    QDate date = index.data(RamObject::Date).value<QDate>();
    if (date < QDate::currentDate()) bgColor = bgColor.darker(175);

    bgBrush.setColor(bgColor);

    QPainterPath path;
    path.addRoundedRect(bgRect, 5, 5);
    painter->fillPath(path, bgBrush);

    // Text color
    QColor textColor;
    if (bgColor.lightness() > 80) textColor = m_abyss;
    else textColor = m_lessLight;
    QPen textPen(textColor);

    // Too Small !
    if (bgRect.height() < 26 )
    {
        drawMore(painter, bgRect, textPen);
        return;
    }

    // icon
    const QRect iconRect( bgRect.left() + m_padding, bgRect.top() +7 , 12, 12 );
    // text
    const QRect textRect( iconRect.right() + 5, iconRect.top()-5, bgRect.width() - 37, iconRect.height()+5 );

    // Get the entry
    if (index.data(RamObject::IsComment).toBool()) // comment
    {
        QString text = index.data(Qt::DisplayRole).toString();
        QStringList lines = text.split("\n");

        // Title
        painter->setPen( textPen );
        QFont titleFont = m_textFont;
        titleFont.setBold(true);
        painter->setFont( titleFont );
        painter->drawText( textRect.adjusted(-12,0,0,0), Qt::AlignCenter | Qt::AlignHCenter, lines.takeFirst());

        // Other lines
        // Comment
        if (bgRect.height() > 35 && lines.count() > 0)
        {
            QRect commentRect( iconRect.left(), bgRect.top() + 35, bgRect.width() - m_padding*2, bgRect.height() - 35);
            painter->setFont( m_textFont );
            painter->drawText( commentRect, Qt::AlignLeft | Qt::AlignTop, lines.join("\n"), &commentRect);
            if (commentRect.bottom() > bgRect.bottom() - 5) drawMore(painter, bgRect, textPen);
        }

        return;
    }

    // icon
    QPixmap icon = index.data(Qt::DecorationRole).value<QPixmap>();
    if (!icon.isNull())
    {
        // icon color
        QColor iconColor;
        if (bgColor.lightness() > 80) iconColor = m_dark;
        else iconColor = m_medium;
        QImage iconImage(12,12, QImage::Format_ARGB32);
        iconImage.fill( iconColor );
        QPainter iconPainter(&iconImage);
        iconPainter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        iconPainter.drawPixmap( QRect(0,0,12,12), icon );
        painter->drawImage(iconRect, iconImage);
    }


    // Title
    painter->setPen( textPen );
    painter->setFont( m_textFont );
    painter->drawText( textRect, Qt::AlignCenter | Qt::AlignHCenter, index.data(Qt::DisplayRole).toString());

    // Comment
    if (bgRect.height() > 35)
    {
        QRect commentRect( iconRect.left(), bgRect.top() + 30, bgRect.width() - m_padding*2, bgRect.height() - 35);
        painter->drawText( commentRect, Qt::AlignLeft | Qt::AlignTop, index.data(RamObject::Comment).toString(), &commentRect);
        if (commentRect.bottom() > bgRect.bottom() - 5) drawMore(painter, bgRect, textPen);
    }
//*/
}

QSize RamScheduleDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)

    //return QSize(100,40);

    QString uuid = index.data(RamObject::UUID).toString();
    if (uuid == "") return QSize(0,40);

    QSize s;
    if (m_details) s = index.data(RamObject::DetailedSizeHint).toSize();
    else s = index.data(RamObject::SizeHint).toSize();

    // In schedules, the height is a bit bigger
    if (s.height() < 40) s.setHeight(40);

    return s;
}

bool RamScheduleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    // Return asap if we don't manage the event
    QEvent::Type type = event->type();
    if (type != QEvent::MouseButtonPress && type != QEvent::MouseButtonRelease && type != QEvent::MouseMove)
        return QStyledItemDelegate::editorEvent( event, model, option, index );

    RamUser *u = Ramses::instance()->currentUser();
    if (!u) return QStyledItemDelegate::editorEvent( event, model, option, index );
    if (u->role() < RamUser::Lead) return QStyledItemDelegate::editorEvent( event, model, option, index );

    if (type == QEvent::MouseButtonPress)
    {
        QMouseEvent *e = static_cast< QMouseEvent * >( event );
        if (e->button() != Qt::LeftButton) return false;
        if (e->modifiers() != Qt::NoModifier) return QStyledItemDelegate::editorEvent( event, model, option, index );
        m_indexPressed = index;
        return true;
    }

    if (type == QEvent::MouseButtonRelease)
    {
        QMouseEvent *e = static_cast< QMouseEvent * >( event );
        if (e->button() != Qt::LeftButton) return QStyledItemDelegate::editorEvent( event, model, option, index );
        if (m_indexPressed == index && m_indexPressed.isValid() && !index.data(RamObject::IsComment).toBool())
        {
            RamProject *proj = Ramses::instance()->currentProject();
            if (proj)
            {
                QMainWindow *mw = GuiUtils::appMainWindow();
                RamObjectComboBox *editor = new RamObjectComboBox( mw );
                editor->setObjectModel(proj->steps(), "Steps");
                QRect rect = option.rect.adjusted(m_padding,2,-m_padding,-2);
                editor->setGeometry( rect );
                editor->move( mw->mapFromGlobal( e->globalPos() )  - e->pos() + QPoint(rect.left(), rect.top() ) );
                // Get current
                QString stepUuid = index.data(Qt::EditRole).toString();
                if (stepUuid != "")
                {
                    RamStep *step = RamStep::get(stepUuid);
                    if (step) editor->setObject(step);
                }
                editor->show();
                editor->showPopup();
                connect(editor, &RamObjectComboBox::currentObjectChanged, this, &RamScheduleDelegate::setEntry);
                connect(editor, &RamObjectComboBox::popupHidden, editor, &RamObjectComboBox::deleteLater);
            }
        }
        return true;
    }

    return QStyledItemDelegate::editorEvent( event, model, option, index );
}

void RamScheduleDelegate::showDetails(bool s)
{
    m_details = s;
}

void RamScheduleDelegate::setEntry(RamObject *step)
{
    if (!m_indexPressed.isValid()) return;

    // Get current entry if any
    QString entryUuid = m_indexPressed.data(RamObject::UUID).toString();
    RamScheduleEntry *entry = nullptr;
    if (entryUuid != "") entry = RamScheduleEntry::get( entryUuid );
    if (!entry && step)
    {
        // Get user
        QString userUuid = m_indexPressed.model()->headerData( m_indexPressed.row(), Qt::Vertical, RamObject::UUID ).toString();
        RamUser *user = nullptr;
        if (userUuid != "") user = RamUser::get( userUuid );
        if (!user) return;
        // Get Date
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        QDateTime date = QDateTime( m_indexPressed.model()->headerData( m_indexPressed.column(), Qt::Horizontal, RamObject::Date).toDate() );
#else
        QDateTime date = m_indexPressed.model()->headerData( m_indexPressed.column(), Qt::Horizontal, RamObject::Date).toDate().startOfDay();
#endif
        if (  m_indexPressed.model()->headerData( m_indexPressed.row(), Qt::Vertical, RamObject::IsPM ).toBool() )
            date.setTime(QTime(12,0));

        entry = new RamScheduleEntry( user, date );
        entry->setStep( RamStep::c( step ) );
        user->schedule()->appendObject(entry->uuid());
    }
    else if (entry)
    {
        if (step)
            entry->setStep( RamStep::c( step ) );
        else {
            RamUser *u = entry->user();
            if (u) u->schedule()->removeObjects(QStringList(entryUuid));
            entry->remove();
        }
    }
}

bool RamScheduleDelegate::details() const
{
    return m_details;
}

void RamScheduleDelegate::drawMore(QPainter *painter, QRect rect, QPen pen) const
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

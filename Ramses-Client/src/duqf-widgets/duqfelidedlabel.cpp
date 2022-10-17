#include "duqfelidedlabel.h"

DuQFElidedLabel::DuQFElidedLabel(QWidget *parent):
    QLabel(parent)
{

}

Qt::TextElideMode DuQFElidedLabel::elideMode() const
{
    return m_elideMode;
}

void DuQFElidedLabel::setElideMode(Qt::TextElideMode newElideMode)
{
    if (newElideMode != m_elideMode)
    {
        m_elideMode = newElideMode;
        this->updateGeometry();
    }
}

QSize DuQFElidedLabel::minimumSizeHint() const
{
    return this->sizeHint();
}

QSize DuQFElidedLabel::sizeHint() const
{
    QFontMetrics fm = this->fontMetrics();
    QSize textSize = fm.boundingRect( this->text() ).size();
    QMargins margins = this->contentsMargins();
    int margin = this->margin() * 2;
    return QSize(
                std::min( this->minimumWidth(), textSize.width() + margins.left() + margins.right() + margin ),
                std::min( fm.height(), textSize.height() + margins.top() + margins.bottom() + margin )
                );
}

void DuQFElidedLabel::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    QFontMetrics fm = this->fontMetrics();
    int margin = this->margin();
    int newMargin = fm.horizontalAdvance("x") / 2;
    QRect rect = this->contentsRect().adjusted( newMargin, margin, -newMargin, -margin);
    painter.drawText(rect, this->alignment(), fm.elidedText(this->text(), m_elideMode, rect.width()) );//*/
}

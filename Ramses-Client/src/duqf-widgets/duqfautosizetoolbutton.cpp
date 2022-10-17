#include "duqfautosizetoolbutton.h"

DuQFAutoSizeToolButton::DuQFAutoSizeToolButton(QWidget *parent) :
    QToolButton(parent)
{

}

void DuQFAutoSizeToolButton::setText(const QString &text)
{
    QFont font = this->font();
    QFontMetrics fontMetrics(font);
    QSize size = fontMetrics.size(Qt::TextSingleLine | Qt::TextShowMnemonic, text);
    this->setMinimumWidth(size.width() + m_margins*2 + this->iconSize().width() + 3);
    QToolButton::setText(text);
}

int DuQFAutoSizeToolButton::margins() const
{
    return m_margins;
}

void DuQFAutoSizeToolButton::setMargins(int newMargins)
{
    if (m_margins == newMargins) return;
    m_margins = newMargins;
    this->setText(this->text());
}

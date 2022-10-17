#include "duqfcolorselector.h"

DuQFColorSelector::DuQFColorSelector(QWidget *parent) : QWidget(parent)
{
    m_color = QColor(25,25,25);
    setupUi();
    connectEvents();
}

void DuQFColorSelector::updateColorEditStyle()
{
    if (ui_colorEdit->text().count() == 7)
    {
        m_color.setNamedColor(ui_colorEdit->text());
        QString style = "background-color: " + m_color.name() + ";";
        if (m_color.lightness() > 80) style += "color: #232323;";
        ui_colorEdit->setStyleSheet(style);
    }
    emit colorChanged(m_color);
}

void DuQFColorSelector::selectColor()
{
    this->setEnabled(false);
    QColorDialog cd( m_color, this );
    cd.setOptions(QColorDialog::DontUseNativeDialog);
    cd.setCurrentColor(m_color);
    //cd.setWindowFlags(Qt::FramelessWindowHint);
    //cd.move(this->parentWidget()->parentWidget()->geometry().center().x()-cd.geometry().width()/2,this->parentWidget()->parentWidget()->geometry().center().y()-cd.geometry().height()/2);
    if (cd.exec())
    {
        m_color = cd.selectedColor();
        ui_colorEdit->setText(m_color.name());
        updateColorEditStyle();
    }
    this->setEnabled(true);
}

void DuQFColorSelector::setupUi()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *colorLayout = new QHBoxLayout(this);
    colorLayout->setSpacing(3);
    colorLayout->setContentsMargins(0,0,0,0);

    ui_colorEdit = new QLineEdit(this);
    colorLayout->addWidget(ui_colorEdit);

    ui_colorButton = new QToolButton(this);
    ui_colorButton->setIcon(QIcon(":/icons/color-dialog"));
    colorLayout->addWidget(ui_colorButton);

}

void DuQFColorSelector::connectEvents()
{
     connect(ui_colorEdit, SIGNAL(editingFinished()), this, SLOT(updateColorEditStyle()));
     connect(ui_colorButton, SIGNAL(clicked()), this, SLOT(selectColor()));
}

const QColor &DuQFColorSelector::color() const
{
    return m_color;
}

void DuQFColorSelector::setColor(const QColor &newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    ui_colorEdit->setText(m_color.name());
    updateColorEditStyle();
}


#include "ramstatebox.h"

#include "ramses.h"

RamStateBox::RamStateBox(QWidget *parent):
    RamObjectComboBox(parent)
{
    this->setModel(Ramses::instance()->states());
    connect(this, &RamObjectComboBox::currentObjectChanged, this, &RamStateBox::changeCurrentState);
}

void RamStateBox::update()
{
    changeCurrentState(this->currentObject());
}

void RamStateBox::changeCurrentState(RamObject *state)
{
    if (!state) return;

    this->setItemData(this->currentIndex(), state->shortName(), Qt::DisplayRole);

    QString colorStyle = "background-color: " + state->color().name() + "; ";
    if (state->color().lightness() > 80) colorStyle += "color: #232323;  ";

    QString style = "QComboBox { ";
    style += colorStyle;
    style += "}";


    this->setStyleSheet(style);

    this->setToolTip(state->name());

    this->repaint();

    emit currentStateChanged(RamState::c( state ));
}

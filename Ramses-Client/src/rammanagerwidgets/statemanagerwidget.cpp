#include "statemanagerwidget.h"

#include "ramses.h"

StateManagerWidget::StateManagerWidget(QWidget *parent) :
    ObjectManagerWidget(
        Ramses::instance()->states(),
        "States",
        QIcon(":icons/state-l"),
        parent)
{
    m_listWidget->setEditMode(ObjectListWidget::RemoveObjects);
    QStringList dontRemove;
    dontRemove << "TODO" << "STB" << "NO" << "WIP" << "OK";
    m_listWidget->setDontRemoveShortNameList(dontRemove);
    m_listWidget->setSortMode(RamObject::Completion);
}

RamState *StateManagerWidget::createObject()
{
    RamState *state = new RamState(
                "NEW",
                "New state");
    state->edit();
    return state;
}


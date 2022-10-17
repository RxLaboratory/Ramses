#ifndef STATEMANAGERWIDGET_H
#define STATEMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramstate.h"

class StateManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    explicit StateManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamState *createObject() override;
};

#endif // STATEMANAGERWIDGET_H

#ifndef APPLICATIONMANAGERWIDGET_H
#define APPLICATIONMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramapplication.h"

class ApplicationManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    ApplicationManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamApplication *createObject() override;
};

#endif // APPLICATIONMANAGERWIDGET_H

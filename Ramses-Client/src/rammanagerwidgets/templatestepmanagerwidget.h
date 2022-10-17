#ifndef TEMPLATESTEPMANAGERWIDGET_H
#define TEMPLATESTEPMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramtemplatestep.h"

class TemplateStepManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    TemplateStepManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamTemplateStep *createObject() override;
};

#endif // TEMPLATESTEPMANAGERWIDGET_H

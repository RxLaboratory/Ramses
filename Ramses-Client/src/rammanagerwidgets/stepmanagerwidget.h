#ifndef STEPMANAGERWIDGET_H
#define STEPMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramobjectmenu.h"
#include "ramstep.h"

class StepManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    StepManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamStep *createObject() override;
private slots:
    void changeProject(RamProject *project);

    void createFromTemplate(RamObject *templateStepObj);

private:
    RamObjectMenu *ui_createMenu;
};

#endif // STEPMANAGERWIDGET_H

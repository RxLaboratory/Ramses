#ifndef PROJECTMANAGERWIDGET_H
#define PROJECTMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramproject.h"

class ProjectManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    ProjectManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamProject *createObject() override;
};

#endif // PROJECTMANAGERWIDGET_H

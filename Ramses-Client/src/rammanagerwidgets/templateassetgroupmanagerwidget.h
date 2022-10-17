#ifndef TEMPLATEASSETGROUPMANAGERWIDGET_H
#define TEMPLATEASSETGROUPMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramtemplateassetgroup.h"

class TemplateAssetGroupManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    TemplateAssetGroupManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamTemplateAssetGroup *createObject() override;
};

#endif // TEMPLATEASSETGROUPMANAGERWIDGET_H

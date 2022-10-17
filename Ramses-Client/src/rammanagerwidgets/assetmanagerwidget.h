#ifndef ASSETMANAGERWIDGET_H
#define ASSETMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramabstractitem.h"

class AssetManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    AssetManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamAbstractItem *createObject() override;

private slots:
    void changeProject(RamProject *project);
};

#endif // ASSETMANAGERWIDGET_H

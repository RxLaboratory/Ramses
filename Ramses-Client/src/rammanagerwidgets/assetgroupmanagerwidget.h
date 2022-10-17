#ifndef ASSETGROUPMANAGERWIDGET_H
#define ASSETGROUPMANAGERWIDGET_H

#include "objectmanagerwidget.h"
#include "ramobjectmenu.h"
#include "ramassetgroup.h"

class AssetGroupManagerWidget : public ObjectManagerWidget
{
    Q_OBJECT
public:
    AssetGroupManagerWidget(QWidget *parent = nullptr);

protected slots:
    RamAssetGroup *createObject() override;

private slots:
    void changeProject(RamProject *project);

    void createFromTemplate(RamObject *templateAGObj);

private:
    RamObjectMenu *ui_createMenu;
};

#endif // ASSETGROUPMANAGERWIDGET_H

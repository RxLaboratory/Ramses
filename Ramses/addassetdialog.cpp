#include "addassetdialog.h"

AddAssetDialog::AddAssetDialog(DBInterface *d, RAMShot *sh, RAMStage *st, Updater *up, QList<RAMAsset*> aa, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    dbi = d;
    shot = sh;
    stage = st;
    updater = up;
    allAssets = aa;

    //get STB status
    foreach(RAMStatus *s,updater->getStatuses())
    {
        if (s->getShortName() == "STB")
        {
            status = s;
            break;
        }
    }

    //get all assets from stage if assets are not provided
    if (allAssets.count() == 0)
    {
        allAssets = stage->getAssets();
    }

    foreach(RAMAsset *asset, allAssets)
    {
        assetList->addItem(asset->getShortName(),asset->getId());
    }

    connect(nameEdit,SIGNAL(returnPressed()),this,SLOT(on_createButton_clicked()));
}

RAMAsset *AddAssetDialog::getAsset()
{
    return asset;
}

void AddAssetDialog::on_addButton_clicked()
{
    if (assetList->currentIndex() < 0) return;
    foreach(RAMAsset *a,allAssets)
    {
        if (assetList->currentData().toInt() == a->getId())
        {
            a->assign(shot,true);
            asset = a;
            break;
        }
    }

    this->accept();
}

void AddAssetDialog::on_createButton_clicked()
{
    if (nameEdit->text() == "") return;

    asset = new RAMAsset(dbi,nameEdit->text(),shortNameEdit->text(),status,stage->getId(),true,"",-1,updater->getCurrentProject()->getId());
    asset->assign(shot,true);
    stage->addAsset(asset);

    this->accept();
}

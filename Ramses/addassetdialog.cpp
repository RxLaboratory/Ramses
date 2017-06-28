#include "addassetdialog.h"

AddAssetDialog::AddAssetDialog(DBInterface *d, RAMShot *sh, RAMStage *st, RAMStatus *sta, QList<RAMAsset *> aa, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    dbi = d;
    shot = sh;
    stage = st;
    status = sta;
    allAssets = aa;
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
    /*if (assetList->currentIndex() < 0) return;
    //shot->addAsset(allAssets[assetList->currentIndex()]);
    this->accept();*/
}

void AddAssetDialog::on_createButton_clicked()
{
    if (nameEdit->text() == "") return;
    //generate id
    int id = 1;
    foreach(RAMAsset *ra,allAssets)
    {
        if (ra->getId() >= id) id = ra->getId()+1;
    }
    asset = new RAMAsset(dbi,id,nameEdit->text(),shortNameEdit->text(),status,"",true);
    asset->assign(stage,shot,true);

    this->accept();
}

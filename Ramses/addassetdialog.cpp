#include "addassetdialog.h"

AddAssetDialog::AddAssetDialog(DBInterface *d, RAMShot *sh, RAMStage *st, int sid, QList<RAMAsset *> aa, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    dbi = d;
    shot = sh;
    stage = st;
    statusId = sid;
    allAssets = aa;
    foreach(RAMAsset *asset, allAssets)
    {
        assetList->addItem(asset->getShortName(),asset->getId());
    }
}

void AddAssetDialog::on_addButton_clicked()
{
    dbi->assignAsset(assetList->currentData().toInt(),shot->getId());
    this->accept();
}

void AddAssetDialog::on_createButton_clicked()
{
    if (nameEdit->text() == "") return;
    dbi->addAsset(nameEdit->text(),shortNameEdit->text(),stage->getId(),statusId,shot->getId());
    this->accept();
}

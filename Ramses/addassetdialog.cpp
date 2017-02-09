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

    connect(nameEdit,SIGNAL(returnPressed()),this,SLOT(on_createButton_clicked()));
}

void AddAssetDialog::on_addButton_clicked()
{
    if (assetList->currentIndex() < 0) return;
    shot->addAsset(allAssets[assetList->currentIndex()]);
    this->accept();
}

void AddAssetDialog::on_createButton_clicked()
{
    if (nameEdit->text() == "") return;
    dbi->addAsset(nameEdit->text(),shortNameEdit->text(),stage->getId(),statusId,shot->getId());
    this->accept();
}

#include "addassetdialog.h"

AddAssetDialog::AddAssetDialog(DBInterface *d, RAMShot *sh, RAMStage *st, int sid, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    dbi = d;
    shot = sh;
    stage = st;
    statusId = sid;
}

void AddAssetDialog::on_addButton_clicked()
{

}

void AddAssetDialog::on_createButton_clicked()
{
    if (nameEdit->text() == "") return;
    dbi->addAsset(nameEdit->text(),shortNameEdit->text(),stage->getId(),statusId,shot->getId());
    this->accept();
}

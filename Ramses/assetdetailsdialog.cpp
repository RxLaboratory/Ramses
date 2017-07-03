#include "assetdetailsdialog.h"

AssetDetailsDialog::AssetDetailsDialog(RAMAsset *a, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);

    asset = a;

    shortNameEdit->setText(asset->getShortName());
    nameEdit->setText(asset->getName());
    commentEdit->setText(asset->getComment());
}

void AssetDetailsDialog::on_buttonBox_accepted()
{
    asset->setShortName(shortNameEdit->text());
    asset->setName(nameEdit->text());
    asset->setComment(commentEdit->toPlainText());
    asset->update();
}

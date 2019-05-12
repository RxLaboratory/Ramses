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

void AssetDetailsDialog::on_deleteButton_clicked()
{
    setEnabled(false);
    //ask for confirmation
    QMessageBox message(QMessageBox::Warning,"Confirm deletion", "Are you sure you want to delete this asset?\n" + asset->getShortName() + "\n" + asset->getName(), QMessageBox::Yes | QMessageBox::No,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    message.setDefaultButton(QMessageBox::No);
    message.setEscapeButton(QMessageBox::No);

    if (message.exec() == QMessageBox::Yes)
    {
        asset->remove(true);
        close();
    }
    setEnabled(true);

}

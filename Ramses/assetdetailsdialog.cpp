#include "assetdetailsdialog.h"

AssetDetailsDialog::AssetDetailsDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

QString AssetDetailsDialog::getComment()
{
    return commentEdit->toHtml();
}

void AssetDetailsDialog::setComment(QString c)
{
    commentEdit->setHtml(c);
}

#include "shotdetailsdialog.h"

ShotDetailsDialog::ShotDetailsDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}

QString ShotDetailsDialog::getComment()
{
    return commentEdit->toHtml();
}

void ShotDetailsDialog::setComment(QString c)
{
    commentEdit->setHtml(c);
}

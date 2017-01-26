#ifndef ASSETDETAILSDIALOG_H
#define ASSETDETAILSDIALOG_H

#include "ui_assetdetailsdialog.h"

class AssetDetailsDialog : public QDialog, private Ui::AssetDetailsDialog
{
    Q_OBJECT

public:
    explicit AssetDetailsDialog(QWidget *parent = 0);
    QString getComment();
    void setComment(QString c);
};

#endif // ASSETDETAILSDIALOG_H

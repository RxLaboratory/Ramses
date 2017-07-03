#ifndef ASSETDETAILSDIALOG_H
#define ASSETDETAILSDIALOG_H

#include "ui_assetdetailsdialog.h"
#include "ramshot.h"
#include "ramasset.h"

class AssetDetailsDialog : public QDialog, private Ui::AssetDetailsDialog
{
    Q_OBJECT

public:
    explicit AssetDetailsDialog(RAMAsset *a, QWidget *parent = 0);

private slots:
    void on_buttonBox_accepted();

private:
    RAMAsset *asset;
};

#endif // ASSETDETAILSDIALOG_H

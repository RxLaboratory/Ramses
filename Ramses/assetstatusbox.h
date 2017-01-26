#ifndef ASSETSTATUSBOX_H
#define ASSETSTATUSBOX_H

#include "ui_assetstatusbox.h"
#include "ramasset.h"
#include "assetdetailsdialog.h"

class AssetStatusBox : public QWidget, private Ui::AssetStatusBox
{
    Q_OBJECT

public:   
    explicit AssetStatusBox(RAMAsset *as, QList<RAMStatus *> sl, QWidget *parent = 0);

private slots:
    void assetStatusChanged();
    void on_comboBox_currentIndexChanged(int index);

    void on_detailsButton_clicked();

signals:
    void dialogShown(bool);

private:
    RAMAsset *asset;
    QList<RAMStatus *> statusesList;
    bool freezeDBI;
    bool freezeUI;
};

#endif // ASSETSTATUSBOX_H

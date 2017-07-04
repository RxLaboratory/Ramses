#ifndef ASSETSTATUSBOX_H
#define ASSETSTATUSBOX_H

#include "ui_assetstatusbox.h"
#include "ramasset.h"
#include "assetdetailsdialog.h"

class AssetStatusBox : public QWidget, private Ui::AssetStatusBox
{
    Q_OBJECT

public:   
    explicit AssetStatusBox(RAMAsset *as, QList<RAMStatus *> sl,RAMShot *s, QWidget *parent = 0);

private slots:
    void assetStatusChanged(RAMAsset*a, RAMStatus*s);
    void on_comboBox_currentIndexChanged(int index);
    void on_removeButton_clicked();
    void on_detailsButton_clicked();
    void nameChanged(QString n);
    void shortNameChanged(QString n);
    void unAssign(RAMShot*s,RAMAsset*a);


signals:
    void dialogShown(bool);

private:
    RAMAsset *asset;
    RAMShot *shot;
    QList<RAMStatus *> statusesList;
    bool freezeDBI;
    bool freezeUI;
};

#endif // ASSETSTATUSBOX_H

#ifndef ASSETSTATUSWIDGET_H
#define ASSETSTATUSWIDGET_H

#include "ui_assetstatuswidget.h"
#include "ramasset.h"
#include "ramshot.h"
#include "ramstage.h"
#include "ramstatus.h"
#include "addassetdialog.h"
#include "dbinterface.h"
#include "assetstatusbox.h"

class AssetStatusWidget : public QWidget, private Ui::AssetStatusWidget
{
    Q_OBJECT

public:
    explicit AssetStatusWidget(RAMShot *s, RAMStage *st, QList<RAMStatus *> sl, QList<RAMAsset *> aa, DBInterface *d, QWidget *parent = 0);
    void addAsset(RAMAsset *asset);

private slots:
    void on_addButton_clicked();
    void setEditing(bool e);
    void assetsListUpdated(QList<RAMAsset *> aa);

signals:
    void editing(bool);
    void newAsset(RAMAsset *asset);

private:
    QList<RAMAsset *> assets;
    QList<RAMStatus *> statusesList;
    QList<RAMAsset *> allAssets;
    RAMShot *shot;
    RAMStage *stage;
    DBInterface *dbi;
};

#endif // ASSETSTATUSWIDGET_H

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
    explicit AssetStatusWidget(RAMShot *s, RAMStage *st, QList<RAMStatus *> sl, DBInterface *d, QWidget *parent = 0);
    void addAsset(RAMAsset *asset);

private slots:
    void on_addButton_clicked();
    void setEditing(bool e);
    void unAssign(RAMShot *s,RAMAsset *a);

signals:
    void editing(bool);
    void newAsset(RAMAsset *asset,RAMStage *stage);

private:
    QList<RAMStatus *> statusesList;
    RAMShot *shot;
    RAMStage *stage;
    DBInterface *dbi;
    QList<RAMAsset*> assignedAssets;
};

#endif // ASSETSTATUSWIDGET_H

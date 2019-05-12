#ifndef ASSETSTATUSWIDGET_H
#define ASSETSTATUSWIDGET_H

#include "ui_shotassetswidget.h"
#include "updater.h"
#include "addassetdialog.h"
#include "dbinterface.h"
#include "assetstatusbox.h"

class ShotAssetsWidget : public QWidget, private Ui::ShotAssetsWidget
{
    Q_OBJECT

public:
    explicit ShotAssetsWidget(RAMShot *s, RAMStage *st, DBInterface *d,Updater *up,QWidget *parent = 0);

public slots:
    void addAsset(RAMAsset *asset);

private slots:
    void on_addButton_clicked();
    void setEditing(bool e);
    void assign(RAMShot *s,RAMAsset *a);
    void unAssign(RAMShot *s,RAMAsset *a);
    void unAssign(RAMAsset *a);

signals:
    void editing(bool);

private:
    RAMShot *shot;
    RAMStage *stage;
    DBInterface *dbi;
    Updater *updater;
    QList<RAMAsset*> assignedAssets;
};

#endif // ASSETSTATUSWIDGET_H

#ifndef ADDASSETDIALOG_H
#define ADDASSETDIALOG_H

#include "ui_addassetdialog.h"
#include "dbinterface.h"
#include "updater.h"

class AddAssetDialog : public QDialog, private Ui::AddAssetDialog
{
    Q_OBJECT

public:
    explicit AddAssetDialog(DBInterface *d, RAMShot *sh, RAMStage *st, Updater *up, QList<RAMAsset *> aa = QList<RAMAsset*>(), QWidget *parent = 0);
    RAMAsset *getAsset();

private slots:
    void on_addButton_clicked();
    void on_createButton_clicked();

private:
    DBInterface *dbi;
    Updater *updater;
    RAMShot * shot;
    RAMStage * stage;
    RAMStatus *status;
    QList<RAMAsset*> allAssets;
    RAMAsset *asset;
};

#endif // ADDASSETDIALOG_H

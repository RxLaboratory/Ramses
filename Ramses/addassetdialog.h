#ifndef ADDASSETDIALOG_H
#define ADDASSETDIALOG_H

#include "ui_addassetdialog.h"
#include "dbinterface.h"
#include "ramshot.h"
#include "ramstage.h"

class AddAssetDialog : public QDialog, private Ui::AddAssetDialog
{
    Q_OBJECT

public:
    explicit AddAssetDialog(DBInterface *d, RAMShot *sh, RAMStage *st, int sid,QList<RAMAsset*> aa, QWidget *parent = 0);

private slots:
    void on_addButton_clicked();

    void on_createButton_clicked();

private:
    DBInterface *dbi;
    RAMShot * shot;
    RAMStage *stage;
    int statusId;
    QList<RAMAsset*> allAssets;
};

#endif // ADDASSETDIALOG_H

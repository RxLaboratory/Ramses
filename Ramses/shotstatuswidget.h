#ifndef SHOTSTATUSWIDGET_H
#define SHOTSTATUSWIDGET_H

#include "ui_shotstatuswidget.h"
#include "ramstatus.h"
#include "ramshot.h"
#include "shotdetailsdialog.h"
#include "dbinterface.h"

class ShotStatusWidget : public QWidget, private Ui::ShotStatusWidget
{
    Q_OBJECT

public:
    explicit ShotStatusWidget(DBInterface *db,RAMShot *sh, RAMStageStatus *ss, QList<RAMStatus *> sl, QWidget *parent = 0);

private slots:
    void stageStatusChanged(RAMStatus *newStatus, RAMStage* st);
    void on_comboBox_currentIndexChanged(int index);
    void on_detailsButton_clicked();

signals:
    void dialogShown(bool);

private:
    QList<RAMStatus *> statusesList;
    RAMShot *shot;
    RAMStageStatus *stageStatus;
    bool freezeDBI;
    bool freezeUI;
    DBInterface *dbi;
};

#endif // SHOTSTATUSWIDGET_H

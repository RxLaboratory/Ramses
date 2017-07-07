#ifndef RENAMEDIALOG_H
#define RENAMEDIALOG_H

#include "ui_renamedialog.h"
#include "ramshot.h"
#include "dbinterface.h"
#include <QRegExp>

class RenameDialog : public QDialog, private Ui::RenameDialog
{
    Q_OBJECT

public:
    explicit RenameDialog(DBInterface *db, QList<RAMShot *> s, QWidget *parent = 0);

private slots:
    void on_buttonBox_accepted();

    void on_nameButton_clicked(bool checked);

private:
    QList<RAMShot*> shots;
    DBInterface *dbi;
};

#endif // RENAMEDIALOG_H

#ifndef ADDSHOTSDIALOG_H
#define ADDSHOTSDIALOG_H

#include "ui_addshotsdialog.h"

class AddShotsDialog : public QDialog, private Ui::AddShotsDialog
{
    Q_OBJECT

public:
    explicit AddShotsDialog(QWidget *parent = 0);
    QStringList getShots();
};

#endif // ADDSHOTS_H

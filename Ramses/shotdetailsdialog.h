#ifndef SHOTDETAILSDIALOG_H
#define SHOTDETAILSDIALOG_H

#include "ui_shotdetailsdialog.h"

class ShotDetailsDialog : public QDialog, private Ui::ShotDetailsDialog
{
    Q_OBJECT

public:
    explicit ShotDetailsDialog(QWidget *parent = 0);
    QString getComment();
    void setComment(QString c);
};

#endif // SHOTDETAILSDIALOG_H

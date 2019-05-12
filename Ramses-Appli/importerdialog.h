#ifndef IMPORTERDIALOG_H
#define IMPORTERDIALOG_H

#include "ui_importerdialog.h"
#include <QFileDialog>

class ImporterDialog : public QDialog, private Ui::ImporterDialog
{
    Q_OBJECT

public:
    explicit ImporterDialog(QWidget *parent = 0);
    QString getPath();
    bool getVideo();
    bool getAudio();
    QString getPrefix();
    QString getSuffix();
private slots:
    void on_browseButton_clicked();
};

#endif // IMPORTERDIALOG_H

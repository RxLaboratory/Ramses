#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "ui_aboutdialog.h"

class AboutDialog : public QDialog, private Ui::AboutDialog
{
public:
    explicit AboutDialog(QWidget *parent = nullptr);
};

#endif // ABOUTDIALOG_H

#ifndef SHOTSCREATIONDIALOG_H
#define SHOTSCREATIONDIALOG_H

#include <QDialog>
#include <QRegExpValidator>

#include "ui_shotscreationdialog.h"
#include "ramproject.h"
#include "ramshot.h"
#include "ramobjectcombobox.h"

class ShotsCreationDialog : public QDialog, private Ui::ShotsCreationDialog
{
    Q_OBJECT

public:
    explicit ShotsCreationDialog(RamProject *proj, QWidget *parent = nullptr);
    void setSequence(RamSequence *seq);

private slots:
    void create();
    void nameExample(QString n);
    void shortNameExample(QString sn);

private:
    int getMidNumber();
    int getNumDigits();
    QString getShortName(int n = -1);
    QString getName(int n = -1);
    RamProject *m_project;
    RamObjectComboBox *ui_sequenceBox;
};

#endif // SHOTSCREATIONDIALOG_H

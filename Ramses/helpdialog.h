#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include "ui_helpdialog.h"
#include <QDialog>
#include <QMouseEvent>
#include <QToolBar>
#include <QPushButton>
#include "separatorwidget.h"

class HelpDialog : public QDialog, private Ui::HelpDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
signals:
    void visibilityChanged(bool);

private:
    bool toolBarClicked;
    QPoint dragPosition;
    QToolBar *toolBar;
protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // HELPDIALOG_H

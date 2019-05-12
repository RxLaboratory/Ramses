#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include "ui_helpdialog.h"
#include <QDialog>
#include <QMouseEvent>
#include <QToolBar>
#include <QPushButton>
#include <QDateTime>
#include "separatorwidget.h"
#include <QFileDialog>
#include <QTextStream>

class HelpDialog : public QDialog, private Ui::HelpDialog
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);
public slots:
    void showDebug(QString m, QString type);
    void showHelp(int i);
    void setToolButtonStyle(int s);
signals:
    void visibilityChanged(bool);
    void dock(bool);
private slots:
    void on_actionAbout_triggered();
    void on_actionDebug_triggered();
    void on_actionHelp_triggered();
    void dockButton_clicked(bool checked);
    void on_fileDebugButton_clicked(bool checked);

private:
    bool toolBarClicked;
    QPoint dragPosition;
    QToolBar *toolBar;
    QPushButton *dockButton;
    void showPage(int i);
    QFile debugFile;
    bool writeDebugToFile;
protected:
    void hideEvent(QHideEvent *e);
    void showEvent(QShowEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);

};

#endif // HELPDIALOG_H

#ifndef DUQFFOLDERDISPLAYWIDGET_H
#define DUQFFOLDERDISPLAYWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QMessageBox>

#include "duqf-widgets/duqfelidedlabel.h"

class DuQFFolderDisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DuQFFolderDisplayWidget(QWidget *parent = nullptr);

public slots:
    void setPath(QString p);
signals:


private slots:
    void exploreButton_clicked();

private:
    void setupUi();
    DuQFElidedLabel *ui_folderLabel;
    QToolButton *ui_exploreButton;
    QString m_path;
};

#endif // DUQFFOLDERDISPLAYWIDGET_H

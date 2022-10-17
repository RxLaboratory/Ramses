#ifndef STATUSHISTORYWIDGET_H
#define STATUSHISTORYWIDGET_H

#include <QSplitter>

#include "ramobjectview.h"
#include "ramstatus.h"

class StatusHistoryWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StatusHistoryWidget(RamObjectModel *history, QWidget *parent = nullptr);

signals:

private slots:
    void editObject(RamStatus *obj) const;

private:
    void setupUi();
    RamObjectView *ui_statusList;
};

#endif // STATUSHISTORYWIDGET_H

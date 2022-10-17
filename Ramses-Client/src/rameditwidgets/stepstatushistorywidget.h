#ifndef STEPSTATUSHISTORYWIDGET_H
#define STEPSTATUSHISTORYWIDGET_H

#include <QWidget>
#include <QClipboard>

#include "objectlistwidget.h"

class StepStatusHistoryWidget : public QWidget
{
public:
    explicit StepStatusHistoryWidget(QWidget *parent = nullptr);
    explicit StepStatusHistoryWidget(RamObjectModel *history, QWidget *parent = nullptr);
    void setHistory(RamObjectModel *history);

private:
    void setupUi();
    void connectEvents();

    ObjectListWidget *m_statusList;

    RamObjectModel *m_history = nullptr;
};

#endif // STEPSTATUSHISTORYWIDGET_H

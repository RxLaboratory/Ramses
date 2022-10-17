#include "stepstatushistorywidget.h"

StepStatusHistoryWidget::StepStatusHistoryWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
    setHistory(nullptr);
    connectEvents();
}

StepStatusHistoryWidget::StepStatusHistoryWidget(RamObjectModel *history, QWidget *parent) : QWidget(parent)
{
    setupUi();
    setHistory(history);
    connectEvents();
}

void StepStatusHistoryWidget::setHistory(RamObjectModel *history)
{
    this->setEnabled(false);

    m_history = history;

    if (!m_history) return;

    m_statusList->setObjectModel(m_history);
    m_statusList->setTitle("Status history");

    this->setEnabled(true);
}

void StepStatusHistoryWidget::setupUi()
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(3);

    m_statusList = new ObjectListWidget(true, RamUser::ProjectAdmin, this);
    m_statusList->setTitle("Status history");
    m_statusList->setEditable(false);
    m_statusList->setSearchable(false);
    m_statusList->listWidget()->showDetails(true);
    layout->addWidget( m_statusList );
}

void StepStatusHistoryWidget::connectEvents()
{

}

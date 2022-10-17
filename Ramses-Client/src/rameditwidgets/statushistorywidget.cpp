#include "statushistorywidget.h"

StatusHistoryWidget::StatusHistoryWidget(RamObjectModel *history, QWidget *parent) : QWidget(parent)
{
    setupUi();
    ui_statusList->setObjectModel(history);
}

void StatusHistoryWidget::editObject(RamStatus *obj) const
{
    if (obj) obj->edit();
}

void StatusHistoryWidget::setupUi()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);
    layout->setSpacing(3);

    ui_statusList = new RamObjectView( RamObjectView::List, this);
    ui_statusList->setEditableObjects(true, RamUser::Admin);
    layout->addWidget( ui_statusList );
}


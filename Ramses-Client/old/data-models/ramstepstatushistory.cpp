#include "ramstepstatushistory.h"

#include "duqf-utils/guiutils.h"
#include "ramabstractitem.h"
#include "ramstep.h"
#include "ramstatus.h"
#include "ramasset.h"
#include "statushistorywidget.h"
#include "mainwindow.h"
#include "ramshot.h"

QMap<QString, RamStepStatusHistory*> RamStepStatusHistory::m_existingObjects = QMap<QString, RamStepStatusHistory*>();

RamStepStatusHistory *RamStepStatusHistory::get(QString uuid)
{
    if (!checkUuid(uuid, StepStatusHistory)) return nullptr;

    if (m_existingObjects.contains(uuid)) return m_existingObjects.value(uuid);

    // Finally return a new instance
    // Check if everything is alright
    RamStepStatusHistory *h = new RamStepStatusHistory(uuid);
    if (h->step())
        return h;
    return nullptr;
}

RamStepStatusHistory *RamStepStatusHistory::c(QObject *o)
{
    return qobject_cast<RamStepStatusHistory*>(o);
}

RamStepStatusHistory::RamStepStatusHistory(RamStep *step, RamAbstractItem *item):
    RamObjectList(step->shortName(), step->name(), Status, ListObject, item, StepStatusHistory)
{
    construct();

    m_item = item;
    m_step = step;

    QJsonObject d = RamAbstractObject::data();
    d.insert("item", m_item->uuid());
    d.insert("step", m_step->uuid());
    RamAbstractObject::setData(d);

    connectEvents();
}

RamStepStatusHistory::RamStepStatusHistory(QString uuid, QObject *parent):
    RamObjectList(uuid, parent, StepStatusHistory)
{
    construct();

    QJsonObject d = RamAbstractObject::data();


    m_step = RamStep::get( d.value("step").toString() );

    if (m_step)
    {
        RamStep::Type t = m_step->type();
        if (t == RamStep::ShotProduction) m_item = RamShot::get(d.value("item").toString() );
        else m_item = RamAsset::get(d.value("item").toString() );

         connectEvents();
    }
}

RamAbstractItem *RamStepStatusHistory::item() const
{
    return m_item;
}

RamStep *RamStepStatusHistory::step() const
{
    return m_step;
}

bool statusSorter(RamObject *a, RamObject *b)
{
    RamStatus *as = qobject_cast<RamStatus*>(a);
    RamStatus *bs = qobject_cast<RamStatus*>(b);
    if (as->date() != bs->date()) return as->date() < bs->date();
    else return a->shortName() < b->shortName();
}

void RamStepStatusHistory::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(column)
    Q_UNUSED(order)

    std::sort(m_objectList.begin(), m_objectList.end(), statusSorter);
}

void RamStepStatusHistory::edit(bool show)
{
    Q_UNUSED(show)
    if (!ui_editWidget)
    {
        ui_editWidget = new QFrame();
        ui_editWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        QVBoxLayout *layout = new QVBoxLayout(ui_editWidget);
        layout->setContentsMargins(3,3,3,3);
        layout->addWidget( new StatusHistoryWidget( this ) );

    }
    MainWindow *mw = (MainWindow*)GuiUtils::appMainWindow();
    mw->setPropertiesDockWidget(ui_editWidget, m_step->name() + " history", ":/icons/step");
}

void RamStepStatusHistory::rowsChanged(QModelIndex parent, int start, int end)
{
    Q_UNUSED(parent)
    Q_UNUSED(start)

    if (end == rowCount() - 1)
    {
        m_step->computeEstimation();
        emit latestStatusChanged(this);
    }
}

void RamStepStatusHistory::changeData(QModelIndex topLeft, QModelIndex bottomRight, QVector<int> roles)
{
    Q_UNUSED(topLeft)
    Q_UNUSED(roles)

    if (bottomRight.row() == rowCount() - 1)
    {
        m_step->computeEstimation();
        emit latestStatusChanged(this);
    }
}

void RamStepStatusHistory::construct()
{
    m_existingObjects[m_uuid] = this;
    m_objectType = StepStatusHistory;
}

void RamStepStatusHistory::connectEvents()
{
    connect(this, &RamStepStatusHistory::rowsAboutToBeRemoved, this, &RamStepStatusHistory::rowsChanged);
    connect(this, &RamStepStatusHistory::rowsInserted, this, &RamStepStatusHistory::rowsChanged);
    connect(this, SIGNAL(dataChanged(QModelIndex,QModelIndex,QVector<int>)), this, SLOT(changeData(QModelIndex,QModelIndex,QVector<int>)));
    if (m_step) connect(m_step, &RamStep::removed, this, &RamStepStatusHistory::remove);
    if (m_item) connect(m_item, &RamAbstractItem::removed, this, &RamStepStatusHistory::remove);
}

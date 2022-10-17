#include "projectselectorwidget.h"

#include "ramses.h"

ProjectSelectorWidget::ProjectSelectorWidget(QWidget *parent):
    RamObjectComboBox( parent )
{
    m_projectFilter = new RamObjectSortFilterProxyModel(this);
    m_projectFilter->setSourceModel(Ramses::instance()->projects());
    this->setModel(m_projectFilter);
    this->setMinimumWidth(200);

    m_pm = ProgressManager::instance();

    connect(this, &RamObjectComboBox::currentObjectChanged, this,  &ProjectSelectorWidget::setCurrentProject);
    connect(Ramses::instance(), SIGNAL(currentProjectChanged(RamProject*)), this, SLOT(currentProjectChanged(RamProject*)));
    connect(Ramses::instance(), &Ramses::userChanged, this, &ProjectSelectorWidget::userChanged);
}

void ProjectSelectorWidget::setCurrentProject(RamObject *projObj)
{
    //if (m_pm->isBusy()) return;
    Ramses::instance()->setCurrentProject( RamProject::c( projObj ) );
}

void ProjectSelectorWidget::currentProjectChanged(RamProject *p)
{
    QSignalBlocker b(this);

    if (!p)
    {
        setCurrentIndex(-1);
        return;
    }

    setObject(p);
}

void ProjectSelectorWidget::userChanged(RamUser *user)
{
    m_projectFilter->clearFilterListUuids();
    if (user) m_projectFilter->addFilterUuid(user->uuid());
}

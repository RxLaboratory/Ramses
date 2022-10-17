#include "settingswidget.h"

#include "duqf-utils/guiutils.h"

SettingsWidget::SettingsWidget(QString title, QWidget *parent) :
    QWidget(parent)
{
    setupUi(title);

    connect(m_titleBar, &DuQFTitleBar::reinitRequested, this, &SettingsWidget::reinitRequested);
    connect(m_titleBar, &DuQFTitleBar::closeRequested, this, &SettingsWidget::closeRequested);
    connect(m_mainList, SIGNAL(currentRowChanged(int)), this, SLOT(mainList_currentRowChanged(int)));
}

void SettingsWidget::setupUi(QString title)
{
    this->setObjectName(QStringLiteral("SettingsWidget"));
    QVBoxLayout *verticalLayout = new QVBoxLayout(this);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    verticalLayout->setSpacing(0);

    // Get the mainwindow to add the titlebar
    QMainWindow *mw = GuiUtils::appMainWindow();
    mw->addToolBarBreak(Qt::TopToolBarArea);

    m_titleBar = new DuQFTitleBar(title, false, mw);
    mw->addToolBar(Qt::TopToolBarArea,m_titleBar);
    //mw->insertToolBar(mw->findChild<QToolBar*>("mainToolBar"), m_titleBar);
    m_titleBar->setFloatable(false);
    m_titleBar->hide();

    QWidget *mainWidget = new QWidget(this);
    verticalLayout->addWidget(mainWidget);

    QHBoxLayout *horizontalLayout = new QHBoxLayout(mainWidget);
    horizontalLayout->setSpacing(0);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    m_splitter = new QSplitter(this);
    m_splitter->setHandleWidth(6);

    m_mainList = new QListWidget(this);
    m_mainList->setFrameShape(QFrame::NoFrame);
    m_mainList->setLineWidth(0);
    m_mainList->setResizeMode(QListView::Adjust);
    m_mainList->setLayoutMode(QListView::SinglePass);

    m_splitter->addWidget(m_mainList);

    m_mainStackWidget = new QStackedWidget(this);
    m_mainStackWidget->setObjectName(QStringLiteral("mainStackWidget"));
    QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Preferred);
    m_mainStackWidget->setSizePolicy(sizePolicy1);

    m_splitter->addWidget(m_mainStackWidget);

    horizontalLayout->addWidget(m_splitter);

    m_mainStackWidget->setCurrentIndex(-1);
}

void SettingsWidget::addPage(QWidget *ui, QString title, QIcon icon)
{
    m_mainStackWidget->addWidget(ui);
    QListWidgetItem *tab = new QListWidgetItem(icon,title);
    m_mainList->addItem(tab);
}

void SettingsWidget::setCurrentIndex(int index)
{
    m_mainStackWidget->setCurrentIndex(index);
}

void SettingsWidget::showEvent(QShowEvent *event)
{
    if (!event->spontaneous()) m_titleBar->show();
    m_splitter->setSizes(QList<int>() << 200 << 1000);
    QWidget::showEvent(event);
}

void SettingsWidget::hideEvent(QHideEvent *event)
{
    if (!event->spontaneous()) m_titleBar->hide();
    QWidget::hideEvent(event);
}

void SettingsWidget::showReinitButton(bool show)
{
    m_titleBar->showReinitButton(show);
}

DuQFTitleBar *SettingsWidget::titleBar()
{
    return m_titleBar;
}

void SettingsWidget::mainList_currentRowChanged(int currentRow)
{
    this->setCurrentIndex(currentRow);
}

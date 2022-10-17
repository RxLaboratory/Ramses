#include "consolewidget.h"

ConsoleWidget::ConsoleWidget(QWidget *parent)
    : QFrame{parent}
{
    setupUi();
    connectEvents();
}

ConsoleWidget::ConsoleWidget(DuQFLoggerObject *o, QWidget *parent)
    : QFrame{parent}
{
    setupUi(o);
}

void ConsoleWidget::levelIndexChanged(int index)
{
    QSettings settings;
    settings.setValue("console/level", index);

    switch(index) {
    case 0:
        ui_consoleEdit->setLevel( DuQFLog::Data );
        break;
    case 1:
        ui_consoleEdit->setLevel( DuQFLog::Debug );
        break;
    case 2:
        ui_consoleEdit->setLevel( DuQFLog::Information );
        break;
    case 3:
        ui_consoleEdit->setLevel( DuQFLog::Warning );
        break;
    case 4:
        ui_consoleEdit->setLevel( DuQFLog::Critical );
        break;
    case 5:
        ui_consoleEdit->setLevel( DuQFLog::Fatal );
        break;
    default:
        ui_consoleEdit->setLevel( DuQFLog::Information );
    }
}

void ConsoleWidget::clear()
{
    ui_consoleEdit->clear();
}

void ConsoleWidget::setupUi(DuQFLoggerObject *o)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(3,3,3,3);
    mainLayout->setSpacing(3);

    ui_levelBox = new QComboBox();
    ui_levelBox->addItem("Data", DuQFLog::Data);
    ui_levelBox->addItem("Debug", DuQFLog::Debug);
    ui_levelBox->addItem("Information", DuQFLog::Information);
    ui_levelBox->addItem("Warning", DuQFLog::Warning);
    ui_levelBox->addItem("Critical", DuQFLog::Critical);
    ui_levelBox->addItem("Fatal", DuQFLog::Fatal);
    mainLayout->addWidget(ui_levelBox);

    if (o) ui_consoleEdit = new DuQFLoggingTextEdit(o);
    else ui_consoleEdit = new DuQFLoggingTextEdit();
    ui_consoleEdit->setLevel(DuQFLog::Information);
    mainLayout->addWidget(ui_consoleEdit);

    ui_clearButton = new QPushButton(tr("Clear"), this);
    ui_clearButton->setIcon(QIcon(":/icons/clean"));
    mainLayout->addWidget(ui_clearButton);

    QSettings settings;
    int level = settings.value("console/level", 2).toInt();
    ui_levelBox->setCurrentIndex( level );
    levelIndexChanged( level );
}

void ConsoleWidget::connectEvents()
{
    connect(ui_levelBox, SIGNAL(currentIndexChanged(int)), this, SLOT(levelIndexChanged(int)));
    connect(ui_clearButton, &QPushButton::clicked, this, &ConsoleWidget::clear);
}

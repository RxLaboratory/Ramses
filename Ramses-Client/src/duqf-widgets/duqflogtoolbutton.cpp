#include "duqflogtoolbutton.h"

DuQFLogToolButton::DuQFLogToolButton(QWidget *parent): QToolButton(parent)
{
    setupUi();

    clear();

    connect(DuQFLogger::instance(), &DuQFLogger::newLog, this, &DuQFLogToolButton::log);
    connect(this, SIGNAL(clicked()), this, SLOT(showLog()));
}

void DuQFLogToolButton::log(DuQFLog m)
{
    DuQFLog::LogType t = m.type();
    if (t > _currentLevel)
    {
        switch(t)
        {
        case DuQFLog::Data: break;
        case DuQFLog::Debug: break;
        case DuQFLog::Information: break;
        case DuQFLog::Warning:
            this->show();
            this->setIcon(QIcon(":/icons/warning"));
            this->setToolTip("Warning! Some errors have occured.");
            break;
        case DuQFLog::Critical:
            this->show();
            this->setIcon(QIcon(":/icons/critical"));
            this->setToolTip("There are critical errors. You should post a bug report and restart the application. Be careful, you may lose some data.");
            break;
        case DuQFLog::Fatal:
            this->show();
            this->setIcon(QIcon(":/icons/fatal"));
            this->setToolTip("A fatal error has occured, we can't continue. Please file a bug report.");
            break;
        }

        _currentLevel = m.type();
    }
}

void DuQFLogToolButton::showLog()
{
    if (_currentLevel <= DuQFLog::Information) return;
    _logDialog->exec();
}

void DuQFLogToolButton::setupUi()
{
    this->setText("");
    this->setToolButtonStyle(Qt::ToolButtonIconOnly);

    _logDialog = new QDialog(this);
    _logDialog->setModal(true);
    QVBoxLayout *layout = new QVBoxLayout(_logDialog);
    layout->setMargin(3);
    layout->setSpacing(3);

    DuQFLoggingTextEdit *logWidget = new DuQFLoggingTextEdit();
    logWidget->setLevel(DuQFLog::Warning);
    layout->addWidget(logWidget);

    QToolButton *cButton = new QToolButton();
    cButton->setText("Clear");
    cButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout->addWidget(cButton);

    _logDialog->setLayout(layout);

    QString cssFile = QSettings().value("appearance/cssFile", ":/styles/default").toString();
    _logDialog->setStyleSheet("QDialog, QTextEdit { background-color:#333; }");

    connect(cButton, SIGNAL(clicked()), logWidget, SLOT(clear()));
    connect(cButton, SIGNAL(clicked()), this, SLOT(clear()));
}

void DuQFLogToolButton::clear()
{
    this->hide();
    _currentLevel = DuQFLog::Information;
    this->setIcon(QIcon(":/icons/ok"));
    _logDialog->close();
}

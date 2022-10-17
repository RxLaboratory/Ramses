#include "duqffolderdisplaywidget.h"
#include "duqf-utils/utils.h"

DuQFFolderDisplayWidget::DuQFFolderDisplayWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();

    m_path = "";

    connect(ui_exploreButton, &QToolButton::clicked, this, &DuQFFolderDisplayWidget::exploreButton_clicked);

    ui_exploreButton->setEnabled(false);
}

void DuQFFolderDisplayWidget::setPath(QString p)
{
    m_path = QDir::toNativeSeparators(p);
    ui_folderLabel->setText(m_path);
    ui_folderLabel->setToolTip(m_path);
    ui_exploreButton->setToolTip("Reveal folder at " + m_path);
    ui_exploreButton->setEnabled(m_path != "");
}

void DuQFFolderDisplayWidget::exploreButton_clicked()
{
    if (m_path == "") return;

    FileUtils::openInExplorer( m_path, true );
}

void DuQFFolderDisplayWidget::setupUi()
{
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setObjectName(QStringLiteral("DuQFFolderDisplayWidget"));
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(3);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);

    ui_folderLabel = new DuQFElidedLabel(this);

    horizontalLayout->addWidget(ui_folderLabel);

    ui_exploreButton = new QToolButton(this);
    ui_exploreButton->setObjectName(QStringLiteral("exploreButton"));
    ui_exploreButton->setIcon(QIcon(":/icons/folder"));

    horizontalLayout->addWidget(ui_exploreButton);
}

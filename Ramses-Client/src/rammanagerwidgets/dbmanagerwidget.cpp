#include "dbmanagerwidget.h"

#include "dbinterface.h"
#include "ramabstractobject.h"

DBManagerWidget::DBManagerWidget(QWidget *parent)
    : QWidget{parent}
{
    setupUi();
    connectEvents();
}

void DBManagerWidget::clean()
{
    int deleteFrom = -1;
    if (ui_deleteCheckBox->isChecked()) deleteFrom = ui_deleteFromEdit->value();
    QString report = DBInterface::instance()->cleanDabaBase( deleteFrom );

    QSet<RamAbstractObject*> invalidObjects = RamAbstractObject::invalidObjects();
    int numInvalidObjects = invalidObjects.count();
    if (numInvalidObjects > 0)
    {
        report += ".\n\n## Invalid Objects\n\n";
        report += "Found " + QString::number(numInvalidObjects) + " invalid objects (with some corrupted data):\n\n";
        report += "These objects will be marked as removed as they can't be used anymore.\n\n";
        foreach(RamAbstractObject *o, invalidObjects)
        {
            report += "- (" + o->objectTypeName() + ") " + o->shortName() + " | " + o->name() + "\n";
        }
    }

#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    ui_reportEdit->setPlainText(report);
#else
    ui_reportEdit->setMarkdown(report);
#endif
    ui_cancelCleanButton->setEnabled(true);
    ui_acceptCleanButton->setEnabled(true);
    ui_cleanButton->setEnabled(false);

    QMessageBox::information(this,
                             tr("Finished cleaning the database"),
                             tr("The database has been cleaned.\n"
                                "Please review the changes before accepting (or cancelling) them.")
                             );
}

void DBManagerWidget::cancel()
{
    QMessageBox::information(this,
                             tr("Cancelling changes"),
                             tr("The application needs to restart to undo all changes.")
                             );

    bool ok = DBInterface::instance()->undoClean();

    if (!ok) QMessageBox::warning(this,
                                  tr("Can't undo!"),
                                  tr("Sorry, I can't undo the cleaning!\n\n"
                                     "Syncing has been disabled for now, you can either check what may be wrong, or close the application.")
                                  );
    ui_cancelCleanButton->setEnabled(false);
    ui_acceptCleanButton->setEnabled(false);
}

void DBManagerWidget::accept()
{
    ui_cancelCleanButton->setEnabled(false);
    ui_acceptCleanButton->setEnabled(false);
    ui_cleanButton->setEnabled(true);

    DBInterface::instance()->acceptClean();
    RamAbstractObject::removeInvalidObjects();
}

void DBManagerWidget::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(3);

    QGridLayout *topLayout = new QGridLayout();
    topLayout->setSpacing(3);
    topLayout->setContentsMargins(0,0,0,0);
    mainLayout->addLayout(topLayout);
    mainLayout->setAlignment(topLayout, Qt::AlignTop | Qt::AlignCenter);

    ui_deleteCheckBox = new QCheckBox(tr("Delete removed objects older than"));
    ui_deleteCheckBox->setChecked(false);
    topLayout->addWidget(ui_deleteCheckBox, 0, 0);

    ui_deleteFromEdit = new AutoSelectSpinBox(this);
    ui_deleteFromEdit->setMaximum(1000);
    ui_deleteFromEdit->setSuffix(" days");
    ui_deleteFromEdit->setValue(180);
    ui_deleteFromEdit->setEnabled(false);
    topLayout->addWidget(ui_deleteFromEdit, 0, 1);

    ui_cleanButton = new QPushButton(tr("Clean Database"), this);
    topLayout->addWidget(ui_cleanButton, 1, 0, 1, 2);

    ui_reportEdit = new QTextEdit();
    ui_reportEdit->setReadOnly(true);
    mainLayout->addWidget(ui_reportEdit);

    QHBoxLayout *validButtonLayout = new QHBoxLayout();
    validButtonLayout->setSpacing(3);
    mainLayout->addLayout(validButtonLayout);

    ui_cancelCleanButton = new QPushButton(tr("Undo changes"), this);
    ui_cancelCleanButton->setEnabled(false);
    validButtonLayout->addWidget(ui_cancelCleanButton);

    ui_acceptCleanButton = new QPushButton(tr("Accept changes"), this);
    ui_acceptCleanButton->setEnabled(false);
    validButtonLayout->addWidget(ui_acceptCleanButton);
}

void DBManagerWidget::connectEvents()
{
    connect(ui_cleanButton, &QPushButton::clicked, this, &DBManagerWidget::clean);
    connect(ui_cancelCleanButton, &QPushButton::clicked, this, &DBManagerWidget::cancel);
    connect(ui_acceptCleanButton, &QPushButton::clicked, this, &DBManagerWidget::accept);
    connect(ui_deleteCheckBox, &QCheckBox::clicked, ui_deleteFromEdit, &AutoSelectSpinBox::setEnabled);
}

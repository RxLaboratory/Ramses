#include "shotscreationdialog.h"
#include "duqf-utils/utils.h"

#include "ramsequence.h"

ShotsCreationDialog::ShotsCreationDialog(RamProject *proj, QWidget *parent) :
    QDialog(parent)
{
    m_project = proj;

    // Build the form
    setupUi(this);

    // Add the Sequence selector
    ui_sequenceBox = new RamObjectComboBox(this);
    ui_sequenceBox->setObjectModel(m_project->sequences());
    ui_sequenceLayout->addWidget(ui_sequenceBox);

    QRegExp rxsn = RegExUtils::getRegExp("shotshortname");
    QRegExpValidator *vsn = new QRegExpValidator(rxsn);
    ui_shortNameEdit->setValidator(vsn);

    QRegExp rxn = RegExUtils::getRegExp("shotname");
    QRegExpValidator *vn = new QRegExpValidator(rxn);
    ui_nameEdit->setValidator(vn);

    QRegExp rxs = RegExUtils::getRegExp("shotnumber");
    QRegExpValidator *vs = new QRegExpValidator(rxs);
    ui_nStartEdit->setValidator(vs);
    ui_nEndEdit->setValidator(vs);

    ui_progressBar->hide();

    connect(ui_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui_createButton, SIGNAL(clicked()), this, SLOT(create()));
    connect(ui_nameEdit, SIGNAL(textEdited(QString)), this, SLOT(nameExample(QString)));
    connect(ui_shortNameEdit, SIGNAL(textEdited(QString)), this, SLOT(shortNameExample(QString)));
    connect(ui_nStartEdit, SIGNAL(textEdited(QString)), this, SLOT(shortNameExample(QString)));
    connect(ui_nEndEdit, SIGNAL(textEdited(QString)), this, SLOT(shortNameExample(QString)));
    connect(ui_nStartEdit, SIGNAL(textEdited(QString)), this, SLOT(nameExample(QString)));
    connect(ui_nEndEdit, SIGNAL(textEdited(QString)), this, SLOT(nameExample(QString)));
}

void ShotsCreationDialog::setSequence(RamSequence *seq)
{
    ui_sequenceBox->setObject(seq);
}

void ShotsCreationDialog::create()
{
    if( !ui_nStartEdit->hasAcceptableInput() )
    {
        QMessageBox::information(this, tr("Invalid number"), tr("You have to choose a start number."));
        return;
    }
    if( !ui_nEndEdit->hasAcceptableInput() )
    {
        QMessageBox::information(this, tr("Invalid number"), tr("You have to choose the end number."));
        return;
    }

    int numDigits = getNumDigits();

    if ( ui_shortNameEdit->text().count() > 9 + numDigits)
    {
        QMessageBox::information(this, tr("Invalid ID"), tr("Sorry, the ID can't have more than 10 characters, including the number."));
        return;
    }

    int startNumber = ui_nStartEdit->text().toInt();
    int endNumber = ui_nEndEdit->text().toInt();

    RamSequence *seq = RamSequence::c( ui_sequenceBox->currentObject() );
    if (!seq)
    {
        QMessageBox::information(this, tr("Invalid Sequence"), tr("You have to choose a sequence."));
        return;
    }

    ui_cancelButton->setEnabled(false);
    ui_createButton->setEnabled(false);
    ui_progressBar->show();
    ui_progressBar->setMaximum( endNumber - startNumber +1);
    ui_progressBar->setValue(0);

    for (int i = startNumber; i <= endNumber; i++)
    {
        RamShot *shot = new RamShot(
                    getShortName(i),
                    getName(i),
                    seq
                    );
        m_project->shots()->appendObject(shot->uuid());
        ui_progressBar->setValue( i - startNumber );
    }

    ui_progressBar->hide();
    ui_cancelButton->setEnabled(true);
    ui_createButton->setEnabled(true);

    accept();
}

void ShotsCreationDialog::nameExample(QString n)
{
    Q_UNUSED(n);
    ui_nameExampleLabel->setText(getName());
}

void ShotsCreationDialog::shortNameExample(QString sn)
{
    Q_UNUSED(sn);
    ui_shortNameExampleLabel->setText(getShortName());
}

int ShotsCreationDialog::getMidNumber()
{
    int start = ui_nStartEdit->text().toInt();
    int end = ui_nEndEdit->text().toInt();
    return (start+end)/2;
}

int ShotsCreationDialog::getNumDigits()
{
    QString startNumberStr = ui_nStartEdit->text();
    QString endNumberStr = ui_nEndEdit->text();
    int numDigits = startNumberStr.count();
    if (endNumberStr.count() > numDigits) numDigits = endNumberStr.count();
    if (numDigits == 0) return 3;
    return numDigits;
}

QString ShotsCreationDialog::getShortName(int n)
{
    int numDigits = getNumDigits();
    if (n == -1) n = getMidNumber();
    QString num = QString::number(n);
    while (num.count() < numDigits) num = "0" + num;
    QString shortName = ui_shortNameEdit->text();
    if (shortName == "") shortName = ui_shortNameEdit->placeholderText();
    if (shortName.contains("%")) return shortName.replace("%", num);
    return shortName + num;
}

QString ShotsCreationDialog::getName(int n)
{
    int numDigits = getNumDigits();
    if (n == -1) n = getMidNumber();
    QString num = QString::number(n);
    while (num.count() < numDigits) num = "0" + num;
    QString name = ui_nameEdit->text();
    if (name == "") name = ui_nameEdit->placeholderText();
    if (name.contains("%")) return name.replace("%", num);
    return name;
}

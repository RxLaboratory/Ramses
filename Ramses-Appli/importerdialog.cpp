#include "importerdialog.h"

ImporterDialog::ImporterDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
}

QString ImporterDialog::getPath()
{
    return pathEdit->text();
}

bool ImporterDialog::getVideo()
{
    return videoButton->isChecked();
}

bool ImporterDialog::getAudio()
{
    return audioButton->isChecked();
}

QString ImporterDialog::getPrefix()
{
    return prefixEdit->text();
}

QString ImporterDialog::getSuffix()
{
    return suffixEdit->text();
}

void ImporterDialog::on_browseButton_clicked()
{
    this->setEnabled(false);
    QString file = QFileDialog::getOpenFileName(this,"Please choose the editing file you want to import.","","All supported edits (*.edl *.xml);;EDL (*.edl);;Final Cut / XML (*.xml);;All Files (*.*)");
    if (file != "")
    {
        pathEdit->setText(file);
    }
    this->setEnabled(true);
}

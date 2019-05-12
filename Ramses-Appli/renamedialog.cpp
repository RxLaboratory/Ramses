#include "renamedialog.h"

RenameDialog::RenameDialog(DBInterface *db, QList<RAMShot*> s, QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    shots = s;
    dbi = db;

    this->setWindowFlags(Qt::FramelessWindowHint);

}

void RenameDialog::on_buttonBox_accepted()
{
    QList<QStringList> renamedShots;

    QString prefix = "";
    if (prefixButton->isChecked()) prefix = prefixEdit->text();

    QString suffix = "";
    if (suffixButton->isChecked()) suffix = suffixEdit->text();

    QString name = "";
    if (nameButton->isChecked())
    {
        name = nameEdit->text();
        if (name == "") name = "####";
    }

    int number = -1;
    if (numberButton->isChecked()) number = numberSpinBox->value();

    foreach(RAMShot *shot,shots)
    {
        QString shotName = name;
        //base name
        if (shotName == "") shotName = shot->getName();
        shotName = prefix + shotName + suffix;

        if (numberButton->isChecked())
        {
            //find the #
            QRegExp re("(#+)");
            int i = re.indexIn(shotName);
            if (i >= 0)
            {
                QString d = re.cap(1);
                QString numberString = QString::number(number);
                while(numberString.count() < d.count())
                {
                    numberString = "0" + numberString;
                }
                shotName.replace(d,numberString);
                number++;
            }
        }

        shot->setName(shotName,false);

        //prepare values for dbi
        QStringList renamedShot;
        renamedShot << shotName << QString::number(shot->getDuration()) << shot->getId();
        renamedShots << renamedShot;
    }

    dbi->updateShots(renamedShots);
}

void RenameDialog::on_nameButton_clicked(bool checked)
{
    if (checked)
    {
        numberButton->setChecked(true);
        numberSpinBox->setEnabled(true);
    }
}

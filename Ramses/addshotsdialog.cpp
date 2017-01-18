#include "addshotsdialog.h"

AddShotsDialog::AddShotsDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
}

QStringList AddShotsDialog::getShots()
{
    QStringList shots;
    int num = numbers->currentIndex()+1;
    QString p = prefix->text();
    QString s = suffix->text();
    for (int i = min->value(); i <= max->value();i++)
    {
        QString n = QString::number(i);
        while (n.count() < num)
        {
            n = "0" + n;
        }
        shots << p + n + s;
    }
    return shots;
}

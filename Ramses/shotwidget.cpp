#include "shotwidget.h"

ShotWidget::ShotWidget(RAMShot *s, QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    shot = s;

    nameLabel->setText(s->getName());
    this->setToolTip(QString::number(s->getDuration()) + "s");

    connect(s,SIGNAL(nameChanged(QString)),this,SLOT(nameChanged(QString)));
    connect(s,SIGNAL(durationChanged(double)),this,SLOT(durationChanged(double)));
    connect(s,SIGNAL(shotRemoved(RAMShot*)),this,SLOT(deleteLater()));

}

void ShotWidget::nameChanged(QString name)
{
    nameLabel->setText(name);
}

void ShotWidget::durationChanged(double duration)
{
    this->setToolTip(QString::number(duration) + "s");
}

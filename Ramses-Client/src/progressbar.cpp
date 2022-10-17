#include "progressbar.h"

ProgressBar::ProgressBar(QWidget *parent):
    QProgressBar(parent)
{
    ProgressManager *pm = ProgressManager::instance();
    connect(pm, &ProgressManager::text, this, &QProgressBar::setFormat);
    connect(pm, &ProgressManager::progress, this, &QProgressBar::setValue);
    connect(pm, &ProgressManager::maximum, this, &QProgressBar::setMaximum);
    connect(pm, &ProgressManager::started, this, &QProgressBar::show);
    connect(pm, &ProgressManager::finished, this, &QProgressBar::hide);
    //connect(pm, SIGNAL(text(QString)), this, SLOT(repaint()));
    //connect(pm, SIGNAL(progress(int)), this, SLOT(repaint()));
    this->hide();
}

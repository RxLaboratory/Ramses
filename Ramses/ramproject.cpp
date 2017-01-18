#include "ramproject.h"
#include <QtDebug>

RAMProject::RAMProject(int i, QString n, QString sN, QObject *parent) : QObject(parent)
{
    projectId = i;
    projectName = n;
    projectShortName = sN;
}

int RAMProject::getId()
{
    return projectId;
}

QString RAMProject::getName()
{
    return projectName;
}

QString RAMProject::getShortName()
{
    return projectShortName;
}

QList<RAMStage *> RAMProject::getStages()
{
    return projectStages;
}

void RAMProject::addStage(RAMStage *s)
{
    projectStages.append(s);
}

void RAMProject::removeStage(RAMStage *s)
{
    projectStages.removeOne(s);
}

QList<RAMShot *> RAMProject::getShots()
{
    return projectShots;
}

void RAMProject::addShot(RAMShot *s)
{
    projectShots.append(s);
}

void RAMProject::removeShot(RAMShot *s)
{
    projectShots.removeOne(s);
}

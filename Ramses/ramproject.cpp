#include "ramproject.h"
#include <QtDebug>

RAMProject::RAMProject(DBInterface *db, int i, QString n, QString sN, bool updateDb, QObject *parent) : QObject(parent)
{
    projectId = i;
    projectName = n;
    projectShortName = sN;
    dbi = db;
    if (updateDb)
    {
        dbi->addProject(projectName,projectShortName,projectId);
    }
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

QList<RAMShot *> RAMProject::getShots()
{
    return projectShots;
}

void RAMProject::setName(QString name, bool updateDb)
{
    projectName = name;
    if (updateDb) update();
}

void RAMProject::setShortName(QString shortName, bool updateDb)
{
    projectShortName = shortName;
    if (updateDb) update();
}

void RAMProject::update()
{
    dbi->updateProject(projectId,projectName,projectShortName);
}

void RAMProject::addStage(RAMStage *s)
{
    projectStages.append(s);
}

void RAMProject::removeStage(RAMStage *s)
{
    projectStages.removeOne(s);
}

void RAMProject::addShot(RAMShot *s)
{
    projectShots.append(s);
}

void RAMProject::removeShot(RAMShot *s)
{
    projectShots.removeOne(s);
}

void RAMProject::remove()
{
    dbi->removeProject(projectId);
}

bool RAMProject::operator==(RAMProject s)
{
    return s.getId() == projectId;
}

bool RAMProject::operator==(RAMProject *s)
{
    return s->getId() == projectId;
}

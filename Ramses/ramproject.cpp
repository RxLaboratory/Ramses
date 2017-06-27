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

void RAMProject::addStage(RAMStage *s, bool updateDb)
{
    projectStages.append(s);
    if (updateDb) dbi->addProjectStage(projectId,s->getId());
}

void RAMProject::removeStage(RAMStage *s, bool updateDb)
{
    projectStages.removeAll(s);
    if (updateDb) dbi->removeProjectStage(projectId,s->getId());
}

void RAMProject::addShot(RAMShot *s, bool updateDb)
{
    projectShots.append(s);
}

void RAMProject::removeShot(RAMShot *s, bool updateDb)
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

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

RAMProject::~RAMProject()
{

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
    return shots;
}

RAMShot *RAMProject::getShot(int id)
{
    foreach(RAMShot *rs,shots)
    {
        if (rs->getId() == id) return rs;
    }
    return 0;
}

void RAMProject::setName(QString name, bool updateDb)
{
    projectName = name;
    if (updateDb) update();
    emit nameChanged(this,name);
}

void RAMProject::setShortName(QString shortName, bool updateDb)
{
    projectShortName = shortName;
    if (updateDb) update();
    emit shortNameChanged(this,shortName);
}

void RAMProject::update()
{
    dbi->updateProject(projectId,projectName,projectShortName);
}

void RAMProject::addShot(RAMShot *shot, int row)
{
    if (row >= 0 && row <= shots.count()) shots.insert(row,shot);
    else shots << shot;
    shot->setParent(this);
    emit shotAdded(this,shot,row);
}

void RAMProject::removeShot(RAMShot *shot)
{
    shots.removeAll(shot);
    emit shotRemoved(this,shot);
}

void RAMProject::resetShotsOrder()
{
    QList<int> ids;
    foreach(RAMShot *rs,shots)
    {
        ids << rs->getId();
    }
    dbi->resetShotsOrder(ids);
}

void RAMProject::addStage(RAMStage *s, bool updateDb)
{
    projectStages.append(s);
    if (updateDb) dbi->addProjectStage(projectId,s->getId());

    //connect
    connect(s,SIGNAL(stageRemoved(RAMStage*)),this,SLOT(stageDeleted(RAMStage*)));

    emit stageAdded(this,s);
}

void RAMProject::removeStage(RAMStage *s, bool updateDb)
{
    projectStages.removeAll(s);
    if (updateDb) dbi->removeProjectStage(projectId,s->getId());
    emit stageRemoved(this,s);
}

void RAMProject::remove()
{
    dbi->removeProject(projectId);
    emit projectRemoved(this);
}

void RAMProject::stageDeleted(RAMStage *s)
{
    removeStage(false);
}

bool RAMProject::operator==(RAMProject s)
{
    return s.getId() == projectId;
}

bool RAMProject::operator==(RAMProject *s)
{
    return s->getId() == projectId;
}

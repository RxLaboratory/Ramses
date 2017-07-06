#include "updater.h"

Updater::Updater(DBInterface *db, QObject *parent) : QObject(parent)
{
    dbi = db;

    mapEvents();
}

void Updater::mapEvents()
{
    connect(dbi,SIGNAL(data(QJsonObject)),this,SLOT(dataReceived(QJsonObject)));
}

void Updater::dataReceived(QJsonObject data)
{
    //extract data
    QString mes = data.value("message").toString();
    QString type = data.value("type").toString();
    bool accepted = data.value("accepted").toBool();
    bool success = data.value("success").toBool();
    QJsonValue content = data.value("content");

    //if not accepted, ignore
    if (!accepted) return;

    if (type == "getStatuses")
    {
        emit message("Loading statuses","debug");
        if (success) gotStatuses(content);
        else emit message("Warning: Status list was not correctly updated from remote server.","warning");
        return;
    }
    else if (type == "getStages")
    {
        emit message("Loading stages","debug");
        if (success) gotStages(content);
        else emit message("Warning: Stages list was not correctly updated from remote server.","warning");
        return;
    }
    else if (type == "getProjects")
    {
        emit message("Loading projects","debug");
        if (success) gotProjects(content);
        else emit message("Warning: Projects list was not correctly updated from remote server.","warning");
        return;
    }
    else if (type == "getShots")
    {
        emit message("Loading shots","debug");
        if (success) gotShots(content);
        else emit message("Warning: Shots list was not correctly updated from remote server.","warning");
        return;
    }
    else if (type == "getAssets")
    {
        emit message("Loading assets","debug");
        if (success) gotAssets(content);
        else emit message("Warning: Assets list was not correctly updated from remote server.","warning");
        return;
    }
}

void Updater::updateAll()
{
    dbi->getStatuses();
}

void Updater::clean()
{
    qDeleteAll(projects);
    projects.clear();
    qDeleteAll(stages);
    stages.clear();
    qDeleteAll(statuses);
    statuses.clear();
    qDeleteAll(removedItems);
    removedItems.clear();
}

QList<RAMProject *> Updater::getProjects()
{
    return projects;
}

void Updater::addProject(RAMProject *project)
{
    projects << project;
    emit projectAdded(project);
}

void Updater::removeProject(RAMProject *project)
{
    projects.removeAll(project);
    removedItems << project;
    emit projectRemoved(project);
}

RAMProject *Updater::getProject(int id)
{
    foreach(RAMProject *project,projects)
    {
        if (project->getId() == id) return project;
    }
    return 0;
}

RAMProject *Updater::getCurrentProject()
{
    return currentProject;
}

void Updater::setCurrentProject(RAMProject *project)
{
    currentProject = project;
    dbi->getShots(currentProject->getId());
    emit currentProjectChanged(currentProject);
}

void Updater::addStatus(RAMStatus *status)
{
    statuses << status;
    emit statusAdded(status);
}

void Updater::removeStatus(RAMStatus *status)
{
    statuses.removeAll(status);
    removedItems << status;
    emit statusRemoved(status);
}

QList<RAMStatus *> Updater::getStatuses()
{
    return statuses;
}

RAMStatus* Updater::getStatus(int id)
{
    foreach(RAMStatus *status,statuses)
    {
        if (status->getId() == id) return status;
    }
    return 0;
}

RAMStatus *Updater::getDefaultStatus()
{
    return defaultStatus;
}

void Updater::addStage(RAMStage *stage)
{
    stages << stage;
    emit stageAdded(stage);
}

void Updater::removeStage(RAMStage *stage)
{
    stages.removeAll(stage);
    removedItems << stage;
    emit stageRemoved(stage);
}

QList<RAMStage *> Updater::getStages()
{
    return stages;
}

RAMStage* Updater::getStage(int id)
{
    foreach(RAMStage *stage,stages)
    {
        if (stage->getId() == id) return stage;
    }
}

void Updater::gotStatuses(QJsonValue newStatuses)
{
    QJsonArray statusesArray = newStatuses.toArray();

    // update statuses in the current list
    for (int rsI = 0 ; rsI < statuses.count() ; rsI++)
    {
        RAMStatus *rs = statuses[rsI];

        //search for status in new list
        bool updated = false;
        for(int i = 0 ; i < statusesArray.count();i++)
        {
            QJsonObject status = statusesArray[i].toObject();
            int id = status.value("id").toInt();

            if (rs->getId() == id)
            {
                //new status
                QString name = status.value("name").toString();
                QString shortName = status.value("shortName").toString();
                QColor color(status.value("color").toString());
                QString description = status.value("description").toString();

                //update
                rs->setName(name);
                rs->setShortName(shortName);
                rs->setColor(color);
                rs->setDescription(description);

                //remove from the new list
                statusesArray.removeAt(i);
                i--;

                updated = true;
            }
        }
        // if the status is not in the new list, remove it
        if (!updated)
        {
            RAMStatus *statusToRemove = statuses.takeAt(rsI);
            emit statusRemoved(statusToRemove);
            removedItems << statusToRemove;
            rsI--;
        }
    }

    //add the remaining new statuses
    for (int i = 0 ; i < statusesArray.count() ; i++)
    {
        //new status
        QJsonObject status = statusesArray[i].toObject();
        QString name = status.value("name").toString();
        QString shortName = status.value("shortName").toString();
        QColor color(status.value("color").toString());
        QString description = status.value("description").toString();
        int id = status.value("id").toInt();

        RAMStatus *rs = new RAMStatus(dbi,id,name,shortName,color,description,false,this);
        statuses << rs;
        emit statusAdded(rs);
    }

    //reset the default status
    foreach(RAMStatus *status,statuses)
    {
        if (status->getShortName() == "STB")
        {
            defaultStatus = status;
            break;
        }
    }

    //get Stages
    dbi->getStages();
}

void Updater::gotStages(QJsonValue newStages)
{
    QJsonArray stagesArray = newStages.toArray();

    // update statuses in the current list
    for (int rsI = 0 ; rsI < stages.count() ; rsI++)
    {
        RAMStage *rs = stages[rsI];

        //search for stage in new list
        bool updated = false;
        for(int i = 0 ; i < stagesArray.count();i++)
        {
            QJsonObject stage = stagesArray[i].toObject();
            int id = stage.value("id").toInt();

            if (rs->getId() == id)
            {
                QString name = stage.value("name").toString();
                QString shortName = stage.value("shortName").toString();

                //update
                rs->setName(name);
                rs->setShortName(shortName);

                //remove from the new list
                stagesArray.removeAt(i);
                i--;

                updated = true;
            }
        }
        // if the stage is not in the new list, remove it
        if (!updated)
        {
            RAMStage *stageToRemove = stages.takeAt(rsI);
            emit stageRemoved(stageToRemove);
            removedItems << stageToRemove;
            rsI--;
        }
    }

    //add the remaining new stages
    for (int i = 0 ; i < stagesArray.count() ; i++)
    {
        //new stage
        QJsonObject stage = stagesArray[i].toObject();
        QString name = stage.value("name").toString();
        QString shortName = stage.value("shortName").toString();
        int id = stage.value("id").toInt();
        //add to UI
        RAMStage *rs = new RAMStage(dbi,name,shortName,id,false,this);
        stages << rs;
        emit stageAdded(rs);
    }

    //get projects
    dbi->getProjects();
}

void Updater::gotProjects(QJsonValue newProjects)
{
    QJsonArray projectsArray = newProjects.toArray();

    // update projects in the current list
    for (int rpI = 0 ; rpI < projects.count() ; rpI++)
    {
        RAMProject *rp = projects[rpI];

        //search for project in new list
        bool updated = false;
        for(int i = 0 ; i < projectsArray.count();i++)
        {
            //new project
            QJsonObject project = projectsArray[i].toObject();
            int id = project.value("id").toInt();

            if (rp->getId() == id)
            {
                QString name = project.value("name").toString();
                QString shortName = project.value("shortName").toString();
                QJsonArray projectStagesArray = project.value("stages").toArray();

                //update project
                rp->setName(name);
                rp->setShortName(shortName);

                //update stages list
                foreach(QJsonValue proStage,projectStagesArray)
                {
                    RAMStage *stage = getStage(proStage.toInt());
                    rp->addStage(stage);
                }

                //remove from the new list
                projectsArray.removeAt(i);
                i--;

                updated = true;
            }
        }
        // if the project is not in the new list, remove it
        if (!updated)
        {
            RAMProject *projectToRemove = projects.takeAt(rpI);
            emit projectRemoved(projectToRemove);
            removedItems << projectToRemove;
            rpI--;
        }
    }

    //add the remaining new projects
    for (int i = 0 ; i < projectsArray.count() ; i++)
    {
        //new project
        QJsonObject project = projectsArray[i].toObject();
        QString name = project.value("name").toString();
        QString shortName = project.value("shortName").toString();
        int id = project.value("id").toInt();
        QJsonArray projectStagesArray = project.value("stages").toArray();

        RAMProject *rp = new RAMProject(dbi,id,name,shortName,defaultStatus,false,this);

        //update stages list
        foreach(QJsonValue proStage,projectStagesArray)
        {
            RAMStage *stage = getStage(proStage.toInt());
            rp->addStage(stage);
        }

        projects << rp;
        //add to UI
        emit projectAdded(rp);
    }

}

void Updater::gotShots(QJsonValue newShots)
{
    QJsonArray shotsArray = newShots.toArray();
    QList<RAMShot*> shots = currentProject->getShots();

    // update shots in the current list
    for (int rsI = 0 ; rsI < shots.count() ; rsI++)
    {
        RAMShot *rs = shots[rsI];

        //search for shot in new list
        bool updated = false;
        for(int i = 0 ; i < shotsArray.count();i++)
        {
            //new shot
            QJsonObject shot = shotsArray[i].toObject();
            int id = shot.value("shotId").toInt();

            if (rs->getId() == id)
            {
                QString name = shot.value("shotName").toString();
                double duration = shot.value("duration").toDouble();

                //update shot
                rs->setName(name);
                rs->setDuration(duration);

                //remove from the new list
                shotsArray.removeAt(i);
                i--;

                updated = true;
            }
        }
        // if the shot is not in the new list, remove it
        if (!updated)
        {
            RAMShot *shotToRemove = shots[rsI];
            currentProject->removeShot(shotToRemove);
            removedItems << shotToRemove;
        }
    }

    //add the remaining new shots
    for (int i = 0 ; i < shotsArray.count() ; i++)
    {
        //new shot
        QJsonObject shot = shotsArray[i].toObject();
        QString name = shot.value("shotName").toString();
        double duration = shot.value("duration").toDouble();
        int order = shot.value("shotOrder").toInt();
        int id = shot.value("shotId").toInt();

        RAMShot *rs = new RAMShot(dbi,id,name,duration,false);

        //add to project
        currentProject->addShot(rs,order);
    }

    //get assets
    if (currentProject != 0) dbi->getAssets(currentProject->getId());

    emit message("Got Shots","debug");
}

void Updater::gotAssets(QJsonValue newAssets)
{
    QJsonArray assetsArray = newAssets.toArray();


    //for each stage
    foreach(RAMStage *stage,currentProject->getStages())
    {
        // update assets in the current list
        QList<RAMAsset*> assets = stage->getAssets();
        for (int raI = 0 ; raI < assets.count() ; raI++)
        {
            RAMAsset *ra = assets[raI];

            //search for asset in new list
            bool updated = false;
            for(int i = 0 ; i < assetsArray.count();i++)
            {
                //new asset
                QJsonObject asset = assetsArray[i].toObject();
                int stageId = asset.value("stageId").toInt();

                //only if in current stage
                if (stageId != stage->getId()) continue;

                int id = asset.value("id").toInt();

                if (ra->getId() == id)
                {
                    QString name = asset.value("name").toString();
                    QString shortName = asset.value("shortName").toString();
                    QString comment = asset.value("comment").toString();
                    QJsonArray assignments = asset.value("assignments").toArray();

                    //check if status is null
                    bool statusReAssigned = false;
                    int statusId = asset.value("statusId").toInt();
                    if (statusId == 0)
                    {
                        //get STB Status
                        foreach(RAMStatus *status,statuses)
                        {
                            if (status->getShortName() == "STB")
                            {
                                statusId = status->getId();
                                statusReAssigned = true;
                                break;
                            }
                        }
                    }

                    //update asset
                    ra->setName(name);
                    ra->setShortName(shortName);
                    ra->setComment(comment);
                    ra->setStatus(getStatus(statusId),statusReAssigned);

                    //check assignments
                    for (int j = 0; j < ra->getAssignments().count() ; j++)
                    {
                        RAMShot *shot = ra->getAssignments()[j];
                        bool ok = false;
                        for (int k = 0; k < assignments.count() ; k++)
                        {
                            int shotId = assignments[k].toInt();
                            if (shot->getId() == shotId)
                            {
                                //remove from new list
                                assignments.removeAt(k);
                                ok = true;
                                break;
                            }
                        }
                        if (!ok)
                        {
                            ra->unAssign(shot,false);
                        }

                        updated = true;
                    }
                    //add remaining new assignemnts
                    for (int j = 0 ; j < assignments.count() ; j++)
                    {
                        int shotId = assignments[j].toInt();
                        RAMShot *shot = currentProject->getShot(shotId);
                        ra->assign(shot,false);
                    }

                    //remove from the new list
                    assetsArray.removeAt(i);
                    i--;
                }
            }
            // if the asset is not in the new list, remove it
            if (!updated)
            {
                RAMAsset *assetToRemove = assets[raI];
                stage->removeAsset(assetToRemove);

                removedItems << assetToRemove;
            }
        }
        //add the remaining new assets
        for (int i = 0 ; i < assetsArray.count() ; i++)
        {
            //new asset
            QJsonObject asset = assetsArray[i].toObject();
            QString name = asset.value("name").toString();
            QString shortName = asset.value("shortName").toString();
            QString comment = asset.value("comment").toString();
            int stageId = asset.value("stageId").toInt();
            int projectId = asset.value("projectId").toInt();
            QJsonArray assignments = asset.value("assignments").toArray();
            int id = asset.value("id").toInt();

            //check if status is null
            bool statusReAssigned = false;
            int statusId = asset.value("statusId").toInt();
            if (statusId == 0)
            {
                //get STB Status
                foreach(RAMStatus *status,statuses)
                {
                    if (status->getShortName() == "STB")
                    {
                        statusId = status->getId();
                        statusReAssigned = true;
                        break;
                    }
                }
            }

            if (stageId != stage->getId()) continue;

            RAMAsset *ra = new RAMAsset(dbi,name,shortName,getStatus(statusId),stageId,false,comment,id,projectId);
            if (statusReAssigned) ra->setStatus(getStatus(statusId),true);
            for (int j = 0;j<assignments.count() ; j++)
            {
                int shotId = assignments[j].toInt();
                ra->assign(currentProject->getShot(shotId),false);
            }

            stage->addAsset(ra);
        }
    }

    emit message("Got Assets","debug");
}

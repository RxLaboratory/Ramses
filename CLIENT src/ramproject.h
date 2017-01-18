#ifndef RAMPROJECT_H
#define RAMPROJECT_H

#include <QObject>
#include <QList>
#include "ramstage.h"
#include "ramshot.h"

class RAMProject : public QObject
{
    Q_OBJECT
public:
    explicit RAMProject(int i, QString n, QString sN,QObject *parent = 0);
    int getId();
    QString getName();
    QString getShortName();
    QList<RAMStage *> getStages();
    QList<RAMShot *> getShots();
    void addStage(RAMStage *s);
    void removeStage(RAMStage *s);
    void addShot(RAMShot *s);
    void removeShot(RAMShot *s);

signals:

public slots:

private:
    int projectId;
    QString projectName;
    QString projectShortName;
    QList<RAMStage *> projectStages;
    QList<RAMShot *> projectShots;
};

#endif // RAMSTATUS_H

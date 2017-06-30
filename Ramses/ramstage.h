#ifndef RAMSTAGE_H
#define RAMSTAGE_H

#include <QObject>
#include "dbinterface.h"

class RAMStage : public QObject
{
    Q_OBJECT
public:
    explicit RAMStage(DBInterface *db, QString n, QString sN, int i, bool updateDb, QObject *parent = 0);
    ~RAMStage();
    int getId();
    QString getName();
    QString getShortName();
    void setId(int id, bool updateDb = false);
    void setName(QString name, bool updateDb = false);
    void setShortName(QString shortName, bool updateDb = false);
    void update();
    void remove();

signals:
    void stageRemoved(RAMStage*);
    void nameChanged(QString);
    void shortNameChanged(QString);

public slots:

private:
    int stageId;
    QString stageName;
    QString stageShortName;
    DBInterface *dbi;
protected:
    bool operator==(RAMStage s);
    bool operator==(RAMStage *s);
};

#endif // RAMSTAGE_H

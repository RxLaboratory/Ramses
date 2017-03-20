#ifndef RAMASSET_H
#define RAMASSET_H

#include <QObject>
#include "ramstagestatus.h"
#include "ramstage.h"

class RAMAsset : public QObject
{
    Q_OBJECT
public:
    explicit RAMAsset(int i, QString n, QString sn, QList<RAMStage *> s, RAMStatus *st, QObject *parent = 0);
    RAMStatus* getStatus();
    QString getName();
    QList<RAMStage*> getStages();
    QString getShortName();
    int getId();
    QString getComment();
    void setComment(QString c);
    void setStatus(RAMStatus *s);

signals:
    void statusChanged(RAMAsset *);
    void statusChanged();

public slots:

private:
    QString name;
    QString shortName;
    int id;
    QString comment;
    RAMStatus *status;
    QList<RAMStage *> stages;
protected:
    bool operator==(RAMAsset a);
    bool operator==(RAMAsset *a);
};

#endif // RAMASSET_H

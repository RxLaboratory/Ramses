#ifndef RAMASSET_H
#define RAMASSET_H

#include <QObject>
#include "ramstagestatus.h"
#include "ramstage.h"

class RAMAsset : public QObject
{
    Q_OBJECT
public:
    explicit RAMAsset(QString name, QString shortName, RAMStage *s, RAMStatus *st, QObject *parent = 0);
    RAMStatus* getStatus();
    QString getName();
    RAMStage* getStage();
    QString getShortName();

signals:

public slots:

private:
    QString name;
    QString shortName;
    RAMStatus *status;
    RAMStage *stage;
};

#endif // RAMASSET_H

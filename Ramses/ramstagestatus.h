#ifndef RAMSTAGESTATUS_H
#define RAMSTAGESTATUS_H

#include <QObject>
#include "ramstatus.h"
#include "ramstage.h"

class RAMStageStatus : public QObject
{
    Q_OBJECT
public:
    explicit RAMStageStatus(RAMStatus *stat = 0, RAMStage *stag = 0, QString com = "", QObject *parent = 0);
    void setStatus(RAMStatus *s);
    void setStage(RAMStage *s);
    void setComment(QString c);
    RAMStage* getStage();
    RAMStatus* getStatus();
    QString getComment();
    int getId();

signals:
    void statusChanged(RAMStatus *, RAMStage *);

public slots:

private:
    RAMStatus *status;
    RAMStage * stage;
    QString comment;
};

#endif // RAMSTAGESTATUS_H

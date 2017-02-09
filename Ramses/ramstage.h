#ifndef RAMSTAGE_H
#define RAMSTAGE_H

#include <QObject>

class RAMStage : public QObject
{
    Q_OBJECT
public:
    explicit RAMStage(int i, QString n, QString sN, QString t, QObject *parent = 0);
    int getId();
    QString getName();
    QString getShortName();
    QString getType();

signals:

public slots:

private:
    int stageId;
    QString stageName;
    QString stageShortName;
    QString stageType;
protected:
    bool operator==(RAMStage s);
    bool operator==(RAMStage *s);
};

#endif // RAMSTAGE_H

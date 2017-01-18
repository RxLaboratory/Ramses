#ifndef RAMSTATUS_H
#define RAMSTATUS_H

#include <QObject>
#include <QColor>

class RAMStatus : public QObject
{
    Q_OBJECT
public:
    explicit RAMStatus(int i, QString n, QString sN, QColor c, QString d, QObject *parent = 0);
    int getId();
    QString getName();
    QString getShortName();
    QColor getColor();
    QString getDescription();

signals:

public slots:

private:
    int statusId;
    QString statusName;
    QString statusShortName;
    QColor statusColor;
    QString statusDescription;
};

#endif // RAMSTATUS_H

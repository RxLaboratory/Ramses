#ifndef RAMSTATUS_H
#define RAMSTATUS_H

#include <QObject>
#include <QColor>
#include "ramuuid.h"
#include "dbinterface.h"

class RAMStatus : public QObject
{
    Q_OBJECT
public:
    explicit RAMStatus(DBInterface *db,  QString n, QString sN, QColor c, QString d, QString id = "", bool updateDb = false, QObject *parent = 0);
    ~RAMStatus();
    // GET
    QString getId();
    QString getName();
    QString getShortName();
    QColor getColor();
    QString getDescription();
    // SET
    void setName(QString name, bool updateDb = false);
    void setShortName(QString shortName, bool updateDb = false);
    void setColor(QColor color, bool updateDb = false);
    void setDescription(QString description, bool updateDb = false);
    void update();
    // REMOVE
    void remove();

signals:
    void statusRemoved(RAMStatus*);
    void nameChanged(QString);
    void shortNameChanged(QString);
    void colorChanged(QColor);
    void descriptionChanged(QString);

public slots:

private:
    QString uuid;
    QString statusName;
    QString statusShortName;
    QColor statusColor;
    QString statusDescription;
    DBInterface *dbi;

protected:
    bool operator==(RAMStatus s);
    bool operator==(RAMStatus *s);
};

#endif // RAMSTATUS_H

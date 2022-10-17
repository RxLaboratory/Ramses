#ifndef DBISTRUCTURES_H
#define DBISTRUCTURES_H

#include <QString>
#include <QDateTime>

struct ScheduleEntryStruct {
    QString uuid = "";
    QString stepUuid = "";
    QString userUuid = "";
    QDateTime date = QDateTime();
    QString comment = "";
};

struct ScheduleCommentStruct {
    QString uuid = "";
    QString projectUuid = "";
    QDateTime date = QDateTime();
    QString comment = "";
    QColor color = QColor(227, 227, 227);
};

#endif // DBISTRUCTURES_H

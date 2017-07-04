#ifndef MAINTABLE_H
#define MAINTABLE_H

#include "ui_maintable.h"
#include "updater.h"
#include "dbinterface.h"
#include "shotwidget.h"
#include "shotassetswidget.h"

class MainTable : public QWidget, private Ui::MainTable
{
    Q_OBJECT

public:
    explicit MainTable(DBInterface *db,Updater *up, QWidget *parent = 0);
    void clean();

public slots:
    void setCurrentProject(RAMProject*);

private slots:
    void removeShot(RAMProject *project, RAMShot *shot);
    void addShot(RAMProject *project, RAMShot *shot, int row);
    void addStage(RAMProject* project, RAMStage *stage);
    void removeStage(RAMProject* project, RAMStage *stage);

private:
    void mapEvents();
    DBInterface *dbi;
    Updater *updater;
};

#endif // MAINTABLE_H

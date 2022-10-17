#ifndef RAMPIPE_H
#define RAMPIPE_H

#include "ramobject.h"

#include "ramproject.h"

class RamStep;

class RamPipe : public RamObject
{
    Q_OBJECT
public:

    // STATIC //

    static RamPipe *get(QString uuid);
    static RamPipe *c(RamObject *o);

    // OTHER //

    RamPipe(RamStep *output, RamStep *input);

    RamStep *outputStep() const;
    void setOutputStep(RamObject *outputStep);
    RamStep *inputStep() const;
    void setInputStep(RamObject *inputStep);

    QString name() const override;

    RamProject *project() const;

    RamObjectModel *pipeFiles() const;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamPipe*> m_existingObjects;
    RamPipe(QString uuid);
    virtual QString folderPath() const override { return ""; };

    static QFrame *ui_editWidget;

private slots:
    void pipeFilesDataChanged();

private:
    void construct();
    void connectEvents();

    RamObjectModel *m_pipeFiles;
};

#endif // RAMPIPE_H

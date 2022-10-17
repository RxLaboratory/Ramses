#ifndef RAMSTEP_H
#define RAMSTEP_H

#include <QDesktopServices>

#include "ramtemplatestep.h"

class RamAssetGroup;
class RamProject;
class RamApplication;
class RamFileType;
class RamUser;
class RamWorkingFolder;
class RamObjectList;

class RamStep : public RamTemplateStep
{
    Q_OBJECT
public:
    static RamStep *get(QString uuid);
    static RamStep *c(RamObject *o);
    static RamStep *createFromTemplate(RamTemplateStep *tempStep, RamProject *project);

    // METHODS //

    RamStep(QString shortName, QString name, RamProject *project);

    RamProject *project() const;

    RamAssetGroup *estimationMultiplyGroup() const;
    void setEstimationMultiplyGroup(RamObject *newEstimationMultiplyGroup);

    qint64 timeSpent() const; //seconds
    float estimation() const; //days
    float completionRatio() const;
    float latenessRatio() const;
    float assignedDays() const;
    float unassignedDays() const;
    float missingDays() const;
    float daysSpent() const;
    float neededDays() const;
    /**
     * @brief stats
     * @return a list of number of days <estimation, completed, scheduled, scheduled in the future>
     */
    QVector<float> stats(RamUser *user);

    /**
     * @brief freezeEstimations stops automatic update of the estimations.
     * Use this to improve performance when loading a bunch of data.
     * @param freeze Whether to freeze the estimations
     * @param reCompute When false, the estimation will not be recomputed on unfreeze
     */
    void freezeEstimations(bool freeze = true, bool reCompute = true);

    void openFile(QString filePath) const;
    QSet<RamWorkingFolder> templateWorkingFolders() const;

    QSet<RamObject *> inputFileTypes();
    QSet<RamObject *> outputFileTypes();

signals:
    void estimationComputed(RamStep*);

public slots:
    virtual void edit(bool show = true) override;

    void computeEstimation();
    void countAssignedDays();

protected:
    static QHash<QString, RamStep*> m_existingObjects;
    RamStep(QString uuid);
    virtual QString folderPath() const override;

    static QFrame *ui_editWidget;

private:
    void construct();

    /**
     * @brief When true, estimations won't be computed.
     * This should be set to true when loading a bunch of data, to improve performance
     * and reset to false afterwards.
     */
    bool m_freezeEstimations = false;

    /**
     * @brief m_estimationChanged is set to true if the estimation needs to be recomputed
     */
    bool m_estimationChanged = false;

    // Estimation cache
    qint64 m_timeSpent = 0;
    float m_estimation = 0;
    float m_completionRatio = 0;
    float m_latenessRatio = 0;
    int m_scheduledHalfDays = 0;
    int m_scheduledFutureHalfDays = 0;
    float m_missingDays = 0;
};

#endif // RAMSTEP_H

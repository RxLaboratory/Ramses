#ifndef RAMTEMPLATESTEP_H
#define RAMTEMPLATESTEP_H

#include "ramobject.h"

#include "ramobjectmodel.h"

class RamTemplateStep : public RamObject
{
    Q_OBJECT
public:

    // ENUMS //

    enum Type{ PreProduction,
               AssetProduction,
               ShotProduction,
               PostProduction,
               All };
    Q_ENUM(Type)

    enum EstimationMethod { EstimatePerShot = 0,
                            EstimatePerSecond = 1 };
    Q_ENUM(EstimationMethod)

    // STATIC METHODS //

    /**
     * @brief stepTypeName gets the name of a type, as used in the Database and API classes
     * @param type
     * @return
     */
    static const QString stepTypeName(Type type);
    static Type stepTypeFromName(QString typeName);

    static RamTemplateStep *get(QString uuid);
    static RamTemplateStep *c(RamObject *o);

    // OTHER METHODS //

    explicit RamTemplateStep(QString shortName, QString name, ObjectType type = TemplateStep);

    RamObjectModel *applications() const;

    Type type() const;
    void setType(const Type &type);
    void setType(QString type);

    QString publishSettings() const;
    void setPublishSettings(const QString &newPublishSettings);

    EstimationMethod estimationMethod() const;
    void setEstimationMethod(const EstimationMethod &newEstimationMethod);

    float estimationVeryEasy() const;
    void setEstimationVeryEasy(float newEstimationVeryEasy);

    float estimationEasy() const;
    void setEstimationEasy(float newEstimationEasy);

    float estimationMedium() const;
    void setEstimationMedium(float newEstimationMedium);

    float estimationHard() const;
    void setEstimationHard(float newEstimationHard);

    float estimationVeryHard() const;
    void setEstimationVeryHard(float newEstimationVeryHard);

    virtual QString iconName() const override;
    virtual QString details() const override;

public slots:
    virtual void edit(bool show = true) override;

protected:
    static QHash<QString, RamTemplateStep*> m_existingObjects;
    RamTemplateStep(QString uuid, ObjectType type = TemplateStep);
    virtual QString folderPath() const override { return ""; };

    static QFrame *ui_editWidget;

protected:

    // STATIC //

    /**
     * @brief m_stepTypeMeta is used to return information from the type,
     * like the corresponding string
     */
    static const QMetaEnum m_stepTypeMeta;


    // ATTRIBUTES //

    RamObjectModel *m_applications;

private:
    void construct();
};

#endif // RAMTEMPLATESTEP_H

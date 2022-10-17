#include "ramtemplatestep.h"

#include "templatestepeditwidget.h"

// STATIC //

QFrame *RamTemplateStep::ui_editWidget = nullptr;

QHash<QString, RamTemplateStep*> RamTemplateStep::m_existingObjects = QHash<QString, RamTemplateStep*>();

QMetaEnum const RamTemplateStep::m_stepTypeMeta = QMetaEnum::fromType<RamTemplateStep::Type>();

const QString RamTemplateStep::stepTypeName(Type type)
{
    return QString(m_stepTypeMeta.valueToKey(type));
}

RamTemplateStep::Type RamTemplateStep::stepTypeFromName(QString typeName)
{
    if (typeName == "PreProduction") return PreProduction;
    if (typeName == "AssetProduction") return AssetProduction;
    if (typeName == "ShotProduction") return ShotProduction;
    if (typeName == "PostProduction") return PostProduction;
    if (typeName == "All") return All;
    return All;
}

RamTemplateStep *RamTemplateStep::get(QString uuid)
{
    if (!checkUuid(uuid, TemplateStep)) return nullptr;

    RamTemplateStep *t = m_existingObjects.value(uuid);
    if (t) return t;

    // Finally return a new instance
    return new RamTemplateStep(uuid);
}

RamTemplateStep *RamTemplateStep::c(RamObject *o)
{
    //return qobject_cast<RamTemplateStep*>(o);
    // For performance, reinterpret_cast, but be careful with the object passed!
    return reinterpret_cast<RamTemplateStep*>(o);
}

// PUBLIC //

RamTemplateStep::RamTemplateStep(QString shortName, QString name, ObjectType type) :
    RamObject(shortName, name, type)
{
    construct();

    // Preprod as default step
    setType("pre");
}

RamTemplateStep::RamTemplateStep(QString uuid, ObjectType type):
    RamObject(uuid, type)
{
    construct();

    loadModel(m_applications, "applications");
}

RamObjectModel *RamTemplateStep::applications() const
{
    return m_applications;
}

RamTemplateStep::Type RamTemplateStep::type() const
{
    QString typeStr = getData("type").toString();

    if (typeStr == "pre") return PreProduction;
    else if (typeStr == "asset") return AssetProduction;
    else if (typeStr == "shot") return ShotProduction;
    else if (typeStr == "post") return PostProduction;
    else return All;
}

void RamTemplateStep::setType(const Type &type)
{
    switch(type){
    case PreProduction:
        insertData("type", "pre");
        break;
    case AssetProduction:
        insertData("type", "asset");
        break;
    case ShotProduction:
        insertData("type", "shot");
        break;
    case PostProduction:
        insertData("type", "post");
        break;
    case All:
        insertData("type", "all");
        break;
    }
}

void RamTemplateStep::setType(QString type)
{
    insertData("type", type);
}

QString RamTemplateStep::publishSettings() const
{
    return getData("publishSettings").toString();
}

void RamTemplateStep::setPublishSettings(const QString &newPublishSettings)
{
    insertData("publishSettings", newPublishSettings);
}

float RamTemplateStep::estimationVeryHard() const
{
    return getData("estimationVeryHard").toDouble(4);
}

void RamTemplateStep::setEstimationVeryHard(float newEstimationVeryHard)
{
    insertData("estimationVeryHard", newEstimationVeryHard);
}

QString RamTemplateStep::iconName() const
{
    switch(type())
    {
    case PreProduction:
        return ":/icons/project";
    case ShotProduction:
        return ":/icons/shot";
    case AssetProduction:
        return ":/icons/asset";
    case PostProduction:
        return ":/icons/film";
    case All:
        return ":/icons/step";
    }
    return ":/icons/step";
}

QString RamTemplateStep::details() const
{
    switch(type())
    {
    case PreProduction:
        return "Pre-production";
    case ShotProduction:
        return "Shot production";
    case AssetProduction:
        return "Asset production";
    case PostProduction:
        return "Post-production";
    case All:
        return "";
    }
    return "";
}

void RamTemplateStep::edit(bool show)
{
    if (!ui_editWidget) ui_editWidget = createEditFrame(new TemplateStepEditWidget(this));

    if (show) showEdit( ui_editWidget );
}

float RamTemplateStep::estimationHard() const
{
    return getData("estimationHard").toDouble(2);
}

void RamTemplateStep::setEstimationHard(float newEstimationHard)
{
    insertData("estimationHard", newEstimationHard);
}

float RamTemplateStep::estimationMedium() const
{
    return getData("estimationMedium").toDouble(1);
}

void RamTemplateStep::setEstimationMedium(float newEstimationMedium)
{
    insertData("estimationMedium", newEstimationMedium);
}

float RamTemplateStep::estimationEasy() const
{
    return getData("estimationEasy").toDouble(0.5);
}

void RamTemplateStep::setEstimationEasy(float newEstimationEasy)
{
    insertData("estimationEasy", newEstimationEasy);
}

float RamTemplateStep::estimationVeryEasy() const
{
    return getData("estimationVeryEasy").toDouble(0.2);
}

void RamTemplateStep::setEstimationVeryEasy(float newEstimationVeryEasy)
{
    insertData("estimationVeryEasy", newEstimationVeryEasy);
}

RamTemplateStep::EstimationMethod RamTemplateStep::estimationMethod() const
{
    QString methodStr = getData("estimationMethod").toString("shot");
    if (methodStr == "shot") return EstimatePerShot;
    else return EstimatePerSecond;
}

void RamTemplateStep::setEstimationMethod(const EstimationMethod &newEstimationMethod)
{
    switch(newEstimationMethod)
    {
    case EstimatePerShot:
        insertData("estimationMethod", "shot");
        break;
    case EstimatePerSecond:
        insertData("estimationMethod", "second");
        break;
    }
}

// PROTECTED //

void RamTemplateStep::construct()
{
    m_existingObjects[m_uuid] = this;
    m_icon = ":/icons/step";
    m_editRole = Admin;

    m_applications = createModel(RamObject::Application, "applications");
}

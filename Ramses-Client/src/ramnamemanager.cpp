#include "ramnamemanager.h"
#include "ramses.h"

// Constants
const QString RamNameManager::ShotType = "S";
const QString RamNameManager::AssetType = "A";
const QString RamNameManager::GeneralType = "G";
const QString RamNameManager::MetaDataFileName = "_ramses_data.json";

RamNameManager::RamNameManager()
{
    init();

    // get state shortnames
    QStringList stateShortNames;
    DBTableModel *states = Ramses::instance()->states();
    for(int i = 0; i < states->rowCount(); i++)
    {

        RamObject *o = states->get( states->index(i, 0) );
        if (o) stateShortNames << o->shortName();
    }

    // get name regex
    m_reName = RegExUtils::getRegularExpression("ramses-filename", "PUB|WIP|V", stateShortNames.join("|"));
    m_reName.setPatternOptions(QRegularExpression::CaseInsensitiveOption | QRegularExpression::MultilineOption);
}

QString RamNameManager::originalFileName() const
{
    return m_fileName;
}

QString RamNameManager::fileName() const
{
    QStringList nameList;

    nameList << m_project;

    if (m_ramType != "")
        nameList << m_ramType;

    if (m_ramType == AssetType || m_ramType == ShotType)
        if (m_shortName != "")
            nameList << m_shortName;

    if (m_step != "")
        nameList << m_step;

    if (m_ramType == GeneralType && m_shortName != "" && m_step != "")
        nameList << m_shortName;

    if (m_resource != "")
        nameList << m_resource;

    if (m_version > 0)
    {
        QString v;
        if (m_state != "") v = m_state;
        QString vNum = QString::number( m_version );
        while(vNum.count() < 3)
            vNum = "0" + vNum;
        v += vNum;
        nameList << v;
    }

    QString name = nameList.join("_");

    if (m_extension != "")
    {
        QString ext;
        if ( !m_extension.startsWith(".") )
            ext = "." + m_extension;
        else
            ext = m_extension;

        name += ext;
    }

    return name;

}

bool RamNameManager::setFileName(QString name)
{

    // remove trailing "/"
    return setFileName(QFileInfo( name.replace(QRegExp("[\\/\\\\]$"), "") ));
}

bool RamNameManager::setFileName(const QFileInfo &name)
{
    init();

    m_fileName = name.fileName();

    QRegularExpressionMatch match = m_reName.match( m_fileName );

    if (!match.hasMatch()) return false;

    m_project = match.captured(1);
    m_ramType = match.captured(2);

    if (m_ramType == AssetType || m_ramType == ShotType)
    {
        m_shortName = match.captured(3);
        m_step = match.captured(4);
    }
    else
    {
        m_step = match.captured(3);
        m_shortName = match.captured(4);
    }

    m_resource = match.captured(5);
    m_state = match.captured(6);

    int v = match.captured(7).toInt();
    if (v >= 0)
        m_version = v;

    m_extension = match.captured(8);

    return true;
}

QString RamNameManager::project() const
{
    return m_project;
}

QString RamNameManager::type() const
{
    return m_ramType;
}

QString RamNameManager::shortName() const
{
    return m_shortName;
}

QString RamNameManager::step() const
{
    return m_step;
}

QString RamNameManager::state() const
{
    return m_state;
}

QString RamNameManager::resource() const
{
    return m_resource;
}

int RamNameManager::version() const
{
    return m_version;
}

QString RamNameManager::extension() const
{
    return m_extension;
}

void RamNameManager::setProject(const QString &proj)
{
    m_project = proj;
}

void RamNameManager::setType(const QString &type)
{
    m_ramType = type;
}

void RamNameManager::setShortName(const QString &shortName)
{
    m_shortName = shortName;
}

void RamNameManager::setStep(const QString &step)
{
    m_step = step;
}

void RamNameManager::setState(const QString &state)
{
    m_state = state;
}

void RamNameManager::setResource(const QString &resource)
{
    m_resource = resource;
}

void RamNameManager::setVersion(const int &version)
{
    m_version = version;
}

void RamNameManager::setExtension(const QString &extension)
{
    m_extension = extension;
}

void RamNameManager::init()
{
    m_project = "";
    m_ramType = "";
    m_shortName = "";
    m_step = "";
    m_resource = "";
    m_state = "";
    m_version = -1;
    m_extension = "";
}

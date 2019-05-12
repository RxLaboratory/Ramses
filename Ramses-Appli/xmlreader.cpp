#include "xmlreader.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

XMLReader::XMLReader():
    QXmlStreamReader()
{
    this->setNamespaceProcessing(false);
}
XMLReader::XMLReader(QIODevice *device):
    QXmlStreamReader(device)
{
    this->setNamespaceProcessing(false);
}

XMLReader::XMLReader(const QByteArray &data):
    QXmlStreamReader(data)
{
    this->setNamespaceProcessing(false);
}

XMLReader::XMLReader(const QString &data):
    QXmlStreamReader(data)
{
    this->setNamespaceProcessing(false);
}

XMLReader::XMLReader(const char *data):
    QXmlStreamReader(data)
{
    this->setNamespaceProcessing(false);
}

bool XMLReader::nextStartElement(QString name, bool recursive)
{
    while (this->readNextStartElement())
    {
        if (this->name().toString() == name)
        {
            return true;
        }
        else if (!recursive) this->skipCurrentElement();
    }
    return false;
}

bool XMLReader::nextStartElement(QStringList names, bool recursive)
{
    while (this->readNextStartElement())
    {
        foreach(QString name,names)
        {
            if (this->name().toString() == name)
            {
                return true;
            }
        }

        if (!recursive) this->skipCurrentElement();
    }
    return false;
}

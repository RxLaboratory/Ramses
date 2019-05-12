#ifndef XMLREADER_H
#define XMLREADER_H

#include <QObject>
#include <QXmlStreamReader>

class XMLReader : public QXmlStreamReader
{
public:
    XMLReader();
    XMLReader(QIODevice *device);
    XMLReader(const QByteArray &data);
    XMLReader(const QString &data);
    XMLReader(const char *data);
    bool nextStartElement(QString name,bool recursive = true);
    bool nextStartElement(QStringList names,bool recursive = true);

signals:

public slots:
};

#endif // XMLREADER_H

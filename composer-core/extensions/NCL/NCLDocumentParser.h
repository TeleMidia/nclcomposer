#ifndef NCLDOCUMENTPARSER_H
#define NCLDOCUMENTPARSER_H

#include <QObject>
#include <QDebug>
#include <QXmlDefaultHandler>
#include <QStack>
#include <QMutex>
#include <QWaitCondition>

#include <core/extensions/IDocumentParser.h>
using namespace composer::core::extension;

namespace composer {
namespace language{

class NCLDocumentParser : public IDocumentParser, public QXmlDefaultHandler
{
    Q_OBJECT
private:
    Document *doc;
    QMutex lockStack;
    QWaitCondition sync;
    QStack<Entity*> elementStack;

public:
    explicit NCLDocumentParser(Document *doc);
    ~NCLDocumentParser();
    bool parseDocument();
    QString getParserName();

protected:
    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI,
                    const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

signals:

public slots:
    void onEntityAddError(QString error);
    void onEntityAdded(QString ID, Entity *entity);

};

}} //end namespace
#endif // NCLDOCUMENTPARSER_H

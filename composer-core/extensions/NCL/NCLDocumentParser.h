/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef NCLDOCUMENTPARSER_H
#define NCLDOCUMENTPARSER_H

#include <QObject>
#include <QDebug>
#include <QXmlDefaultHandler>
#include <QStack>
#include <QMutex>
#include <QWaitCondition>

#include <core/extensions/IDocumentParser.h>
using namespace composer::extension;

namespace composer {
    namespace language{

class NCLDocumentParser : public IDocumentParser, public QXmlDefaultHandler
{
    Q_OBJECT
private:
    Project *project;
    QMutex lockStack;
    QWaitCondition sync;
    QStack<Entity*> elementStack;

public:
    explicit NCLDocumentParser(Project *project);
    ~NCLDocumentParser();
    bool parseDocument(); // \deprecated
    bool parseContent(const QString &str);
    QString getParserName();

    bool serialize();

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

    bool endDocument();

signals:
    void parseFinished();

public slots:
    void onEntityAddError(QString error);
    void onEntityAdded(QString ID, Entity *entity);

};

}} //end namespace
#endif // NCLDOCUMENTPARSER_H

/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include "util/ComposerCoreControl_global.h"
#include "../model/Project.h"
using namespace composer::core::model;

#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QXmlContentHandler>

#include <QStack>
#include <QMutex>
#include <QWaitCondition>
#include <QXmlInputSource>

#include <QFile>
#include <util/Utilities.h>

namespace composer {
  namespace core {
/*!
 * \brief The class responsible to parse the Composer Project and generate fills
 *  the data of a Project object.
 */
class COMPOSERCORESHARED_EXPORT ProjectReader: public QXmlDefaultHandler
{
public:
  ProjectReader();
  virtual ~ProjectReader();

  Project *readFile(const QString &location);

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

private:
  Project *project;
  Entity *currentEntity;
  QMutex lockStack;
  QWaitCondition sync;
  QStack<Entity*> elementStack;

  bool parseModelString(const QString &str);
};

} } //end namespace

#endif

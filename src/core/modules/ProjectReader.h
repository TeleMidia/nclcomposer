/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef PROJECTREADER_H
#define PROJECTREADER_H

#include "util/ComposerCore_global.h"
#include "model/Project.h"
using namespace cpr::core;

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

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief It is responsible for parsing the NCL Composer Project and
 * generating the data of a Project object.
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
  QDomDocument _domDocument;
  Project *_project;
  Entity *_currentEntity;
  QMutex _lockStack;
  QWaitCondition _sync;
  QStack<Entity*> _elementStack;

  bool parseModelString(const QString &str);
};

CPR_CORE_END_NAMESPACE

#endif

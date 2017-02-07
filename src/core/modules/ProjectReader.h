/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public
 License along with Foobar.  If not, see <http://www.gnu.org/licenses/>. */

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

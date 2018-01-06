/* Copyright (C) 2011-2018 Telemidia/PUC-Rio.
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

#include "NCLLanguageProfile_global.h"

#include <QDebug>
#include <QMutex>
#include <QObject>
#include <QStack>
#include <QWaitCondition>
#include <QXmlDefaultHandler>

#include <extensions/IDocumentParser.h>
using namespace cpr::core;

CPR_NCLPROFILE_BEGIN_NAMESPACE

class NCLLANGUAGEPROFILESHARED_EXPORT NCLDocumentParser
    : public IDocumentParser,
      public QXmlDefaultHandler
{
  Q_OBJECT

public:
  explicit NCLDocumentParser (Project *_project);
  ~NCLDocumentParser ();

  bool parseDocument () override; // \deprecated
  bool parseContent (const QString &str) override;
  QString parserName () override;

  bool serialize ();

public slots:
  void onEntityAddError (const QString &error) override;
  void onEntityAdded (const QString &ID, Entity *entity) override;

signals:
  void parseFinished ();

protected:
  bool startElement (const QString &namespaceURI, const QString &localName,
                     const QString &qName,
                     const QXmlAttributes &attributes) override;
  bool endElement (const QString &namespaceURI, const QString &localName,
                   const QString &qName) override;

  bool characters (const QString &str) override;
  bool fatalError (const QXmlParseException &exception) override;

  bool endDocument () override;

private:
  Project *_project;
  QMutex _lockStack;
  QWaitCondition _sync;
  QStack<Entity *> _elementStack;
};

CPR_NCLPROFILE_END_NAMESPACE

#endif // NCLDOCUMENTPARSER_H

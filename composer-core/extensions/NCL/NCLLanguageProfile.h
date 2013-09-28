/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef NCLLANGUAGEPROFILE_H
#define NCLLANGUAGEPROFILE_H

#include "NCLLanguageProfile_global.h"
#include <core/extensions/ILanguageProfile.h>

using namespace composer::extension;

namespace composer {
namespace language {

class NCLLANGUAGEPROFILESHARED_EXPORT NCLLanguageProfile : public QObject,
    public ILanguageProfile {

  Q_OBJECT
  Q_INTERFACES(ILanguageProfile)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID ILanguageProfile_iid FILE "nclLanguageProfile.json")
#endif

public:
  NCLLanguageProfile();
  virtual ~NCLLanguageProfile();

  LanguageType getLanguageType();
  QString getProfileName();
  QList<QString> getOutputDocumentTypes();

  IDocumentParser* createParser(Project *project);
  void releaseDocumentParser (IDocumentParser *parser);

  map <QString, map <QString, char> *> *getNesting();
  map <QString, bool> *getAttributes (QString element);
  map <QString, char> *getChildren (QString tagname);

  vector <AttributeReferences *> getReferences(QString element, QString attr);

};
} } //end namespace
#endif // NCLLANGUAGEPROFILE_H

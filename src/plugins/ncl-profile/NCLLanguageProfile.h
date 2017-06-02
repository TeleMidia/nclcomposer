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
#include <extensions/ILanguageProfile.h>

using namespace cpr::core;

namespace composer {
namespace language {

class NCLLANGUAGEPROFILESHARED_EXPORT NCLLanguageProfile :
    public QObject,
    public ILanguageProfile
{
  Q_OBJECT
  Q_INTERFACES(ILanguageProfile)
  Q_PLUGIN_METADATA(IID ILanguageProfile_iid FILE "ncl-profile.json")

public:
  NCLLanguageProfile();
  virtual ~NCLLanguageProfile();

  LanguageType getLanguageType();
  QString getProfileName();
  QList<QString> getOutputDocumentTypes();

  IDocumentParser* createParser(Project *project);
  void releaseDocumentParser (IDocumentParser *parser);

  map <QString, map <QString, char> *> *getNesting();
  map <QString, bool> *getAttributes (const QString &element);
  map <QString, char> *getChildren (const QString &tagname);

  vector <AttributeReferences *> getReferences(const QString &element,
                                               const QString &attr);
};

} } //end namespace
#endif // NCLLANGUAGEPROFILE_H

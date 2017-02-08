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

#ifndef ILANGUAGE_H
#define ILANGUAGE_H

#include <QtPlugin>
#include <QList>
#include <QString>

#include "IDocumentParser.h"
using namespace composer::extension;

#include "../util/Utilities.h"
using namespace composer::core::util;

#include "../model/Project.h"
using namespace composer::core::model;

#include "../util/AttributeReferences.h"
using namespace composer::util;

namespace composer {
  namespace extension {
/**!
  * This class must be implemented by anyone that want to support
  * a new language in Composer.
  */
class COMPOSERCORESHARED_EXPORT ILanguageProfile
{
public:
  virtual ~ILanguageProfile() {}

  virtual LanguageType getLanguageType()  = 0;

  virtual QString getProfileName() = 0;

  virtual QList<QString> getOutputDocumentTypes() = 0;

  virtual IDocumentParser* createParser(Project *project) = 0;

  virtual void releaseDocumentParser (IDocumentParser *parser) = 0;

  //FUNCTIONS RELATED TO LANGUAGE STRUCTURE
  virtual map <QString, map <QString, char> *> *getNesting() = 0;

  virtual map <QString, bool> *getAttributes (const QString &element) = 0;

  virtual map <QString, char> *getChildren (const QString &tagname) = 0;

  virtual vector <AttributeReferences *>
    getReferences (const QString &element, const QString &attr) = 0;
};

} } //end namespace


/**
  * This is require for the QTPlugin system work.
  * Declaring the interface that is going to be used by external plugins.
  */
#define ILanguageProfile_iid "br.puc_rio.telemidia.composer.ILanguageProfile"
Q_DECLARE_INTERFACE(ILanguageProfile, ILanguageProfile_iid)

#endif // ILANGUAGE_H

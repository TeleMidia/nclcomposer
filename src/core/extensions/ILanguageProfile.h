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

#include <QList>
#include <QString>
#include <QtPlugin>

#include "IDocumentParser.h"
#include "model/Project.h"
#include "util/AttributeReferences.h"
#include "util/Utilities.h"
using namespace cpr::core;

CPR_CORE_BEGIN_NAMESPACE

/*!
 * \ingroup core
 * \brief This class must be implemented by anyone that want to support
 * a new language in Composer.
 */
class COMPOSERCORESHARED_EXPORT ILanguageProfile
{
public:
  virtual ~ILanguageProfile () {}

  virtual LanguageType languageType () = 0;

  virtual QString profileName () = 0;

  virtual QList<QString> outputDocumentTypes () = 0;

  virtual IDocumentParser *createParser (Project *project) = 0;

  virtual void releaseDocumentParser (IDocumentParser *parser) = 0;

  // FUNCTIONS RELATED TO LANGUAGE STRUCTURE
  virtual map<QString, map<QString, char> *> *nesting () = 0;

  virtual map<QString, bool> *attrs (const QString &element) = 0;

  virtual map<QString, char> *children (const QString &tagname) = 0;

  virtual vector<AttributeReferences *> references (const QString &element,
                                                    const QString &attr)
      = 0;
};

CPR_CORE_END_NAMESPACE

/**
  * This is require for the QTPlugin system work.
  * Declaring the interface that is going to be used by external plugins.
  */
#define ILanguageProfile_iid "br.puc_rio.telemidia.composer.ILanguageProfile"
Q_DECLARE_INTERFACE (ILanguageProfile, ILanguageProfile_iid)

#endif // ILANGUAGE_H

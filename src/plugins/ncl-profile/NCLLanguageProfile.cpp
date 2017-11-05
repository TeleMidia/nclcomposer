/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "NCLLanguageProfile.h"
#include "NCLDocumentParser.h"
#include "NCLStructure.h"

CPR_NCLPROFILE_BEGIN_NAMESPACE

NCLLanguageProfile::NCLLanguageProfile () {}

NCLLanguageProfile::~NCLLanguageProfile () {}

LanguageType
NCLLanguageProfile::languageType ()
{
  return NCL;
}

QString
NCLLanguageProfile::profileName ()
{
  return "Nested Context Language Profile";
}

QList<QString>
NCLLanguageProfile::outputDocumentTypes ()
{
  QList<QString> list;
  list.append ("ncl");
  return list;
}

IDocumentParser *
NCLLanguageProfile::createParser (Project *project)
{
  return new NCLDocumentParser (project);
}

void
NCLLanguageProfile::releaseDocumentParser (IDocumentParser *parser)
{
  if (parser)
  {
    delete parser;
    parser = nullptr;
  }
}

map<QString, map<QString, char> *> *
NCLLanguageProfile::nesting ()
{
  return NCLStructure::instance ()->getNesting ();
}

map<QString, bool> *
NCLLanguageProfile::attrs (const QString &element)
{
  return NCLStructure::instance ()->getAttributes (element);
}

map<QString, char> *
NCLLanguageProfile::children (const QString &tagname)
{
  return NCLStructure::instance ()->getChildren (tagname);
}

vector<AttributeReferences *>
NCLLanguageProfile::references (const QString &element, const QString &attr)
{
  return NCLStructure::instance ()->getReferences (element, attr);
}

CPR_NCLPROFILE_END_NAMESPACE

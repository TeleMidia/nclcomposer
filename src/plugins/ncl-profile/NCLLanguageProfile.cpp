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

NCLLanguageProfile::NCLLanguageProfile()
{
}

NCLLanguageProfile::~NCLLanguageProfile()
{

}

LanguageType NCLLanguageProfile::getLanguageType()
{
  return NCL;
}

QString NCLLanguageProfile::getProfileName()
{
  return "Nested Context Language Profile";
}

QList<QString> NCLLanguageProfile::getOutputDocumentTypes()
{
  QList<QString> list;
  list.append("ncl");
  return list;
}

IDocumentParser* NCLLanguageProfile::createParser (Project *project)
{
  return new NCLDocumentParser (project);
}

void NCLLanguageProfile::releaseDocumentParser (IDocumentParser *parser)
{
  if (parser)
  {
    delete parser;
    parser = nullptr;
  }
}

map <QString, map <QString, char> *> *NCLLanguageProfile::getNesting()
{
  return NCLStructure::getInstance()->getNesting();
}

map <QString, bool> *NCLLanguageProfile::getAttributes (const QString &element)
{
  return NCLStructure::getInstance()->getAttributes(element);
}

map <QString, char> *NCLLanguageProfile::getChildren (const QString &tagname)
{
  return NCLStructure::getInstance()->getChildren(tagname);
}

vector <AttributeReferences *>
        NCLLanguageProfile::getReferences (const QString &element,
                                           const QString &attr)
{
  return NCLStructure::getInstance()->getReferences(element, attr);
}

CPR_NCLPROFILE_END_NAMESPACE

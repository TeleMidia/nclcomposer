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

#include "util/AttributeReferences.h"

CPR_CORE_BEGIN_NAMESPACE

AttributeReferences::AttributeReferences (const QString &element,
                                          const QString &attr,
                                          const QString &ref_element,
                                          const QString &ref_attr,
                                          REFERENCE_SCOPE scope)
{
  this->element = element;
  this->attr = attr;
  this->ref_element = ref_element;
  this->ref_attr = ref_attr;
  this->scope = scope;

  initializeStringToScope ();
}

AttributeReferences::AttributeReferences (const QString &element,
                                          const QString &attr,
                                          const QString &ref_element,
                                          const QString &ref_attr,
                                          const QString &scope)
{
  this->element = element;
  this->attr = attr;
  this->ref_element = ref_element;
  this->ref_attr = ref_attr;

  initializeStringToScope ();

  if (stringToScope.contains (scope))
    this->scope = stringToScope.value (scope);
  else
  {
    this->scope = USERDEFINED_SCOPE;
    this->userDefinedScope = scope;
  }
}

QString
AttributeReferences::getElement ()
{
  return this->element;
}

QString
AttributeReferences::getAttribute ()
{
  return this->attr;
}

QString
AttributeReferences::getRefElement ()
{
  return this->ref_element;
}

QString
AttributeReferences::getRefAttribute ()
{
  return this->ref_attr;
}

AttributeReferences::REFERENCE_SCOPE
AttributeReferences::getScope ()
{
  return this->scope;
}

void
AttributeReferences::initializeStringToScope ()
{
  if (!stringToScope.size ())
  {
    stringToScope.insert (QString ("SAME"), SAME_SCOPE);
    stringToScope.insert (QString ("ANY"), ANY_SCOPE);
  }
}

QString
AttributeReferences::getUserDefinedScope ()
{
  return this->userDefinedScope;
}

CPR_CORE_END_NAMESPACE

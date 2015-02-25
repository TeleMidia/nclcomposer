/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "util/AttributeReferences.h"

namespace composer {
    namespace util {

AttributeReferences::AttributeReferences ( QString element,
                                           QString attr,
                                           QString ref_element,
                                           QString ref_attr,
                                           REFERENCE_SCOPE scope)
{
  this->element = element;
  this->attr = attr;
  this->ref_element = ref_element;
  this->ref_attr = ref_attr;
  this->scope = scope;

  initializeStringToScope();
}

AttributeReferences::AttributeReferences ( QString element,
                                           QString attr,
                                           QString ref_element,
                                           QString ref_attr,
                                           QString scope)
{
  this->element = element;
  this->attr = attr;
  this->ref_element = ref_element;
  this->ref_attr = ref_attr;

  initializeStringToScope();

  if(stringToScope.contains(scope))
    this->scope = stringToScope.value(scope);
  else
  {
    this->scope = USERDEFINED_SCOPE;
    this->userDefinedScope = scope;
  }
}

QString AttributeReferences::getElement()
{
  return this->element;
}

QString AttributeReferences::getAttribute()
{
  return this->attr;
}

QString AttributeReferences::getRefElement()
{
  return this->ref_element;
}

QString AttributeReferences::getRefAttribute()
{
  return this->ref_attr;
}

AttributeReferences::REFERENCE_SCOPE AttributeReferences::getScope()
{
  return this->scope;
}

void AttributeReferences::initializeStringToScope()
{
  if(!stringToScope.size())
  {
    stringToScope.insert(QString("SAME"), SAME_SCOPE);
    stringToScope.insert(QString("ANY"), ANY_SCOPE);
  }
}

QString AttributeReferences::getUserDefinedScope()
{
  return this->userDefinedScope;
}

} } //end namespace

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

AttributeReferences::AttributeReferences (const QString &elt,
                                          const QString &attr,
                                          const QString &ref_elt,
                                          const QString &ref_attr,
                                          Scope scope)
{
  this->_elt = elt;
  this->_attr = attr;
  this->_ref_elt = ref_elt;
  this->_ref_attr = ref_attr;
  this->_scope = scope;

  initStr2Scope ();
}

AttributeReferences::AttributeReferences (const QString &elt,
                                          const QString &attr,
                                          const QString &ref_elt,
                                          const QString &ref_attr,
                                          const QString &scope)
{
  this->_elt = elt;
  this->_attr = attr;
  this->_ref_elt = ref_elt;
  this->_ref_attr = ref_attr;

  initStr2Scope ();

  if (_str2scope.contains (scope))
    this->_scope = _str2scope.value (scope);
  else
  {
    this->_scope = USERDEFINED_SCOPE;
    this->_userDefinedScope = scope;
  }
}

QString
AttributeReferences::element ()
{
  return this->_elt;
}

QString
AttributeReferences::attribute ()
{
  return this->_attr;
}

QString
AttributeReferences::refElement ()
{
  return this->_ref_elt;
}

QString
AttributeReferences::refAttribute ()
{
  return this->_ref_attr;
}

AttributeReferences::Scope
AttributeReferences::getScope ()
{
  return this->_scope;
}

void
AttributeReferences::initStr2Scope ()
{
  if (!_str2scope.size ())
  {
    _str2scope
        = { { QString ("SAME"), SAME_SCOPE }, { QString ("ANY"), ANY_SCOPE } };
  }
}

QString
AttributeReferences::userDefinedScope ()
{
  return this->_userDefinedScope;
}

CPR_CORE_END_NAMESPACE

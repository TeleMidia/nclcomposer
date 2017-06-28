/*
 * Copyright 2011-2013 Laws/UFMA.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#include "AttributeStructure.h"

AttributeStructure::AttributeStructure () {}

AttributeStructure::AttributeStructure (const string &_name) { name = _name; }

AttributeStructure::~AttributeStructure () {}

void
AttributeStructure::addElement (const string &_parent, bool _required)
{
  elements[_parent] = _required;
}

void
AttributeStructure::addRegex (const string &regex, const string &element)
{
  datatypes[element] = regex;
}

string
AttributeStructure::getName () const
{
  return name;
}

map<string, bool>
AttributeStructure::getElements () const
{
  return elements;
}

string
AttributeStructure::getDatatype (const string &element) const
{
  return datatypes.at (element);
}

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

#include "ElementStructure.h"

ElementStructure::ElementStructure () {}

/*
 *
 */
ElementStructure::ElementStructure (string _name, bool _defineScope)
{
  name = _name;
  defineScope = _defineScope;
}

ElementStructure::~ElementStructure () {}

/*
 * Add an attribute to element
 */
void
ElementStructure::addAtt (string attName, bool required)
{
  atts[attName] = required;
}

/*
 * Add a parent to element
 */
void
ElementStructure::addParent (string _parent, char _cardinality)
{
  parents[_parent] = _cardinality;
}

/*
 * Get element name
 */
string
ElementStructure::getName () const
{
  return name;
}

/*
 * Get element parents
 */
map<string, char>
ElementStructure::getParents () const
{
  return parents;
}

/*
 */
char
ElementStructure::getCardinality (string _parent)
{
  return parents[_parent];
}

/*
 *
 */
map<string, bool>
ElementStructure::getAtts () const
{
  return atts;
}

/*
 *
 */
bool
ElementStructure::isScopeDefined () const
{
  return defineScope;
}

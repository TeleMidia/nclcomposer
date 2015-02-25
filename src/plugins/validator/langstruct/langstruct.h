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

#ifndef LANGSTRUCT_H_
#define LANGSTRUCT_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include "components/ElementStructure.h"
#include "components/AttributeStructure.h"
#include "components/ReferenceStructure.h"


using namespace std;

class Langstruct
{
public:
  static void init ();
  static bool existElement (const string);
  static bool existAttribute (const string, const string);
  static bool existParent (const string, const string);
  static bool isValidAttribute (const string, const string, const string);
  static bool isElementReferenceDependent (const string);
  static bool isElementReferenced (const string);
  static bool isAttributeReferenceDependent (const string &element,
                                             const string &att);
  static bool defineScope (const string);

  /* Returns the cardinality of an element in relation to the given parent. */
  static char getCardinality (const string &element, const string &parent);

  /* Returns the name of the children of a given element. */
  static vector<string> getChildrenNames (const string);
  static map<string, bool> getAttributes (const string);
  static set<string> getRequiredAttributes (const string);
  static const ReferenceStructure *getReference (const string &elFrom,
                                                 const string &elFromAtt,
                                                 const string &elTo);
  static const vector <ReferenceStructure> getReferences (const string &elFrom,
                                                          const string
                                                          &elFromAtt);

private:
  static map<string, ElementStructure> _elements;
  static map<string, AttributeStructure> _attributes;
  static map<string, ReferenceStructure> _references;
  static map<string, string> _datatypes;
};


#endif /* LANGSTRUCT_H_ */

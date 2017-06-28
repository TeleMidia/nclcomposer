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

#ifndef ATTRIBUTESTRUCTURE_H_
#define ATTRIBUTESTRUCTURE_H_

#include <map>
#include <string>

using namespace std;

class AttributeStructure
{
private:
  string name;
  map<string, /*this required*/ bool> elements;
  map</*element*/ string, string> datatypes;

public:
  AttributeStructure ();
  AttributeStructure (const string &);
  virtual ~AttributeStructure ();

  void addElement (const string &, bool);
  void addRegex (const string &, const string &);

  string getName () const;
  map<string, bool> getElements () const; // TODO: return const reference?
  string getDatatype (const string &element) const;
};

#endif /* ATTRIBUTESTRUCTURE_H_ */

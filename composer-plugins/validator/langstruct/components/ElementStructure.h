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

#ifndef ELEMENT_H_
#define ELEMENT_H_

#include <string>
#include <vector>
#include <map>


using namespace std;

class ElementStructure
{
private:
  string name;
  map<string, /*required*/bool> atts;
  map</*parent.name*/string, /*this cardinality*/char> parents;
  bool defineScope;

public:
  ElementStructure();
  ElementStructure(string, bool);

  void addAtt(string _name, bool _required);
  void addParent(string _parent, char _cardinality);

  string getName() const;
  map<string, char> getParents () const;
  char getCardinality(string _parent);
  map<string, bool> getAtts() const;

  bool isScopeDefined () const;

  virtual ~ElementStructure();
};



#endif /* ELEMENT_H_ */

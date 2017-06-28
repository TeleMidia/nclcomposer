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

#ifndef DEFINITIONS_H_
#define DEFINITIONS_H_

#include <iostream>

using namespace std;

/**
 * Utilizado internamente pelo modelo e pelo validador
 */
typedef string virtualId;

/**
 * Definição de um Attribute
 */
class Attribute
{
public:
  inline explicit Attribute (){};
  inline explicit Attribute (string name, string value)
  {
    _name = name;
    _value = value;
  }

  inline string
  name () const
  {
    return _name;
  }
  inline string
  value () const
  {
    return _value;
  }

  inline void
  setName (string name)
  {
    _name = name;
  }
  inline void
  setValue (string value)
  {
    _value = value;
  }

private:
  string _name;
  string _value;
};

#endif /* DEFINITIONS_H_ */

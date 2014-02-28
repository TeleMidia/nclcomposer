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

#ifndef REFERENCESTRUCTURE_H_
#define REFERENCESTRUCTURE_H_

#include <string>

using namespace std;

class ReferenceStructure
{
private:
  string from;
  string fromAtt;
  string to;
  string toAtt;
  string perspective;
  string perspectiveAtt;

public:
  ReferenceStructure(string, string, string, string, string, string);
  virtual ~ReferenceStructure();

  string getFrom() const;
  string getFromAtt() const;
  string getTo() const;
  string getToAtt() const;
  string getPerspective() const;
  string getPerspectiveAtt() const;
};


#endif /* REFERENCESTRUCTURE_H_ */

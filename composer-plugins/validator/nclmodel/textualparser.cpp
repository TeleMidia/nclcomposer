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

#include "textualparser.h"

bool TextualParser::startElement(const QString &, const QString &,
                                 const QString & qName,
                                 const QXmlAttributes & atts )
{
  bool flag = false;

  vector <Attribute> att;
  for (int i = 0; i < atts.count(); i++)
  {
    string name (atts.qName(i).toStdString());
    string value(atts.value(i).toStdString());

    att.push_back(Attribute (name, value));

    if (name == "id" && value == _idToFind)
    {
      flag = true;
    }

  }

  if (flag)
  {
    _element->setAttributes(att);
    _element->setElementName(qName.toStdString());
  }
  return true;
}


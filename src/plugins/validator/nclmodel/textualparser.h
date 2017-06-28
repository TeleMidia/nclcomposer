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

#ifndef TEXTUALPARSER_H
#define TEXTUALPARSER_H

#include "nclmodel.h"
#include <QString>
#include <QXmlDefaultHandler>
#include <iostream>
#include <string>
#include <vector>

class ModelElement;

using namespace std;

class TextualParser : public QXmlDefaultHandler
{
public:
  TextualParser (ModelElement *element, string toFind)
  {
    _element = element;
    _idToFind = toFind;
  }

  bool startElement (const QString &, const QString &, const QString &,
                     const QXmlAttributes &);

private:
  ModelElement *_element;
  string _idToFind;
};

#endif // TEXTUALPARSER_H

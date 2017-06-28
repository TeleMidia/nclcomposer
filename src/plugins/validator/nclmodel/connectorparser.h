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

#ifndef CONNECTORPARSER_H
#define CONNECTORPARSER_H

#include "nclmodel.h"
#include <QXmlDefaultHandler>

class ModelElement;
class Model;

using namespace std;

class ConnectorParser : public QXmlDefaultHandler
{
public:
  ConnectorParser (Model *, string);
  bool startElement (const QString &namespaceURI, const QString &localName,
                     const QString &qName, const QXmlAttributes &atts);
  bool endElement (const QString &namespaceURI, const QString &localName,
                   const QString &qName);

  map<string, pair<int, int> >
  getRolesMap () const
  {
    return _roles;
  }

private:
  Model *_model;
  string _connectorId;
  map<string, pair<int, int> > _roles;
  bool flag;
  bool completed;
};

#endif // CONNECTORPARSER_H

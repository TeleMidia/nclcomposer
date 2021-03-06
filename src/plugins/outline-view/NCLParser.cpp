/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#include "NCLParser.h"

NCLParser::NCLParser (NCLTreeWidget *tree)
{
  _treeWidget = tree;
  _currentItem = 0;
}

bool
NCLParser::startElement (const QString & /* namespaceURI */,
                         const QString & /*localName */, const QString &qName,
                         const QXmlAttributes &attributes)
{
  QMap<QString, QString> attrs;
  for (int i = 0; i < attributes.length (); i++)
  {
    attrs[attributes.localName (i)] = attributes.value (i);
  }

  if (_currentItem)
  {
    _currentItem = _treeWidget->addElement (
        _currentItem, _currentItem->childCount (), qName,
        attributes.value ("id"), attrs, _locator->lineNumber (),
        _locator->columnNumber ());
  }
  else
  {
    _currentItem = _treeWidget->addElement (
        0, -1, qName, attributes.value ("id"), attrs, _locator->lineNumber (),
        _locator->columnNumber ());
  }
  // TODO: send a signal to all interested plugins.
  return true;
}

bool
NCLParser::characters (const QString &str)
{
  _currentText += str;
  return true;
}

bool
NCLParser::endElement (const QString & /* namespaceURI */,
                       const QString & /* localName */,
                       const QString & /* qName */)
{
  _currentItem = _currentItem->parent ();
  return true;
}

bool
NCLParser::fatalError (const QXmlParseException &exception)
{
  if (_currentItem != nullptr)
  {
    _currentItem->setIcon (0, QIcon (":/images/error-icon-16.png"));
    _currentItem->setTextColor (0, QColor ("#FF0000"));
    //      currentItem->setBackgroundColor(0, QColor("#FF0000"));
  }

  qDebug () << QObject::tr ("Parse error at line %1, column "
                            "%2:\n%3.")
                   .arg (exception.lineNumber ())
                   .arg (exception.columnNumber ())
                   .arg (exception.message ());

  emit fatalErrorFound (exception.message (), QString ("FILENAME"),
                        exception.lineNumber (), exception.columnNumber (), 0);

  return false;
}

void
NCLParser::setDocumentLocator (QXmlLocator *locator)
{
  this->_locator = locator;
}

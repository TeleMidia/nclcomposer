/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#include "modules/ProjectReader.h"

CPR_CORE_BEGIN_NAMESPACE

ProjectReader::ProjectReader () {}

ProjectReader::~ProjectReader () {}

Project *
ProjectReader::readFile (const QString &location)
{
  QFile file (location);
  bool error = false;
  _project = nullptr;

  if (!file.open (QIODevice::ReadOnly))
  {
    qCDebug (CPR_CORE) << "ERROR: Could not open the file " << location;
    error = true;
  }

  QByteArray data = file.readAll ();
  QString content (qUncompress (data));
  //  QString content (data);
  /* READING MODEL */
  QString startCpModelStr = "#COMPOSER_MODEL#\n";
  QString endCpModelStr = "\n#END_COMPOSER_MODEL#";

  int startCpModel
      = content.indexOf (startCpModelStr) + startCpModelStr.size ();
  int endCpModel = content.indexOf (endCpModelStr);

  _domDocument = QDomDocument ("cprDoc");
  _project = new Project (_domDocument);
  _project->setLocation (location);

  if (content != "")
  {
    QString modelStr = content.mid (startCpModel, endCpModel - startCpModel);
    parseModelString (modelStr);
  }
  /* FINISH READING MODEL */

  /* READING PLUGIN DATA */
  QString pluginsData = content.mid (endCpModel + endCpModelStr.size ());
  QString startPluginDataStr = "#COMPOSER_PLUGIN_DATA ";
  QString endPluginDataStr = "\n#END_COMPOSER_PLUGIN_DATA#";

  int pos = 0;
  while (pos >= 0 && pos < pluginsData.size ())
  {
    int startPluginData = pluginsData.indexOf (startPluginDataStr, pos);
    int endStartPluginData = pluginsData.indexOf (
        "#\n", startPluginData + startPluginDataStr.size ());

    QString pluginID = pluginsData.mid (
        startPluginData + startPluginDataStr.size (),
        endStartPluginData - (startPluginData + startPluginDataStr.size ()));

    int endPluginData
        = pluginsData.indexOf (endPluginDataStr, startPluginData);

    // +2 and -2 because of the previously #\n
    QString data = pluginsData.mid (endStartPluginData + 2,
                                    endPluginData - endStartPluginData - 2);

    pos = endPluginData + endPluginDataStr.size () + 1;

    _project->setPluginData (pluginID, data.toLatin1 ());
  }
  /* FINISH READING MODEL */

  if (error)
    qCDebug (CPR_CORE) << "ERROR: File is corrupted " << location;

  file.close ();

  return _project;
}

bool
ProjectReader::parseModelString (const QString &str)
{
  QXmlInputSource inputSource;
  inputSource.setData (str);

  QXmlSimpleReader reader;
  reader.setContentHandler (this);
  reader.setErrorHandler (this);

  return reader.parse (inputSource);
}

bool
ProjectReader::startElement (const QString &namespaceURI,
                             const QString &localName, const QString &qName,
                             const QXmlAttributes &attributes)
{
  Q_UNUSED (namespaceURI);
  Q_UNUSED (localName);

  QMap<QString, QString> atts;
  QString uniqueId = "";

  Node *parentNode = nullptr;
  if (qName != "document")
  {
    _lockStack.lock ();
    parentNode = _elementStack.top ();
    _lockStack.unlock ();
  }

  for (int i = 0; i < attributes.count (); i++)
  {
    if (attributes.qName (i) != "uniqueEntityId")
      atts[attributes.qName (i)] = attributes.value (i);
    else
      uniqueId = attributes.value (i);
  }

  Node *node = nullptr;
  if (qName != "document" && parentNode != nullptr)
  {
    if (uniqueId == "")
      qCDebug (CPR_CORE) << "trying to add an entity whithout an uniqueId";
    else if (qName == "cpr_comment")
    {
      Comment *comment = new Comment (uniqueId, atts["content"],
                                      _project->getDomDocument (), _project);
      _project->addComment (comment, parentNode->uid ());
      node = comment;
    }
    else
    {
      Entity *entity = new Entity (uniqueId, qName, atts,
                                   _project->getDomDocument (), _project);
      _project->addEntity (entity, parentNode->uid ());
      node = entity;
    }
  }
  else
    node = _project;

  _lockStack.lock ();
  _elementStack.push (node);
  _lockStack.unlock ();

  return true;
}

bool
ProjectReader::endElement (const QString &namespaceURI,
                           const QString &localName, const QString &qName)
{
  Q_UNUSED (namespaceURI)
  Q_UNUSED (localName)
  Q_UNUSED (qName)

  _lockStack.lock ();
  if (_elementStack.size () != 0)
    _elementStack.pop ();
  _lockStack.unlock ();

  return true;
}

bool
ProjectReader::characters (const QString &str)
{
  Q_UNUSED (str)

  return true;
}

bool
ProjectReader::fatalError (const QXmlParseException &exception)
{
  qCDebug (CPR_CORE) << "Fatal error on line" << exception.lineNumber ()
                     << ", column" << exception.columnNumber () << ":"
                     << exception.message ();

  return false;
}

CPR_CORE_END_NAMESPACE

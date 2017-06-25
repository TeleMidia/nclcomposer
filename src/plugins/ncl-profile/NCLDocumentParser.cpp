/* Copyright (c) 2011 Telemidia/PUC-Rio.
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the Eclipse Public License v1.0
* which accompanies this distribution, and is available at
* http://www.eclipse.org/legal/epl-v10.html
*
* Contributors:
*    Telemidia/PUC-Rio - initial API and implementation
*/
#include "NCLDocumentParser.h"

CPR_NCLPROFILE_BEGIN_NAMESPACE

NCLDocumentParser::NCLDocumentParser(Project *project)
{
  this->_project = project;
}

NCLDocumentParser::~NCLDocumentParser()
{

}

QString NCLDocumentParser::getParserName()
{
  return "NCLDocumentParser";
}

bool NCLDocumentParser::parseDocument()
{
  QString uri = _project->getLocation();

  QFile *file = new QFile(uri, this);
  if (!file->open(QIODevice::ReadWrite))
  {
    qDebug() << "DocumentParser::parseDocument"
             << tr("Could not open file %1\n").arg(uri);
    return false;
  }

  file->close();

  QXmlInputSource inputSource(file);
  QXmlSimpleReader reader;
  reader.setContentHandler(this);
  reader.setErrorHandler(this);

  return reader.parse(inputSource);
}

bool NCLDocumentParser::parseContent(const QString &str)
{
  QXmlInputSource inputSource;
  inputSource.setData(str);

  QXmlSimpleReader reader;
  reader.setContentHandler(this);
  reader.setErrorHandler(this);

  return reader.parse(inputSource);
}

bool NCLDocumentParser::startElement(const QString &,
                                     const QString &,
                                     const QString &qName,
                                     const QXmlAttributes &attributes)
{
  QMap<QString,QString> atts;
  QString parentId = _project->getUniqueId();

  if (qName != "ncl")
  {
    if(_elementStack.size())
    {
      _lockStack.lock();
      Entity *parentEntity = _elementStack.top();
      _lockStack.unlock();
      parentId = parentEntity->getUniqueId();
    }
  }

  for (int i=0 ;i < attributes.count(); i++)
    atts[attributes.qName(i)] = attributes.value(i);

  emit addEntity(qName, parentId, atts);

  return true;
}

bool NCLDocumentParser::endElement(const QString &namespaceURI,
                                   const QString &localName,
                                   const QString &qName)
{
  Q_UNUSED(namespaceURI)
  Q_UNUSED(localName)
  Q_UNUSED(qName)

  _lockStack.lock();
  if(_elementStack.size())
    _elementStack.pop();
  _lockStack.unlock();

  return true;
}

bool NCLDocumentParser::characters(const QString &)
{
  return true;
}

bool NCLDocumentParser::endDocument()
{
  emit parseFinished();
  return true;
}

bool NCLDocumentParser::fatalError(const QXmlParseException &exception)
{
  qDebug() << "Fatal error on line" << exception.lineNumber()
           << ", column" << exception.columnNumber() << ":"
           << exception.message();

  return false;
}

void NCLDocumentParser::onEntityAdded(const QString&, Entity *entity)
{
  _lockStack.lock();
  _elementStack.push(entity);
  _lockStack.unlock();
}

void NCLDocumentParser::onEntityAddError(const QString &error)
{
  qDebug() << "NCLDocumentParser::onEntityAddError(" << error << ")";
}

CPR_NCLPROFILE_END_NAMESPACE

/*
 * Copyright 2011 TeleMidia/PUC-Rio.
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
#include "NCLTextualViewPlugin.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QMessageBox>

#include <QApplication>
#include <QProgressDialog>
#include <QDomDocument>
#include <QTextStream>

namespace composer {
namespace plugin {
namespace textual {

NCLTextualViewPlugin::NCLTextualViewPlugin()
{
  window = new NCLTextEditorMainWindow();
  nclTextEditor = window->getTextEditor();
  tmpNclTextEditor = NULL;

  project = NULL;
  connect( window,
          SIGNAL(elementAdded(QString,QString,QMap<QString,QString>&,bool)),
          this,
          SIGNAL(addEntity(QString,QString,QMap<QString,QString>&,bool)));

  isSyncing = false;

  updateModelShortcut = new QShortcut(window);
  updateModelShortcut->setKey(QKeySequence("F5"));

  connect(updateModelShortcut, SIGNAL(activated()),
          this, SLOT(updateCoreModel()) );

  connect(nclTextEditor, SIGNAL(focusLosted(QFocusEvent*)),
          this, SLOT(manageFocusLost(QFocusEvent*)));

  connect(nclTextEditor, SIGNAL(textChanged()),
          this, SIGNAL(setCurrentProjectAsDirty()));
}

NCLTextualViewPlugin::~NCLTextualViewPlugin()
{
  delete window;
  window = NULL;
}

void NCLTextualViewPlugin::init()
{
  QString data = project
      ->getPluginData("br.puc-rio.telemidia.NCLTextualView");

  QString startEntitiesSep = "$START_ENTITIES_LINES$";
  QString endEntitiesSep = "$END_ENTITIES_LINES$";
  int indexOfStartEntities = data.indexOf(startEntitiesSep);
  int indexOfEndEntities = data.indexOf(endEntitiesSep);

  // Just for safety clearing start and end line previous saved.
  QString key;
  foreach(key, startEntityOffset.keys())
    startEntityOffset.remove(key);
  foreach(key, endEntityOffset.keys())
    endEntityOffset.remove(key);

  QString text = data.left(indexOfStartEntities);
  nclTextEditor->insert(text);

  int indexOfStartEntitiesContent = indexOfStartEntities +
      startEntitiesSep.length();
  QString startLines = data.mid(indexOfStartEntitiesContent,
                                indexOfEndEntities - indexOfStartEntitiesContent);

  QString endLines = data.right(data.length() -
                                (indexOfEndEntities+endEntitiesSep.length()));

  QStringList listStart = startLines.split(",");
  QStringList listEnd = endLines.split(",");

  for(int i = 0; i < listStart.size()-1; i +=2)
  {
    startEntityOffset[listStart[i]] = listStart[i+1].toInt();
    endEntityOffset[listEnd[i]] = listEnd[i+1].toInt();
  }

  nclTextEditor->setDocumentUrl(project->getLocation());
}

QWidget* NCLTextualViewPlugin::getWidget()
{
  return window;
}

void NCLTextualViewPlugin::updateFromModel()
{
  incrementalUpdateFromModel();
}

void NCLTextualViewPlugin::incrementalUpdateFromModel()
{
  nclTextEditor->clear();
  if(project->getChildren().size())
  {
    Entity *entity = project;
    QList <Entity *> entities;
    entities.push_back(entity);
    bool first = true;
    while(entities.size())
    {
      entity = entities.front();
      entities.pop_front();

      if(!first) //ignore the project root
        onEntityAdded("xxx", entity);
      else
        first = false;
      QVector<Entity *> children = entity->getChildren();
      for(int i = 0; i < children.size(); i++)
      {
        entities.push_back(children.at(i));
      }
    }
  }
}

void NCLTextualViewPlugin::nonIncrementalUpdateFromModel()
{
  nclTextEditor->clear();
  QDomDocument doc ("document");
  if(project->getChildren().size())
  {
    Entity *entity = project;
    QList <Entity *> entities;
    QList <QDomNode> elements;
    QDomNode current;
    entities.push_back(entity);
    elements.push_back(doc);

    bool first = true;
    while(entities.size())
    {
      entity = entities.front();
      entities.pop_front();
      current = elements.front();
      elements.pop_front();

      if(!first) //ignore the project root
      {
//      current->appendChild(el);
//      onEntityAdded("xxx", entity);
      }
      else
      {
        first = false;
        elements.push_back(doc);
      }

      QVector<Entity *> children = entity->getChildren();
      for(int i = 0; i < children.size(); i++)
      {
        entities.push_back(children.at(i));
        QDomElement el = doc.createElement(children[i]->getType());
        el.setAttribute("oi", "oi2");
        el.setAttribute("oi2", "oi3");
        el.setAttribute("oi3", "oi4");
        elements.push_back(el);
        current.appendChild(el);
      }
    }

    QString *text = new QString();
    QTextStream textStream(text);
    doc.save(textStream, QDomNode::EncodingFromTextStream);
    nclTextEditor->setText(textStream.readAll());
  }
}

void NCLTextualViewPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
  //Return if this is my call to onEntityAdded
  // qDebug() << "isSyncing=" << isSyncing;
  if(pluginID == getPluginInstanceID() && !isSyncing)
    return;

  QString line = "<" + entity->getType() + "";
  int insertAtOffset = 0;

  //get the line number where the new element must be inserted
  if(entity->getParentUniqueId() != NULL) {
    // Test if exists before access from operator[] becaus if doesn't exist
    // this operator will create a new (and we don't want this!).
    if(endEntityOffset.count(entity->getParentUniqueId()))
    {
      insertAtOffset = endEntityOffset[entity->getParentUniqueId()];
    }
  }

  QMap <QString, QString>::iterator begin, end, it;
  entity->getAttributeIterator(begin, end);
  for (it = begin; it != end; ++it)
  {
    line += " " + it.key() + "=\"" + it.value() + "\"";
  }
  line += ">\n";
  int startEntitySize = line.size();
  line += "</" + entity->getType() + ">\n";

  nclTextEditor->insertAtPos(line, insertAtOffset);

  //update all previous offset numbers (when necessary)
  updateEntitiesOffset(insertAtOffset, line.size());

  startEntityOffset[entity->getUniqueId()] = insertAtOffset;
  endEntityOffset[entity->getUniqueId()] = insertAtOffset + startEntitySize;

  // qDebug() << "*** Begin ***";
  // printEntitiesOffset();

  /* Fix Indentation */
  int insertAtLine = nclTextEditor->SendScintilla(
        QsciScintilla::SCI_LINEFROMPOSITION,
        insertAtOffset);

  // get the indentation for the next line
  int lineIndent = nclTextEditor
      ->SendScintilla( QsciScintilla::SCI_GETLINEINDENTATION,
                      insertAtLine+2);

  if(insertAtLine) lineIndent += 8;

  // qDebug() << "Line: " << insertAtLine;
  // qDebug() << "Line ident: " << lineIndent;

  nclTextEditor->SendScintilla( QsciScintilla::SCI_SETLINEINDENTATION,
                               insertAtLine,
                               lineIndent);
  nclTextEditor->SendScintilla( QsciScintilla::SCI_SETLINEINDENTATION,
                               insertAtLine+1,
                               lineIndent);

  int previousStarted = startEntityOffset[entity->getUniqueId()]-1;
  updateEntitiesOffset(previousStarted, lineIndent/8);
  // qDebug() << "*** First pass ***";
  // printEntitiesOffset();

  previousStarted = endEntityOffset[entity->getUniqueId()];
  // qDebug() << "*** Second pass ***";
  updateEntitiesOffset(previousStarted+1, lineIndent/8);
  //    printEntitiesOffset();

  window->getTextEditor()->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);
  /*
    window->getTextEditor()->setCursorPosition(insertAtLine, 0);
    window->getTextEditor()->ensureLineVisible(insertAtLine);
  */

//  emit TextualPluginHasAddedEntity(pluginID, entity);

  /* qDebug() << "NCLTextualViewPlugin::onEntityAdded" <<
       entity->getType() << " " << insertAtLine; */
}

void NCLTextualViewPlugin::errorMessage(QString error)
{
  //qDebug() << "NCLTextualViewPlugin::onEntityAddError(" << error << ")";
}

void NCLTextualViewPlugin::onEntityChanged(QString pluginID, Entity *entity)
{
  qDebug() << "PLUGIN (" + pluginID + ") changed the Entity (" +
              entity->getType() + " - " + entity->getUniqueId() +")";

  //Return if this is my call to onEntityAdded
  if(pluginID == getPluginInstanceID() && !isSyncing)
    return;

  QString line = "<" + entity->getType() + "";

  int insertAtOffset = startEntityOffset.value(entity->getUniqueId());

  QMap <QString, QString>::iterator begin, end, it;
  entity->getAttributeIterator(begin, end);
  for (it = begin; it != end; ++it)
  {
    if(it.value() != "")
      line += " " + it.key() + "=\"" + it.value() + "\"";
  }
  line += ">";

  int previous_length = 0;
  char curChar = nclTextEditor->SendScintilla(QsciScintilla::SCI_GETCHARAT,
                                              insertAtOffset+previous_length);
  while(curChar != '>' &&
        (insertAtOffset+previous_length) < nclTextEditor->text().size())
  {
    previous_length++;
    curChar = nclTextEditor->SendScintilla(QsciScintilla::SCI_GETCHARAT,
                                           insertAtOffset+previous_length);
  }

  if((insertAtOffset+previous_length)
      == nclTextEditor->text().size())
  {
    qWarning() << "TextEditor could not perform the requested action.";
    return;
  }
  previous_length+=1;

  // qDebug() << previous_length;
  // store the current identation (this must keep equal even with the
  // modifications)
  /* int lineident = window->getTextEditor()
                    ->SendScintilla( QsciScintilla::SCI_GETLINEINDENTATION,
                                     insertAtLine);*/

  nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONSTART,
                               insertAtOffset);
  nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONEND,
                               insertAtOffset+previous_length);
  nclTextEditor->removeSelectedText();

  nclTextEditor->insertAtPos(line, insertAtOffset);

  //update all previous entities line numbers (when necessary)
  int diff_size = line.size() - previous_length;
  updateEntitiesOffset(insertAtOffset, diff_size);

  nclTextEditor->SendScintilla( QsciScintilla::SCI_GOTOPOS, insertAtOffset);
  //TODO: fix indentation
}

void NCLTextualViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
  // skip if this is my own call to onEntityRemoved
  if(pluginID == getPluginInstanceID() && !isSyncing)
    return;

  int startOffset = startEntityOffset[entityID];
  int endOffset = endEntityOffset[entityID];

  char curChar = nclTextEditor->SendScintilla(
        QsciScintilla::SCI_GETCHARAT,
        startOffset);

  while(curChar != '>' && startOffset >= 0)
  {
    startOffset--;
    curChar = nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                           startOffset);
  }
  if(curChar == '>')
    startOffset++; // does not include the '>' character

  curChar = nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                         endOffset);

  while(curChar != '>' && endOffset < nclTextEditor->text().size())
  {
    endOffset++;
    curChar = nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                           endOffset);
  }
  if(endOffset == nclTextEditor->text().size())
  {
    qWarning() << "TextEditor could not perform the requested action.";
    return;
  }

  endOffset++; // includes the '>' character

  while(isspace(curChar) && endOffset < nclTextEditor->text().size())
  {
    endOffset++;
    curChar = nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                           endOffset);
  }

  nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONSTART,
                               startOffset);
  nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONEND,
                               endOffset);
  nclTextEditor->removeSelectedText();

  QString key;
  QList<QString> mustRemoveEntity;

  // check all entities that is removed together with entityID, i.e.
  // its children
  // P.S. This could be get from model
  foreach(key, startEntityOffset.keys())
  {
    // if the element is inside the entity that will be removed:
    if(startEntityOffset[key] >= startOffset &&
        endEntityOffset[key]+2 <= endOffset)
    {
      mustRemoveEntity.append(key);
    }
    else
    {
      // otherwise if necessary we must update the start and end line of
      // the entity.
      if(startEntityOffset[key] >= startOffset)
      {
        startEntityOffset[key] -= (endOffset - startOffset);
      }

      if(endEntityOffset[key] >= endOffset)
      {
        endEntityOffset[key] -= (endOffset - startOffset);
      }
    }
  }

  //Remove the content in text and update the structures that keep line number
  // of all entities.
  QListIterator<QString> iterator( mustRemoveEntity );
  while( iterator.hasNext() ){
    key = iterator.next();
    startEntityOffset.remove(key);
    endEntityOffset.remove(key);
  }

  /*foreach(key, startEntityOffset.keys())
    {
        qDebug() << " startOffset=" << startEntityOffset[key]
             << " endOffset=" << endEntityOffset[key];
    }*/
}

bool NCLTextualViewPlugin::saveSubsession()
{
  QByteArray data;
  data.append(nclTextEditor->text());
  data.append("$START_ENTITIES_LINES$");
  QString key;
  bool first = true;
  foreach (key, startEntityOffset.keys())
  {
    if(!first)
      data.append(",");
    else
      first = false;
    data.append(key + ", " + QString::number(startEntityOffset[key]));
  }
  data.append("$END_ENTITIES_LINES$");
  first = true;
  foreach (key, endEntityOffset.keys())
  {
    if(!first)
      data.append(",");
    else
      first = false;
    data.append(key + "," + QString::number(endEntityOffset[key]));
  }

  emit setPluginData(data);

  return true;
}

void NCLTextualViewPlugin::changeSelectedEntity(QString pluginID, void *param)
{
  QString *id = (QString*)param;
  if(startEntityOffset.contains(*id))
  {
    int entityOffset = startEntityOffset.value(*id);
    //  int entityLine = window->getTextEditor()->SendScintilla(
    //                                  QsciScintilla::SCI_LINEFROMPOSITION,
    //                                  entityOffset);

    nclTextEditor->SendScintilla(QsciScintilla::SCI_GOTOPOS,
                                 entityOffset);
    nclTextEditor->SendScintilla(QsciScintilla::SCI_SETFOCUS,
                                 true);
  }
  else
  {
    qDebug() << "NCLTextualViewPlugin::changeSelectedEntity() "
               << "Entity doesn't exists!";
  }
}

void NCLTextualViewPlugin::updateCoreModel()
{
  syncMutex.lock();
  bool rebuildComposerModelFromScratch = false;

  QString text = nclTextEditor->text();
  //Create a DOM document with the new content
  if(!xmlDoc.setContent(text))
  {
    //if the current XML is not well formed.
    QMessageBox::information(NULL, tr("Error"),
                             tr("Your document is not a Well-formed XML"));
    nclTextEditor->keepFocused();
    return;
  }

  isSyncing = true; //set our current state as syncing

  //double-buffering
  tmpNclTextEditor = nclTextEditor;
  nclTextEditor = new NCLTextEditor(0);
  nclTextEditor->setDocumentUrl(project->getLocation());
  nclTextEditor->setText(tmpNclTextEditor->textWithoutUserInteraction());

  if(rebuildComposerModelFromScratch)
    nonIncrementalUpdateCoreModel();
  else
    incrementalUpdateCoreModel();

  emit syncFinished();

  syncMutex.unlock();
}

void NCLTextualViewPlugin::nonIncrementalUpdateCoreModel()
{
  //delete the content of the current project
  if(project->getChildren().size())
    emit removeEntity(project->getChildren().at(0), true);

  // clear the entities offset
  nclTextEditor->clear();
  startEntityOffset.clear();
  endEntityOffset.clear();

  QList <QString> parentUids;
  QString parentUId = project->getUniqueId();
  parentUids.push_back(parentUId);

  QList <QDomElement> nodes;
  QDomElement current = xmlDoc.firstChildElement();
  nodes.push_back(current);

  while(!nodes.empty())
  {
    current = nodes.front();
    nodes.pop_front();
    parentUId = parentUids.front();
    parentUids.pop_front();

    //Process the node
    QMap<QString,QString> atts;

    QDomNamedNodeMap attributes = current.attributes();
    for (int i = 0; i < attributes.length(); i++)
    {
      QDomAttr item = attributes.item(i).toAttr();
      atts[item.name()] = item.value();
    }

    //Send the addEntity to the core plugin
    emit addEntity(current.tagName(), parentUId, atts, false);
    parentUId = currentEntity->getUniqueId();

    QDomElement child = current.firstChildElement();
    while(!child.isNull())
    {
      nodes.push_back(child);
      parentUids.push_back(parentUId);
      child = child.nextSiblingElement();
    }
  }
}

void NCLTextualViewPlugin::incrementalUpdateCoreModel()
{
  QProgressDialog dialog(tr("Synchronizing with other plugins..."),
                         tr("Cancel"), 0, 100,
                         nclTextEditor);
  dialog.setAutoClose(true);
  dialog.show();

  //incremental update
  QList <QDomNode> nodes;
  QDomNode current = xmlDoc;
  nodes.push_back(current);
  Entity *curEntity = project;
  QList <Entity *> entities;
  entities.push_back(curEntity);

  //count how many nodes to update
  int total_nodes = 0, progress = 0;
  while(!nodes.empty())
  {
    current = nodes.front();
    nodes.pop_front();

    total_nodes++;

    QDomElement child = current.firstChildElement();
    while(!child.isNull())
    {
      nodes.push_back(child);
      child = child.nextSiblingElement();
    }
  }

  dialog.setRange(0, total_nodes);
  current = xmlDoc;
  nodes.push_back(current);
  while(!nodes.empty())
  {
    dialog.setValue(progress++);
    dialog.update();
    QApplication::processEvents();

    current = nodes.front();
    nodes.pop_front();
    curEntity = entities.front();
    entities.pop_front();

    QVector <QDomElement> children;
    QDomElement child = current.firstChildElement();
    while(!child.isNull())
    {
      children.push_back(child);
      child = child.nextSiblingElement();
    }

    QVector <Entity *> entityChildren = curEntity->getChildren();

    int i, j;
    for(i = 0, j = 0;
        i < children.size() && j < entityChildren.size();
        i++, j++)
    {
      if(children[i].tagName() == entityChildren[j]->getType())
      {
        // if the same type, just update the attributes
        //TODO: Compare attributes
        QMap<QString, QString> atts;
        QDomNamedNodeMap attributes = children[i].attributes();
        for (int k = 0; k < attributes.length(); k++)
        {
          QDomNode item = attributes.item(k);
          qDebug() << item.nodeName() << item.nodeValue();
          atts.insert(item.nodeName(), item.nodeValue());
        }

        emit setAttributes(entityChildren[j], atts, false);
      }
      else
      {
        //if type are differents, then we should change the type
        //i.e. remove the entity and insert a new entity with the
        //required type.
        emit removeEntity(entityChildren[j], true);
        //add new entity
        QMap<QString,QString> atts;
        QDomNamedNodeMap attributes = children[i].attributes();
        for (int k = 0; k < attributes.length(); k++)
        {
          QDomNode item = attributes.item(k);
          atts[item.nodeName()] = item.nodeValue();
        }
        emit addEntity(children[i].tagName(), curEntity->getUniqueId(), atts,
                       false);
      }
    }

    if(i == children.size())
    {
      // if there are more entity in the composer model than in the XML
      for(; j < entityChildren.size(); j++)
        emit removeEntity(entityChildren[j], true);
    }
    else if(j == entityChildren.size())
    {
      // if there are more entity in the XML than in the composer model
      for(; i < children.size(); i++)
      {
        //add new entity
        QMap<QString,QString> atts;
        QDomNamedNodeMap attributes = children[i].attributes();
        for (int k = 0; k < attributes.length(); k++)
        {
          QDomNode item = attributes.item(k);
          atts[item.nodeName()] = item.nodeValue();
        }
        emit addEntity(children[i].tagName(), curEntity->getUniqueId(), atts,
                       false);
      }
    }

    child = current.firstChildElement();
    while(!child.isNull())
    {
      nodes.push_back(child);
      child = child.nextSiblingElement();
    }
    entityChildren = curEntity->getChildren();
    for(int i = 0; i < entityChildren.size(); i++)
      entities.push_back(entityChildren[i]);
  }

  dialog.setValue(100);
}

void NCLTextualViewPlugin::syncFinished()
{
  // tmpNclTextEditor->setText(nclTextEditor->text());
  delete nclTextEditor;
  nclTextEditor = tmpNclTextEditor;
  tmpNclTextEditor = NULL;
  updateFromModel();
  isSyncing = false;

  updateErrorMessages();
}

void NCLTextualViewPlugin::updateEntitiesOffset( int startFrom,
                                                int insertedChars)
{
  /* qDebug() << "NCLTextualViewPlugin::updateEntitiesOffset(" << startFrom
             << ", " << insertedChars << ")"; */

  if(!insertedChars) //nothing to do
    return;

  QString key;
  foreach(key, startEntityOffset.keys())
  {
    if(startEntityOffset[key] > startFrom)
      startEntityOffset[key] += insertedChars;

    if(endEntityOffset[key] >= startFrom)
      endEntityOffset[key] += insertedChars;
  }
}

void NCLTextualViewPlugin::printEntitiesOffset()
{
  QString key;
  foreach(key, startEntityOffset.keys())
  {
    qDebug() << "key="<< key << "; start=" << startEntityOffset[key]
             << "; end=" << endEntityOffset[key];
  }
}

void NCLTextualViewPlugin::manageFocusLost(QFocusEvent *event)
{
#ifndef NCLEDITOR_STANDALONE

  // When AutoComplete list gets the focus, the QApplication::focusWidget
  // has a NULL value. This is an QScintilla issues.
  // When the focus goes to AutoComplete list we don't want to synchronize with
  // the core, that is why the test "QApplication::focusWidget() != NULL" is
  // here.
  // qDebug() << nclTextEditor << QApplication::focusWidget();
  if(nclTextEditor->textWithoutUserInteraction() != nclTextEditor->text()
     && !isSyncing
     && (QApplication::focusWidget() != NULL))
  {
    int ret = QMessageBox::question(window,
                                    tr("Textual View synchronization"),
                                    tr("You have changed the textual content of the NCL \
                                       Document. Do you want to synchronize this text with \
                                       other views?"),
                                                    QMessageBox::Yes |
                                                    QMessageBox::No |
                                                    QMessageBox::Cancel,

                                       QMessageBox::Cancel);

        switch(ret)
    {
      case QMessageBox::Yes:
        updateCoreModel();
        break;
      case QMessageBox::No:
        nclTextEditor->setText(nclTextEditor->textWithoutUserInteraction());
        break;
      case QMessageBox::Cancel:
        nclTextEditor->keepFocused();
        break;
    }
  }
  else if(QApplication::focusWidget() == NULL)
  {
    // If the focus goes to AutoComplete list we force Qt keeps the focus in the
    // NCLTextEditor!!!
    nclTextEditor->keepFocused();
  }
#endif
}

void NCLTextualViewPlugin::updateErrorMessages()
{
  clearValidationMessages(this->pluginInstanceID, NULL);

  emit sendBroadcastMessage("askAllValidationMessages", NULL);
}

void NCLTextualViewPlugin::clearValidationMessages(QString, void *param)
{
  nclTextEditor->clearErrorIndicators();
}

void NCLTextualViewPlugin::validationError(QString pluginID, void * param)
{
  if (param) {
     pair <QString , QString> *p = (pair <QString, QString> *) param;

     int offset = startEntityOffset[p->first];

     int line = nclTextEditor->SendScintilla(
                                     QsciScintilla::SCI_LINEFROMPOSITION,
                                     offset);

     nclTextEditor->markError(p->second, "", line);
  }
}

} } } //end namespace

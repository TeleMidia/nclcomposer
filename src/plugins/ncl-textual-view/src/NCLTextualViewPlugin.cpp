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
#include <deque>
using namespace std;

Q_LOGGING_CATEGORY(CPR_PLUGIN_TEXTUAL, "cpr.plugin.textual")

const QString PROLOG ("<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<!-- Generated by NCL Composer -->\n");

NCLTextualViewPlugin::NCLTextualViewPlugin()
{
  _window = new NCLTextEditorMainWindow();
  _nclTextEditor = _window->getTextEditor();

  _tmpNclTextEditor = nullptr;

  project = nullptr;
  connect( _window,
           SIGNAL(elementAdded(const QString&, const QString&, const QMap<QString,QString>&, bool)),
           this,
           SIGNAL(addEntity(const QString&, const QString&, const QMap<QString,QString>&, bool)));

  _isSyncing = false;

  connect(_nclTextEditor, SIGNAL(focusLosted(QFocusEvent*)),
          this, SLOT(manageFocusLost(QFocusEvent*)));

  connect(_nclTextEditor, SIGNAL(textChanged()),
          this, SIGNAL(setCurrentProjectAsDirty()));
}

NCLTextualViewPlugin::~NCLTextualViewPlugin()
{
  delete _window;
  _window = nullptr;
}

void NCLTextualViewPlugin::init()
{
  QString data = project->getPluginData("br.puc-rio.telemidia.NCLTextualView");

  QString startEntitiesSep = "$START_ENTITIES_LINES$";
  QString endEntitiesSep = "$END_ENTITIES_LINES$";
  int indexOfStartEntities = data.indexOf(startEntitiesSep);
  int indexOfEndEntities = data.indexOf(endEntitiesSep);

  // Just for safety clearing start and end line previous saved.
  QString key;
  foreach(key, _startEntityOffset.keys())
    _startEntityOffset.remove(key);
  foreach(key, _endEntityOffset.keys())
    _endEntityOffset.remove(key);

  QString text = data.left(indexOfStartEntities);
  if(text.isEmpty() || text.isNull())
    _nclTextEditor->setText(PROLOG);
  else
    _nclTextEditor->setText(text);

  int indexOfStartEntitiesContent = indexOfStartEntities +
      startEntitiesSep.length();
  QString startLines = data.mid(indexOfStartEntitiesContent,
                                indexOfEndEntities - indexOfStartEntitiesContent);

  QString endLines = data.right(data.length() -
                                (indexOfEndEntities+endEntitiesSep.length()));

  QStringList listStart = startLines.split(",");
  QStringList listEnd = endLines.split(",");

  int i;

  for(i = 0; i < listStart.size()-1 && i < listEnd.size()-1; i +=2)
  {
    _startEntityOffset[listStart[i]] = listStart[i+1].toInt();
    _endEntityOffset[listEnd[i]] = listEnd[i+1].toInt();
  }

  // qCDebug (CPR_PLUGIN_TEXTUAL) << i << listStart.size() << listEnd.size();
  if( i != listStart.size() || i != listEnd.size())
  {
    qCDebug (CPR_PLUGIN_TEXTUAL) << "The data saved in the file is corrupted. Forcing"
             << " updateFromModel";

    updateFromModel();
  }

  _nclTextEditor->setDocumentUrl(project->getLocation());

  updateErrorMessages();
}

QWidget* NCLTextualViewPlugin::getWidget()
{
  return _window;
}

void NCLTextualViewPlugin::updateFromModel()
{
  //  qCDebug (CPR_PLUGIN_TEXTUAL) << "NCLTextualViewPlugin::updateFromModel";
  incrementalUpdateFromModel();

  updateErrorMessages();
}

void NCLTextualViewPlugin::incrementalUpdateFromModel()
{
  _nclTextEditor->clear();
  _nclTextEditor->setText(PROLOG);
  _startEntityOffset.clear();
  _endEntityOffset.clear();

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
  _nclTextEditor->clear();
  _nclTextEditor->setText(PROLOG);
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
    _nclTextEditor->setText(PROLOG);
    _nclTextEditor->insertAtPos(textStream.readAll(), PROLOG.size());
  }
}

void NCLTextualViewPlugin::onEntityAdded(const QString &pluginID, Entity *entity)
{
  // Return if this is my call to onEntityAdded
  // qCDebug (CPR_PLUGIN_TEXTUAL) << " isSyncing= " << isSyncing;
  if(pluginID == getPluginInstanceID())
  {
    _currentEntity = entity;
    return;
  }

  QString line = "<" + entity->getType() + "";
  int insertAtOffset = PROLOG.size();
  bool hasOpennedTag = false;

  // get the line number where the new element must be inserted
  if(entity->getParentUniqueId() != nullptr &&
     entity->getParent()->getType() != "project")
  {
    // Test if exists before access from operator[] because if doesn't exist
    // this operator will create a new (and we don't want this!).
    if(_endEntityOffset.count(entity->getParentUniqueId()))
    {
      if(isStartEndTag(entity->getParent()))
      {
        openStartEndTag(entity->getParent());
        hasOpennedTag = true;
        //        printEntitiesOffset();
      }

      insertAtOffset = _endEntityOffset[entity->getParentUniqueId()];
    }
  }

  line += getEntityAttributesAsString(entity);
  line += "/>\n";

  int startEntitySize = line.size();

  if(insertAtOffset >= 0 && insertAtOffset <= _nclTextEditor->text().length())
  {
    _nclTextEditor->insertAtPos(line, insertAtOffset);

    //update all previous offset numbers (when necessary)
    updateEntitiesOffset(insertAtOffset, line.size());

    _startEntityOffset[entity->getUniqueId()] = insertAtOffset;
    _endEntityOffset[entity->getUniqueId()] = insertAtOffset + startEntitySize-2;

    _window->getTextEditor()->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);

    if(hasOpennedTag) //Add a new tab to the new inserted element.
      fixIdentation(insertAtOffset, true);
    else              // keep the previous tabulation
      fixIdentation(insertAtOffset, false);

    _currentEntity = entity;
  }
  else
    qCWarning(CPR_PLUGIN_TEXTUAL)
        << "NCLTextEditor::onEntityAdded Trying to insert a media in a "
           "position greater than the text size. It will be ignored!"
        << insertAtOffset;

  //  printEntitiesOffset();
}

void NCLTextualViewPlugin::errorMessage(const QString &error)
{
  Q_UNUSED(error)
  //  qCDebug (CPR_PLUGIN_TEXTUAL) << "NCLTextualViewPlugin::onEntityAddError(" << error << ")";
}

void NCLTextualViewPlugin::onEntityChanged( const QString &pluginID,
                                            Entity *entity)
{
  qCDebug (CPR_PLUGIN_TEXTUAL) << "PLUGIN (" + pluginID + ") changed the Entity (" +
              entity->getType() + " - " + entity->getUniqueId() +")";

  //Return if this is my call to onEntityAdded
  if(pluginID == getPluginInstanceID() && !_isSyncing)
    return;

  QString line = "<" + entity->getType() + "";

  line += getEntityAttributesAsString(entity);

  int insertAtOffset = 0;
  if(_startEntityOffset.contains(entity->getUniqueId()))
  {
    insertAtOffset = _startEntityOffset.value(entity->getUniqueId());
    // endEntityOffset = _endEntityOffset.value(entity->getUniqueId());

    if( insertAtOffset >= 0 &&
        insertAtOffset <= _nclTextEditor->text().size() )
    {
      int previous_length = 0;
      char curChar = _nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                                    insertAtOffset + previous_length );
      while(curChar != '>' &&
            (insertAtOffset+previous_length) < _nclTextEditor->text().size())
      {
        previous_length++;
        curChar = _nclTextEditor->SendScintilla(QsciScintilla::SCI_GETCHARAT,
                                                insertAtOffset+previous_length);
      }

      curChar = _nclTextEditor->SendScintilla(QsciScintilla::SCI_GETCHARAT,
                                            insertAtOffset+previous_length-1);
      if(curChar == '/')
      {
        line += "/>";
      }
      else
        line += ">";

      if((insertAtOffset+previous_length) == _nclTextEditor->text().size())
      {
        qCWarning(CPR_PLUGIN_TEXTUAL)
            << "TextEditor could not perform the requested action.";
        return;
      }

      previous_length += 1;

      // qCDebug (CPR_PLUGIN_TEXTUAL) << previous_length;
      // store the current identation (this must keep equal even with the
      // modifications)
      /* int lineident = window->getTextEditor()
                        ->SendScintilla( QsciScintilla::SCI_GETLINEINDENTATION,
                                       insertAtLine);*/

      _nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONSTART,
                                    insertAtOffset);
      _nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONEND,
                                  insertAtOffset+previous_length);
      _nclTextEditor->removeSelectedText();

      _nclTextEditor->insertAtPos(line, insertAtOffset);

      //update all previous entities line numbers (when necessary)
      int diff_size = line.size() - previous_length;
      updateEntitiesOffset(insertAtOffset, diff_size);

      _nclTextEditor->SendScintilla( QsciScintilla::SCI_GOTOPOS, insertAtOffset);
      //TODO: fix indentation
    }
    else
      qCWarning(CPR_PLUGIN_TEXTUAL)
          << "NCLTextEditor::onEntityAdded Trying to insert a media in a "
             "position greater than the text size. It will be ignored!"
          << insertAtOffset;
  }
}

void NCLTextualViewPlugin::onEntityRemoved(const QString &pluginID, const QString &entityID)
{
  // skip if this is my own call to onEntityRemoved
  if(pluginID == getPluginInstanceID() && !_isSyncing)
    return;

  int startOffset = _startEntityOffset[entityID];
  int endOffset = _endEntityOffset[entityID];

  char curChar = _nclTextEditor->SendScintilla(
        QsciScintilla::SCI_GETCHARAT,
        startOffset);

  while(curChar != '>' && startOffset >= 0)
  {
    startOffset--;
    curChar = _nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                             startOffset);
  }
  if(curChar == '>')
    startOffset++; // does not include the '>' character

  curChar = _nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                           endOffset);

  while(curChar != '>' && endOffset < _nclTextEditor->text().size())
  {
    endOffset++;
    curChar = _nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                             endOffset);
  }
  if(endOffset == _nclTextEditor->text().size())
  {
    qCWarning(CPR_PLUGIN_TEXTUAL)
        << "TextEditor could not perform the requested action.";
    return;
  }

  endOffset++; // includes the '>' character

  while(isspace(curChar) && endOffset < _nclTextEditor->text().size())
  {
    endOffset++;
    curChar = _nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                             endOffset);
  }

  _nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONSTART,
                                startOffset);
  _nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONEND,
                                endOffset);
  _nclTextEditor->removeSelectedText();

  QString key;
  QList<QString> mustRemoveEntity;

  // check all entities that is removed together with entityID, i.e.
  // its children
  // P.S. This could be get from model
  foreach(key, _startEntityOffset.keys())
  {
    // if the element is inside the entity that will be removed:
    if(_startEntityOffset[key] >= startOffset &&
       _endEntityOffset[key]+2 <= endOffset)
    {
      mustRemoveEntity.append(key);
    }
    else
    {
      // otherwise if necessary we must update the start and end line of
      // the entity.
      if(_startEntityOffset[key] >= startOffset)
      {
        _startEntityOffset[key] -= (endOffset - startOffset);
      }

      if(_endEntityOffset[key] >= endOffset)
      {
        _endEntityOffset[key] -= (endOffset - startOffset);
      }
    }
  }

  // Remove the content in text and update the structures that keep line number
  // of all entities.
  QListIterator<QString> iterator( mustRemoveEntity );
  while( iterator.hasNext() ){
    key = iterator.next();
    _startEntityOffset.remove(key);
    _endEntityOffset.remove(key);
  }

  /* foreach(key, startEntityOffset.keys())
     {
        qCDebug (CPR_PLUGIN_TEXTUAL) << " startOffset=" << startEntityOffset[key]
             << " endOffset=" << endEntityOffset[key];
     } */
}

bool NCLTextualViewPlugin::saveSubsession()
{
  QByteArray data;
  data.append(_nclTextEditor->text());
  data.append("$START_ENTITIES_LINES$");
  QString key;
  bool first = true;
  foreach (key, _startEntityOffset.keys())
  {
    if(!first)
      data.append(",");
    else
      first = false;
    data.append(key + ", " + QString::number(_startEntityOffset[key]));
  }
  data.append("$END_ENTITIES_LINES$");
  first = true;
  foreach (key, _endEntityOffset.keys())
  {
    if(!first)
      data.append(",");
    else
      first = false;
    data.append(key + "," + QString::number(_endEntityOffset[key]));
  }

  emit setPluginData(data);

  return true;
}

void NCLTextualViewPlugin::changeSelectedEntity(QString pluginID, void *param)
{
  Q_UNUSED(pluginID)

  if(_isSyncing)
    return; // do nothing;

  QString *id = (QString*)param;
  if(_startEntityOffset.contains(*id))
  {
    int entityOffset = _startEntityOffset.value(*id);
    //  int entityLine = window->getTextEditor()->SendScintilla(
    //                                  QsciScintilla::SCI_LINEFROMPOSITION,
    //                                  entityOffset);

    if(entityOffset < _nclTextEditor->text().size())
    {
      _nclTextEditor->SendScintilla(QsciScintilla::SCI_GOTOPOS,
                                    entityOffset);
      _nclTextEditor->SendScintilla(QsciScintilla::SCI_SETFOCUS,
                                    true);
    }
  }
  else
  {
    qCDebug (CPR_PLUGIN_TEXTUAL) << "NCLTextualViewPlugin::changeSelectedEntity() "
             << "Entity doesn't exists!";
  }
}

void NCLTextualViewPlugin::updateCoreModel()
{
  _syncMutex.lock();
  bool rebuildComposerModelFromScratch = true;

  _isSyncing = true; //set our current state as syncing

  QString text = _nclTextEditor->text();
  QString errorMessage;
  int errorLine, errorColumn;
  //Create a DOM document with the new content
  _nclTextEditor->clearErrorIndicators();
  if(!_xmlDoc.setContent(text, &errorMessage, &errorLine, &errorColumn))
  {
    //if the current XML is not well formed.
    QMessageBox::warning(_nclTextEditor, tr("Error"),
                         tr("Your document is not a Well-formed XML"));
    _nclTextEditor->keepFocused();
    _nclTextEditor->markError(errorMessage, "", errorLine-1, errorColumn);

    _isSyncing = false;
    _syncMutex.unlock();
    return;
  }

  //  int line, column;
  //  nclTextEditor->getCursorPosition(&line, &column);
  sendBroadcastMessage("textualStartSync", nullptr);

  //double-buffering
  _tmpNclTextEditor = _nclTextEditor;
  _nclTextEditor = new NCLTextEditor(0);
  _nclTextEditor->setDocumentUrl(project->getLocation());
  _nclTextEditor->setText(_tmpNclTextEditor->textWithoutUserInteraction());
  updateFromModel();  // this is just a precaution

  if(rebuildComposerModelFromScratch)
    nonIncrementalUpdateCoreModel();
  else
    //    incrementalUpdateCoreModelById();
    incrementalUpdateCoreModel();
  emit syncFinished();
  sendBroadcastMessage("textualFinishSync", nullptr);

  //  nclTextEditor->setCursorPosition(line, column); //go back to the previous position

  _syncMutex.unlock();
}

void NCLTextualViewPlugin::nonIncrementalUpdateCoreModel()
{
  //delete the content of the current project
  while (project->getChildren().size())
  {
    emit removeEntity(project->getChildren().at(0), true);
  }

  // clear the entities offset
  _nclTextEditor->clear();
  _nclTextEditor->setText("<?xml version=1.0 encoding=ISO-8859-1?>");
  _startEntityOffset.clear();
  _endEntityOffset.clear();

  QList <QString> parentUids;
  QString parentUId = project->getUniqueId();
  parentUids.push_back(parentUId);

  QList <QDomElement> nodes;
  QDomElement current = _xmlDoc.firstChildElement();
  nodes.push_back(current);

  while(!nodes.empty())
  {
    current = nodes.front();
    nodes.pop_front();
    parentUId = parentUids.front();
    parentUids.pop_front();

    if(current.tagName() == "ncl" && !current.hasAttribute("id"))
    {
      current.setAttribute("id", "myNCLDocID");
    }

    //Process the node
    QMap<QString,QString> atts;

    QDomNamedNodeMap attributes = current.attributes();
    for (uint i = 0; i < attributes.length(); i++)
    {
      QDomAttr item = attributes.item(i).toAttr();
      atts[item.name()] = item.value();
    }

    //Send the addEntity to the core plugin
    emit addEntity(current.tagName(), parentUId, atts, false);
    parentUId = _currentEntity->getUniqueId();

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
                         _nclTextEditor);
  dialog.setAutoClose(true);
  dialog.show();

  //incremental update
  QList <QDomNode> nodes;
  QDomNode current = _xmlDoc;
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
  current = _xmlDoc;
  nodes.push_back(current);
  while(!nodes.empty())
  {
    dialog.setValue(progress++);
    //dialog.update();
    //QApplication::processEvents();

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
      bool sameNCLID = false;

      if(children[i].hasAttribute("id") && entityChildren[j]->hasAttribute("id"))
      {
        if(children[i].attribute("id") == entityChildren[j]->getAttribute("id"))
          sameNCLID = true;
      }
      else if(    children[i].hasAttribute("name")
                  && entityChildren[j]->hasAttribute("name"))
      {
        if(children[i].attribute("name")
           == entityChildren[j]->getAttribute("name"))
          sameNCLID = true;
      }
      // testing for alias - remove after
      else if(children[i].hasAttribute("alias")
              && entityChildren[j]->hasAttribute("alias"))
      {
        if(children[i].attribute("alias")
           == entityChildren[j]->getAttribute("alias"))
          sameNCLID = true;
      }
      else
        sameNCLID = true;

      if(   children[i].tagName() == entityChildren[j]->getType()
            && sameNCLID)
      {
        //if the same type, just update the attributes
        //TODO: Compare attributes
        QMap<QString, QString> atts;
        QDomNamedNodeMap attributes = children[i].attributes();
        for (uint k = 0; k < attributes.length(); k++)
        {
          QDomNode item = attributes.item(k);
//          qCDebug (CPR_PLUGIN_TEXTUAL) << item.nodeName() << item.nodeValue();
          atts.insert(item.nodeName(), item.nodeValue());
        }

        QMap <QString, QString>::iterator begin, end, it;
        entityChildren[j]->getAttributeIterator(begin, end);

        bool changed = false;
        int entityChildrenAttrSize = 0;
        for (it = begin; it != end; ++it)
        {
          if(atts.contains(it.key()) && atts[it.key()]== it.value())
            continue;
          else
            changed = true;
          entityChildrenAttrSize++;
        }

        if(entityChildrenAttrSize != atts.size())
          changed = true;

        if(changed)
          emit setAttributes(entityChildren[j], atts, false);
      }
      else
      {
//        qCDebug (CPR_PLUGIN_TEXTUAL) << entityChildren[j]->getType() << children[i].tagName();
        //if type are not equal, then we should change the type
        //i.e. remove the entity
        emit removeEntity(entityChildren[j], true);
        //and insert a new entity with the required type.
        QMap<QString,QString> atts;
        QDomNamedNodeMap attributes = children[i].attributes();
        for (uint k = 0; k < attributes.length(); k++)
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
      // if there are more entities in the composer model than in the XML
      for(; j < entityChildren.size(); j++)
        emit removeEntity(entityChildren[j], true);
    }
    else if(j == entityChildren.size())
    {
      // if there are more entities in the XML than in the composer model
      for(; i < children.size(); i++)
      {
        //add new entity
        QMap<QString,QString> atts;
        QDomNamedNodeMap attributes = children[i].attributes();
        for (uint k = 0; k < attributes.length(); k++)
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
  delete _nclTextEditor;
  _nclTextEditor = _tmpNclTextEditor;
  _tmpNclTextEditor = nullptr;
  updateFromModel();
  _nclTextEditor->setTextWithoutUserInteraction(_nclTextEditor->text());
  _isSyncing = false;

  updateErrorMessages();
}

bool NCLTextualViewPlugin::isStartEndTag(Entity *entity)
{
  int endOffset = _endEntityOffset[entity->getUniqueId()];

  //Check if I am at a START_END_TAG />
  char curChar = _nclTextEditor->SendScintilla(QsciScintilla::SCI_GETCHARAT,
                                               endOffset-1);

  if(curChar == '/')
  {
    // qCDebug (CPR_PLUGIN_TEXTUAL) << "isStartEndTag returns true";
    return true;
  }
  else return false;
}

void NCLTextualViewPlugin::openStartEndTag(Entity *entity)
{
  if(isStartEndTag(entity))
  {
    int endOffset = _endEntityOffset[entity->getUniqueId()];

    //    printEntitiesOffset(); qCDebug (CPR_PLUGIN_TEXTUAL) << endl;
    // If the parent is a START_END, then we should separate the START from
    // the END.
    _nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONSTART,
                                  endOffset-1);

    _nclTextEditor->SendScintilla(QsciScintilla::SCI_SETSELECTIONEND,
                                  endOffset+1);

    _nclTextEditor->removeSelectedText();

    QString endTag = ">\n</" + entity->getType() + ">";
    // Openning the START_END_TAG
    _nclTextEditor->insertAtPos(endTag, endOffset-1);
    // before we have "/>\n" and now we have to remove update with the difference
    // i.e. endtag.size() - 3
    //    printEntitiesOffset();
    //    qCDebug (CPR_PLUGIN_TEXTUAL) << endl;
    updateEntitiesOffset(endOffset, endTag.size() - 2);
    //    printEntitiesOffset(); qCDebug (CPR_PLUGIN_TEXTUAL) << endl;

    fixIdentation(endOffset+2, false);
    _endEntityOffset[entity->getUniqueId()] = endOffset + 1;
  }
}

void NCLTextualViewPlugin::fixIdentation(int offset, bool mustAddTab)
{
  /* Fix Indentation */
  int insertAtLine = _nclTextEditor->SendScintilla(
        QsciScintilla::SCI_LINEFROMPOSITION, offset);

  int totalLines = _nclTextEditor->
      SendScintilla( QsciScintilla::SCI_GETLINECOUNT);

//  qDebug () << totalLines << insertAtLine;

  if(insertAtLine + 1 >= totalLines) return; // do nothing
  // get the identation for the next line
  int lineIndent = _nclTextEditor
      ->SendScintilla( QsciScintilla::SCI_GETLINEINDENTATION,
                       insertAtLine-1);

  if(insertAtLine > 1 && mustAddTab)
    lineIndent += _nclTextEditor->tabWidth();

  _nclTextEditor->SendScintilla( QsciScintilla::SCI_SETLINEINDENTATION,
                                 insertAtLine,
                                 lineIndent);

  updateEntitiesOffset(offset-1, lineIndent/_nclTextEditor->tabWidth());
}

void NCLTextualViewPlugin::updateEntitiesOffset( int startFrom,
                                                 int insertedChars)
{
  /* qCDebug (CPR_PLUGIN_TEXTUAL) << "NCLTextualViewPlugin::updateEntitiesOffset(" << startFrom
             << ", " << insertedChars << ")"; */

  if(!insertedChars) //nothing to do
    return;

  QString key;
  foreach(key, _startEntityOffset.keys())
  {
    if(_startEntityOffset[key] > startFrom)
      _startEntityOffset[key] += insertedChars;

    if(_endEntityOffset[key] >= startFrom)
      _endEntityOffset[key] += insertedChars;
  }
}

void NCLTextualViewPlugin::printEntitiesOffset()
{
  QString key;
  foreach(key, _startEntityOffset.keys())
  {
    int startOffSet = _startEntityOffset[key];
    char startChar = _nclTextEditor->SendScintilla(QsciScintilla::SCI_GETCHARAT,
                                                   startOffSet);
    int endOffSet = _endEntityOffset[key];
    char endChar = _nclTextEditor->SendScintilla(QsciScintilla::SCI_GETCHARAT,
                                                 endOffSet);

    qCDebug (CPR_PLUGIN_TEXTUAL) << "key="<< key << "(" << project->getEntityById(key)->getType()
             << "; start=" << startOffSet
             << "; start_char=" << startChar << "; end=" << endOffSet
             << "; end_char=" << endChar << endl;
  }
}

void NCLTextualViewPlugin::manageFocusLost(QFocusEvent *event)
{
  Q_UNUSED(event)

#ifndef NCLEDITOR_STANDALONE

  // When AutoComplete list gets the focus, the QApplication::focusWidget
  // has a nullptr value. This is an QScintilla issues.
  // When the focus goes to AutoComplete list we don't want to synchronize with
  // the core, that is why the test "QApplication::focusWidget() != nullptr" is
  // here.
  // qCDebug (CPR_PLUGIN_TEXTUAL) << nclTextEditor << QApplication::focusWidget();
  if(_nclTextEditor->textWithoutUserInteraction() != _nclTextEditor->text()
     && !_isSyncing
     && (QApplication::focusWidget() != nullptr))
  {
    /*int ret = QMessageBox::question(_window,
                                    tr("Textual View synchronization"),
                                    tr("You have changed the textual content of the NCL "
                                       "Document. Do you want to synchronize this text with "
                                       "other views?"),
                                                   QMessageBox::Yes |
                                                   QMessageBox::No |
                                                   QMessageBox::Cancel,

                                       QMessageBox::Cancel);

        switch(ret)
    {
        case QMessageBox::Yes: */
        updateCoreModel();
    /*    break;
        case QMessageBox::No:
        _nclTextEditor->setText(_nclTextEditor->textWithoutUserInteraction());
        break;
        case QMessageBox::Cancel:
        _nclTextEditor->keepFocused();
        break;
    } */
  }
  else if(QApplication::focusWidget() == nullptr)
  {
    // If the focus goes to AutoComplete list we force Qt keeps the focus in the
    // NCLTextEditor!!!
    _nclTextEditor->keepFocused();
  }
#endif
}

void NCLTextualViewPlugin::updateErrorMessages()
{
  if(_isSyncing)
    return;

  clearValidationMessages(this->pluginInstanceID, nullptr);

  emit sendBroadcastMessage("askAllValidationMessages", nullptr);
}

void NCLTextualViewPlugin::clearValidationMessages(QString, void *param)
{
  Q_UNUSED(param)
  _nclTextEditor->clearErrorIndicators();
}

void NCLTextualViewPlugin::validationError(QString pluginID, void * param)
{
  Q_UNUSED(pluginID)

  if(_isSyncing)
    return;

  if (param) {
    pair <QString , QString> *p = (pair <QString, QString> *) param;

    int offset = _startEntityOffset[p->first];

    int line = _nclTextEditor->SendScintilla(
          QsciScintilla::SCI_LINEFROMPOSITION,
          offset);

    _nclTextEditor->markError(p->second, "", line);
  }
}

QString NCLTextualViewPlugin::getEntityAttributesAsString(/*const */ Entity *entity)
{
  QString line;
  // fill the attributes (ordered)
  deque <QString> *attributes_ordered =
      NCLStructure::getInstance()->getAttributesOrdered(entity->getType());

  if(attributes_ordered != nullptr)
  {
    for(uint i = 0; i < attributes_ordered->size(); i++)
    {
      if(entity->hasAttribute((*attributes_ordered)[i]))
      {
        line += " " + (*attributes_ordered)[i] +
            "=\"" + entity->getAttribute((*attributes_ordered)[i]) + "\"";
      }
    }
  }

  map <QString, bool> *attributes =
      NCLStructure::getInstance()->getAttributes(entity->getType());
  // Search if there is any other attribute that is not part of NCL Language
  // but the user fills it.
  QMap <QString, QString>::iterator begin, end, it;
  entity->getAttributeIterator(begin, end);
  for (it = begin; it != end; ++it)
  {
    if(attributes == nullptr || !attributes->count(it.key()))
      line += " " + it.key() + "=\"" + it.value() + "\"";
  }

  return line;
}

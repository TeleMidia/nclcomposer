/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "NCLTextualViewPlugin.h"

#include <QMetaObject>
#include <QMetaMethod>
#include <QMessageBox>

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

    connect( updateModelShortcut, SIGNAL(activated()),
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
}

QWidget* NCLTextualViewPlugin::getWidget()
{
    return window;
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

    emit TextualPluginHasAddedEntity(pluginID, entity);

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
    if(pluginID == getPluginInstanceID())
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
    char curChar = nclTextEditor->SendScintilla(
                                                QsciScintilla::SCI_GETCHARAT,
                                                insertAtOffset+previous_length);
    while(curChar != '>' &&
     (insertAtOffset+previous_length) < nclTextEditor->text().size())
    {
        previous_length++;
        curChar = nclTextEditor->SendScintilla( QsciScintilla::SCI_GETCHARAT,
                                                insertAtOffset+previous_length);
    }

    if((insertAtOffset+previous_length)
            == nclTextEditor->text().size())
    {
        qWarning() << "TextEditor could not perform the requested action.";
        return;
    }
    previous_length+=1;

    qDebug() << previous_length;
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

    nclTextEditor->SendScintilla( QsciScintilla::SCI_GOTOPOS,
                                            insertAtOffset);

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

    /* foreach(key, startEntityOffset.keys())
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
//        int entityLine = window->getTextEditor()->SendScintilla(
//                                    QsciScintilla::SCI_LINEFROMPOSITION,
//                                    entityOffset);

        nclTextEditor->SendScintilla(QsciScintilla::SCI_GOTOPOS,
                                               entityOffset);
       nclTextEditor->SendScintilla(QsciScintilla::SCI_SETFOCUS,
                                                true);
    }
    else
    {
        qWarning() << "NCLTextualViewPlugin::changeSelectedEntity() "
                << "Entity doesn't exists!";
    }
}

void NCLTextualViewPlugin::updateCoreModel()
{
    NCLDocumentParser parser(project);
    QString text = nclTextEditor->text();

    isSyncing = true;

    tmpNclTextEditor = nclTextEditor;
    nclTextEditor = new NCLTextEditor(0);
    nclTextEditor->setText(tmpNclTextEditor->text());

    if(project->getChildren().size())
        emit removeEntity(project->getChildren().at(0), true);

    connect(this,
            SIGNAL(TextualPluginHasAddedEntity(QString, Entity*)),
            &parser,
            SLOT(onEntityAdded(QString, Entity*)),
            Qt::DirectConnection);

    connect(&parser,
            SIGNAL(parseFinished()),
            this,
            SLOT(syncFinished()),
            Qt::QueuedConnection);

    connect( &parser,
             SIGNAL(addEntity(QString, QString, QMap<QString,QString>&,bool)),
             this,
             SIGNAL(addEntity(QString, QString, QMap<QString,QString>&,bool)),
             Qt::DirectConnection);

    nclTextEditor->clear();
    startEntityOffset.clear();
    endEntityOffset.clear();
    parser.parseContent(text);
}

void NCLTextualViewPlugin::syncFinished()
{
    tmpNclTextEditor->setText(nclTextEditor->text());
    delete nclTextEditor;
    nclTextEditor = tmpNclTextEditor;
    tmpNclTextEditor = NULL;
    isSyncing = false;
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
    if(nclTextEditor->textWithoutUserInteraction() != nclTextEditor->text())
    {
        int ret = QMessageBox::question(window,
                        tr("Text synchronization"),
                        tr("You have change the textual content of the NCL \
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
                nclTextEditor->setText(
                                    nclTextEditor->textWithoutUserInteraction());
                break;
            case QMessageBox::Cancel:
                nclTextEditor->keepFocused();
                break;
        }
    }
#endif
}

} } } //end namespace

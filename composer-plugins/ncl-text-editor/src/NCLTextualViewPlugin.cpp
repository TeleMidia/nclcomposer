#include "NCLTextualViewPlugin.h"

NCLTextualViewPlugin::NCLTextualViewPlugin()
{
    window = new NCLTextEditorMainWindow();
    doc = NULL;
    connect( window,
             SIGNAL(elementAdded(QString,QString,QMap<QString,QString>&,bool)),
             this,
             SIGNAL(addEntity(QString,QString,QMap<QString,QString>&,bool)));
}

NCLTextualViewPlugin::~NCLTextualViewPlugin()
{
    delete window;
    window = NULL;
}

QWidget* NCLTextualViewPlugin::getWidget()
{
    return window->getTextEditor();
}

void NCLTextualViewPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    //Return if this is my call to onEntityAdded
    if(pluginID == getPluginInstanceID())
        return;

    QString line = "<" + entity->getType() + "";

    int insertAtLine = 0;
    //get the number line where the new element must be inserted
    if(entity->getParentUniqueId() != NULL) {
        insertAtLine = endLineOfEntity.value(entity->getParentUniqueId());
    }

    QMap <QString, QString>::iterator begin, end, it;
    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
        line += " " + it.key() + "=\"" + it.value() + "\"";
    }
    line += ">\n";
    line += "</" + entity->getType() + ">\n";

    //update all previous entities line numbers (when necessary)
    QString key;
    foreach(key, startLineOfEntity.keys())
    {
        if(startLineOfEntity[key] >= insertAtLine)
            startLineOfEntity[key] += 2;

        if(endLineOfEntity[key] >= insertAtLine)
            endLineOfEntity[key] += 2;

    }

    window->getTextEditor()->insertAt(line, insertAtLine, 0);
    startLineOfEntity[entity->getUniqueId()] = insertAtLine;
    endLineOfEntity[entity->getUniqueId()] = insertAtLine+1;

    //fix indentation
    int lineident = window->getTextEditor()
                    ->SendScintilla( QsciScintilla::SCI_GETLINEINDENTATION,
                                     insertAtLine+2);

    if(insertAtLine == 0) //The first entity
        lineident = 0;
    else
        lineident += 8;

    window->getTextEditor()
            ->SendScintilla( QsciScintilla::SCI_SETLINEINDENTATION,
                             insertAtLine,
                             lineident);

    window->getTextEditor()
            ->SendScintilla( QsciScintilla::SCI_SETLINEINDENTATION,
                             insertAtLine+1,
                             lineident);

    window->getTextEditor()->setCursorPosition(insertAtLine, 0);
    window->getTextEditor()->ensureLineVisible(insertAtLine);
    window->getTextEditor()->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);

    /* qDebug() << "NCLTextualViewPlugin::onEntityAdded" <<
            entity->getType() << " " << insertAtLine; */
}

void NCLTextualViewPlugin::errorMessage(QString error)
{
    //qDebug() << "NCLTextualViewPlugin::onEntityAddError(" << error << ")";
}

void NCLTextualViewPlugin::onEntityChanged(QString ID, Entity *entity)
{
    QString line = "PLUGIN (" + ID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    //TODO: All
}

void NCLTextualViewPlugin::onEntityAboutToRemove(Entity *)
{

}

void NCLTextualViewPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
    //skip if this is my own call to onEntityRemoved
    if(pluginID == getPluginInstanceID())
        return;

    int startLineOfRemovedEntity = startLineOfEntity[entityID];
    int endLineOfRemovedEntity = endLineOfEntity[entityID];

    window->getTextEditor()->setSelection( startLineOfRemovedEntity,
                                           0,
                                           endLineOfRemovedEntity,
                                           window->getTextEditor()
                                           ->lineLength(endLineOfRemovedEntity)
                                           );

    window->getTextEditor()->removeSelectedText();

    QString key;
    QList<QString> mustRemoveEntity;

    // check all entities that is removed together with entityID, i.e.
    // its children
    // P.S. This could be get from model
    qDebug() << startLineOfRemovedEntity << " " << endLineOfRemovedEntity;
    foreach(key, startLineOfEntity.keys())
    {
        // if the element is inside the entity that will be removed:
        if(startLineOfEntity[key] >= startLineOfRemovedEntity &&
           endLineOfEntity[key] <= endLineOfRemovedEntity)
        {
            mustRemoveEntity.append(key);
        }
        else {
            // otherwise if necessary we must update the start and end line of
            // the entity.
            if(startLineOfEntity[key] > startLineOfRemovedEntity)
            {
                startLineOfEntity[key] -=
                        (endLineOfRemovedEntity - startLineOfRemovedEntity)+1;
            }

            if(endLineOfEntity[key] > endLineOfRemovedEntity)
            {
                endLineOfEntity[key] -=
                        (endLineOfRemovedEntity - startLineOfRemovedEntity)+1;
            }
        }

    }

    //Remove the content in text and update the structures that keep line number
    // of all entities.
    QListIterator<QString> iterator( mustRemoveEntity );
    while( iterator.hasNext() ){
        key = iterator.next();
        startLineOfEntity.remove(key);
        endLineOfEntity.remove(key);
    }
    foreach(key, startLineOfEntity.keys())
    {
        qDebug() << doc->getEntityBydId(key)->getType()
             << " startLine=" << startLineOfEntity[key]
             << " endLine=" << endLineOfEntity[key];

    }
}

bool NCLTextualViewPlugin::saveSubsession(){
    //TODO: All
    return true;
}

void NCLTextualViewPlugin::updateFromModel(){

}



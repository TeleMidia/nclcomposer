#include "NCLTextualViewPlugin.h"

namespace composer {
    namespace plugin {
        namespace textual {

NCLTextualViewPlugin::NCLTextualViewPlugin()
{
    window = new NCLTextEditorMainWindow();
    project = NULL;
    connect( window,
             SIGNAL(elementAdded(QString,QString,QMap<QString,QString>&,bool)),
             this,
             SIGNAL(addEntity(QString,QString,QMap<QString,QString>&,bool)));


    QToolBar *toolbar = window->addToolBar("Synchronize");
    QAction *synchronizeAct = toolbar->addAction(
                                      QIcon(":/images/synchronize-icon-24.png"),
                                      tr("&Synchronize"));

    synchronizeAct->setStatusTip(tr("Synchronize current text with the others"
                                    "plugins."));
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
    foreach(key, startLineOfEntity.keys())
        startLineOfEntity.remove(key);
    foreach(key, startLineOfEntity.keys())
        endLineOfEntity.remove(key);

    QString text = data.left(indexOfStartEntities);
    int indexOfStartEntitiesContent = indexOfStartEntities +
                                      startEntitiesSep.length();
    QString startLines = data.mid(indexOfStartEntitiesContent,
                              indexOfEndEntities - indexOfStartEntitiesContent);

    QString endLines = data.right(data.length() -
                                  (indexOfEndEntities+endEntitiesSep.length()));

    QStringList listStart = startLines.split(",");
    QStringList listEnd = endLines.split(",");

    qDebug() << "endLines = " << endLines;

    for(int i = 0; i < listStart.size()-1; i +=2)
    {
        startLineOfEntity[listStart[i]] = listStart[i+1].toInt();
        endLineOfEntity[listEnd[i]] = listEnd[i+1].toInt();
    }

    window->getTextEditor()->setText(text);
}

QWidget* NCLTextualViewPlugin::getWidget()
{
    return window;
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
        // Test if exists before access from operator[] becaus if doesn't exist
        // this operator will create a new (and we don't want this!).
        if(endLineOfEntity.count(entity->getParentUniqueId()))
            insertAtLine = endLineOfEntity[entity->getParentUniqueId()];
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

    /*  qDebug() << "NCLTextualViewPlugin::onEntityAdded" <<
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

    int insertAtLine = startLineOfEntity.value(entity->getUniqueId());

    QMap <QString, QString>::iterator begin, end, it;
    entity->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
        if(it.value() != "")
            line += " " + it.key() + "=\"" + it.value() + "\"";
    }
    line += ">";

    int previous_length = window->getTextEditor()->SendScintilla(
                                                  QsciScintilla::SCI_LINELENGTH,
                                                  insertAtLine);
    // store the current identation (this must keep equal even with the
    //  modifications)
    int lineident = window->getTextEditor()
                    ->SendScintilla( QsciScintilla::SCI_GETLINEINDENTATION,
                                     insertAtLine);

    window->getTextEditor()->setSelection( insertAtLine, 0,
                                           insertAtLine, previous_length-1);
    window->getTextEditor()->removeSelectedText();

    window->getTextEditor()->insertAt(line, insertAtLine, 0);

    //fix indentation
    window->getTextEditor()
            ->SendScintilla( QsciScintilla::SCI_SETLINEINDENTATION,
                             insertAtLine,
                             lineident);

    window->getTextEditor()->setCursorPosition(insertAtLine, 0);
    window->getTextEditor()->ensureLineVisible(insertAtLine);
    window->getTextEditor()->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);
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
        qDebug() << project->getEntityById(key)->getType()
             << " startLine=" << startLineOfEntity[key]
             << " endLine=" << endLineOfEntity[key];
    }
}

bool NCLTextualViewPlugin::saveSubsession()
{
    QByteArray data;
    data.append(window->getTextEditor()->text());
    data.append("$START_ENTITIES_LINES$");
    QString key;
    bool first = true;
    foreach (key, startLineOfEntity.keys())
    {
        if(!first)
            data.append(",");
        else
            first = false;
        data.append(key + ", " + QString::number(startLineOfEntity[key]));
   }
    data.append("$END_ENTITIES_LINES$");
    first = true;
    qDebug() << endLineOfEntity;
    foreach (key, endLineOfEntity.keys())
    {
        if(!first)
            data.append(",");
        else
            first = false;
        data.append(key + "," + QString::number(endLineOfEntity[key]));
    }

    emit setPluginData(data);
    return true;
}

void NCLTextualViewPlugin::changeSelectedEntity(void *param){
    QString *id = (QString*)param;
    int entityLine = startLineOfEntity.value(*id);
    int size = window->getTextEditor()->lineLength(entityLine);

    window->getTextEditor()->setCursorPosition(entityLine, 0);
    window->getTextEditor()->ensureLineVisible(entityLine);
    window->getTextEditor()->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);
}

} } } //end namespace

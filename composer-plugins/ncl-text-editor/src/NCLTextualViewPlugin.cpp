#include "NCLTextualViewPlugin.h"

NCLTextualViewPlugin::NCLTextualViewPlugin()
{
    window = new NCLTextEditorMainWindow();
    doc = NULL;

    connect(window, SIGNAL(elementAdded(QString,QString,QMap<QString,QString>&,bool)),
            this, SIGNAL(addEntity(QString,QString,QMap<QString,QString>&,bool)));
}

NCLTextualViewPlugin::~NCLTextualViewPlugin()
{
    delete window;
    window = NULL;
}

QWidget* NCLTextualViewPlugin::getWidget()
{
    return window;
}

void NCLTextualViewPlugin::onEntityAdded(QString ID, Entity *entity)
{
    QString line = "<" + entity->getType() + "> </" + entity->getType() + ">\n";

    window->getTextEditor()->append(line);
    //TODO: All
}

void NCLTextualViewPlugin::onEntityAddError(QString error)
{
    qDebug() << "NCLTextualViewPlugin::onEntityAddError(" << error << ")";
}

void NCLTextualViewPlugin::onEntityChanged(QString ID, Entity * entity)
{
    QString line = "PLUGIN (" + ID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    //TODO: All
}

void NCLTextualViewPlugin::onEntityChangeError(QString error)
{
    //TODO: All
}

void NCLTextualViewPlugin::onEntityAboutToRemove(Entity *)
{

}

void NCLTextualViewPlugin::onEntityRemoved(QString ID, QString entityID)
{
    QString line = "PLUGIN (" + ID + ") removed Entity (" +
                   entityID + ")";
    //TODO: All
}

void NCLTextualViewPlugin::onEntityRemoveError(QString error)
{
    //TODO: All
}

bool NCLTextualViewPlugin::save(){
    //TODO: All
}

void NCLTextualViewPlugin::updateFromModel(){

}



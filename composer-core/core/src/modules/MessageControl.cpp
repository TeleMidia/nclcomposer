#include "modules/MessageControl.h"
#include "modules/PluginControl.h"

#include <QMetaObject>
#include <QMetaMethod>

namespace composer {
    namespace core {

MessageControl::MessageControl(Project *project)
{
    this->project = project;

    //Register MetaType allowing to be used by invoke
    qRegisterMetaType<void *>("Entity *");
}

MessageControl::~MessageControl()
{

}

void MessageControl::onAddEntity( QString type,
               QString parentEntityId, QMap<QString,QString>& atts,
               bool force)
{

    /* Cast to IPlugin to make sure it's a plugin */
    IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
    IDocumentParser *parser = qobject_cast<IDocumentParser*>
                              (QObject::sender());
    QString pluginID;

    if(plugin) pluginID = plugin->getPluginInstanceID();
    else if (parser) pluginID = parser->getParserName();

    Entity *ent = NULL;

    try
    {
        ent = new Entity(atts);
        ent->setType(type);
        //TODO - calll validator to check
        project->addEntity(ent, parentEntityId);

        //send message to All PLUGINS interested in this message.
        sendEntityAddedMessageToPlugins(pluginID, ent);
    }
    catch(exception& e)
    {
        if (plugin)
            plugin->errorMessage(e.what());
        else if (parser)
            parser->onEntityAddError(e.what());

        delete ent;
        ent = NULL;

        return;
    }
}

void MessageControl::onEditEntity(Entity *entity,
                                  QMap<QString,QString> atts, bool force)
{
    IPlugin *plugin = qobject_cast<IPlugin *>(QObject::sender());

    if(plugin) {
        QString pluginID = plugin->getPluginInstanceID();
        try {
            /*! \todo Call validator to check */
           entity->setAtrributes(atts);

           //send message to all plugins interested in this message.
           sendEntityChangedMessageToPlugins(pluginID, entity);
        }
        catch(exception e)
        {
            plugin->errorMessage(e.what());
            return;
        }
    } else {
        //TODO -- erro on casting
        return;
    }

}

void MessageControl::onRemoveEntity( Entity *entity,
                                     bool force)
{
    IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
    if(plugin)
    {
        QString pluginID = plugin->getPluginInstanceID();
        try
        {
            if(entity)
            {
                // Send message to all plugins interested in this message.
                // This message is send before the real delete to try to avoid
                // the plugins keep pointers to invalid memory location.
                sendEntityRemovedMessageToPlugins(pluginID, entity);
                /*!
                \todo remember to change, the append should come from the
                    plugin.
                */
                project->removeEntity(entity, true);
            }
            else
            {
                plugin->errorMessage(tr("You have tried to remove a NULL \
                                        entity!!"));
            }
        }
        catch(exception e)
        {
            plugin->errorMessage(e.what());
            return;
        }
    }
    else
    {
        /*! \todo error on casting management
        */
        return;
    }
}

void MessageControl::setListenFilter(QStringList entityList)
{
    qDebug() << "MessageControl::setListenFilter";
    IPlugin *plugin = qobject_cast<IPlugin *> (QObject::sender());
    if(plugin)
    {
        listenEntities[plugin->getPluginInstanceID()] = entityList;
    }
}

/*
 * \todo The implementation of the folling three implementations should be merge
 *          in one function.
 */
void MessageControl::sendEntityAddedMessageToPlugins( QString pluginInstanceId,
                                                      Entity *entity)
{
    QList<IPlugin*>::iterator it;
    QList<IPlugin*> instances =
            PluginControl::getInstance()
                            ->getPluginInstances(this->project->getLocation());

    QString slotName("onEntityAdded(QString,Entity*)");

    for (it = instances.begin(); it != instances.end(); it++)
    {
       IPlugin *inst = *it;

       if(pluginIsInterestedIn(inst, entity))
       {
           int idxSlot = inst->metaObject()
                                     ->indexOfSlot(slotName.toStdString().c_str());
           if(idxSlot != -1)
           {
               QMetaMethod method = inst->metaObject()->method(idxSlot);
               method.invoke(inst, Qt::QueuedConnection,
                             Q_ARG(QString, pluginInstanceId),
                             Q_ARG(Entity*, entity));
           }
       }
    }

    emit entityAdded(pluginInstanceId, entity);
}


void MessageControl::sendEntityChangedMessageToPlugins(QString pluginInstanceId,
                                                       Entity *entity)
{
    QList<IPlugin*>::iterator it;
    QList<IPlugin*> instances =
            PluginControl::getInstance()
                            ->getPluginInstances(this->project->getLocation());

    QString slotName("onEntityChanged(QString,Entity*)");

    for (it = instances.begin(); it != instances.end(); it++)
    {
       IPlugin *inst = *it;

       if(pluginIsInterestedIn(inst, entity))
       {
           int idxSlot = inst->metaObject()
                                     ->indexOfSlot(slotName.toStdString().c_str());
           if(idxSlot != -1)
           {
               QMetaMethod method = inst->metaObject()->method(idxSlot);
               method.invoke(inst, Qt::QueuedConnection,
                             Q_ARG(QString, pluginInstanceId),
                             Q_ARG(Entity*, entity));
           }
       }
    }
}

void MessageControl::sendEntityRemovedMessageToPlugins(QString pluginInstanceId,
                                                       Entity *entity)
{
    QList<IPlugin*>::iterator it;
    QList<IPlugin*> instances =
            PluginControl::getInstance()
                            ->getPluginInstances(this->project->getLocation());

    QString slotName("onEntityRemoved(QString,QString)");
    QString entityId = entity->getUniqueId();

    for (it = instances.begin(); it != instances.end(); it++)
    {
       IPlugin *inst = *it;

       if(pluginIsInterestedIn(inst, entity))
       {
           int idxSlot = inst->metaObject()
                                     ->indexOfSlot(slotName.toStdString().c_str());
           if(idxSlot != -1)
           {
               QMetaMethod method = inst->metaObject()->method(idxSlot);
               method.invoke(inst, Qt::QueuedConnection,
                             Q_ARG(QString, pluginInstanceId),
                             Q_ARG(QString, entityId));
           }
       }
    }
}

bool MessageControl::pluginIsInterestedIn(IPlugin *plugin, Entity *entity)
{
    if(!listenEntities.contains(plugin->getPluginInstanceID()))
    {
        // Default: Plugin is interested in ALL entities
        return true;
    }

    if(listenEntities.value(
            plugin->getPluginInstanceID()).contains(entity->getType())
        )
        return true;

    return false;
}

} } //end namespace

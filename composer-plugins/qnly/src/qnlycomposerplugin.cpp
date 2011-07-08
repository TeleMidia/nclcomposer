#include "qnlycomposerplugin.h"

namespace composer {
    namespace plugin {
        namespace layout {

QnlyComposerPlugin::QnlyComposerPlugin(QObject* parent)
{
    setParent(parent);

    createView();
    createConnections();
}

QnlyComposerPlugin::~QnlyComposerPlugin()
{
    delete(view);
}

void QnlyComposerPlugin::createView()
{
    view = new QnlyView();
}

void QnlyComposerPlugin::createConnections()
{
    connect(view,
        SIGNAL(regionAdded(QString,QString,QString,QMap<QString,QString>)),
        SLOT(addRegion(QString,QString,QString,QMap<QString,QString>)));

    connect(view,
        SIGNAL(regionRemoved(QString,QString)),
        SLOT(removeRegion(QString,QString)));

    connect(view,
        SIGNAL(regionSelected(QString,QString)),
        SLOT(selectRegion(QString,QString)));

    connect(view,
        SIGNAL(regionChanged(QString,QString,QMap<QString,QString>)),
        SLOT(changeRegion(QString,QString,QMap<QString,QString>)));

    connect(view,
        SIGNAL(regionBaseAdded(QString,QMap<QString,QString>)),
        SLOT(addRegionBase(QString,QMap<QString,QString>)));

    connect(view,
        SIGNAL(regionBaseChanged(QString,QMap<QString,QString>)),
        SLOT(changeRegionBase(QString,QMap<QString,QString>)));

    connect(view,
        SIGNAL(regionBaseSelected(QString)),
        SLOT(selectRegionBase(QString)));

    connect(view,
        SIGNAL(regionBaseRemoved(QString)),
        SLOT(removeRegionBase(QString)));
}

QWidget* QnlyComposerPlugin::getWidget()
{
    return view;
}

bool QnlyComposerPlugin::saveSubsession()
{
    return false;
}

void QnlyComposerPlugin::init()
{
    // \todo Load specific contents.
    QStack <Entity*> stack;
    stack.push(project);

    while(stack.size())
    {
        Entity *current = stack.top();
        stack.pop();

        qDebug() << " QnlyComposerPlugin::init " << current->getType();
        if(current->getType() == "regionBase")
        {
            addRegionBaseToView(current);
        }
        else if(current->getType() == "region")
        {
            addRegionToView(current);
        }

        QVector <Entity *> children = current->getChildren();
        for(int i = 0; i < children.size(); i++)
        {
            stack.push(children.at(i));
        }
    }
}

void QnlyComposerPlugin::errorMessage(QString error)
{
    //TODO: void QnlyComposerPlugin::errorMessage(QString error)
}

void QnlyComposerPlugin::onEntityAdded(QString pluginID, Entity *entity)
{    
    if (entity != NULL)
    {
        if (entity->getType() == "region")
        {
            addRegionToView(entity);
        }
        else if (entity->getType() == "regionBase")
        {
            addRegionBaseToView(entity);
        }
    }
}

void QnlyComposerPlugin::onEntityRemoved(QString pluginID, QString entityID)
{
    if (!entityID.isEmpty())
    {
        if (regions.contains(entityID))
        {
            removeRegionFromView(entityID);
        }
        else if (regionbases.contains(entityID))
        {
            removeRegionBaseFromView(entityID);
        }
    }
}

void QnlyComposerPlugin::onEntityChanged(QString pluginID, Entity *entity)
{
    if (entity != NULL)
    {
        if (entity->getType() == "region")
        {
            changeRegionInView(entity);
        }
        else if (entity->getType() == "regionBase")
        {
            changeRegionBaseInView(entity);
        }
    }
}

void QnlyComposerPlugin::changeSelectedEntity (void* param)
{
    QString* entityUID = (QString*) param;

    if (regions.contains(*entityUID))
    {
        selectRegionInView(*entityUID);
    }
    else if (regionbases.contains(*entityUID))
    {
        selectRegionBaseInView(*entityUID);
    }
}

void QnlyComposerPlugin::addRegionToView(Entity* entity)
{
    if (entity != NULL)
    {
        if (entity->getType() == "region")
        {
            // setting
            QString entityUID;

            if (!entity->getUniqueId().isEmpty())
            {
                entityUID = entity->getUniqueId();
            }
            else
            {
                qWarning() << "QnlyComposerPlugin::addRegion:"
                           << "Tried to add a region with empty UID.";

                return; // abort addition
            }

            QString regionUID;

            regionUID = entityUID;

            QString parentUID;

            if (regions.contains(entity->getParentUniqueId()))
            {
                parentUID = entity->getParentUniqueId();
            }
            else
            {
                parentUID = "";
            }

            QString regionbaseUID;

            if (regions.contains(parentUID))
            {
                regionbaseUID = relations[parentUID];

            }
            else if (regionbases.contains(entity->getParentUniqueId()))
            {
                regionbaseUID = entity->getParentUniqueId();

            }
            else
            {
                qWarning() << "QnlyComposerPlugin::addRegion:"
                           << "Tried to add a region without regionbase UID.";

                return; // abort addition
            }

            QMap<QString, QString> attributes;

            if (!entity->getAttribute("id").isEmpty())
            {
                attributes["id"] =  entity->getAttribute("id");
            }

            if (!entity->getAttribute("title").isEmpty())
            {
                attributes["title"] =  entity->getAttribute("title");
            }

            if (!entity->getAttribute("zIndex").isEmpty())
            {
                attributes["zIndex"] =  entity->getAttribute("zIndex");
            }

            if (!entity->getAttribute("top").isEmpty())
            {
                attributes["top"] =  entity->getAttribute("top");
            }

            if (!entity->getAttribute("left").isEmpty())
            {
                attributes["left"] =  entity->getAttribute("left");
            }

            if (!entity->getAttribute("bottom").isEmpty())
            {
                attributes["bottom"] =  entity->getAttribute("bottom");
            }

            if (!entity->getAttribute("right").isEmpty())
            {
                attributes["right"] =  entity->getAttribute("right");
            }

            if (!entity->getAttribute("width").isEmpty())
            {
                attributes["width"] =  entity->getAttribute("width");
            }

            if (!entity->getAttribute("height").isEmpty())
            {
                attributes["height"] =  entity->getAttribute("height");
            }

            // adding
            regions[regionUID] = entity;

            relations[regionUID] = regionbaseUID;

            view->addRegion(regionUID,
                            parentUID,
                            regionbaseUID,
                            attributes);
        }
    }
}

void QnlyComposerPlugin::removeRegionFromView(QString entityUID)
{
    if (!entityUID.isEmpty())
    {
        if (regions.contains(entityUID))
        {
            // setting
            QString regionUID;

            regionUID = entityUID;

            QString regionbaseUID;

            if (relations.contains(regionUID))
            {
                regionbaseUID = relations[regionUID];
            }
            else
            {
                qWarning() << "QnlyComposerPlugin::removeRegion:"
                           << "Tried to remove a region without"
                           << "regionbase UID.";

                return; // abort remotion
            }

            // removing
            view->removeRegion(regionUID, regionbaseUID);
        }
    }
}

void QnlyComposerPlugin::changeRegionInView(Entity* entity)
{
    if (entity != NULL)
    {
        if (entity->getType() == "region")
        {
            // setting
            QString entityUID;

            if (!entity->getUniqueId().isEmpty())
            {
                entityUID = entity->getUniqueId();
            }
            else
            {
                qWarning() << "QnlyComposerPlugin::addRegion:"
                           << "Tried to add an region with empty UID.";

                return; // abort addition
            }

            QString regionUID;

            regionUID = entityUID;

            QString parentUID;

            if (regions.contains(entity->getParentUniqueId()))
            {
                parentUID = entity->getParentUniqueId();
            }
            else
            {
                parentUID = "";
            }

            QString regionbaseUID;

            if (regions.contains(parentUID))
            {
                regionbaseUID = relations[parentUID];

            }
            else if (regionbases.contains(entity->getParentUniqueId()))
            {
                regionbaseUID = entity->getParentUniqueId();

            }
            else
            {
                qWarning() << "QnlyComposerPlugin::addRegion:"
                           << "Tried to add an region without"
                           << "regionbase UID.";

                return; // abort addition
            }

            QMap<QString, QString> attributes;

            if (!entity->getAttribute("id").isEmpty())
            {
                attributes["id"] =  entity->getAttribute("id");
            }

            if (!entity->getAttribute("title").isEmpty())
            {
                attributes["title"] =  entity->getAttribute("title");
            }

            if (!entity->getAttribute("zIndex").isEmpty())
            {
                attributes["zIndex"] =  entity->getAttribute("zIndex");
            }

            if (!entity->getAttribute("top").isEmpty())
            {
                attributes["top"] =  entity->getAttribute("top");
            }

            if (!entity->getAttribute("left").isEmpty())
            {
                attributes["left"] =  entity->getAttribute("left");
            }

            if (!entity->getAttribute("bottom").isEmpty())
            {
                attributes["bottom"] =  entity->getAttribute("bottom");
            }

            if (!entity->getAttribute("right").isEmpty())
            {
                attributes["right"] =  entity->getAttribute("right");
            }

            if (!entity->getAttribute("width").isEmpty())
            {
                attributes["width"] =  entity->getAttribute("width");
            }

            if (!entity->getAttribute("height").isEmpty())
            {
                attributes["height"] =  entity->getAttribute("height");
            }

            // change
            view->changeRegion(regionUID,
                               regionbaseUID,
                               attributes);
        }
    }
}

void QnlyComposerPlugin::selectRegionInView(QString entityUID)
{
    if (!entityUID.isEmpty())
    {
        if (regions.contains(entityUID))
        {
            // setting
            QString regionUID;

            regionUID = entityUID;

            QString regionbaseUID;

            if (relations.contains(regionUID))
            {
                regionbaseUID = relations[regionUID];
            }
            else
            {
                qWarning() << "QnlyComposerPlugin::selectRegion:"
                           << "Tried to select a region without"
                           << "regionbase UID.";

                return; // abort remotion
            }

            // select
            view->selectRegion(regionUID, regionbaseUID);
        }
    }
}

void QnlyComposerPlugin::addRegionBaseToView(Entity* entity)
{
    if (entity != NULL)
    {
        if (entity->getType() == "regionBase")
        {
            // setting
            QString entityUID;

            if (!entity->getUniqueId().isEmpty())
            {
                entityUID = entity->getUniqueId();
            }
            else
            {
                qWarning() << "QnlyComposerPlugin::addRegion:"
                           << "Tried to add a region with empty UID.";

                return; // abort addition
            }

            QString regionbaseUID;

            regionbaseUID = entityUID;

            QMap<QString, QString> attributes;

            if (!entity->getAttribute("id").isEmpty())
            {
                attributes["id"] =  entity->getAttribute("id");
            }

            if (!entity->getAttribute("region").isEmpty())
            {
                attributes["region"] =  entity->getAttribute("region");
            }

            if (!entity->getAttribute("device").isEmpty())
            {
                attributes["device"] =  entity->getAttribute("device");
            }

            // adding
            regionbases[regionbaseUID] = entity;

            view->addRegionBase(regionbaseUID,
                                attributes);
        }
    }
}

void QnlyComposerPlugin::removeRegionBaseFromView(QString entityUID)
{
    if (!entityUID.isEmpty())
    {
        if (regionbases.contains(entityUID))
        {
            // setting
            QString regionbaseUID;

            regionbaseUID = entityUID;

            // select
            view->removeRegionBase(regionbaseUID);
        }
    }
}

void QnlyComposerPlugin::changeRegionBaseInView(Entity* entity)
{
    if (entity != NULL)
    {
        if (entity->getType() == "regionBase")
        {
            // setting
            QString entityUID;

            if (!entity->getUniqueId().isEmpty())
            {
                entityUID = entity->getUniqueId();
            }
            else
            {
                qWarning() << "QnlyComposerPlugin::addRegion:"
                           << "Tried to add a region with empty UID.";
                return; // abort addition
            }

            QString regionbaseUID;

            regionbaseUID = entityUID;

            QMap<QString, QString> attributes;

            if (!entity->getAttribute("id").isEmpty())
            {
                attributes["id"] =  entity->getAttribute("id");
            }

            if (!entity->getAttribute("region").isEmpty())
            {
                attributes["region"] =  entity->getAttribute("region");
            }

            if (!entity->getAttribute("device").isEmpty())
            {
                attributes["device"] =  entity->getAttribute("device");
            }

            // adding
            regionbases[regionbaseUID] = entity;

            view->changeRegionBase(regionbaseUID,
                                attributes);
        }
    }
}

void QnlyComposerPlugin::selectRegionBaseInView(QString entityUID)
{
    if (!entityUID.isEmpty())
    {
        if (regionbases.contains(entityUID))
        {
            // setting
            QString regionbaseUID;

            regionbaseUID = entityUID;

            // select
            view->selectRegionBase(regionbaseUID);
        }
    }
}

void QnlyComposerPlugin::addRegion(const QString regionUID,
                                   const QString parentUID,
                                   const QString regionbaseUID,
                                   const QMap<QString, QString> attributes)
{
    // setting
    QMap<QString, QString> standard;

    if (attributes.contains("id"))
        standard["id"] = attributes["id"];

    if (attributes.contains("title"))
        standard["title"] = attributes["title"];

    if (attributes.contains("top"))
        standard["top"] = attributes["top"];

    if (attributes.contains("left"))
        standard["left"] = attributes["left"];

    if (attributes.contains("bottom"))
        standard["bottom"] = attributes["bottom"];

    if (attributes.contains("right"))
        standard["right"] = attributes["right"];

    if (attributes.contains("width"))
        standard["width"] = attributes["width"];

    if (attributes.contains("height"))
        standard["height"] = attributes["height"];

    if (attributes.contains("zIndex"))
        standard["zIndex"] = attributes["zIndex"];

    // emitting
    if (!parentUID.isEmpty())
    {
        emit addEntity("region", parentUID, standard, false);
    }
    else
    {
        emit addEntity("region", regionbaseUID, standard, false);
    }
}

void QnlyComposerPlugin::removeRegion(const QString regionUID,
                                      const QString regionbaseUID)
{
    if (regions.contains(regionUID))
    {
        emit removeEntity(regions[regionUID], false);
    }
}

void QnlyComposerPlugin::changeRegion(const QString regionUID,
                                      const QString regionbaseUID,
                                      const QMap<QString, QString> attributes)
{
    if (regions.contains(regionUID))
    {
        // setting
        QMap<QString, QString> standard;

        if (attributes.contains("id"))
            standard["id"] = attributes["id"];

        if (attributes.contains("title"))
            standard["title"] = attributes["title"];

        if (attributes.contains("top"))
            standard["top"] = attributes["top"];

        if (attributes.contains("left"))
            standard["left"] = attributes["left"];

        if (attributes.contains("bottom"))
            standard["bottom"] = attributes["bottom"];

        if (attributes.contains("right"))
            standard["right"] = attributes["right"];

        if (attributes.contains("width"))
            standard["width"] = attributes["width"];

        if (attributes.contains("height"))
            standard["height"] = attributes["height"];

        if (attributes.contains("zIndex"))
            standard["zIndex"] = attributes["zIndex"];

        // emitting
        emit setAttributes(regions[regionUID], standard, false);
    }
}

void QnlyComposerPlugin::selectRegion(const QString regionUID,
                                      const QString regionbaseUID)
{
    if (!regionUID.isEmpty())
    {
        emit sendBroadcastMessage("changeSelectedEntity",
                                  new QString(regionUID));
    }
}

void QnlyComposerPlugin::addRegionBase(const QString regionbaseUID,
                   const QMap<QString, QString> attributes)
{
      // setting
    QMap<QString, QString> standard;

    if (attributes.contains("id"))
        standard["id"] = attributes["id"];

    if (attributes.contains("region"))
        standard["region"] = attributes["region"];

    if (attributes.contains("device"))
        standard["device"] = attributes["device"];

    if (getProject()->getEntitiesbyType("head").isEmpty())
    {
        if (getProject()->getEntitiesbyType("ncl").isEmpty())
        {
            QMap<QString, QString> atts;

            emit addEntity("ncl",
                           getProject()->getUniqueId(),
                           atts,
                           false);
        }

        QString nclUID =
            getProject()->getEntitiesbyType("ncl").at(0)->getUniqueId();

        QMap<QString, QString> atts;

        emit addEntity("head",
                       nclUID,
                       atts,
                       false);
    }

    QString headUID =
        getProject()->getEntitiesbyType("head").at(0)->getUniqueId();

    // emitting
    emit addEntity("regionBase", headUID, standard, false);
}

void QnlyComposerPlugin::removeRegionBase(const QString regionbaseUID)
{
    if (regionbases.contains(regionbaseUID))
    {
        emit removeEntity(regionbases[regionbaseUID], false);
    }
}

void QnlyComposerPlugin::changeRegionBase(const QString regionbaseUID,
                      const QMap<QString, QString> attributes)
{
    if (regionbases.contains(regionbaseUID))
    {
        // setting
        QMap<QString, QString> standard;

        if (attributes.contains("id"))
            standard["id"] = attributes["id"];

        if (attributes.contains("title"))
            standard["title"] = attributes["title"];

        if (attributes.contains("top"))
            standard["top"] = attributes["top"];

        if (attributes.contains("left"))
            standard["left"] = attributes["left"];

        if (attributes.contains("bottom"))
            standard["bottom"] = attributes["bottom"];

        if (attributes.contains("right"))
            standard["right"] = attributes["right"];

        if (attributes.contains("width"))
            standard["width"] = attributes["width"];

        if (attributes.contains("height"))
            standard["height"] = attributes["height"];

        if (attributes.contains("zIndex"))
            standard["zIndex"] = attributes["zIndex"];

        // emitting
        emit setAttributes(regionbases[regionbaseUID],standard, false);
    }
}

void QnlyComposerPlugin::selectRegionBase(const QString regionbaseUID)
{
    if (!regionbaseUID.isEmpty())
    {
        emit sendBroadcastMessage("changeSelectedEntity",
                                  new QString(regionbaseUID));
    }
}

} } } // end namespace

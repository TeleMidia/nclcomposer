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
#include "qnlycomposerplugin.h"

#include <QAbstractButton>
#include <QPushButton>
#include <QMessageBox>

namespace composer {
    namespace plugin {
        namespace layout {

QnlyComposerPlugin::QnlyComposerPlugin(QObject* parent)
{
    setParent(parent);

    createView();
    createConnections();

    selectedId = NULL;
}

QnlyComposerPlugin::~QnlyComposerPlugin()
{
    if(selectedId != NULL)
        delete selectedId;
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

    connect(view,
            SIGNAL(mediaOverRegionAction(QString, QString)),
            SLOT(performMediaOverRegionAction(QString, QString)));
}

void QnlyComposerPlugin::updateFromModel()
{
    clear();

    loadRegionbase();
}

void QnlyComposerPlugin::loadRegionbase()
{
    QList<Entity*> regionbaseList = getProject()->getEntitiesbyType("regionBase");

    if (!regionbaseList.isEmpty()){
        foreach(Entity* regionbaseEntity, regionbaseList){
            addRegionBaseToView(regionbaseEntity);

            QVector<Entity*> children = regionbaseEntity->getChildren();

            foreach(Entity* child, children){
                loadRegion(child);
            }
        }
    }
}

void QnlyComposerPlugin::loadRegion(Entity* region)
{
    if (region != NULL){
        addRegionToView(region);

        QVector<Entity*> children = region->getChildren();

        foreach(Entity* child, children){
            loadRegion(child);
        }
    }
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

void QnlyComposerPlugin::changeSelectedEntity (QString pluginID, void* param)
{
    //if(pluginID != this->pluginInstanceID)
    // {
    QString* entityUID = (QString*) param;

    if(entityUID != NULL)
    {
        if (regions.contains(*entityUID))
            selectRegionInView(*entityUID);
        else if (regionbases.contains(*entityUID))
            selectRegionBaseInView(*entityUID);
    }
    // }
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

            regions.remove(regionUID);

            relations.remove(entityUID);

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

            regionbases.remove(regionbaseUID);

            relations.remove(entityUID);

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
    if(selectedId != NULL)
    {
        delete selectedId;
        selectedId = NULL;
    }

    if (!regionUID.isEmpty())
    {
        selectedId = new QString(regionUID);
        emit sendBroadcastMessage("changeSelectedEntity", selectedId);
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


    QString headUID = getHeadUid();
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

void QnlyComposerPlugin::clear()
{
    while(!regions.empty()){
        Entity* regionEntity = regions.begin().value();

        qDebug() << regionEntity->getAttribute("id");

        removeRegionFromView(regionEntity->getUniqueId());
    }

    while(!regionbases.empty()){
        Entity* regionbaseEntity = regionbases.begin().value();

        removeRegionBaseFromView(regionbaseEntity->getUniqueId());
    }

    regions.clear();
    regionbases.clear();
    relations.clear();
}

void QnlyComposerPlugin::selectRegionBase(const QString regionbaseUID)
{
    if(selectedId != NULL)
    {
        delete selectedId;
        selectedId = NULL;
    }

    if (!regionbaseUID.isEmpty())
    {
        selectedId = new QString(regionbaseUID);
        emit sendBroadcastMessage("changeSelectedEntity", selectedId);
    }
}

QString QnlyComposerPlugin::getHeadUid()
{
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

    emit addEntity("head", nclUID, atts, false);
  }

  return getProject()->getEntitiesbyType("head").at(0)->getUniqueId();
}

void QnlyComposerPlugin::performMediaOverRegionAction(const QString mediaId,
                                                      const QString regionUID)
{
  bool error = false;
  Entity *region = project->getEntityById(regionUID);
  Entity *media;
  QList <Entity*> medias = project->getEntitiesbyType("media");
  for(int i = 0; i < medias.size(); i++)
  {
    if(medias.at(i)->hasAttribute("id") &&
       medias.at(i)->getAttribute("id") == mediaId)
    {
      media = medias.at(i);
      break;
    }
  }

  if(region == NULL)
  {
    qWarning() << "QnlyComposerPlugin::performMediaOverRegionAction Region does not exists. Nothing will be done." ;
    error = 1;
  }

  if(media == NULL)
  {
    qWarning() << "QnlyComposerPlugin::performMediaOverRegionAction Media was not found! Nothing will be done." ;
    error = 1;
  }

  if(!error)
  {
    QMessageBox msgBox;
    msgBox.setText(tr("Please, tell what do you want to do"));
    // \todo Enable detailed text.
//    msgBox.setDetailedText(tr("Detailed text"));

    QPushButton *createDescButton =
        msgBox.addButton( tr("Create a new descriptor"),
                          QMessageBox::ActionRole);

    QPushButton *importPropButton =
        msgBox.addButton(tr("Import region properties to media object"),
                         QMessageBox::ActionRole);
    importPropButton->setEnabled(false);

    QPushButton *cancelButton =
        msgBox.addButton(tr("Nothing!"),
                         QMessageBox::ActionRole);

//    msgBox.setIcon(QMessageBox::Question);
    msgBox.exec();
    if (msgBox.clickedButton() == createDescButton) // create a new descriptor
    {
      qDebug() << "Creating a new descriptor";
      QMap <QString,QString> attrs;
      QList <Entity*> descritorBases =
          project->getEntitiesbyType("descriptorBase");

      if(!descritorBases.size())//if does not exists any descriptorBase create one
        emit addEntity("descriptorBase", getHeadUid(), attrs, false);

      Entity *descriptorBase =
          project->getEntitiesbyType("descriptorBase").at(0);

      // create the descriptor
      QString newDescriptorID = project->generateUniqueNCLId("descriptor");
      attrs.insert("id", newDescriptorID);
      attrs.insert("region", region->getAttribute("id"));
      emit addEntity("descriptor", descriptorBase->getUniqueId(), attrs, false);

      //update the media to refer to this descriptor
      attrs.clear();
      QMap <QString, QString>::iterator begin, end, it;
      media->getAttributeIterator(begin, end);
      for (it = begin; it != end; ++it)
      {
        attrs[it.key()] = it.value();
      }
      attrs["descriptor"] = newDescriptorID;
      emit setAttributes(media, attrs, false);
    }
    // import properties from region to media element
    else if (msgBox.clickedButton() == importPropButton)
    {
      QMap <QString, QString> propertyNameToUID;
      QVector <Entity *> currentProperties = media->getChildren();
      for(int i = 0; i < currentProperties.size(); i++)
      {
        Entity *propEntity = currentProperties.at(i);
        if(propEntity->hasAttribute("name"))
        {
          propertyNameToUID.insert(propEntity->getAttribute("name"),
                                   propEntity->getUniqueId());
        }
      }

      qDebug() << "Import attributes as properties of media element.";
      QMap <QString, QString>::iterator begin, end, it;
      region->getAttributeIterator(begin, end);
      for (it = begin; it != end; ++it)
      {
        if(it.key() == "id" || it.key() == "title")
          continue; //this attributes will not became a property of the media

        QMap <QString, QString> attrs;
        attrs.insert("name", it.key());
        attrs.insert("value", it.value());

        if(propertyNameToUID.keys().contains(it.key()))
        {
          QString propUID = propertyNameToUID.value(it.key());
          emit setAttributes(project->getEntityById(propUID), attrs, false);
        }
        else
          emit addEntity("property", media->getUniqueId(), attrs, false);
      }
    }
  }
}

} } } // end namespace

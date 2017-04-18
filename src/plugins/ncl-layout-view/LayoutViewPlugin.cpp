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
#include "LayoutViewPlugin.h"

#include <QAbstractButton>
#include <QPushButton>
#include <QMessageBox>
#include <QInputDialog>
#include <QToolBar>

NCLLayoutViewPlugin::NCLLayoutViewPlugin(QObject* parent)
{
  setParent(parent);

  createView();
  createConnections();

  selectedId = NULL;
}

NCLLayoutViewPlugin::~NCLLayoutViewPlugin()
{
  if(selectedId != NULL)
    delete selectedId;
  delete(view);
}

void NCLLayoutViewPlugin::createView()
{
  mainWindow = new LayoutWindow();

  view = new LayoutView(mainWindow);
  mainWindow->setQnlyView(view);
}

void NCLLayoutViewPlugin::createConnections()
{
  /* ADD */
  connect(view,
          SIGNAL(regionAdded(QString,QString,QString,QMap<QString,QString>)),
          SLOT(addRegion(QString,QString,QString,QMap<QString,QString>)));

  connect(view,
          SIGNAL(regionBaseAdded(QString,QMap<QString,QString>)),
          SLOT(addRegionBase(QString,QMap<QString,QString>)));

  /* REMOVE */
  connect(view,
          SIGNAL(regionRemoved(QString,QString)),
          SLOT(removeRegion(QString,QString)));

  connect(view,
          SIGNAL(regionBaseRemoved(QString)),
          SLOT(removeRegionBase(QString)));

  /* CHANGE */
  connect(view,
          SIGNAL(regionChanged(QString,QString,QMap<QString,QString>)),
          SLOT(changeRegion(QString,QString,QMap<QString,QString>)));

  connect(view,
          SIGNAL(regionBaseChanged(QString,QMap<QString,QString>)),
          SLOT(changeRegionBase(QString,QMap<QString,QString>)));

  /* SELECTION */
  connect(view,
          SIGNAL(regionSelected(QString,QString)),
          SLOT(selectRegion(QString,QString)));

  connect(view,
          SIGNAL(regionBaseSelected(QString)),
          SLOT(selectRegionBase(QString)));

  connect(mainWindow,
          SIGNAL(regionBaseSelectedFromComboBox(QString)),
          SLOT(selectRegionBase(QString)));

  connect(mainWindow,
          SIGNAL(createNewRegionBase(QString,QMap<QString,QString>)),
          SLOT(addRegionBase(QString,QMap<QString,QString>)));

  /* OTHERS */
  connect(view,
          SIGNAL(mediaOverRegionAction(QString, QString)),
          SLOT(performMediaOverRegionAction(QString, QString)));
}

void NCLLayoutViewPlugin::updateFromModel()
{
  QRectF previousRect;
  bool isGridVisible = false;

  LayoutRegionBase *currentRegionBase = view->getSelectedRegionBase();
  if(currentRegionBase != nullptr)
  {
    previousRect = currentRegionBase->sceneRect();
    isGridVisible = currentRegionBase->isGridVisible();
  }

  clear();

  loadRegionbase();

  currentRegionBase = view->getSelectedRegionBase();
  if(currentRegionBase != nullptr)
  {
    currentRegionBase->changeResolution(previousRect.width(),
                                        previousRect.height());
    currentRegionBase->setGridVisible(isGridVisible);
  }
}

void NCLLayoutViewPlugin::loadRegionbase()
{
  QList<Entity*> regionbaseList =
      getProject()->getEntitiesbyType("regionBase");

  if (!regionbaseList.isEmpty())
  {
    foreach(Entity* regionbaseEntity, regionbaseList)
    {
      addRegionBaseToView(regionbaseEntity);

      QVector<Entity*> children = regionbaseEntity->getChildren();

      foreach(Entity* child, children)
      {
        loadRegion(child);
      }
    }
  }
}

void NCLLayoutViewPlugin::loadRegion(Entity* region)
{
  if (region != NULL)
  {
    addRegionToView(region);

    QVector<Entity*> children = region->getChildren();
    foreach(Entity* child, children)
    {
      loadRegion(child);
    }
  }
}

QWidget* NCLLayoutViewPlugin::getWidget()
{
  return mainWindow;
}

bool NCLLayoutViewPlugin::saveSubsession()
{
  QByteArray data;
  QString line;
  QVariant gridVisible(QVariant::Bool),
      resolutionWidth(QVariant::Int),
      resolutionHeight(QVariant::Int);

  if(view->getSelectedRegionBase() != NULL)
  {
    gridVisible.setValue(
          view->getSelectedRegionBase()->isGridVisible());
    resolutionWidth.setValue(
          view->getSelectedRegionBase()->sceneRect().width());
    resolutionHeight.setValue(
          view->getSelectedRegionBase()->sceneRect().height());
  }
  else
  {
    gridVisible.setValue(false);
    resolutionWidth.setValue(0);
    resolutionHeight.setValue(0);
  }

  line += QString("gridVisible=") + gridVisible.toString() + "\n";
  line += QString("resolutionWidth=") + resolutionWidth.toString() + "\n";
  line += QString("resolutionHeight=") + resolutionHeight.toString() + "\n";
  data.append(line);
  qDebug() << "[QNLY] saveSubsession() data is " << data;
  emit setPluginData(data);
  return true;
}

void NCLLayoutViewPlugin::init()
{
  // \todo Load specific contents.
  QString data = project->getPluginData("br.puc-rio.telemidia.qncllayout");
  qDebug() << "[QNLY] data = " << data;
  QStringList lines = data.split("\n");
  bool gridVisible = false, ok = true;
  int resolutionWidth = 0, resolutionHeight = 0;

  for(int i = 0; i < lines.size(); i++)
  {
    QStringList list = lines[i].split("=");
    for(int j = 0; !(list.size()%2) && j < list.size(); j += 2)
    {
      QString key = list[j];
      QString value = list[j+1];
      if(key == "gridVisible")
      {
        qDebug() << "[QNLY] gridVisible = " << value;
        if(value == "true")
          gridVisible = true;
      }
      else if(key == "resolutionWidth")
      {
        resolutionWidth = value.toInt(&ok);
        if(!ok) resolutionWidth = 0;
      }
      else if(key == "resolutionHeight")
      {
        resolutionHeight = value.toInt(&ok);
        if(!ok) resolutionHeight = 0;
      }
    }
  }

  qDebug() << resolutionWidth << resolutionHeight;
  // Update layout model from core model.
  QStack <Entity*> stack;
  stack.push(project);

  while(stack.size())
  {
    Entity *current = stack.top();
    stack.pop();

    if(current->getType() == "regionBase")
    {
      addRegionBaseToView(current);

      // \todo In the future we should support saving individual
      // resolutions foreach regionBase
      if (view->getSelectedRegion())
        view->getSelectedRegionBase()->changeResolution(resolutionWidth, resolutionHeight);
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

  view->setGridVisible(gridVisible);
}

void NCLLayoutViewPlugin::errorMessage(const QString &error)
{
  Q_UNUSED(error)
  //TODO: void QnlyComposerPlugin::errorMessage(QString error)
}

void NCLLayoutViewPlugin::onEntityAdded(const QString &pluginID, Entity *entity)
{
  Q_UNUSED(pluginID)

  if (entity != NULL)
  {
    if (entity->getType() == "region")
    {
      addRegionToView(entity);
      updateDescriptors();
    }
    else if (entity->getType() == "regionBase")
    {
      addRegionBaseToView(entity);
    }
    else if (entity->getType() == "descriptor")
    {
      addDescriptorToView(entity);
      updateDescriptors();
    }
  }
}

void NCLLayoutViewPlugin::onEntityRemoved( const QString &pluginID,
                                           const QString &entityID )
{
  Q_UNUSED(pluginID)

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
    else if (descriptors.contains(entityID))
    {
      removeDescriptorFromView(entityID);
      updateDescriptors();
    }
  }
}

void NCLLayoutViewPlugin::onEntityChanged(const QString &pluginID, Entity *entity)
{
  Q_UNUSED(pluginID)

  if (entity != NULL)
  {
    if (entity->getType() == "region")
    {
      changeRegionInView(entity);
      updateDescriptors();
    }
    else if (entity->getType() == "regionBase")
    {
      changeRegionBaseInView(entity);
    }
    else if (entity->getType() == "descriptor")
    {
      addDescriptorToView(entity);
      updateDescriptors();
    }
  }
}

void NCLLayoutViewPlugin::changeSelectedEntity (const QString &pluginID, void* param)
{
  Q_UNUSED(pluginID)

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

void NCLLayoutViewPlugin::addRegionToView(Entity* entity)
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

      QString regionUID = entityUID;

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

void NCLLayoutViewPlugin::removeRegionFromView(QString entityUID)
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

void NCLLayoutViewPlugin::changeRegionInView(Entity* entity)
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

void NCLLayoutViewPlugin::selectRegionInView(QString entityUID)
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

void NCLLayoutViewPlugin::addRegionBaseToView(Entity* entity)
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

      view->addRegionBase(regionbaseUID, attributes);
      mainWindow->addRegionBaseToCombobox(regionbaseUID, attributes);
      mainWindow->selectRegionBaseInComboBox(regionbaseUID);
    }
  }
}

void NCLLayoutViewPlugin::removeRegionBaseFromView(QString entityUID)
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

      regionbases.remove(regionbaseUID);

      relations.remove(entityUID);

      mainWindow->removeRegionBaseFromCombobox(regionbaseUID);
    }
  }
}

void NCLLayoutViewPlugin::changeRegionBaseInView(Entity* entity)
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

      mainWindow->updateRegionBaseInComboBox(regionbaseUID, attributes);
    }
  }
}

void NCLLayoutViewPlugin::selectRegionBaseInView(QString entityUID)
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

      mainWindow->selectRegionBaseInComboBox(regionbaseUID);
    }
  }
}

void NCLLayoutViewPlugin::addDescriptorToView(Entity* entity)
{
  if (entity != NULL)
  {
    if (entity->getType() == "descriptor" && !entity->getAttribute("id").isEmpty() && !entity->getAttribute("region").isEmpty())
    {
      //QList<Entity*> model_regions = project->getEntitiesbyType("region");
      QMap<QString, QString> descriptorsIDs;
      descriptorsIDs[entity->getAttribute("region")] = entity->getAttribute("id");
      descriptors[entity->getUniqueId()] = descriptorsIDs;
      /*
      for(Entity *rg : regions.values())
      {
        if (entity->getAttribute("region") == rg->getAttribute("id"))
        {
          view->addDescriptor(rg->getUniqueId(),entity->getAttribute("id"));
        }
      }
      */ //is this step done twice if updateDescriptors is after add?
    }
  }
}

void NCLLayoutViewPlugin::removeDescriptorFromView(const QString &entityUID)
{
  if(descriptors.contains(entityUID))
  {
    /*
    for(Entity *rg : regions.values())
    {
      if (descriptors[entityUID].contains(rg->getAttribute("id")))
      {
        view->addDescriptor(rg->getUniqueId(),"");// remove descriptor id from view, passing ""
      }
    }*/ //done twice if used before updateDescriptors?
    descriptors.remove(entityUID);
  }
}

void NCLLayoutViewPlugin::updateDescriptors()
{
  for(Entity *rg : regions.values())
  {
    bool visited = false;
    for(QMap<QString,QString> descriptorsID : descriptors.values())
    {
      if(descriptorsID.contains(rg->getAttribute("id")) )
      {
        view->addDescriptor(rg->getUniqueId(),descriptorsID.value(rg->getAttribute("id")));
        visited = true;
      }
    }
    if(!visited)
    {
      view->addDescriptor(rg->getUniqueId(),"");// remove descriptor id from view, passing ""
    }
  }
}

void NCLLayoutViewPlugin::addRegion(const QString &regionUID,
                                    const QString &parentUID,
                                    const QString &regionbaseUID,
                                    const QMap<QString, QString> &attributes)
{
  Q_UNUSED(regionUID)

  // setting
  QMap<QString, QString> standard;

  if (attributes.contains("id"))
    standard["id"] = attributes["id"];
  else
    standard["id"] = project->generateUniqueNCLId("region");

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
    emit addEntity("region", parentUID, standard, false);
  else
    emit addEntity("region", regionbaseUID, standard, false);
}

void NCLLayoutViewPlugin::removeRegion(const QString &regionUID,
                                       const QString &regionbaseUID)
{
  Q_UNUSED(regionbaseUID)

  if (regions.contains(regionUID))
  {
    emit removeEntity(regions[regionUID], false);
  }
}

void NCLLayoutViewPlugin::changeRegion(const QString &regionUID,
                                       const QString &regionbaseUID,
                                       const QMap<QString, QString> &attributes)
{
  Q_UNUSED(regionbaseUID)

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

void NCLLayoutViewPlugin::selectRegion(const QString &regionUID,
                                       const QString &regionbaseUID)
{
  Q_UNUSED(regionbaseUID)

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

void NCLLayoutViewPlugin::addRegionBase(const QString &regionbaseUID,
                                        const QMap<QString, QString> &attributes)
{
  Q_UNUSED(regionbaseUID)

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

void NCLLayoutViewPlugin::removeRegionBase(const QString &regionbaseUID)
{
  if (regionbases.contains(regionbaseUID))
  {
    mainWindow->removeRegionBaseFromCombobox(regionbaseUID);

    emit removeEntity(regionbases[regionbaseUID], false);
  }
}

void NCLLayoutViewPlugin::changeRegionBase(
    const QString &regionbaseUID,
    const QMap<QString, QString> &attributes)
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

    mainWindow->updateRegionBaseInComboBox(regionbaseUID, attributes);
  }
}

void NCLLayoutViewPlugin::clear()
{
  while(!regions.empty())
  {
    Entity* regionEntity = regions.begin().value();

    qDebug() << regionEntity->getAttribute("id");

    removeRegionFromView(regionEntity->getUniqueId());
  }

  while(!regionbases.empty())
  {
    Entity* regionbaseEntity = regionbases.begin().value();

    removeRegionBaseFromView(regionbaseEntity->getUniqueId());
  }

  regions.clear();
  regionbases.clear();
  relations.clear();
}

void NCLLayoutViewPlugin::selectRegionBase(const QString &regionbaseUID)
{
  if(selectedId != NULL)
  {
    if(*selectedId == regionbaseUID)
      return; // Do nothing! It is already selected

    delete selectedId;
    selectedId = NULL;
  }

  if (!regionbaseUID.isEmpty())
  {
    selectedId = new QString(regionbaseUID);
    mainWindow->selectRegionBaseInComboBox(regionbaseUID);

    emit sendBroadcastMessage("changeSelectedEntity", selectedId);
  }
}

QString NCLLayoutViewPlugin::getHeadUid()
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

QMap <QString, QString> NCLLayoutViewPlugin::getRegionAttributes(Entity *region)
{
  QMap <QString, QString>::iterator begin, end, it;
  QVector <double> widths;
  QVector <double> heights;
  QVector <double> tops;
  QVector <double> lefts;

  double top = 1.0, left = 1.0;
  double cvalue = 0.0;

  Entity *currentRegion = region;

  // Here, we will compose all the hierachy values to get the final values of
  // left, top, width and height.
  while(currentRegion != NULL && currentRegion->getType() == "region")
  {
    currentRegion->getAttributeIterator(begin, end);
    for (it = begin; it != end; ++it)
    {
      QString name = it.key();
      QString value = it.value();
      if(value.endsWith("%"))
        cvalue = value.mid(0, value.indexOf("%")).toDouble();
      else
        cvalue = 100.0;

      cvalue /= 100.0;

      if(name == "width") widths.push_back(cvalue);
      else if(name == "height") heights.push_back(cvalue);
      else if(name == "top") tops.push_back(cvalue);
      else if(name == "left") lefts.push_back(cvalue);
    }
    currentRegion = currentRegion->getParent();
  }

  int i;
  //width
  heights.push_back(1.0f);
  widths.push_back(1.0f);

  for(i = widths.size()-1; i > 0; --i)
    widths[i-1] *= widths[i];
  //height
  for(i = heights.size()-1; i > 0; --i)
    heights[i-1] *= heights[i];

  //left
  left = top = 0.0;
  for(i = lefts.size()-1; i >= 0; --i)
    left += lefts[i] * widths[i+1];

  // qDebug() << tops;
  for(i = tops.size()-1; i >= 0; --i)
    top += tops[i] * heights[i+1];

  QMap <QString, QString> attrs;
  attrs.insert( "width",  QString::number(widths[0]*100.0)+"%" );
  attrs.insert( "height", QString::number(heights[0]*100.0)+"%" );
  attrs.insert( "left",   QString::number(left*100.0)+"%" );
  attrs.insert( "top",    QString::number(top*100.0)+"%" );

  // Add also the zIndex to the list of properties.
  if(region->hasAttribute("zIndex"))
    attrs.insert("zIndex", region->getAttribute("zIndex"));
  else
    attrs.insert("zIndex", "0");

  return attrs;
}

void NCLLayoutViewPlugin::performMediaOverRegionAction(const QString &mediaId,
                                                       const QString &regionUID)
{
  bool error = false;
  Entity *region = project->getEntityById(regionUID);
  Entity *media = NULL;
  QList <Entity*> medias = project->getEntitiesbyType("media");

  const bool askDescOrProperties = true; // \todo This must be a preference.

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
    qWarning() << "QnlyComposerPlugin::performMediaOverRegionAction Region\
                  does not exists. Nothing will be done." ;
    error = 1;
  }

  if(media == NULL)
  {
    int ret = QMessageBox::warning(this->getWidget(), tr("Error"),
                                   tr("Sorry. \"") + mediaId +
                                   tr("\" is not a valid media."),
                                   QMessageBox::Ok,
                                   QMessageBox::Ok);

    Q_UNUSED(ret)
    error = 1;
  }

  if(!error)
  {
    if(askDescOrProperties)
    {
      QMessageBox msgBox;
      msgBox.setText(tr("Please, tell what do you want to do"));
      // \todo Enable detailed text.
      //    msgBox.setDetailedText(tr("Detailed text"));

      QPushButton *createDescButton =
          msgBox.addButton( tr("Use a descriptor"),
                            QMessageBox::ActionRole);

      QPushButton *importPropButton =
          msgBox.addButton(tr("Import region properties to media object"),
                           QMessageBox::ActionRole);
      //  importPropButton->setEnabled(false);

      QPushButton *cancelButton =
          msgBox.addButton(tr("Nothing!"),
                           QMessageBox::ActionRole);
      Q_UNUSED(cancelButton)

      //  msgBox.setIcon(QMessageBox::Question);
      msgBox.exec();
      if (msgBox.clickedButton() == createDescButton) // create a new descriptor
      {
        qDebug() << "Creating a new descriptor";
        QMap <QString,QString> attrs;
        QList <Entity*> descritorBases =
            project->getEntitiesbyType("descriptorBase");

        // create the descriptor
        QString newDescriptorID = project->generateUniqueNCLId("descriptor");
        QStringList alredyExistentsDescriptorsIds, descriptorsIds;
        descriptorsIds << newDescriptorID;

        QList <Entity*> descriptors = project->getEntitiesbyType("descriptor");
        foreach (Entity *desc, descriptors) {
          if(desc != NULL)
          {
            if(desc->getAttribute("region") == region->getAttribute("id"))
            {
              alredyExistentsDescriptorsIds << desc->getAttribute("id");
              descriptorsIds << desc->getAttribute("id");
            }
          }
        }

        bool ok;
        newDescriptorID = QInputDialog::getItem(NULL,
                                                tr("Descriptor id:"),
                                                tr("Please, enter the "
                                                   "descriptor id"),
                                                descriptorsIds,
                                                0,
                                                true,
                                                &ok);

        if(ok)
        {
          if(!descritorBases.size()) // if does not exists any descriptorBase
            // create one
            emit addEntity("descriptorBase", getHeadUid(), attrs, false);

          // If we choose the new descriptor (create it)
          if(!alredyExistentsDescriptorsIds.contains(newDescriptorID))
          {
            Entity *descriptorBase =
                project->getEntitiesbyType("descriptorBase").at(0);
            attrs.insert("id", newDescriptorID);
            attrs.insert("region", region->getAttribute("id"));

            emit addEntity("descriptor", descriptorBase->getUniqueId(), attrs,
                           false);
          }

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
        QMap <QString, QString> newAttrs = getRegionAttributes(region);
        QString key;
        foreach(key, newAttrs.keys())
        {
          QMap <QString, QString> attrs;
          attrs.insert("name", key);
          attrs.insert("value", newAttrs.value(key));

          if(propertyNameToUID.keys().contains(key))
          {
            QString propUID = propertyNameToUID.value(key);
            emit setAttributes(project->getEntityById(propUID), attrs, false);
          }
          else
            emit addEntity("property", media->getUniqueId(), attrs, false);
        }
      }
    }
    else
    {
      // Does not ask the descriptor or media properties
      qDebug() << "Creating a new descriptor";

      QMap <QString,QString> attrs;
      QList <Entity*> descritorBases =
          project->getEntitiesbyType("descriptorBase");

      // create the descriptor
      QString newDescriptorID = project->generateUniqueNCLId("descriptor");

      if(!descritorBases.size()) // if does not exists any descriptorBase
        // create one
        emit addEntity("descriptorBase", getHeadUid(), attrs, false);

      Entity *descriptorBase =
          project->getEntitiesbyType("descriptorBase").at(0);
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

      if(media->hasAttribute("type") &&
         media->getAttribute("type") == "text/html")
      {
        attrs.clear();
        QMap <QString, QString>::iterator begin, end, it;
        QList <Entity*> descriptors = project->getEntityByAttrId(newDescriptorID);

        if(descriptors.size())
        {
          Entity *desc = descriptors.at(0);
          desc->getAttributeIterator(begin, end);
          for (it = begin; it != end; ++it)
          {
            attrs[it.key()] = it.value();
          }
          attrs["focusIndex"] = "1";
          if(descriptors.size())
            emit setAttributes(desc, attrs, false);
        }
      }
    }
  }
}

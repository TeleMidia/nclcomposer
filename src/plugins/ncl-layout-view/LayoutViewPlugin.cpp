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

#include "LayoutViewPlugin_global.h"

#include <QAbstractButton>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QToolBar>

Q_LOGGING_CATEGORY (CPR_PLUGIN_LAYOUT, "cpr.plugin.layout")

NCLLayoutViewPlugin::NCLLayoutViewPlugin (QObject *parent)
{
  setParent (parent);

  createView ();
  createConnections ();

  _selectedId = NULL;
}

NCLLayoutViewPlugin::~NCLLayoutViewPlugin ()
{
  if (_selectedId != nullptr)
    delete _selectedId;
  delete (_view);
}

void
NCLLayoutViewPlugin::createView ()
{
  _mainWindow = new LayoutWindow ();

  _view = new LayoutView (_mainWindow);
  _mainWindow->setQnlyView (_view);
}

void
NCLLayoutViewPlugin::createConnections ()
{
  /* ADD */
  connect (
      _view,
      SIGNAL (regionAdded (QString, QString, QString, QMap<QString, QString>)),
      SLOT (addRegion (QString, QString, QString, QMap<QString, QString>)));

  connect (_view, SIGNAL (regionBaseAdded (QString, QMap<QString, QString>)),
           SLOT (addRegionBase (QString, QMap<QString, QString>)));

  /* REMOVE */
  connect (_view, SIGNAL (regionRemoved (QString, QString)),
           SLOT (removeRegion (QString, QString)));

  connect (_view, SIGNAL (regionBaseRemoved (QString)),
           SLOT (removeRegionBase (QString)));

  /* CHANGE */
  connect (_view,
           SIGNAL (regionChanged (QString, QString, QMap<QString, QString>)),
           SLOT (changeRegion (QString, QString, QMap<QString, QString>)));

  connect (_view, SIGNAL (regionBaseChanged (QString, QMap<QString, QString>)),
           SLOT (changeRegionBase (QString, QMap<QString, QString>)));

  /* SELECTION */
  connect (_view, SIGNAL (regionSelected (QString, QString)),
           SLOT (selectRegion (QString, QString)));

  connect (_view, SIGNAL (regionBaseSelected (QString)),
           SLOT (selectRegionBase (QString)));

  connect (_mainWindow, SIGNAL (regionBaseSelectedFromComboBox (QString)),
           SLOT (selectRegionBase (QString)));

  connect (_mainWindow,
           SIGNAL (createNewRegionBase (QString, QMap<QString, QString>)),
           SLOT (addRegionBase (QString, QMap<QString, QString>)));

  /* OTHERS */
  connect (_view, SIGNAL (mediaOverRegionAction (QString, QString)),
           SLOT (performMediaOverRegionAction (QString, QString)));
}

void
NCLLayoutViewPlugin::updateFromModel ()
{
  QRectF previousRect;
  bool isGridVisible = false;

  LayoutRegionBase *currentRegionBase = _view->getSelectedRegionBase ();
  if (currentRegionBase != nullptr)
  {
    previousRect = currentRegionBase->sceneRect ();
    isGridVisible = currentRegionBase->isGridVisible ();
  }

  clear ();

  loadRegionbase ();

  currentRegionBase = _view->getSelectedRegionBase ();
  if (currentRegionBase != nullptr)
  {
    currentRegionBase->changeResolution (previousRect.width (),
                                         previousRect.height ());
    currentRegionBase->setGridVisible (isGridVisible);
  }
}

void
NCLLayoutViewPlugin::loadRegionbase ()
{
  QList<Entity *> regionbaseList
      = project ()->getEntitiesbyType ("regionBase");

  if (!regionbaseList.isEmpty ())
  {
    foreach (Entity *regionbaseEntity, regionbaseList)
    {
      addRegionBaseToView (regionbaseEntity);

      QList<Entity *> children = regionbaseEntity->entityChildren ();

      foreach (Entity *child, children)
      {
        loadRegion (child);
      }
    }
  }
}

void
NCLLayoutViewPlugin::loadRegion (Entity *region)
{
  if (region != NULL)
  {
    addRegionToView (region);

    foreach (Entity *child, region->entityChildren ())
    {
      loadRegion (child);
    }
  }
}

QWidget *
NCLLayoutViewPlugin::widget ()
{
  return _mainWindow;
}

bool
NCLLayoutViewPlugin::saveSubsession ()
{
  QByteArray data;
  QString line;
  QVariant gridVisible (QVariant::Bool), resolutionWidth (QVariant::Int),
      resolutionHeight (QVariant::Int);

  if (_view->getSelectedRegionBase () != NULL)
  {
    gridVisible.setValue (_view->getSelectedRegionBase ()->isGridVisible ());
    resolutionWidth.setValue (
        _view->getSelectedRegionBase ()->sceneRect ().width ());
    resolutionHeight.setValue (
        _view->getSelectedRegionBase ()->sceneRect ().height ());
  }
  else
  {
    gridVisible.setValue (false);
    resolutionWidth.setValue (0);
    resolutionHeight.setValue (0);
  }

  line += QString ("gridVisible=") + gridVisible.toString () + "\n";
  line += QString ("resolutionWidth=") + resolutionWidth.toString () + "\n";
  line += QString ("resolutionHeight=") + resolutionHeight.toString () + "\n";
  data.append (line);

  emit setPluginData (data);

  return true;
}

void
NCLLayoutViewPlugin::init ()
{
  // \todo Load specific contents.
  QString data = project ()->getPluginData ("br.puc-rio.telemidia.qncllayout");
  QStringList lines = data.split ("\n");

  bool gridVisible = false, ok = true;
  int resolutionWidth = 0, resolutionHeight = 0;

  for (int i = 0; i < lines.size (); i++)
  {
    QStringList list = lines[i].split ("=");
    for (int j = 0; !(list.size () % 2) && j < list.size (); j += 2)
    {
      QString key = list[j];
      QString value = list[j + 1];
      if (key == "gridVisible")
      {
        if (value == "true")
          gridVisible = true;
      }
      else if (key == "resolutionWidth")
      {
        resolutionWidth = value.toInt (&ok);
        if (!ok)
          resolutionWidth = 0;
      }
      else if (key == "resolutionHeight")
      {
        resolutionHeight = value.toInt (&ok);
        if (!ok)
          resolutionHeight = 0;
      }
    }
  }

  // Update layout model from core model.
  QStack<Entity *> stack;
  stack.push (project ());

  while (stack.size ())
  {
    Entity *current = stack.top ();
    stack.pop ();

    if (current->type () == "regionBase")
    {
      addRegionBaseToView (current);

      // \todo In the future we should support saving individual
      // resolutions foreach regionBase
      if (_view->getSelectedRegion ())
        _view->getSelectedRegionBase ()->changeResolution (resolutionWidth,
                                                           resolutionHeight);
    }
    else if (current->type () == "region")
    {
      addRegionToView (current);
    }

    foreach (Entity *child, current->entityChildren ())
    {
      stack.push (child);
    }
  }

  _view->setGridVisible (gridVisible);
}

void
NCLLayoutViewPlugin::errorMessage (const QString &error)
{
  Q_UNUSED (error)
  // TODO: void QnlyComposerPlugin::errorMessage(QString error)
}

void
NCLLayoutViewPlugin::onEntityAdded (const QString &pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)

  if (entity != NULL)
  {
    if (entity->type () == "region")
    {
      addRegionToView (entity);
      updateDescriptors ();
    }
    else if (entity->type () == "regionBase")
    {
      addRegionBaseToView (entity);
    }
    else if (entity->type () == "descriptor")
    {
      addDescriptorToView (entity);
      updateDescriptors ();
    }
  }
}

void
NCLLayoutViewPlugin::onEntityRemoved (const QString &pluginID,
                                      const QString &entityID)
{
  Q_UNUSED (pluginID)

  if (!entityID.isEmpty ())
  {
    if (_regions.contains (entityID))
    {
      removeRegionFromView (entityID);
    }
    else if (_regionbases.contains (entityID))
    {
      removeRegionBaseFromView (entityID);
    }
    else if (_descriptors.contains (entityID))
    {
      removeDescriptorFromView (entityID);
      updateDescriptors ();
    }
  }
}

void
NCLLayoutViewPlugin::onEntityChanged (const QString &pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)

  if (entity != NULL)
  {
    if (entity->type () == "region")
    {
      changeRegionInView (entity);
      updateDescriptors ();
    }
    else if (entity->type () == "regionBase")
    {
      changeRegionBaseInView (entity);
    }
    else if (entity->type () == "descriptor")
    {
      addDescriptorToView (entity);
      updateDescriptors ();
    }
  }
}

void
NCLLayoutViewPlugin::changeSelectedEntity (const QString &pluginID,
                                           void *param)
{
  Q_UNUSED (pluginID)

  // if(pluginID != this->pluginInstanceID)
  // {
  QString *entityUID = (QString *)param;

  if (entityUID != NULL)
  {
    if (_regions.contains (*entityUID))
      selectRegionInView (*entityUID);
    else if (_regionbases.contains (*entityUID))
      selectRegionBaseInView (*entityUID);
  }
  // }
}

void
NCLLayoutViewPlugin::addRegionToView (Entity *entity)
{
  if (entity != NULL)
  {
    if (entity->type () == "region")
    {
      // setting
      QString entityUID;

      if (!entity->uid ().isEmpty ())
      {
        entityUID = entity->uid ();
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT)
            << "QnlyComposerPlugin::addRegion:"
            << "Tried to add a region with empty UID.";

        return; // abort addition
      }

      QString regionUID = entityUID;

      QString parentUID;

      if (_regions.contains (entity->parentUid ()))
      {
        parentUID = entity->parentUid ();
      }
      else
      {
        parentUID = "";
      }

      QString regionbaseUID;

      if (_regions.contains (parentUID))
      {
        regionbaseUID = _relations[parentUID];
      }
      else if (_regionbases.contains (entity->parentUid ()))
      {
        regionbaseUID = entity->parentUid ();
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT)
            << "QnlyComposerPlugin::addRegion:"
            << "Tried to add a region without regionbase UID.";

        return; // abort addition
      }

      QMap<QString, QString> attributes;

      if (!entity->attr ("id").isEmpty ())
      {
        attributes["id"] = entity->attr ("id");
      }

      if (!entity->attr ("title").isEmpty ())
      {
        attributes["title"] = entity->attr ("title");
      }

      if (!entity->attr ("zIndex").isEmpty ())
      {
        attributes["zIndex"] = entity->attr ("zIndex");
      }

      if (!entity->attr ("top").isEmpty ())
      {
        attributes["top"] = entity->attr ("top");
      }

      if (!entity->attr ("left").isEmpty ())
      {
        attributes["left"] = entity->attr ("left");
      }

      if (!entity->attr ("bottom").isEmpty ())
      {
        attributes["bottom"] = entity->attr ("bottom");
      }

      if (!entity->attr ("right").isEmpty ())
      {
        attributes["right"] = entity->attr ("right");
      }

      if (!entity->attr ("width").isEmpty ())
      {
        attributes["width"] = entity->attr ("width");
      }

      if (!entity->attr ("height").isEmpty ())
      {
        attributes["height"] = entity->attr ("height");
      }

      // adding
      _regions[regionUID] = entity;

      _relations[regionUID] = regionbaseUID;

      _view->addRegion (regionUID, parentUID, regionbaseUID, attributes);
    }
  }
}

void
NCLLayoutViewPlugin::removeRegionFromView (QString entityUID)
{
  if (!entityUID.isEmpty ())
  {
    if (_regions.contains (entityUID))
    {
      // setting
      QString regionUID;

      regionUID = entityUID;

      QString regionbaseUID;

      if (_relations.contains (regionUID))
      {
        regionbaseUID = _relations[regionUID];
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT) << "QnlyComposerPlugin::removeRegion:"
                                      << "Tried to remove a region without"
                                      << "regionbase UID.";

        return; // abort remotion
      }

      _regions.remove (regionUID);

      _relations.remove (entityUID);

      // removing
      _view->removeRegion (regionUID, regionbaseUID);
    }
  }
}

void
NCLLayoutViewPlugin::changeRegionInView (Entity *entity)
{
  if (entity != NULL)
  {
    if (entity->type () == "region")
    {
      // setting
      QString entityUID;

      if (!entity->uid ().isEmpty ())
      {
        entityUID = entity->uid ();
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT)
            << "QnlyComposerPlugin::addRegion:"
            << "Tried to add an region with empty UID.";

        return; // abort addition
      }

      QString regionUID;

      regionUID = entityUID;

      QString parentUID;

      if (_regions.contains (entity->parentUid ()))
      {
        parentUID = entity->parentUid ();
      }
      else
      {
        parentUID = "";
      }

      QString regionbaseUID;

      if (_regions.contains (parentUID))
      {
        regionbaseUID = _relations[parentUID];
      }
      else if (_regionbases.contains (entity->parentUid ()))
      {
        regionbaseUID = entity->parentUid ();
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT) << "QnlyComposerPlugin::addRegion:"
                                      << "Tried to add an region without"
                                      << "regionbase UID.";

        return; // abort addition
      }

      QMap<QString, QString> attributes;

      if (!entity->attr ("id").isEmpty ())
      {
        attributes["id"] = entity->attr ("id");
      }

      if (!entity->attr ("title").isEmpty ())
      {
        attributes["title"] = entity->attr ("title");
      }

      if (!entity->attr ("zIndex").isEmpty ())
      {
        attributes["zIndex"] = entity->attr ("zIndex");
      }

      if (!entity->attr ("top").isEmpty ())
      {
        attributes["top"] = entity->attr ("top");
      }

      if (!entity->attr ("left").isEmpty ())
      {
        attributes["left"] = entity->attr ("left");
      }

      if (!entity->attr ("bottom").isEmpty ())
      {
        attributes["bottom"] = entity->attr ("bottom");
      }

      if (!entity->attr ("right").isEmpty ())
      {
        attributes["right"] = entity->attr ("right");
      }

      if (!entity->attr ("width").isEmpty ())
      {
        attributes["width"] = entity->attr ("width");
      }

      if (!entity->attr ("height").isEmpty ())
      {
        attributes["height"] = entity->attr ("height");
      }

      // change
      _view->changeRegion (regionUID, regionbaseUID, attributes);
    }
  }
}

void
NCLLayoutViewPlugin::selectRegionInView (QString entityUID)
{
  if (!entityUID.isEmpty ())
  {
    if (_regions.contains (entityUID))
    {
      // setting
      QString regionUID;

      regionUID = entityUID;

      QString regionbaseUID;

      if (_relations.contains (regionUID))
      {
        regionbaseUID = _relations[regionUID];
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT) << "QnlyComposerPlugin::selectRegion:"
                                      << "Tried to select a region without"
                                      << "regionbase UID.";

        return; // abort remotion
      }

      // select
      _view->selectRegion (regionUID, regionbaseUID);
    }
  }
}

void
NCLLayoutViewPlugin::addRegionBaseToView (Entity *entity)
{
  if (entity != NULL)
  {
    if (entity->type () == "regionBase")
    {
      // setting
      QString entityUID;

      if (!entity->uid ().isEmpty ())
      {
        entityUID = entity->uid ();
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT)
            << "QnlyComposerPlugin::addRegion:"
            << "Tried to add a region with empty UID.";

        return; // abort addition
      }

      QString regionbaseUID;

      regionbaseUID = entityUID;

      QMap<QString, QString> attributes;

      if (!entity->attr ("id").isEmpty ())
      {
        attributes["id"] = entity->attr ("id");
      }

      if (!entity->attr ("region").isEmpty ())
      {
        attributes["region"] = entity->attr ("region");
      }

      if (!entity->attr ("device").isEmpty ())
      {
        attributes["device"] = entity->attr ("device");
      }

      // adding
      _regionbases[regionbaseUID] = entity;

      _view->addRegionBase (regionbaseUID, attributes);
      _mainWindow->addRegionBaseToCombobox (regionbaseUID, attributes);
      _mainWindow->selectRegionBaseInComboBox (regionbaseUID);
    }
  }
}

void
NCLLayoutViewPlugin::removeRegionBaseFromView (QString entityUID)
{
  if (!entityUID.isEmpty ())
  {
    if (_regionbases.contains (entityUID))
    {
      // setting
      QString regionbaseUID;

      regionbaseUID = entityUID;

      // select
      _view->removeRegionBase (regionbaseUID);

      _regionbases.remove (regionbaseUID);

      _relations.remove (entityUID);

      _mainWindow->removeRegionBaseFromCombobox (regionbaseUID);
    }
  }
}

void
NCLLayoutViewPlugin::changeRegionBaseInView (Entity *entity)
{
  if (entity != NULL)
  {
    if (entity->type () == "regionBase")
    {
      // setting
      QString entityUID;

      if (!entity->uid ().isEmpty ())
      {
        entityUID = entity->uid ();
      }
      else
      {
        qCWarning (CPR_PLUGIN_LAYOUT)
            << "QnlyComposerPlugin::addRegion:"
            << "Tried to add a region with empty UID.";
        return; // abort addition
      }

      QString regionbaseUID;
      regionbaseUID = entityUID;

      QMap<QString, QString> attributes;

      if (!entity->attr ("id").isEmpty ())
      {
        attributes["id"] = entity->attr ("id");
      }

      if (!entity->attr ("region").isEmpty ())
      {
        attributes["region"] = entity->attr ("region");
      }

      if (!entity->attr ("device").isEmpty ())
      {
        attributes["device"] = entity->attr ("device");
      }

      // adding
      _regionbases[regionbaseUID] = entity;

      _view->changeRegionBase (regionbaseUID, attributes);

      _mainWindow->updateRegionBaseInComboBox (regionbaseUID, attributes);
    }
  }
}

void
NCLLayoutViewPlugin::selectRegionBaseInView (QString entityUID)
{
  if (!entityUID.isEmpty ())
  {
    if (_regionbases.contains (entityUID))
    {
      // setting
      QString regionbaseUID;

      regionbaseUID = entityUID;

      // select
      _view->selectRegionBase (regionbaseUID);

      _mainWindow->selectRegionBaseInComboBox (regionbaseUID);
    }
  }
}

void
NCLLayoutViewPlugin::addDescriptorToView (Entity *entity)
{
  if (entity != NULL)
  {
    if (entity->type () == "descriptor" && !entity->attr ("id").isEmpty ()
        && !entity->attr ("region").isEmpty ())
    {
      // QList<Entity*> model_regions = project
      // ()->getEntitiesbyType("region");
      QMap<QString, QString> descriptorsIDs;
      descriptorsIDs[entity->attr ("region")] = entity->attr ("id");
      _descriptors[entity->uid ()] = descriptorsIDs;
      /*
      for(Entity *rg : regions.values())
      {
        if (entity->getAttr("region") == rg->getAttr("id"))
        {
          view->addDescriptor(rg->uniqueId(),entity->getAttr("id"));
        }
      }
      */ // is this step done twice if updateDescriptors is after add?
    }
  }
}

void
NCLLayoutViewPlugin::removeDescriptorFromView (const QString &entityUID)
{
  if (_descriptors.contains (entityUID))
  {
    /*
    for(Entity *rg : regions.values())
    {
      if (descriptors[entityUID].contains(rg->getAttr("id")))
      {
        view->addDescriptor(rg->uniqueId(),"");// remove descriptor id from view, passing ""
      }
    }*/ // done twice if used before updateDescriptors?
    _descriptors.remove (entityUID);
  }
}

void
NCLLayoutViewPlugin::updateDescriptors ()
{
  for (Entity *rg : _regions.values ())
  {
    bool visited = false;
    for (QMap<QString, QString> descriptorsID : _descriptors.values ())
    {
      if (descriptorsID.contains (rg->attr ("id")))
      {
        _view->addDescriptor (rg->uid (),
                              descriptorsID.value (rg->attr ("id")));
        visited = true;
      }
    }
    if (!visited)
    {
      _view->addDescriptor (rg->uid (),
                            ""); // remove descriptor id from view, passing ""
    }
  }
}

void
NCLLayoutViewPlugin::addRegion (const QString &regionUID,
                                const QString &parentUID,
                                const QString &regionbaseUID,
                                const QMap<QString, QString> &attributes)
{
  Q_UNUSED (regionUID)

  // setting
  QMap<QString, QString> standard;

  if (attributes.contains ("id"))
    standard["id"] = attributes["id"];
  else
    standard["id"] = project ()->generateUniqueAttrId ("region");

  if (attributes.contains ("title"))
    standard["title"] = attributes["title"];

  if (attributes.contains ("top"))
    standard["top"] = attributes["top"];

  if (attributes.contains ("left"))
    standard["left"] = attributes["left"];

  if (attributes.contains ("bottom"))
    standard["bottom"] = attributes["bottom"];

  if (attributes.contains ("right"))
    standard["right"] = attributes["right"];

  if (attributes.contains ("width"))
    standard["width"] = attributes["width"];

  if (attributes.contains ("height"))
    standard["height"] = attributes["height"];

  if (attributes.contains ("zIndex"))
    standard["zIndex"] = attributes["zIndex"];

  // emitting
  if (!parentUID.isEmpty ())
    emit addEntity ("region", parentUID, standard);
  else
    emit addEntity ("region", regionbaseUID, standard);
}

void
NCLLayoutViewPlugin::removeRegion (const QString &regionUID,
                                   const QString &regionbaseUID)
{
  Q_UNUSED (regionbaseUID)

  if (_regions.contains (regionUID))
  {
    emit removeEntity (_regions[regionUID]);
  }
}

void
NCLLayoutViewPlugin::changeRegion (const QString &regionUID,
                                   const QString &regionbaseUID,
                                   const QMap<QString, QString> &attributes)
{
  Q_UNUSED (regionbaseUID)

  if (_regions.contains (regionUID))
  {
    // setting
    QMap<QString, QString> standard;

    if (attributes.contains ("id"))
      standard["id"] = attributes["id"];

    if (attributes.contains ("title"))
      standard["title"] = attributes["title"];

    if (attributes.contains ("top"))
      standard["top"] = attributes["top"];

    if (attributes.contains ("left"))
      standard["left"] = attributes["left"];

    if (attributes.contains ("bottom"))
      standard["bottom"] = attributes["bottom"];

    if (attributes.contains ("right"))
      standard["right"] = attributes["right"];

    if (attributes.contains ("width"))
      standard["width"] = attributes["width"];

    if (attributes.contains ("height"))
      standard["height"] = attributes["height"];

    if (attributes.contains ("zIndex"))
      standard["zIndex"] = attributes["zIndex"];

    // emitting
    emit setAttributes (_regions[regionUID], standard);
  }
}

void
NCLLayoutViewPlugin::selectRegion (const QString &regionUID,
                                   const QString &regionbaseUID)
{
  Q_UNUSED (regionbaseUID)

  if (_selectedId != NULL)
  {
    delete _selectedId;
    _selectedId = NULL;
  }

  if (!regionUID.isEmpty ())
  {
    _selectedId = new QString (regionUID);
    emit sendBroadcastMessage ("changeSelectedEntity", _selectedId);
  }
}

void
NCLLayoutViewPlugin::addRegionBase (const QString &regionbaseUID,
                                    const QMap<QString, QString> &attributes)
{
  Q_UNUSED (regionbaseUID)

  // setting
  QMap<QString, QString> standard;

  if (attributes.contains ("id"))
    standard["id"] = attributes["id"];

  if (attributes.contains ("region"))
    standard["region"] = attributes["region"];

  if (attributes.contains ("device"))
    standard["device"] = attributes["device"];

  QString headUID = headUid ();
  // emitting
  emit addEntity ("regionBase", headUID, standard);
}

void
NCLLayoutViewPlugin::removeRegionBase (const QString &regionbaseUID)
{
  if (_regionbases.contains (regionbaseUID))
  {
    _mainWindow->removeRegionBaseFromCombobox (regionbaseUID);

    emit removeEntity (_regionbases[regionbaseUID]);
  }
}

void
NCLLayoutViewPlugin::changeRegionBase (
    const QString &regionbaseUID, const QMap<QString, QString> &attributes)
{
  if (_regionbases.contains (regionbaseUID))
  {
    // setting
    QMap<QString, QString> standard;

    if (attributes.contains ("id"))
      standard["id"] = attributes["id"];

    if (attributes.contains ("title"))
      standard["title"] = attributes["title"];

    if (attributes.contains ("top"))
      standard["top"] = attributes["top"];

    if (attributes.contains ("left"))
      standard["left"] = attributes["left"];

    if (attributes.contains ("bottom"))
      standard["bottom"] = attributes["bottom"];

    if (attributes.contains ("right"))
      standard["right"] = attributes["right"];

    if (attributes.contains ("width"))
      standard["width"] = attributes["width"];

    if (attributes.contains ("height"))
      standard["height"] = attributes["height"];

    if (attributes.contains ("zIndex"))
      standard["zIndex"] = attributes["zIndex"];

    // emitting
    emit setAttributes (_regionbases[regionbaseUID], standard);

    _mainWindow->updateRegionBaseInComboBox (regionbaseUID, attributes);
  }
}

void
NCLLayoutViewPlugin::clear ()
{
  while (!_regions.empty ())
  {
    Entity *regionEntity = _regions.begin ().value ();

    removeRegionFromView (regionEntity->uid ());
  }

  while (!_regionbases.empty ())
  {
    Entity *regionbaseEntity = _regionbases.begin ().value ();

    removeRegionBaseFromView (regionbaseEntity->uid ());
  }

  _regions.clear ();
  _regionbases.clear ();
  _relations.clear ();
}

void
NCLLayoutViewPlugin::selectRegionBase (const QString &regionbaseUID)
{
  if (_selectedId != NULL)
  {
    if (*_selectedId == regionbaseUID)
      return; // Do nothing! It is already selected

    delete _selectedId;
    _selectedId = NULL;
  }

  if (!regionbaseUID.isEmpty ())
  {
    _selectedId = new QString (regionbaseUID);
    _mainWindow->selectRegionBaseInComboBox (regionbaseUID);

    emit sendBroadcastMessage ("changeSelectedEntity", _selectedId);
  }
}

QString
NCLLayoutViewPlugin::headUid ()
{
  if (project ()->getEntitiesbyType ("head").isEmpty ())
  {
    if (project ()->getEntitiesbyType ("ncl").isEmpty ())
    {
      QMap<QString, QString> atts;

      emit addEntity ("ncl", project ()->uid (), atts);
    }

    QString nclUID = project ()->getEntitiesbyType ("ncl").at (0)->uid ();

    QMap<QString, QString> atts;

    emit addEntity ("head", nclUID, atts);
  }

  return project ()->getEntitiesbyType ("head").at (0)->uid ();
}

QMap<QString, QString>
NCLLayoutViewPlugin::regionAttributes (Entity *region)
{
  QVector<double> widths;
  QVector<double> heights;
  QVector<double> tops;
  QVector<double> lefts;

  double top = 1.0, left = 1.0;
  double cvalue = 0.0;

  Entity *currentRegion = region;

  // Here, we will compose all the hierachy values to get the final values of
  // left, top, width and height.
  while (currentRegion != NULL && currentRegion->type () == "region")
  {
    QMap<QString, QString> attrs = currentRegion->attrs ();
    foreach (const QString &key, attrs.keys ())
    {
      QString name = key;
      QString value = attrs[key];
      if (value.endsWith ("%"))
        cvalue = value.mid (0, value.indexOf ("%")).toDouble ();
      else
        cvalue = 100.0;

      cvalue /= 100.0;

      if (name == "width")
        widths.push_back (cvalue);
      else if (name == "height")
        heights.push_back (cvalue);
      else if (name == "top")
        tops.push_back (cvalue);
      else if (name == "left")
        lefts.push_back (cvalue);
    }

    currentRegion = static_cast<Entity *> (currentRegion->parent ());
  }

  int i;
  // width
  heights.push_back (1.0f);
  widths.push_back (1.0f);

  for (i = widths.size () - 1; i > 0; --i)
    widths[i - 1] *= widths[i];
  // height
  for (i = heights.size () - 1; i > 0; --i)
    heights[i - 1] *= heights[i];

  // left
  left = top = 0.0;
  for (i = lefts.size () - 1; i >= 0; --i)
    left += lefts[i] * widths[i + 1];

  for (i = tops.size () - 1; i >= 0; --i)
    top += tops[i] * heights[i + 1];

  QMap<QString, QString> attrs;
  attrs.insert ("width", QString::number (widths[0] * 100.0) + "%");
  attrs.insert ("height", QString::number (heights[0] * 100.0) + "%");
  attrs.insert ("left", QString::number (left * 100.0) + "%");
  attrs.insert ("top", QString::number (top * 100.0) + "%");

  // Add also the zIndex to the list of properties.
  if (region->hasAttr ("zIndex"))
    attrs.insert ("zIndex", region->attr ("zIndex"));
  else
    attrs.insert ("zIndex", "0");

  return attrs;
}

void
NCLLayoutViewPlugin::performMediaOverRegionAction (const QString &mediaId,
                                                   const QString &regionUID)
{
  bool error = false;
  Entity *region = project ()->getEntityById (regionUID);
  Entity *media = NULL;
  QList<Entity *> medias = project ()->getEntitiesbyType ("media");

  const bool askDescOrProperties = true; // \todo This must be a preference.

  for (int i = 0; i < medias.size (); i++)
  {
    if (medias.at (i)->hasAttr ("id") && medias.at (i)->attr ("id") == mediaId)
    {
      media = medias.at (i);
      break;
    }
  }

  if (region == NULL)
  {
    qCWarning (CPR_PLUGIN_LAYOUT)
        << "QnlyComposerPlugin::performMediaOverRegionAction Region\
                  does not exists. Nothing will be done.";
    error = 1;
  }

  if (media == NULL)
  {
    int ret = QMessageBox::warning (this->widget (), tr ("Error"),
                                    tr ("Sorry. \"") + mediaId
                                        + tr ("\" is not a valid media."),
                                    QMessageBox::Ok, QMessageBox::Ok);

    Q_UNUSED (ret)
    error = 1;
  }

  if (!error)
  {
    if (askDescOrProperties)
    {
      QMessageBox msgBox;
      msgBox.setText (tr ("Please, tell what do you want to do"));
      // \todo Enable detailed text.
      //    msgBox.setDetailedText(tr("Detailed text"));

      QPushButton *createDescButton = msgBox.addButton (
          tr ("Use a descriptor"), QMessageBox::ActionRole);

      QPushButton *importPropButton
          = msgBox.addButton (tr ("Import region properties to media object"),
                              QMessageBox::ActionRole);
      //  importPropButton->setEnabled(false);

      QPushButton *cancelButton
          = msgBox.addButton (tr ("Nothing!"), QMessageBox::ActionRole);
      Q_UNUSED (cancelButton)

      //  msgBox.setIcon(QMessageBox::Question);
      msgBox.exec ();
      if (msgBox.clickedButton ()
          == createDescButton) // create a new descriptor
      {
        QMap<QString, QString> attrs;
        QList<Entity *> descritorBases
            = project ()->getEntitiesbyType ("descriptorBase");

        // create the descriptor
        QString newDescriptorID
            = project ()->generateUniqueAttrId ("descriptor");
        QStringList alredyExistentsDescriptorsIds, descriptorsIds;
        descriptorsIds << newDescriptorID;

        QList<Entity *> descriptors
            = project ()->getEntitiesbyType ("descriptor");
        foreach (Entity *desc, descriptors)
        {
          if (desc != NULL)
          {
            if (desc->attr ("region") == region->attr ("id"))
            {
              alredyExistentsDescriptorsIds << desc->attr ("id");
              descriptorsIds << desc->attr ("id");
            }
          }
        }

        bool ok;
        newDescriptorID = QInputDialog::getItem (NULL, tr ("Descriptor id:"),
                                                 tr ("Please, enter the "
                                                     "descriptor id"),
                                                 descriptorsIds, 0, true, &ok);

        if (ok)
        {
          if (!descritorBases.size ()) // if does not exists any descriptorBase
            // create one
            emit addEntity ("descriptorBase", headUid (), attrs);

          // If we choose the new descriptor (create it)
          if (!alredyExistentsDescriptorsIds.contains (newDescriptorID))
          {
            Entity *descriptorBase
                = project ()->getEntitiesbyType ("descriptorBase").at (0);
            attrs.insert ("id", newDescriptorID);
            attrs.insert ("region", region->attr ("id"));

            emit addEntity ("descriptor", descriptorBase->uid (), attrs);
          }

          // update the media to refer to this descriptor
          attrs.clear ();
          attrs = media->attrs ();
          attrs["descriptor"] = newDescriptorID;
          emit setAttributes (media, attrs);
        }
      }
      // import properties from region to media element
      else if (msgBox.clickedButton () == importPropButton)
      {
        QMap<QString, QString> propertyNameToUID;

        foreach (Entity *propEntity, media->entityChildren ())
        {
          if (propEntity->hasAttr ("name"))
          {
            propertyNameToUID.insert (propEntity->attr ("name"),
                                      propEntity->uid ());
          }
        }

        QMap<QString, QString> newAttrs = regionAttributes (region);
        QString key;
        foreach (key, newAttrs.keys ())
        {
          QMap<QString, QString> attrs;
          attrs.insert ("name", key);
          attrs.insert ("value", newAttrs.value (key));

          if (propertyNameToUID.keys ().contains (key))
          {
            QString propUID = propertyNameToUID.value (key);
            emit setAttributes (project ()->getEntityById (propUID), attrs);
          }
          else
            emit addEntity ("property", media->uid (), attrs);
        }
      }
    }
    else
    {
      // Does not ask the descriptor or media properties

      QMap<QString, QString> attrs;
      QList<Entity *> descritorBases
          = project ()->getEntitiesbyType ("descriptorBase");

      // create the descriptor
      QString newDescriptorID
          = project ()->generateUniqueAttrId ("descriptor");

      if (!descritorBases.size ()) // if does not exists any descriptorBase
        // create one
        emit addEntity ("descriptorBase", headUid (), attrs);

      Entity *descriptorBase
          = project ()->getEntitiesbyType ("descriptorBase").at (0);
      attrs.insert ("id", newDescriptorID);
      attrs.insert ("region", region->attr ("id"));

      emit addEntity ("descriptor", descriptorBase->uid (), attrs);

      // update the media to refer to this descriptor
      attrs.clear ();
      attrs = media->attrs ();
      attrs["descriptor"] = newDescriptorID;
      emit setAttributes (media, attrs);

      if (media->hasAttr ("type") && media->attr ("type") == "text/html")
      {
        attrs.clear ();
        QMap<QString, QString>::iterator begin, end, it;
        QList<Entity *> descriptors
            = project ()->getEntityByAttrId (newDescriptorID);

        if (descriptors.size ())
        {
          Entity *desc = descriptors.at (0);
          attrs = desc->attrs ();
          attrs["focusIndex"] = "1";
          if (descriptors.size ())
            emit setAttributes (desc, attrs);
        }
      }
    }
  }
}

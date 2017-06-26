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
#include "PropertiesViewPlugin.h"
#include "modules/LanguageControl.h"

#include <QDockWidget>

PropertiesViewPlugin::PropertiesViewPlugin ()
{
  _window = new PropertyEditor (0);
  project = nullptr;
  _currentEntity = nullptr;

  connect (_window, SIGNAL (propertyChanged (QString, QString)), this,
           SLOT (updateCurrentEntityAttr (QString, QString)));
}

PropertiesViewPlugin::~PropertiesViewPlugin ()
{
  /*!  \todo Review PropertiesViewPlugin::~PropertiesViewPlugin().
             Why not delete 'window'? Memory leak? */
  // delete window;
}

QWidget *
PropertiesViewPlugin::getWidget ()
{
  return _window;
}

void
PropertiesViewPlugin::onEntityAdded (QString pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)
  Q_UNUSED (entity)
}

void
PropertiesViewPlugin::errorMessage (QString error)
{
  qDebug () << "PropertiesViewPlugin::onEntityAddError(" << error << ")";
}

void
PropertiesViewPlugin::onEntityChanged (QString pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)

  if (entity != nullptr && _currentEntity != nullptr)
  {
    if (entity->getUniqueId () == _currentEntity->getUniqueId ())
      updateCurrentEntity ();
  }
}

void
PropertiesViewPlugin::onEntityRemoved (QString pluginID, QString entityID)
{
  Q_UNUSED (pluginID)

  if (entityID == _currentEntityId)
  {
    _currentEntity = nullptr;
    _window->setTagname ("", "");
    _currentEntityId = "";
  }
}

bool
PropertiesViewPlugin::saveSubsession ()
{
  /*!< TODO: Implement PropertiesViewPlugin::saveSubsession() */
  return true;
}

void
PropertiesViewPlugin::init ()
{
  /*!< TODO: Implement PropertiesViewPlugin::init() */
  // TODO: All
  /*
    QPushButton *refresh = new QPushButton(window);
    refresh->setIcon(QIcon(":/mainwindow/refreshplugin"));
    ((QDockWidget*)window->parent())->titleBarWidget()->layout()
        ->addWidget(refresh);
   */
}

void
PropertiesViewPlugin::changeSelectedEntity (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)

  QString *id = (QString *)param;
  if (id != nullptr && *id != "")
  {
    _currentEntity = project->getEntityById (*id);
    _currentEntityId = *id;
  }

  if (_currentEntity != nullptr)
  {
    _window->setTagname (_currentEntity->getType (), "");
    updateCurrentEntity ();
  }

  emit sendBroadcastMessage ("askAllValidationMessages", nullptr);
}

void
PropertiesViewPlugin::updateCurrentEntity (QString errorMessage)
{
  QString name;
  if (_currentEntity->hasAttribute ("id"))
    name = _currentEntity->getAttribute ("id");
  else if (_currentEntity->hasAttribute ("name"))
    name = _currentEntity->getAttribute ("name");
  else
    name = "Unknown";

  if (_currentEntity->getType () != _window->getTagname ())
    _window->setTagname (_currentEntity->getType (), name);
  else if (_window->getCurrentName () != name)
    _window->setCurrentName (name);

  _window->setErrorMessage (errorMessage);

  QMap<QString, QString> attrs = _currentEntity->getAttributes ();
  foreach (const QString &key, attrs)
  {
    _window->setAttributeValue (key, attrs[key]);
  }
}

void
PropertiesViewPlugin::updateCurrentEntityAttr (QString attr, QString value)
{
  if (_currentEntity != nullptr)
  {
    if (_currentEntity->hasAttribute (attr)
        && _currentEntity->getAttribute (attr) == value)
    {
      // do nothing
      return;
    }
    else
    {
      QMap<QString, QString> attrs;
      QMap<QString, QString> entityAttrs = _currentEntity->getAttributes ();

      foreach (const QString &key, entityAttrs.keys ())
      {
        if (key == attr)
        {
          if (!value.isNull () && !value.isEmpty ())
          {
            if (NCLStructure::getInstance ()->getAttributeDatatype (
                    _currentEntity->getType (), attr)
                == "URI")
            {
              try
              {
                value = Utilities::relativePath (project->getLocation (),
                                                 value, true);
              }
              catch (...)
              {
                qDebug () << "Do not changing to a relative path";
              }
            }
            attrs.insert (attr, value);
          }
        }
        else
          attrs.insert (key, entityAttrs[key]);
      }

      if (!attrs.contains (attr))
      {
        if (!value.isNull () && !value.isEmpty () && value != "")
        {
          if (NCLStructure::getInstance ()->getAttributeDatatype (
                  _currentEntity->getType (), attr)
              == "URI")
          {
            try
            {
              value = Utilities::relativePath (project->getLocation (), value,
                                               true);
            }
            catch (...)
            {
              qDebug () << "Do not changing to a relative path";
            }
          }
          attrs.insert (attr, value);
        }
      }

      emit setAttributes (_currentEntity, attrs);
    }
  }
}

void
PropertiesViewPlugin::validationError (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)

  if (param)
  {
    pair<QString, QString> *p = (pair<QString, QString> *)param;

    QString uid = p->first;

    if (_currentEntity == project->getEntityById (uid))
      updateCurrentEntity (p->second);
  }
}

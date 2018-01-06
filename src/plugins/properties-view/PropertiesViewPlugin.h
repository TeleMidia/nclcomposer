/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#ifndef PROPERTIESVIEWPLUGIN_H
#define PROPERTIESVIEWPLUGIN_H

#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QPushButton>

#include <extensions/IPlugin.h>
#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "PropertiesEditor.h"

/*!
 * \brief PropertiesViewPlugin is a Composer plugin that shows and enable the
 *      user to edit the a current selected Entity.
 */
class PropertiesViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  /*!
     * \brief Constructor.
     */
  explicit PropertiesViewPlugin ();

  /*!
     * \brief Destructor.
     */
  ~PropertiesViewPlugin ();

  /*!
     * \brief
     *
     * This function is part of the IPlugin API.
     */
  void init () override;

  /*!
     * \brief Returns the widget of that plugin. This widget will be
   * presentated
     *      to the user.
     *
     *  This function is part of the IPlugin API.
     *
     * \return QWidget* the widget that represents this plugin. If nullptr, the
     *      plugin has not a visual representation
     */
  QWidget *widget () override;

  /*!
     * \brief Save the specific data of this plugin.
     *
     *  This function is part of the IPlugin API.
     *
     * \return bool
     */
  bool saveSubsession () override;

public slots:
  /*!
     * \brief This is called by the core when a new Entity is added.
     *
     * This function is part of IPlugin API.
     *
     * \param pluginID the plugin instance id that first called the changes to
     *     the entity.
     * \param entity the Entity that was added.
     */
  void onEntityAdded (const QString &pluginID, Entity *) override;

  /*!
     * \brief Called by the core when an Entity is changed.
     *
     * This function is part of IPlugin API.
     *
     * \param pluginID the plugin instance id that first called the changes to
     *     the entity.
     * \param entity The entity that was modified.
     */
  void onEntityChanged (const QString &pluginID, Entity *) override;

  /*!
     * \brief Called by the core after the entity entityID is removed.
     *
     * \param pluginID the plugin instance id that first called the changes to
     *     the entity.
     * \param entityID the entity's identifier that was removed.
     */
  void onEntityRemoved (const QString &pluginID,
                        const QString &entityID) override;

  /*!
   * \brief This is called by the core when some error triggered by this
   *      plugin instance occurs.
   *
   * \param error A description of the error.
   */
  void errorMessage (const QString &error) override;

  /*!
   * \brief
   *
   * \param
   */
  void changeSelectedEntity (QString pluginID, void *);

  /*!
   * \brief
   */
  void validationError (QString pluginID, void *param);

private slots:
  /*!
   * \brief Update all the attributes from the currentEntity.
   */
  void updateCurrentEntity (QString errorMessage = "");

  /*!
   * \brief Update an individual attribute of the currentEntity.
   *
   * \param attr The attribute to be updated.
   * \param value The new value of this attribute.
   */
  void updateCurrentEntityAttr (QString attr, QString value);

private:
  QListWidget *_list;        /*! TODO */
  PropertiesEditor *_window; /*! TODO */
  Entity *_currentEntity;    /*! \todo Change pointer reference to
                                      the currentEntityId */
  QString _currentEntityId;

  QStringList getAttributes (const QString &tagname);
  QStringList getAttributesDatatype (const QString &tagname);
  QList<QStringList> getAttributeSuggestions (const QString &tagname);
};

/*!
 \brief Handles the creation and deletion of PropertyView objects.

*/
class PropertiesViewFactory : public QObject,
                              public IPluginFactoryTpl<PropertiesViewPlugin>
{
  Q_OBJECT
  CPR_PLUGIN_METADATA ("br.puc-rio.telemidia.properties",
                       "properties-view.json")
};

#endif // PROPERTIESVIEWPLUGIN_H

/*
 * Copyright 2011-2013 TeleMidia/PUC-Rio.
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
#ifndef OUTLINEVIEWPLUGIN_H
#define OUTLINEVIEWPLUGIN_H

#include <QObject>

#include <extensions/IPlugin.h>
#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "NCLTreeWidget.h"
#include "OutlineViewPlugin_global.h"

/*!
 \brief OutlineViewPlugin is a Composer plugin that shows the internal model
            as a tree allowing ease navigation.
*/
class OutlineViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor.
   */
  explicit OutlineViewPlugin ();
  /*!
   * \brief Destructor.
   */
  ~OutlineViewPlugin ();
  /*!
   * \brief
   *
   * This function is part of the IPlugin API.
   */
  void init () override;
  /*!
   * \brief Returns the widget of that plugin. This widget will be presentated
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
   * \param ID The plugin identifier that called to add the Entity.
   * \param Entity* the Entity that was added.
   */
  void onEntityAdded (const QString &pluginID, Entity *) override;

  /*!
   * \brief
   *
   * This function is part of the IPlugin API.
   *
   * \param pluginID
   * \param
   */
  void onEntityChanged (const QString &ID, Entity *) override;

  /*!
   * \brief
   *
   * This function is part of the IPlugin API.
   *
   * \param ID
   * \param entityID
   */
  void onEntityRemoved (const QString &pluginID,
                        const QString &entityID) override;

  /*!
   * \brief This slot calls the outline plugin to update its own model.
   *
   */
  void updateFromModel () override;

  /*!
   * \brief
   *
   * This function is part of the IPlugin API.
   *
   * \param error
   */
  void errorMessage (const QString &error) override;

  /* Comunication from Debug to me */
  /*!
   * \brief
   *
   * \param obj
   */
  void debugHasSendClearAll (QString pluginIDm, void *obj);

  /*!
   * \brief
   *
   * \param param
   */
  void changeSelectedEntity (QString pluginId, void *param);

  /*!
   * \brief
   */
  void textualStartSync (QString, void *);

  /*!
   * \brief
   */
  void textualFinishSync (QString, void *);

  // COMUNICATION WITH VALIDATION PLUGIN //
  /*!
   * \brief This message is called by Validator plugin.
   *
   */
  void clearValidationMessages (QString, void *param);

  /*!
   * \brief This message is called by Validator plugin.
   */
  void validationError (QString pluginID, void *param);
  // END COMMUNICATION WITH VALIDATION PLUGIN

  /*!
   * \brief
   */
  void clearErrorMessages ();

private:
  NCLTreeWidget *_window, *_windowBuffering;  /*!< TODO */
  QString *_selectedId;                       /*!< TODO */
  QMap<QString, QTreeWidgetItem *> _idToItem; /*!< TODO */
  bool _isSyncingFromTextual;

private slots:
  /*!
   * \brief
   *
   * \param id
   *
   */
  void elementRemovedByUser (QString id);

  /*!
   * \brief
   *
   * \param id
   */
  void elementAddedByUser (QString type, QString parentId,
                           QMap<QString, QString> &atts);
  /*!
   * \brief
   */
  void itemSelectionChanged ();

  void openWithDefaultSystemEditor (QString entityId);
};

/*!
 \brief Handles the creation and deletion of OutlineView objects.
*/
class OutlineViewFactory : public QObject,
                           public IPluginFactoryTpl<OutlineViewPlugin>
{
  Q_OBJECT
  CPR_PLUGIN_METADATA ("br.puc-rio.telemidia.outline", "outline-view.json")
};

#endif // OUTLINEVIEWPLUGIN_H

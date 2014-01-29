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
#ifndef NCLTEXTUALVIEWPLUGIN_H
#define NCLTEXTUALVIEWPLUGIN_H

#include <QObject>

#include "NCLTextEditor.h"
#include "NCLTextEditorMainWindow.h"

#include "NCLTextualViewPlugin_global.h"

#include <QShortcut>
#include <QKeySequence>
#include <QDomElement>
#include <QMutex>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

/*!
 \brief Encapsulates the NCLTextEditor as a Composer plugin.
*/
class NCLTextualViewPlugin :
    public IPlugin
{
  Q_OBJECT

private:
  NCLTextEditorMainWindow *window; /*!< TODO */
  NCLTextEditor *nclTextEditor; /*! < The current NCLTextEditor */
  NCLTextEditor *tmpNclTextEditor; /*!< This variable is used to keep a
                                            buffer with text and syntax coloring
                                            while the text content is
                                            synchronized to the core */

  QMap <QString, int> startEntityOffset; /*!< Keep the offset of the startTag
                                              element */
  QMap <QString, int> endEntityOffset; /*!< Keep the offset of endTag offset */

  QShortcut *updateModelShortcut;

  bool isSyncing;
  Entity *currentEntity;

  QDomDocument xmlDoc;
  QMutex syncMutex;

public:
  /*!
   * \brief Constructor.
   */
  explicit NCLTextualViewPlugin();

  /*!
   * \brief Destructor.
   */
  ~NCLTextualViewPlugin();

  /*!
   * \brief Init the plugin. This function is part of the IPlugin API.
   */
  void init();

  /*!
   * \brief Save the NCL Text Editor specific data.
   *
   * This function is part of the IPlugin API.
   * \return bool
   */
  bool saveSubsession();

  /*!
   * \brief Returns the main widget of the NCLTextEdfitor.
   *  This function is part of the IPlugin API.
   *
   * \return QWidget *
   */
  QWidget* getWidget();

public slots:
  /*!
   * \brief This function is part of the IPlugin API.
   *
   * \param pluginID
   * \param
   */
  void onEntityAdded(QString pluginID, Entity *);

  /*!
   * \brief This function is part of the IPlugin API.
   *
   * \param pluginID
   * \param
   */
  void onEntityChanged(QString pluginID, Entity *);

  /*!
   * \brief This function is part of the IPlugin API.
   * \param pluginID
   * \param entityID
   */
  void onEntityRemoved(QString pluginID, QString entityID);

  /*!
   * \brief This function is part of the IPlugin API.
   *
   * \param error
   */
  void errorMessage(QString error);

  /*!
   * \brief
   *
   * This function is part of the IPlugin API.
   *
   * \param param
   */
  void changeSelectedEntity(QString pluginID, void* param);

  /*!
   * \brief This slot calls the textual plugin to update its own model.
   *
   */
  void updateFromModel();

  /*!
   * \brief Incremental update from core model.
   *
   * This function will works almost equal the core send each individual
   * command for add every entity in the model.
   */
  void incrementalUpdateFromModel();

  /*!
   * \brief Non-Incremental update from Composer core model.
   *
   * This function will construct the DOM model, and then will generate the
   *
   * \todo Update entities indexes cache.
   */
  void nonIncrementalUpdateFromModel();

  /*!
     * \brief Clear all error messages and calls the validator to send all
     *  messages again.
     */
  void updateErrorMessages();

  // COMUNICATION WITH VALIDATION PLUGIN //
  /*!
   * \brief This message is called by Validator plugin.
   *
   */
  void clearValidationMessages(QString, void *param);
  /*!
   * \brief This message is called by Validator plugin.
   */
  void validationError(QString pluginID, void *param);
  // END COMMUNICATION WITH VALIDATION PLUGIN

private slots:
  /*!
   * \brief Updates the core model with the current content of the NCL Text
   * Editor.
   */
  void updateCoreModel();

  /*!
   * \brief Updates the core model with the current content of the NCL Text
   *    Editor using an incremental approach.
   */
  void incrementalUpdateCoreModel();

  /*!
   * \brief Updates the core model with the current content of the NCL Text
   *    Editor clearing the core model and rebuilding it from scratch.
   */
  void nonIncrementalUpdateCoreModel();

  /*!
   * \brief Called when the parser finishes its work.
   */
  void syncFinished();

  /*!
   * \brief Called when the user change the focus from the textual plugin.
   */
  virtual void manageFocusLost(QFocusEvent *event);

signals:
  void TextualPluginHasAddedEntity(QString pluginID, Entity *entity);

protected:
  void updateEntitiesOffset(int startFrom = 0, int insertedChars = 0);
  void printEntitiesOffset();

  /*!
   * \brief
   */
  bool isStartEndTag(Entity *entity);

  /*!
   * \brief
   */
  void openStartEndTag(Entity *entity);

  /*!
   * \brief
   */
  void fixIdentation(int offset, bool mustAddTab = false);
};

#endif // DEBUGCONSOLEPLUGIN_H

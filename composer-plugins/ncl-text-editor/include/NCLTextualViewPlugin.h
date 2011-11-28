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
#ifndef NCLTEXTUALVIEWPLUGIN_H
#define NCLTEXTUALVIEWPLUGIN_H

#include <QObject>

#include "NCLTextEditor.h"
#include "NCLTextEditorMainWindow.h"

#include "NCLTextualViewPlugin_global.h"

#include <QShortcut>
#include <QKeySequence>
#include <QDomElement>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

namespace composer {
    namespace plugin {
        namespace textual {

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
    QMap <QString, int> startEntityOffset; /*!< TODO */
    QMap <QString, int> endEntityOffset; /*!< TODO */

    QShortcut *updateModelShortcut;

    bool isSyncing;
    Entity *currentEntity;

public:
    /*!
     \brief Constructor.
    */
    explicit NCLTextualViewPlugin();
    /*!
     \brief Destructor.
    */
    ~NCLTextualViewPlugin();
    /*!
     \brief Init the plugin.
        This function is part of the IPlugin API.
    */
    void init();
    /*!
     \brief Save the NCL Text Editor specific data.

        This function is part of the IPlugin API.

     \return bool
    */
    bool saveSubsession();
    /*!
     \brief Returns the main widget of the NCLTextEdfitor.

        This function is part of the IPlugin API.

     \return QWidget *
    */
    QWidget* getWidget();

public slots:
    /*!
     \brief
        This function is part of the IPlugin API.

     \param pluginID
     \param
    */
    void onEntityAdded(QString pluginID, Entity *);
    /*!
     \brief

        This function is part of the IPlugin API.

     \param pluginID
     \param
    */
    void onEntityChanged(QString pluginID, Entity *);
    /*!
     \brief
        This function is part of the IPlugin API.

     \param pluginID
     \param entityID
    */
    void onEntityRemoved(QString pluginID, QString entityID);

    /*!
     \brief
        This function is part of the IPlugin API.

     \param error
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
     */
    void updateFromModel();

private slots:
    /*!
     * \brief Updates the core model with the content of the NCL Text Editor.
     */
    void updateCoreModel();

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

};

} } } //end namespace

#endif // DEBUGCONSOLEPLUGIN_H

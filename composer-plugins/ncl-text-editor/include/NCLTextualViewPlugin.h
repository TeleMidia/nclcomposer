/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef NCLTEXTUALVIEWPLUGIN_H
#define NCLTEXTUALVIEWPLUGIN_H

#include <QObject>

#include "NCLTextEditor.h"
#include "NCLTextEditorMainWindow.h"

#include "extensions/NCLDocumentParser.h"

#include <QShortcut>
#include <QKeySequence>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

namespace composer {
    namespace plugin {
        namespace textual {

/*!
 \brief Encapsulates the NCLTextEditor as a Composer plugin.

*/
class NCLTextualViewPlugin : public IPlugin
{
    Q_OBJECT

private:
    NCLTextEditorMainWindow *window; /*!< TODO */
    QMap <QString, int> startEntityOffset; /*!< TODO */
    QMap <QString, int> endEntityOffset; /*!< TODO */

    QShortcut *updateModelShortcut;

    bool isSyncing;

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

private slots:
    /*!
     * \brief Updates the core model with the content of the NCL Text Editor.
     */
    void updateCoreModel();

    /*!
     * \brief Called when the parser finishes its work.
     */
    void syncFinished();

signals:
    void TextualPluginHasAddedEntity(QString pluginID, Entity *entity);
};

}}} //end namespace

#endif // DEBUGCONSOLEPLUGIN_H

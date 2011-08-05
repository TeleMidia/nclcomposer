/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef OUTLINEVIEWPLUGIN_H
#define OUTLINEVIEWPLUGIN_H

#include <QObject>

#include "NCLTreeWidget.h"
#include <core/extensions/IPlugin.h>
using namespace composer::extension;


namespace composer {
    namespace plugin {
        namespace outline {

/*!
 \brief OutlineViewPlugin is a Composer plugin that shows the internal model
            as a tree allowing ease navigation.
*/
class OutlineViewPlugin : public IPlugin
{
    Q_OBJECT

private:
    NCLTreeWidget *window; /*!< TODO */
    QString *selectedId; /*!< TODO */
    QMap <QString, QTreeWidgetItem*> idToItem; /*!< TODO */

public:
    /*!
     * \brief Constructor.
     */
    explicit OutlineViewPlugin();
    /*!
     * \brief Destructor.
     */
    ~OutlineViewPlugin();
    /*!
     * \brief
     *
     * This function is part of the IPlugin API.
     */
    void init();
    /*!
     * \brief Returns the widget of that plugin. This widget will be presentated
     *      to the user.
     *
     *  This function is part of the IPlugin API.
     *
     * \return QWidget* the widget that represents this plugin. If NULL, the
     *      plugin has not a visual representation
     */
    QWidget* getWidget();
    /*!
     * \brief Save the specific data of this plugin.
     *
     *  This function is part of the IPlugin API.
     *
     * \return bool
     */
    bool saveSubsession();

public slots:
    /*!
     * \brief This is called by the core when a new Entity is added.
     *
     * This function is part of IPlugin API.
     *
     * \param ID The plugin identifier that called to add the Entity.
     * \param Entity* the Entity that was added.
     */
    void onEntityAdded(QString pluginID, Entity *);
    /*!
     * \brief
     *
     * This function is part of the IPlugin API.
     *
     * \param pluginID
     * \param
     */
    void onEntityChanged(QString ID, Entity *);
    /*!
     * \brief
     *
     * This function is part of the IPlugin API.
     *
     * \param ID
     * \param entityID
     */
    void onEntityRemoved(QString ID, QString entityID);

    /*!
     * \brief
     *
     * This function is part of the IPlugin API.
     *
     * \param error
     */
    void errorMessage(QString error);

    /* Comunication from Debug to me */
    /*!
     * \brief
     *
     * \param obj
     */
    void debugHasSendClearAll(QString pluginIDm, void *obj);

    /*!
     * \brief
     *
     * \param param
     */
    void changeSelectedEntity(QString pluginId, void* param);

private slots:
    /*!
     * \brief
     *
     * \param id
     *
     */
    void elementRemovedByUser(QString id);
    /*!
     * \brief
     *
     * \param id
     */
    void elementAddedByUser( QString type,
                             QString parentId,
                             QMap <QString, QString> & atts,
                             bool force);
    /*!
     * \brief
     */
    void itemSelectionChanged();
};

} } } //end namespace

#endif // OUTLINEVIEWPLUGIN_H

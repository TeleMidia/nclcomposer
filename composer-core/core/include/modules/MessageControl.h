/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef MESSAGECONTROL_H
#define MESSAGECONTROL_H

#include <QObject>
#include <QDebug>

#include <exception>
using namespace std;

#include "util/ComposerCoreControl_global.h"

#include "../extensions/IDocumentParser.h"
using namespace composer::extension;

#include "../model/Entity.h"
#include "../model/Project.h"
using namespace composer::core::model;

#include "../extensions/IPlugin.h"
using namespace composer::extension;

namespace composer {
    namespace core {
/*!
 \brief Manages the messages send from plugins to NCL Composer Core and
    vice-versa.
 */
class COMPOSERCORESHARED_EXPORT MessageControl : public QObject
{
    Q_OBJECT

private:
    Project *project; /*!< TODO */
    QMap <QString, QStringList> listenEntities;
    /*!< pluginInstanceId to list of entity this plugin is */

public:
    /*!
     \brief Constructor.

     \param project
    */
    MessageControl(Project *project);
    /*!
     \brief Destructor.
    */
    ~MessageControl();

private:
    /*!
     * \brief TODO
     */
    void sendEntityAddedMessageToPlugins( QString pluginInstanceId,
                                          Entity *entity);
    /*!
     * \brief TODO
     */
    void sendEntityChangedMessageToPlugins( QString pluginInstanceId,
                                            Entity *entity);
    /*!
     * \brief TODO
     */
    void sendEntityRemovedMessageToPlugins( QString pluginInstanceId,
                                            Entity *entity);


    /*!
     * \brief TODO
     */
    bool pluginIsInterestedIn(IPlugin *plugin, Entity *entity);

public slots:
    /*!
     \brief

     \param type
     \param parentEntityId
     \param atts
     \param force
    */
    void onAddEntity( QString type,
                      QString parentEntityId,
                      QMap<QString,QString>& atts,
                      bool force);
    /*!
     \brief

     \param entity
     \param atts
     \param force
    */
    void onEditEntity(Entity *entity, QMap<QString,QString> atts, bool force);
    /*!
     \brief

     \param
     \param force
    */
    void onRemoveEntity(Entity *, bool force);
    /*!
     * \brief TODO
     */
    void setListenFilter(QStringList list);

    /*!
     * \brief TODO
     */
    void setPluginData(QByteArray data);

    /*!
     * \brief TODO
     */
    void setCurrentProjectAsDirty();

signals:
    void entityAdded(QString, Entity*);

};
} } //end namespace
#endif // MESSAGECONTROL_H

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
#ifndef QNSTCOMPOSERPLUGIN_H
#define QNSTCOMPOSERPLUGIN_H

#include <QMap>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

#include "ui/view/qnstview.h"

namespace composer {
    namespace plugin {
        namespace layout {

class QnstComposerPlugin : public IPlugin
{
    Q_OBJECT

public:
    QnstComposerPlugin(QObject* parent = 0);

    ~QnstComposerPlugin();

    virtual void init();

    virtual QWidget* getWidget();

    virtual bool saveSubsession();

public slots:
    virtual void onEntityAdded(QString pluginID, Entity *entity);

    virtual void errorMessage(QString error);

    virtual void onEntityChanged(QString pluginID, Entity *entity);

    virtual void onEntityRemoved(QString pluginID, QString entityID);

    virtual void changeSelectedEntity(QString pluginID, void* entityUID);

    void requestEntityAddition(Entity* entity);

    void requestEntityRemotion(Entity* entity);

    void requestEntityChange(Entity* entity);

    void requestEntitySelection(Entity* entity);

    void requestEntityAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void requestEntityRemotion(const QString uid);

    void requestEntityChange(const QString uid, const QMap<QString, QString> properties);

    void requestEntitySelection(const QString uid);

private:
    void createWidgets();

    void createConnections();

    void requestBodyAddition(Entity* entity);

    void requestBodyChange(Entity* entity);

    void requestContextAddition(Entity* entity);

    void requestContextChange(Entity* entity);

    void requestSwitchAddition(Entity* entity);

    void requestSwitchChange(Entity* entity);

    void requestMediaAddition(Entity* entity);

    void requestMediaChange(Entity* entity);

    void requestPortAddition(Entity* entity);

    void requestPortChange(Entity* entity);

    void requestBodyAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void requestBodyChange(const QString uid, const QMap<QString, QString> properties);

    void requestBodyDependence();

    void requestContextAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void requestContextChange(const QString uid, const QMap<QString, QString> properties);

    void requestSwitchAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void requestSwitchChange(const QString uid, const QMap<QString, QString> properties);

    void requestMediaAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void requestMediaChange(const QString uid, const QMap<QString, QString> properties);

    void requestPortAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void requestPortChange(const QString uid, const QMap<QString, QString> properties);

    void requestConnectorAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void requestConnectorDependence();

    void requestConnectorBaseDependence();

    void requestLinkAddition(const QString uid, const QString parent, const QMap<QString, QString> properties);

    int n;

    QnstView* view;

    QString request;

    QMap<QString, QString> entites;
};

} } } // end namespace

#endif // QNSTCOMPOSERPLUGIN_H

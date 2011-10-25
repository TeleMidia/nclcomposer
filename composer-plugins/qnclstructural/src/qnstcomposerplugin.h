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

#include <QString>
#include <QMap>
#include <QStack>

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

private:
    void createWidgets();

    void createConnections();

    void addBody();

    QString addConnector(QString condition, QString action);

    QnstView* view;

    QMap<QString, QString> m_view_to_comp;

    QMap<QString, QString> m_comp_to_view;

    QStack<QString> bodyStack;
    QStack<QString> mediaStack;
    QStack<QString> contextStack;
    QStack<QString> switchStack;
    QStack<QString> portStack;
    QStack<QString> areaStack;
    QStack<QString> propertyStack;
    QStack<QString> linkStack;

    QStack<QString> bindStack;

    QSet<QString> connectors;

    QString* selectedId;

private slots:
    void requestBodyAddition(const QString entityUID,
                             const QString parentUID,
                             const QMap<QString, QString> attributes);

    void requestContextAddition(const QString entityUID,
                                const QString parentUID,
                                const QMap<QString, QString> attributes);

    void requestMediaAddition(const QString entityUID,
                              const QString parentUID,
                              const QMap<QString, QString> attributes);

    void requestSwitchAddition(const QString entityUID,
                               const QString parentUID,
                               const QMap<QString, QString> attributes);

    void requestPortAddition(const QString entityUID,
                               const QString parentUID,
                               const QMap<QString, QString> attributes);

    void requestPropertyAddition(const QString entityUID,
                               const QString parentUID,
                               const QMap<QString, QString> attributes);

    void requestAreaAddition(const QString entityUID,
                             const QString parentUID,
                             const QMap<QString, QString> attributes);

    void requestLinkAddition(const QString entityUID,
                             const QString parentUID,
                             const QMap<QString, QString> attributes);

    void requestEntitySelection(const QString entityUID);

    void requestEntityRemotion(const QString entityUID);
};

} } } // end namespace

#endif // QNSTCOMPOSERPLUGIN_H

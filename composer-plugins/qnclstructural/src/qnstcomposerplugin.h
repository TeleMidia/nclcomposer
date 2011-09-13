/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
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
};

} } } // end namespace

#endif // QNSTCOMPOSERPLUGIN_H

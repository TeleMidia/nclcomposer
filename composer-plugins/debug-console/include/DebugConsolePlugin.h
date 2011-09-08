/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef DEBUGCONSOLEPLUGIN_H
#define DEBUGCONSOLEPLUGIN_H

#include <QObject>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

namespace composer {
    namespace plugin {
        namespace debug {
/*!
 * \brief Debug Console is a simple plugin that just show
 *    all messages received.
 *
 * It also is a simple way to learn how to create a new plugin.
 */
class DebugConsolePlugin : public IPlugin
{
        Q_OBJECT
    private:
        QListWidget *list;
        QWidget *window;
    public:
        explicit DebugConsolePlugin();
        ~DebugConsolePlugin();

        void init();
        QWidget* getWidget();
        bool saveSubsession();

    public slots:
        void onEntityAdded(QString ID, Entity *);
        void onEntityChanged(QString ID, Entity *);
        /*void onEntityAboutToRemove(Entity *);*/
        void onEntityRemoved(QString ID, QString entityID);

        void errorMessage(QString error);

        void sendToAll();
};

} } }//end namespace
#endif // DEBUGCONSOLEPLUGIN_H

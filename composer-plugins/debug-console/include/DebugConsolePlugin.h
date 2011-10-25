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

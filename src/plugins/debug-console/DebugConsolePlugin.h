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
#ifndef DEBUGCONSOLEPLUGIN_H
#define DEBUGCONSOLEPLUGIN_H

#include <QGridLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <QPushButton>

#include <extensions/IPlugin.h>
#include <extensions/IPluginFactory.h>
using namespace cpr::core;

/*!
 * \brief Debug Console is a simple plugin that just show
 *    all messages received.
 *
 * It also is a simple way to learn how to create a new plugin.
 */
class DebugConsolePlugin : public IPlugin
{
  Q_OBJECT

public:
  explicit DebugConsolePlugin ();
  ~DebugConsolePlugin ();

  void init () override;
  QWidget *widget () override;
  bool saveSubsession () override;

public slots:
  void onEntityAdded (const QString &, Entity *) override;
  void onEntityChanged (const QString &, Entity *) override;
  void onEntityRemoved (const QString &, const QString &) override;

  void errorMessage (const QString &) override;

  void sendToAll ();

private:
  QListWidget *_list;
  QWidget *_window;
};

/*!
 * \brief Handles the creation and deletion of DebugConsolePlugin objects.
 */
class DebugConsoleFactory : public QObject,
                            public IPluginFactoryTpl<DebugConsolePlugin>
{
  Q_OBJECT
  CPR_PLUGIN_METADATA ("debug-console.json")

public:
  QString
  id () const override
  {
    return "br.puc-rio.telemidia.DebugConsole";
  }
  QIcon
  icon () const override
  {
    return QIcon (":/images/icon.png");
  }
};

#endif // DEBUGCONSOLEPLUGIN_H

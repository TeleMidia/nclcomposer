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
#ifndef NCLEXPORTPLUGIN_H
#define NCLEXPORTPLUGIN_H

#include <QObject>
#include <QFile>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

/*!
 * \brief Debug Console is a simple plugin that just show
 *    all messages received.
 *
 * It also is a simple way to learn how to create a new plugin.
 */
class nclExportPlugin : public IPlugin
{
  Q_OBJECT

private:
  //QListWidget *list;
  QWidget *window;
public:
  explicit nclExportPlugin();
  ~nclExportPlugin();

  void init();
  QWidget* getWidget();

public slots:
  bool saveSubsession();
};

#endif // NCLEXPORTPLUGIN_H

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
#ifndef STRUCTUALVIEWPLUGIN_H
#define STRUCTUALVIEWPLUGIN_H

#include <QMap>
#include <QVector>

#include <extensions/IPlugin.h>
#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "StructuralScene.h"
#include "StructuralView.h"
#include "StructuralWindow.h"

class StructuralViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  StructuralViewPlugin (QObject *parent = 0);
  virtual ~StructuralViewPlugin () {}

  virtual void init ();
  virtual QWidget *widget ();
  virtual bool saveSubsession ();

public slots:
  virtual void updateFromModel ();

  virtual void onEntityAdded (const QString &pluginID, Entity *ent);
  virtual void onEntityChanged (const QString &pluginID, Entity *ent);
  virtual void onEntityRemoved (const QString &pluginID, const QString &entID);
  virtual void changeSelectedEntity (const QString &pluginID, void *entUID);

  virtual void errorMessage (const QString &error);
  void clearValidationMessages (QString pluginID, void *param);
  void validationError (QString pluginID, void *param);

  void insertInView (Entity *entity, bool undo = true);
  void removeInView (Entity *entity, bool undo = true);
  void changeInView (Entity *entity);
  void selectInView (Entity *entity);

  void insertInCore (QString uid, QString parent, QStrMap props, QStrMap stgs);
  void removeInCore (QString uid, QStrMap stgs);
  void changeInCore (QString uid, QStrMap props, QStrMap previous,
                     QStrMap settings);
  void selectInCore (QString uid);

  void adjustConnectors ();

  void textualStartSync (QString, void *);  /* from textual plugin */
  void textualFinishSync (QString, void *); /* from textual plugin */

private:
  void createConnections ();

  QString uidById (const QString &id, Entity *ent = nullptr);

  void setReferences (QStrMap &props);

  QStrMap viewPropsFromCoreEntity (const Entity *ent);
  QStrMap coreAttrsFromStructuralEntity (const QStrMap &props);

  void clean ();

private:
  bool _syncing;
  bool _waiting;

  QString _notified;
  QString _selected;

  StructuralWindow _window;

  QStrMap _core2view, _view2core;
  StructuralScene *_struct_scene;
  StructuralView *_struct_view;
};

class StructuralViewFactory : public QObject,
                              public IPluginFactoryTpl<StructuralViewPlugin>
{
  Q_OBJECT
  CPR_PLUGIN_METADATA ("br.puc-rio.telemidia.structural",
                       "ncl-structural-view.json")
};

#endif // STRUCTUALVIEWPLUGIN_H

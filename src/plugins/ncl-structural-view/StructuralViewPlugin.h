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

  void init () override;
  QWidget *widget () override;
  bool saveSubsession () override;

public slots:
  void updateFromModel () override;

  void onEntityAdded (const QString &plgID, Entity *ent) override;
  void onEntityChanged (const QString &plgID, Entity *ent) override;
  void onEntityRemoved (const QString &plgID, const QString &entID) override;
  void changeSelectedEntity (const QString &plgID, void *entUID);

  void errorMessage (const QString &error) override;

  void clearValidationMessages (QString plgID, void *param);
  void validationError (QString plgID, void *param);

  void insertInView (Entity *ent, bool undo = true);
  void removeInView (Entity *ent, bool undo = true);
  void changeInView (Entity *ent);
  void selectInView (Entity *ent);

  void insertInCore (QString uid, QString parent, QStrMap props, QStrMap stgs);
  void removeInCore (QString uid, QStrMap stgs);
  void changeInCore (QString uid, QStrMap props, QStrMap previous,
                     QStrMap stgs);
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

  bimap<QString, QString> _core_view_bimap;
  #define _core2view _core_view_bimap.to ()
  #define _view2core _core_view_bimap.from ()

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

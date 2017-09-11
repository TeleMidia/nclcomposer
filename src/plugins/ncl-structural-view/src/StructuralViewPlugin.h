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

#include <extensions/IPlugin.h>
using namespace cpr::core;

#include "StructuralScene.h"
#include "StructuralView.h"
#include "StructuralWindow.h"

class StructuralViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  StructuralViewPlugin (QObject *parent = 0);
  virtual ~StructuralViewPlugin ();

  virtual void init ();
  virtual QWidget *getWidget ();
  virtual bool saveSubsession ();

public slots:
  virtual void updateFromModel ();

  virtual void onEntityAdded (const QString &pluginID, Entity *entity);
  virtual void onEntityChanged (const QString &pluginID, Entity *entity);
  virtual void onEntityRemoved (const QString &pluginID, const QString &entityID);
  virtual void changeSelectedEntity (const QString &pluginID, void *entityUID);

  virtual void errorMessage (const QString &error);
  void clearValidationMessages (QString pluginID, void *param);
  void validationError (QString pluginID, void *param);

  void insertInView (Entity *entity, bool undo = true);
  void removeInView (Entity *entity, bool undo = true);
  void changeInView (Entity *entity);
  void selectInView (Entity *entity);

  void insertInCore (QString uid, QString parent,
                     QMap<QString, QString> properties,
                     QMap<QString, QString> settings);
  void removeInCore (QString uid, QMap<QString, QString> settings);
  void changeInCore (QString uid, QMap<QString, QString> properties,
                     QMap<QString, QString> previous,
                     QMap<QString, QString> settings);
  void selectInCore (QString uid, QMap<QString, QString> settings);

  void adjustConnectors ();

  void textualStartSync (QString, void *);  /* from textual plugin */
  void textualFinishSync (QString, void *); /* from textual plugin */

private:
  void createWidgets ();
  void createConnections ();

  QString createCacheId(StructuralEntity* entity);

  QString getUidById (const QString &id);
  QString getUidById (const QString &id, Entity *entity);
  QString getUidByName (const QString &name, Entity *entity);

  void setReferences (QMap<QString, QString> &properties);

  void clean ();

private:
  bool _syncing;
  bool _waiting;

  QString _notified;
  QString _selected;

  StructuralWindow *_window;

  QMap<QString, QString> _mapCoreToView;
  QMap<QString, QString> _mapViewToCore;
};

#endif // STRUCTUALVIEWPLUGIN_H

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

#include "StructuralWindow.h"
#include "StructuralView.h"
#include "StructuralScene.h"

class StructuralViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  StructuralViewPlugin(QObject* parent = 0);
  virtual ~StructuralViewPlugin();

  virtual void init();
  virtual QWidget* getWidget();
  virtual bool saveSubsession();

public slots:
  virtual void updateFromModel();

  virtual void onEntityAdded(QString pluginID, Entity *entity);
  virtual void onEntityChanged(QString pluginID, Entity *entity);
  virtual void onEntityRemoved(QString pluginID, QString entityID);
  virtual void changeSelectedEntity(QString pluginID, void* entityUID);

  virtual void errorMessage(QString error);
  void clearValidationMessages(QString pluginID, void *param);
  void validationError(QString pluginID, void *param);

  /* From Core */
  void requestEntitySelection(const QString uid);
  void requestEntityAddition(Entity* entity, bool enableUndo = true);
  void requestEntityRemotion(Entity* entity, bool enableUndo = true);
  void requestEntityChange(Entity* entity);
  void requestEntitySelection(Entity* entity);

  void updateConnectorsDataInView();

  /* From View */
  void notifyEntityAddedInView (const QString uid, const QString parent,
                                QMap<QString, QString> properties, QMap<QString, QString> settings);

  void notifyEntityDeletedInView(const QString uid, QMap<QString, QString> settings);

  void notifyEntitySelectionInView(const QString uid, QMap<QString, QString> settings);

  void notifyEntityChangedInView(const QString uid,
                                 QMap<QString, QString> properties,
                                 QMap<QString, QString> previous,
                                 QMap<QString, QString> settings);

private:
  void clear();

  QString getUidById(const QString &id);
  QString getUidById(const QString &id, Entity* entity);
  QString getUidByName(const QString &name, Entity* entity);

  void createWidgets();

  void createConnections();

  /* FROM QNSTVIEW */
  void requestBodyDependence();

private:

  bool isConnector;
  StructuralWindow* _window;

  QString _notified;
  QMap <QString, QString> entities; // core -> structural
  QMap <QString, QString> nclIDtoStructural; // nclId -> structural ID

  QMap <QString, QString> xconnector_viewLink; // nclId -> structural ID
  QList <QString> dirtyEntities;
  QList <QString> previousCoreID;

  QString lastSelected;

  QString *_selectedId;

  bool _synching;
  bool _waiting;


public slots:
  void textualStartSync(QString, void*); /* from textual plugin */
  void textualFinishSync(QString, void*); /* from textual plugin */
  /* End "synchronization with core". */
};

#endif // QNSTCOMPOSERPLUGIN_H

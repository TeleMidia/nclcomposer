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

#include "StructuralView.h"
#include "StructuralWindow.h"

//
// ATTENTION: This code needs a refactoring.
//

class StructuralViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  StructuralViewPlugin(QObject* parent = 0);

  ~StructuralViewPlugin();

  virtual void init();

  virtual QWidget* getWidget();

  virtual bool saveSubsession();

public slots:
  virtual void updateFromModel();

  virtual void onEntityAdded(QString pluginID, Entity *entity);

  virtual void errorMessage(QString error);

  virtual void onEntityChanged(QString pluginID, Entity *entity);

  virtual void onEntityRemoved(QString pluginID, QString entityID);

  virtual void changeSelectedEntity(QString pluginID, void* entityUID);

  void clearValidationError(QString pluginID, void *param);

  void validationError(QString pluginID, void *param);


  /* From Core */
  void requestEntitySelection(const QString uid);

  void requestEntityAddition(Entity* entity);

  void requestEntityRemotion(Entity* entity);

  void requestEntityChange(Entity* entity);

  void requestEntitySelection(Entity* entity);

  /* From View */
  void notifyEntityAddedInView (const QString uid, const QString parent,
                                QMap<QString, QString> properties);

  void notifyEntityDeletedInView(const QString uid);

  void notifyEntityChangedInView(const QString uid,
                                 QMap<QString, QString> properties);

private:
  QString getUidById(QString id);

  QString getUidById(QString id, Entity* entity);

  QString getUidByName(QString name, Entity* entity);

  void createWidgets();

  void createConnections();

  /* FROM NCL COMPOSER CORE */
  void requestImportBaseAddition(Entity* entity);

  void requestImportBaseChange(Entity* entity);

  void requestCausalConnectorAddition(Entity* entity);

  void requestCausalConnectorChange(Entity* entity);

  void requestSimpleConditionAddition(Entity* entity);

  void requestSimpleConditionChange(Entity* entity);

  void requestSimpleActionAddition(Entity* entity);

  void requestSimpleActionChange(Entity* entity);

  void requestConnectorParamAddition(Entity* entity);

  void requestConnectorParamChange(Entity* entity);
  /* END FROM NCL COMPOSER CORE */



  /* FROM QNSTVIEW */
  void requestBodyDependence();

  void requestConnectorAddition(const QString uid, const QString parent,
                                const QMap<QString, QString> &properties);

  void requestComplexConnectorAddition(const QString uid,
                                       const QString parent,
                                       const QMap<QString, QString> &properties);

  void requestBindAddition(const QString uid, const QString parent,
                           const QMap<QString, QString> &properties);

  void requestConnectorDependence();

  void requestConnectorBaseDependence();
  /* END FROM QNSTVIEW */


private:
  int n;

  //View* view;
  StructuralWindow* window;

  QString request;
  QMap <QString, QString> entities; // core -> structural
  QMap <QString, QString> nclIDtoStructural; // nclId -> structural ID
  QList <QString> dirtyEntities;
  QList <QString> previousCoreID;

  QString lastSelected;

  /* Functions to handle the "sinchronization with core" */
  bool isSyncingFromTextual;
  void cacheNCLIds();
  QString insertNCLIDIfEmpty(Entity *entity);
  QString getNCLIdFromEntity(Entity *entity);
  bool isEntityHandled(Entity *entity);
  void syncNCLIdsWithStructuralIds();

public slots:
  void textualStartSync(QString, void*); /* from textual plugin */
  void textualFinishSync(QString, void*); /* from textual plugin */
  /* End "synchronization with core". */
};

#endif // QNSTCOMPOSERPLUGIN_H

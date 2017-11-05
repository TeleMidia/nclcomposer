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
#ifndef QNLYCOMPOSERPLUGIN_H
#define QNLYCOMPOSERPLUGIN_H

#include <QMap>
#include <QString>

#include <extensions/IPlugin.h>
using namespace cpr::core;

#include "LayoutView.h"
#include "LayoutWindow.h"

class NCLLayoutViewPlugin : public IPlugin
{
  Q_OBJECT

public:
  explicit NCLLayoutViewPlugin (QObject *parent = 0);
  virtual ~NCLLayoutViewPlugin ();

  virtual void init () override;
  virtual QWidget *widget () override;
  virtual bool saveSubsession () override;

public slots:
  virtual void onEntityAdded (const QString &pluginID,
                              Entity *entity) override;
  virtual void errorMessage (const QString &error) override;
  virtual void onEntityChanged (const QString &pluginID,
                                Entity *entity) override;
  virtual void onEntityRemoved (const QString &pluginID,
                                const QString &entityID) override;
  virtual void updateFromModel () override;

  void changeSelectedEntity (const QString &pluginID, void *entityUID);

protected slots:
  void addRegionToView (Entity *entity);
  void removeRegionFromView (QString entityUID);
  void changeRegionInView (Entity *entity);
  void selectRegionInView (QString entityUID);

  void addRegionBaseToView (Entity *entity);
  void removeRegionBaseFromView (QString entityUID);
  void changeRegionBaseInView (Entity *entity);
  void selectRegionBaseInView (QString entityUID);

  void addDescriptorToView (Entity *entity);
  void removeDescriptorFromView (const QString &entityUID);
  void updateDescriptors ();

  void addRegion (const QString &regionUID, const QString &parentUID,
                  const QString &regionbaseUID,
                  const QMap<QString, QString> &attrs);
  void removeRegion (const QString &regionUID, const QString &regionbaseUID);
  void changeRegion (const QString &regionUID, const QString &regionbaseUID,
                     const QMap<QString, QString> &attrs);
  void selectRegion (const QString &regionUID, const QString &regionbaseUID);

  void addRegionBase (const QString &regionbaseUID,
                      const QMap<QString, QString> &attrs);
  void removeRegionBase (const QString &regionbaseUID);
  void changeRegionBase (const QString &regionbaseUID,
                         const QMap<QString, QString> &attrs);
  void selectRegionBase (const QString &regionbaseUID);

  void performMediaOverRegionAction (const QString &mediaId,
                                     const QString &regionUID);

private:
  void clear ();
  void loadRegionbase ();
  void loadRegion (Entity *region);

  void createView ();
  void createConnections ();

  /*!
   * \brief Get the head id of the document. If this head does not exists
   *  this function will create it.
   */
  QString headUid ();
  QMap<QString, QString> regionAttributes (Entity *region);

  QString *_selectedId;

  LayoutWindow *_mainWindow;
  LayoutView *_view;

  QMap<QString, Entity *> _regions;
  QMap<QString, Entity *> _regionbases;
  QMap<QString, QString> _relations;
  QMap<QString, QMap<QString, QString> > _descriptors;
};

#endif // QNLYCOMPOSERPLUGIN_H

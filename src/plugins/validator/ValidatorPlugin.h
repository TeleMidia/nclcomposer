/*
 * Copyright 2011-2013 Laws/UFMA.
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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef VALIDATORPLUGIN_H
#define VALIDATORPLUGIN_H

#include <QHBoxLayout>
#include <QVector>
#include <QWidget>
#include <QSize>

#include <extensions/IPlugin.h>
#include <extensions/IPluginFactory.h>
using namespace cpr::core;

#include "NCLComposerAdapter.h"
#include "nclmodel/nclmodel.h"
#include "validation/Validator.h"
#include "ValidatorTreewidgetItem.h"

class ValidatorTreeWidget : public QTreeWidget
{
public:
  ValidatorTreeWidget (QWidget *parent = 0) : QTreeWidget (parent) {}

  QSize
  sizeHint () const
  {
    return QSize (900, 200);
  }
};

class ValidatorPlugin : public IPlugin
{
  Q_OBJECT
public:
  ValidatorPlugin ();

  QWidget *
  getWidget ()
  {
    return table;
  }
  bool
  saveSubsession ()
  {
    return true;
  }
  void init ();

public slots:
  void onEntityAdded (const QString &pluginID, Entity *);
  void onEntityChanged (const QString &pluginID, Entity *);
  void onEntityRemoved (const QString &pluginID, const QString &entityID);

  void errorMessage (const QString &error);

  void itemSelected (QTreeWidgetItem *);
  void itemDoubleClickedSelected (QTreeWidgetItem *);

  // Composer Messages
  void clearValidationMessages (QString, void *);
  void validationError (QString, void *);
  void validationErrorSelected (QString, void *);
  void askAllValidationMessages (QString, void *);

private:
  void updateModel (Entity *);
  void updateMessages (std::vector<pair<void *, string> >);

  QVector<pair<QString, QString> > pairsMessages;

  QWidget *window;
  ValidatorTreeWidget *table;
  ComposerNCLAdapter adapter;
};

/*!
 * \brief The ValidatorFactory class
 */
class ValidatorFactory : public QObject,
    public IPluginFactoryTpl<ValidatorPlugin>
{
  Q_OBJECT
  Q_INTERFACES (IPluginFactory)
  Q_PLUGIN_METADATA (IID IPluginFactory_iid FILE "validator-plugin.json")

public:
  QString id () const { return "br.ufma.deinf.laws.validator"; }

  QIcon icon () const { return QIcon (); }
};

#endif // VALIDATORPLUGIN_H

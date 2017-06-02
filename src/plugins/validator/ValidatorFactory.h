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

#ifndef PLUGINVALIDATOR_H
#define PLUGINVALIDATOR_H

#include <QWidget>
#include <validation/Validator.h>
#include <extensions/IPluginFactory.h>
#include "ValidatorPlugin.h"

using namespace cpr::core;

class ValidatorFactory :
        public QObject, public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "validator-plugin.json")

public:
  ValidatorFactory(QWidget *parent = 0);
  ~ValidatorFactory();

  IPlugin* createPluginInstance();
  void releasePluginInstance(IPlugin *);
  QList<LanguageType> getSupportedLanguages();
  QString id() const {return "br.ufma.deinf.laws.validator";}
  QIcon icon() const {return QIcon ();}
};



#endif // PLUGINVALIDATOR_H

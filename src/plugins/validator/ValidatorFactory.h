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

using namespace composer::extension;

class ValidatorFactory : public QObject,
    public IPluginFactory
{
  Q_OBJECT
  Q_INTERFACES(IPluginFactory)

#if QT_VERSION >= 0x050000
  Q_PLUGIN_METADATA(IID IPluginFactory_iid FILE "validator_plugin.json")
#endif

public:
  ValidatorFactory(QWidget *parent = 0);
  ~ValidatorFactory();

  IPlugin* createPluginInstance();

  void releasePluginInstance(IPlugin *);

  QList<LanguageType> getSupportedLanguages();

  QString id() const {return "br.ufma.deinf.laws.validator";}

  QIcon icon() const {return QIcon ();}

  QWidget* getPreferencePageWidget() {return 0;}

  void setDefaultValues() {};

  void applyValues() {};

#if QT_VERSION < 0x050000

  /* Informações úteis que devem estar no plugin (em um XML?)*/
  QString name() { return "Validator View"; }
  QString version() { return NCLCOMPOSER_PLUGINS_VERSION;  }
  QString compatVersion() {return "0.1";}
  QString vendor() {return "Laws Lab";}
  QString copyright() {return "Laws Lab/UFMA";}
  QString license() {return "LGPL";}
  QString description() {return tr("Validator View validates the current "
                                   "document being edited and shows error "
                                   "messages.");}
  QString url() {return "http://www.laws.deinf.ufma.br/nclvalidator";}
  QString category() {return "NCL";}
  
#endif

};



#endif // PLUGINVALIDATOR_H

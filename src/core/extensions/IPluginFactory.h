/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#ifndef ILAYOUTPLUGIN_H
#define ILAYOUTPLUGIN_H

#include <QtPlugin>
#include <QString>
#include <QtGui/QIcon>

#include <QJsonObject>

#include "../util/ComposerCoreControl_global.h"

#include "../util/Utilities.h"
using namespace composer::core::util;

#include "IPlugin.h"
using namespace composer::extension;

namespace composer {
  namespace extension {
/*!
 * \brief A Factory interface for building plugin instances.
 *
 * This extension point is used by the core to build instances of an specific
 * plugin. Each instance can be related to a different open project. Each plugin
 * instance is binded with its project instance during the setup of this
 * project. The whole process is transparent for plugin developers.
 */
class COMPOSERCORESHARED_EXPORT IPluginFactory
{
public:
  virtual ~IPluginFactory() { }

  /*!
   * \brief Through this call the core is able to create a new plugin instance.
   *
   * \return The new plugin instance.
   */
  virtual IPlugin* createPluginInstance() = 0;

  /*!
   * \brief When an Project is closed by the user and it is no longer
   *     necessary, Composer Core will use this call to release the plugin
   *     instance.
   *
   * When an Project is closed by the user and it is no longer necessary,
   * Composer Core will use this call to release the plugin instance.that was
   * associated with this Project. By default, the plugin developer should
   * free all memory it is using. But, this is he/she choice, once his
   * implementation can also share some data among all the plugins instances.
   *
   * \param The plugin instance.
   */
  virtual void releasePluginInstance(IPlugin *) = 0;

  virtual QString id() const = 0;

  virtual QIcon icon() const { return QIcon(); }
  virtual QWidget* getPreferencePageWidget() { return nullptr; }
  virtual void setDefaultValues() {}
  virtual void applyValues() {}

  virtual QList<LanguageType> getSupportedLanguages()
  {
    QList<LanguageType> lTypes;
    lTypes.append(NCL);
    return lTypes;
  }

  void setMetadata(const QJsonObject &obj) { this->metadata_obj = obj; }
  QJsonObject metadata() { return this->metadata_obj; }

private:
    QJsonObject metadata_obj;
};

} } //end namespace

/**
 * This is required for the QTPlugin system works.
 * Declaring the interface that is going to be used by external plugins.
 */
#define IPluginFactory_iid "br.puc_rio.telemidia.composer.IPluginFactory"
Q_DECLARE_INTERFACE(IPluginFactory, IPluginFactory_iid)
#endif

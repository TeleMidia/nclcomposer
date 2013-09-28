/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef ILAYOUTPLUGIN_H
#define ILAYOUTPLUGIN_H

#include <QtPlugin>
#include <QString>
#include <QtGui/QIcon>

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
class COMPOSERCORESHARED_EXPORT IPluginFactory {

public:
    virtual ~IPluginFactory() {}

    /*!
     * \brief Through this call the core is able to create a new plugin
     *      instance.
     *
     * \return The new plugin instance.
     */
    virtual IPlugin* createPluginInstance()  = 0;

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
    virtual QString name() const = 0;

    virtual QIcon icon() const { return QIcon(); }
    virtual QWidget* getPreferencePageWidget() { return NULL; }
    virtual void setDefaultValues() {}
    virtual void applyValues() {}

    virtual QList<LanguageType> getSupportedLanguages()
    {
        QList<LanguageType> lTypes;
        lTypes.append(NCL);
        return lTypes;
    }

    /*Useful information about the plugin (go to XML?)*/
    virtual QString version() { return "Unknown";}
    virtual QString compatVersion() {return "Unknown";}
    virtual QString vendor() {return "Unknown";}
    virtual QString copyright() {return "Unknown";}
    virtual QString license() {return "Unknown";}
    virtual QString description() {return "Unknown";}
    virtual QString url() {return "Unknown";}
    virtual QString category() {return "Unknown";}
};

} } //end namespace

/**
 * This is required for the QTPlugin system works.
 * Declaring the interface that is going to be used by external plugins.
 */
#define IPluginFactory_iid "br.puc_rio.telemidia.composer.IPluginFactory"
Q_DECLARE_INTERFACE(IPluginFactory, IPluginFactory_iid)

#endif // ILAYOUTPLUGIN_H

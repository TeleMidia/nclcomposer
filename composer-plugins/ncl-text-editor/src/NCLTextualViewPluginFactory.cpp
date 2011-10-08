/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "NCLTextualViewPluginFactory.h"
#include "ui_TextualPluginPreferencesWidget.h"

namespace composer {
    namespace plugin {
        namespace textual {

NCLTextualViewPluginFactory::NCLTextualViewPluginFactory()
{
    prefPageWidget = new QFrame();
    prefPageUi = new Ui::TextPluginPreferencesWidget();
    prefPageUi->setupUi(prefPageWidget);
}

IPlugin* NCLTextualViewPluginFactory::createPluginInstance()
{
    return new NCLTextualViewPlugin();
}

void NCLTextualViewPluginFactory::releasePluginInstance(IPlugin *plugin)
{
    NCLTextualViewPlugin *textualView =
            qobject_cast<NCLTextualViewPlugin*>(plugin);

    if (textualView)
    {
        delete textualView;
        textualView = NULL;
    }
}

QString NCLTextualViewPluginFactory::id() const
{
    return "br.puc-rio.telemidia.NCLTextualView";
}

QString NCLTextualViewPluginFactory::name() const
{
    return "NCL Textual View";
}

QIcon NCLTextualViewPluginFactory::icon() const
{
    return QIcon(":/images/ncl.png");
}

QWidget* NCLTextualViewPluginFactory::getPreferencePageWidget()
{
    return prefPageWidget;
}

void NCLTextualViewPluginFactory::setDefaultValues()
{
    //TODO
}

void NCLTextualViewPluginFactory::applyValues()
{
    //TODO
}

Q_EXPORT_PLUGIN2(ncl_textual_plugin,NCLTextualViewPluginFactory)

}}} // end namespace

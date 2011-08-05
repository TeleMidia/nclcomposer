/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "NCLTextEditorQtDesignerPlugin.h"

NCLTextEditorPlugin::NCLTextEditorPlugin()
{
}

QString NCLTextEditorPlugin::name() const
{
    return "NCLTextEditor";
}

QString NCLTextEditorPlugin::group() const
{
    return "Input";
}

QString NCLTextEditorPlugin::toolTip() const
{
    return "NCL Text Editor Widget";
}

QWidget* NCLTextEditorPlugin::createWidget (QWidget* parent)
{
    return new NCLTextEditor(parent);
}

QString NCLTextEditorPlugin::includeFile() const
{
        return "NCLTextEditor.h";
}

QIcon NCLTextEditorPlugin::icon() const
{
    return QIcon();
}

QString NCLTextEditorPlugin::whatsThis() const
{
    return "A widget to create and edit a NCL file";
}

bool NCLTextEditorPlugin::isContainer() const
{
    return false;
}


Q_EXPORT_PLUGIN2(ncltexteditorplugin, NCLTextEditorPlugin)

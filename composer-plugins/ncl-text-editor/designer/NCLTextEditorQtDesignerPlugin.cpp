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

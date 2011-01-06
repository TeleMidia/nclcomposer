#include "NCLTextEditorPlugin.h"

NCLTextEditorPlugin::NCLTextEditorPlugin()
{
}

QString NCLTextEditorPlugin::name() const
{
    return "NCLTextEdit";
}

QString NCLTextEditorPlugin::group() const
{
    return "Input";
}

QString NCLTextEditorPlugin::toolTip() const
{
    return "NCL Text Editor Widget";
}

QWidget* NCLTextEditorPlugin::createWidget(QWidget* parent)
{
    return new NCLTextEdit(parent);
}

QString NCLTextEditorPlugin::includeFile() const
{
        return "NCLTextEdit.h";
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

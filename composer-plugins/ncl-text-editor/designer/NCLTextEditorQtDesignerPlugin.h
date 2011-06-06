#ifndef NCLTEXTEDITORPLUGIN_H
#define NCLTEXTEDITORPLUGIN_H

#include <QtPlugin>
#include <QList>
#include <QDesignerCustomWidgetInterface>

#include "NCLTextEditor.h"

class NCLTextEditorPlugin : public QObject, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetInterface)
public:
    NCLTextEditorPlugin();

    QString name() const;
    QString group() const;
    QString toolTip() const;
    QString whatsThis() const;
    QString includeFile() const;
    QIcon icon() const;
    bool isContainer() const;

    QWidget *createWidget(QWidget *parent);

public slots:

signals:

};

#endif // NCLTEXTEDITORPLUGIN_H

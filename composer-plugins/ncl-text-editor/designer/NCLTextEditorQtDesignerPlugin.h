/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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

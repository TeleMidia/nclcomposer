/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef WORKSPACESWITCH_H
#define WORKSPACESWITCH_H

#include <QDialog>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QFileDialog>
#include <QDir>

namespace composer {
    namespace gui {

/*!
 * \deprecated
 */
class WorkspaceSwitch : public QDialog
{
    Q_OBJECT
public:
    explicit WorkspaceSwitch(QWidget *parent = 0);
    inline QString getWorspacePath() { return wsPath->text(); }
private:
    QLineEdit *wsPath;
    QPushButton *openDir;
    QPushButton *bOk;
    QPushButton *bCancel;

    void init();

signals:

private slots:
    void openDirectory();

};

}} //end namespace

#endif // WORKSPACESWITCH_H

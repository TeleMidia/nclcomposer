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

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

#include "WorkspaceSwitch.h"

namespace composer {
    namespace gui {

WorkspaceSwitch::WorkspaceSwitch(QWidget *parent) :
    QDialog(parent)
{
    init();
}

void WorkspaceSwitch::init()
{
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Choose the workspace "
                                    "path for this session"),this));
    wsPath = new QLineEdit(this);
    wsPath->setEnabled(false);
    layout->addWidget(wsPath,1,0);
    openDir = new QPushButton(tr("Choose Directory"),this);
    connect(openDir,SIGNAL(clicked()),SLOT(openDirectory()));
    layout->addWidget(openDir,1,1);
    QFrame *line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line,4,0,1,2);

    bOk = new QPushButton(tr("OK"),this);
    connect(bOk,SIGNAL(clicked()),SLOT(accept()));
    bCancel = new QPushButton(tr("Cancel"), this);
    connect(bCancel,SIGNAL(clicked()),SLOT(reject()));

    QHBoxLayout *bL = new QHBoxLayout;
    bL->addWidget(bCancel);
    bL->addWidget(bOk);
    layout->addLayout(bL,5,1);

}

void WorkspaceSwitch::openDirectory()
{
    QFileDialog::Options options = QFileDialog::ShowDirsOnly;
    QDir *dir = new QDir(QDir::homePath());
    QString directoryURI = QFileDialog::getExistingDirectory(this,
                                tr("Select the target directory"),
                                dir->absolutePath(),
                                options);
    wsPath->setText(directoryURI);
}

}} //end namespace

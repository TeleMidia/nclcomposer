/*
 * Copyright 2011-2013 TeleMidia/PUC-Rio.
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
#include "nclExportPlugin.h"

nclExportPlugin::nclExportPlugin()
{
  /*
  window = new QWidget();
  QGridLayout *layout = new QGridLayout(window);
  QPushButton *bt = new QPushButton(window);
  bt->setText(tr("Clear"));
  layout->addWidget(bt);
  list = new QListWidget(window);
  list->setAlternatingRowColors(true);
  layout->addWidget(list);
  connect(bt, SIGNAL(clicked()), list, SLOT(clear()));
  connect(bt, SIGNAL(clicked()), this, SLOT(sendToAll()));
  window->setLayout(layout);
  window->setWindowIcon(QIcon(":/images/icon.png"));
  project = NULL;
  */
}

nclExportPlugin::~nclExportPlugin()
{
  /*
  if(window != NULL)
    delete window;
  window = NULL;
  */
}

void nclExportPlugin::init()
{
  qDebug() << "EXPORT SAYS:IT STARTS" ;
}

QWidget* nclExportPlugin::getWidget()
{
  return NULL; //No window to return
}

bool nclExportPlugin::saveSubsession()
{
  //qDebug() << "EXPORT DISPLAYS " << project->getChildren().at(0)->toString(0, false);
  QString nclfilepath = project->getLocation();
  nclfilepath.chop(4);
  nclfilepath += ".ncl";
  QFile file(nclfilepath);
  if(file.open(QFile::WriteOnly | QIODevice::Truncate))
  {
    // Write FILE!!
    if(project->getChildren().size())
      file.write(project->getChildren().at(0)->toString(0, false).toLatin1());

    file.close();
  }
  return true;
}



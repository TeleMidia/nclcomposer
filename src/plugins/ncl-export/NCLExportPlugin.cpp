/*
 * Copyright 2011-2018 TeleMidia/PUC-Rio.
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
#include "NCLExportPlugin.h"

nclExportPlugin::nclExportPlugin() { }

nclExportPlugin::~nclExportPlugin() { }

void nclExportPlugin::init()
{
  qDebug() << "EXPORT SAYS:IT STARTS" ;
}

QWidget* nclExportPlugin::getWidget()
{
  return nullptr; //No window to return
}

bool nclExportPlugin::saveSubsession()
{
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


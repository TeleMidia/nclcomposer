/*
 * Copyright 2011-2013 Laws/UFMA.
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
 * License along with this library. If not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef COMPOSERNCLADAPTER_H
#define COMPOSERNCLADAPTER_H

#include "message/message.h"
#include "nclmodel/nclmodel.h"
#include "validation/Validator.h"
#include "message/message.h"
#include <model/Entity.h>
#include <QDebug>
#include <QMap>
#include <vector>
#include <string>

using namespace cpr::core;

class ComposerNCLAdapter
{
public:
  ComposerNCLAdapter();

  void addElement (Entity *);
  void removeElement (QString);
  void changeElement (Entity *);

  inline Model &getModel ()
  {
    return nclModel;
  }
  inline void clearModel ()
  {
    nclModel.clear();
  }
  inline void setRelativePath (QString path)
  {
    nclModel.setRelativePath(path.toStdString());
  }

  std::vector <std::pair<void *, std::string> > validate ();

private:
  std::vector<Attribute> createVectorAttribute (Entity *);
  Model nclModel;
  QMap <QString, virtualId> idToVirtualId;
  QString language;
};

#endif // COMPOSERNCLADAPTER_H

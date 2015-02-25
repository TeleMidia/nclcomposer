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

#include "composerncladapter.h"

ComposerNCLAdapter::ComposerNCLAdapter()
{
  GlobalSettings settings;
  settings.beginGroup("languages");
  language = settings.value("currentLanguage",QString("en")).toString();
  settings.endGroup();
}

void ComposerNCLAdapter::addElement(Entity *entity)
{
  if (!entity)
    return;

  std::vector <Attribute> attributes =
      createVectorAttribute(entity);

  if (!idToVirtualId.count(entity->getParentUniqueId()))
  {
    Entity *parent = entity->getParent();

    if (parent->getType() != "document")
      addElement(parent);
  }

  virtualId virtualId =
      nclModel.addElement(entity->getType().toStdString(), attributes);

  if (virtualId != "")
    idToVirtualId.insert(entity->getUniqueId(), virtualId);

  if (entity->getParent()->getType() != "document")
  {
    nclModel.addChild(idToVirtualId[entity->getParentUniqueId()], virtualId);
  }

  ModelElement *el = nclModel.element(virtualId);
  if (el)
  {
    el->setData(entity);
  }
}

void ComposerNCLAdapter::changeElement(Entity * entity)
{  
  virtualId virtualId = idToVirtualId[entity->getUniqueId()];

  if (virtualId == "")
    return;

  std::vector <Attribute> attributes =
      createVectorAttribute(entity);

  nclModel.editElement(virtualId, attributes);
}

void ComposerNCLAdapter::removeElement(QString entityID)
{  
  virtualId virtualId = idToVirtualId [entityID];

  if (virtualId != "")
  {
    nclModel.removeElement(virtualId);
  }
}

std::vector <std::pair<void *, std::string> > ComposerNCLAdapter::validate()
{
  std::vector<std::pair<void *, std::string> > msgs =
      Validator::validate(nclModel, language.toStdString());

  nclModel.clearMarkedElements();

  return msgs;
}

std::vector <Attribute>
ComposerNCLAdapter::createVectorAttribute(Entity * entity)
{
  QMap <QString, QString>::iterator begin;
  QMap <QString, QString>::iterator end;

  entity->getAttributeIterator(begin, end);

  std::vector <Attribute> attributes;

  while (begin != end)
  {
    QString name = begin.key();
    QString value = begin.value();

    Attribute attr (name.toStdString(), value.toStdString());

    attributes.push_back(attr);
    begin++;
  }

  return attributes;
}

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

#include "ValidatorPlugin.h"


ValidatorPlugin::ValidatorPlugin()
{
  table = new ValidatorTreeWidget();
  table->setColumnCount(3);
  QStringList headers;
  headers << "" << "Elemento" << "Mensagem";
  table->setHeaderLabels(headers);
  table->setColumnWidth(0, 40);

  Langstruct::init();

  connect(table, SIGNAL (itemClicked ( QTreeWidgetItem *, int)),
          this, SLOT (itemSelected (QTreeWidgetItem *)));
  connect(table, SIGNAL (itemDoubleClicked  ( QTreeWidgetItem *, int)),
          this, SLOT (itemDoubleClickedSelected (QTreeWidgetItem *)));

}

void ValidatorPlugin::init()
{ 
  Project * project = getProject();

  QString location = project->getLocation();
  int index = location.lastIndexOf("/");
  location.remove(index + 1, location.size() - (index + 1));

  adapter.setRelativePath(location);

  if (!project) return;

  foreach (Entity *entity, project->getChildren())
    updateModel(entity);


  std::vector <pair<void *, string> > msgs = adapter.validate();
  updateMessages(msgs);

  adapter.getModel().clearMarkedElements();
}

void ValidatorPlugin::updateModel (Entity *entity)
{
  adapter.addElement(entity);

  foreach (Entity *child, entity->getChildren())
  {
    updateModel(child);
  }
}

void ValidatorPlugin::updateMessages(std::vector<pair<void *, string> > msgs)
{
  table->clear();
  pairsMessages.clear();
  emit sendBroadcastMessage("clearValidationMessages", 0);

  set <pair <Entity *, QString> > messages;
  for (int i = 0; i < msgs.size(); i++)
  {
    Entity *entity = (Entity *) msgs[i].first;
    if (entity != NULL)
    {
      qDebug() << entity->getUniqueId();
      messages.insert(make_pair (entity,
                                 QString::fromStdString(msgs [i].second)));
    }

  }

  for (set <pair <Entity *, QString> >::iterator it = messages.begin();
       it != messages.end(); it++)
  {
    pair <Entity *, QString> p = *it;
    pairsMessages.push_back(make_pair (p.first->getUniqueId(), p.second));

    ValidatorTreeWidgetItem *item = new ValidatorTreeWidgetItem (
          table, p.first, p.second);

    item->setIcon(0, QIcon(":/images/error.png"));
    table->addTopLevelItem (item);

    emit sendBroadcastMessage("validationError", &pairsMessages.back());
  }

}

void ValidatorPlugin::onEntityAdded(QString ID, Entity *entity)
{
  if (!entity) return;

  adapter.addElement(entity);

  std::vector <pair<void *, string> > msgs = adapter.validate();
  updateMessages(msgs);

}

void ValidatorPlugin::onEntityChanged(QString ID, Entity * entity)
{
  adapter.changeElement(entity);
  std::vector <pair<void *, string> > msgs = adapter.validate();
  updateMessages(msgs);

}

void ValidatorPlugin::onEntityRemoved(QString ID, QString entityID)
{
  qDebug () << "Validator: Entity Removed: " << ID;
  adapter.removeElement(entityID);

  std::vector <pair<void *, string> > msgs = adapter.validate();
  updateMessages(msgs);
}


void ValidatorPlugin::errorMessage(QString error)
{
  qDebug () << "Validator: Error: " << error;
}


void ValidatorPlugin::clearValidationMessages(QString pluginID, void * param)
{
  //    qDebug() << "clearValidationMessages:";
  //    qDebug() << pluginID;
  //    qDebug() << (param == 0) ;
}


void ValidatorPlugin::validationError(QString pluginID, void * param)
{

}

void ValidatorPlugin::askAllValidationMessages(QString, void *)
{
  for (int i = 0; i < pairsMessages.size(); i++)
    emit sendBroadcastMessage("validationError", &pairsMessages[i]);

}

void ValidatorPlugin::validationErrorSelected(QString, void *param)
{
  if (param)
  {
    pair <QString, QString> * p = (pair <QString, QString>*) param;

    if (p)
    {
      qDebug( p->first.toStdString().c_str() );
      qDebug( p->second.toStdString().c_str() );
    }
  }
}


void ValidatorPlugin::itemSelected(QTreeWidgetItem * item)
{
  if (item)
  {
    ValidatorTreeWidgetItem *validatorTreeItem =
        dynamic_cast <ValidatorTreeWidgetItem *> (item);
    if (validatorTreeItem)
    {
      for (int i = 0; i < pairsMessages.size(); i++)
      {
        pair<QString, QString> p = pairsMessages[i];

        if (p.first == validatorTreeItem->entity()->getUniqueId() &&
            p.second == validatorTreeItem->message())
        {

          emit sendBroadcastMessage("validationErrorSelected", &p);
          break;
        }
      }
    }
  }
}

void ValidatorPlugin::itemDoubleClickedSelected (QTreeWidgetItem *item)
{
  if (item)
  {
    ValidatorTreeWidgetItem *validatorTreeItem =
        dynamic_cast <ValidatorTreeWidgetItem *> (item);
    if (validatorTreeItem)
    {
      for (int i = 0; i < pairsMessages.size(); i++)
      {
        pair <QString, QString> p = pairsMessages[i];

        if (p.first == validatorTreeItem->entity()->getUniqueId())
        {
          emit sendBroadcastMessage("changeSelectedEntity", &p.first);
        }
      }
    }
  }
}


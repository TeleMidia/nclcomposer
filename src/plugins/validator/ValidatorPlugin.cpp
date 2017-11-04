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

ValidatorPlugin::ValidatorPlugin ()
{
  table = new ValidatorTreeWidget ();
  table->setColumnCount (3);
  QStringList headers;
  headers << "" << tr ("Elemento") << tr ("Mensagem");
  table->setHeaderLabels (headers);
  table->setColumnWidth (0, 40);

  Langstruct::init ();

  connect (table, SIGNAL (itemClicked (QTreeWidgetItem *, int)), this,
           SLOT (itemSelected (QTreeWidgetItem *)));
  connect (table, SIGNAL (itemDoubleClicked (QTreeWidgetItem *, int)), this,
           SLOT (itemDoubleClickedSelected (QTreeWidgetItem *)));
}

void
ValidatorPlugin::init ()
{
  Project *project = getProject ();

  QString location = project->getLocation ();
  int index = location.lastIndexOf ("/");
  location.remove (index + 1, location.size () - (index + 1));

  adapter.setRelativePath (location);

  if (!project)
    return;

  foreach (Entity *entity, project->entityChildren ())
    updateModel (entity);

  std::vector<pair<void *, string> > msgs = adapter.validate ();
  updateMessages (msgs);

  adapter.getModel ().clearMarkedElements ();
}

void
ValidatorPlugin::updateModel (Entity *entity)
{
  adapter.addElement (entity);

  foreach (Entity *child, entity->entityChildren ())
  {
    updateModel (child);
  }
}

void
ValidatorPlugin::updateMessages (std::vector<pair<void *, string> > msgs)
{
  table->clear ();
  pairsMessages.clear ();
  emit sendBroadcastMessage ("clearValidationMessages", 0);

  set<pair<Entity *, QString> > messages;
  for (uint i = 0; i < msgs.size (); i++)
  {
    Entity *entity = (Entity *)msgs[i].first;
    if (entity != NULL)
    {
      messages.insert (
          make_pair (entity, QString::fromStdString (msgs[i].second)));
    }
  }

  for (set<pair<Entity *, QString> >::iterator it = messages.begin ();
       it != messages.end (); it++)
  {
    pair<Entity *, QString> p = *it;
    pairsMessages.push_back (make_pair (p.first->uid (), p.second));

    ValidatorTreeWidgetItem *item
        = new ValidatorTreeWidgetItem (table, p.first, p.second);

    item->setIcon (0, QIcon (":/icon/error"));
    table->addTopLevelItem (item);

    emit sendBroadcastMessage ("validationError", &pairsMessages.back ());
  }
}

void
ValidatorPlugin::onEntityAdded (const QString &pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)

  if (!entity)
    return;

  adapter.addElement (entity);

  std::vector<pair<void *, string> > msgs = adapter.validate ();
  updateMessages (msgs);
}

void
ValidatorPlugin::onEntityChanged (const QString &pluginID, Entity *entity)
{
  Q_UNUSED (pluginID)

  adapter.changeElement (entity);
  std::vector<pair<void *, string> > msgs = adapter.validate ();
  updateMessages (msgs);
}

void
ValidatorPlugin::onEntityRemoved (const QString &pluginID,
                                  const QString &entityID)
{
  Q_UNUSED (pluginID)

  adapter.removeElement (entityID);

  std::vector<pair<void *, string> > msgs = adapter.validate ();
  updateMessages (msgs);
}

void
ValidatorPlugin::errorMessage (const QString &error)
{
  qDebug () << "Validator: Error: " << error;
}

void
ValidatorPlugin::clearValidationMessages (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)
  Q_UNUSED (param)
  //    qDebug() << "clearValidationMessages:";
  //    qDebug() << pluginID;
  //    qDebug() << (param == 0) ;
}

void
ValidatorPlugin::validationError (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)
  Q_UNUSED (param)
}

void
ValidatorPlugin::askAllValidationMessages (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)
  Q_UNUSED (param)

  for (int i = 0; i < pairsMessages.size (); i++)
    emit sendBroadcastMessage ("validationError", &pairsMessages[i]);
}

void
ValidatorPlugin::validationErrorSelected (QString pluginID, void *param)
{
  Q_UNUSED (pluginID)

  if (param)
  {
    pair<QString, QString> *p = (pair<QString, QString> *)param;

    if (p)
    {
      qDebug () << p->first;
      qDebug () << p->second;
    }
  }
}

void
ValidatorPlugin::itemSelected (QTreeWidgetItem *item)
{
  if (item)
  {
    ValidatorTreeWidgetItem *validatorTreeItem
        = dynamic_cast<ValidatorTreeWidgetItem *> (item);
    if (validatorTreeItem)
    {
      for (int i = 0; i < pairsMessages.size (); i++)
      {
        pair<QString, QString> p = pairsMessages[i];

        if (p.first == validatorTreeItem->entity ()->uid ()
            && p.second == validatorTreeItem->message ())
        {

          emit sendBroadcastMessage ("validationErrorSelected", &p);
          break;
        }
      }
    }
  }
}

void
ValidatorPlugin::itemDoubleClickedSelected (QTreeWidgetItem *item)
{
  if (item)
  {
    ValidatorTreeWidgetItem *validatorTreeItem
        = dynamic_cast<ValidatorTreeWidgetItem *> (item);
    if (validatorTreeItem)
    {
      for (int i = 0; i < pairsMessages.size (); i++)
      {
        pair<QString, QString> p = pairsMessages[i];

        if (p.first == validatorTreeItem->entity ()->uid ())
        {
          emit sendBroadcastMessage ("changeSelectedEntity", &p.first);
        }
      }
    }
  }
}

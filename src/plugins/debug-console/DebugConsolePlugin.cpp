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
#include "DebugConsolePlugin.h"

DebugConsolePlugin::DebugConsolePlugin ()
{
  _window = new QWidget ();
  QGridLayout *layout = new QGridLayout (_window);
  QPushButton *bt = new QPushButton (_window);
  bt->setText (tr ("Clear"));
  layout->addWidget (bt);
  _list = new QListWidget (_window);
  _list->setAlternatingRowColors (true);
  layout->addWidget (_list);
  connect (bt, SIGNAL (clicked ()), _list, SLOT (clear ()));
  connect (bt, SIGNAL (clicked ()), this, SLOT (sendToAll ()));
  _window->setLayout (layout);
  _window->setWindowIcon (QIcon (":/images/icon.png"));
}

DebugConsolePlugin::~DebugConsolePlugin ()
{
  if (_window != nullptr)
    delete _window;
  _window = nullptr;
}

void
DebugConsolePlugin::init ()
{
  // TODO: All
}

QWidget *
DebugConsolePlugin::widget ()
{
  return _window;
}

void
DebugConsolePlugin::onEntityAdded (const QString &pluginID, Entity *entity)
{
  QString line = "PLUGIN (" + pluginID + ") added the Entity ("
                 + entity->type () + " - " + entity->uid () + ")";
  _list->insertItem (0, new QListWidgetItem (line));
}

void
DebugConsolePlugin::errorMessage (const QString &error)
{
  //    list->insertItem(0, new QListWidgetItem(error));
  if (_list->count ())
    _list->item (0)->setText (error);
  else
    _list->addItem (new QListWidgetItem (error));
}

void
DebugConsolePlugin::onEntityChanged (const QString &pluginID, Entity *entity)
{
  QString line = "PLUGIN (" + pluginID + ") changed the Entity ("
                 + entity->type () + " - " + entity->uid () + ")";
  _list->insertItem (0, new QListWidgetItem (line));
  /*  if(list->count())
    list->item(0)->setText(line);
  else
    list->addItem(new QListWidgetItem(line)); */
}

/*void DebugConsolePlugin::onEntityAboutToRemove(Entity *)
{

}*/

void
DebugConsolePlugin::onEntityRemoved (const QString &pluginID,
                                     const QString &entityID)
{
  QString line = "PLUGIN (" + pluginID + ") removed Entity (" + entityID + ")";
  _list->insertItem (0, new QListWidgetItem (line));
  /*if(list->count())
    list->item(0)->setText(line);
  else
    list->addItem(new QListWidgetItem(line));*/
}

bool
DebugConsolePlugin::saveSubsession ()
{
  // TODO: All
  return true;
}

void
DebugConsolePlugin::sendToAll ()
{
  /* Invoker <Result> in;
    in.addArgument<int>(10);
    in.addArgument<int>(20);
    in.addArgument<int>(30);

    in.sendBroadcastMessage("debugHasSendClearAll"); */

  emit sendBroadcastMessage ("debugHasSendClearAll", nullptr);
}

/*
 * Copyright 2011 TeleMidia/PUC-Rio.
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

namespace composer {
namespace plugin {
namespace debug {

DebugConsolePlugin::DebugConsolePlugin()
{
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

}

DebugConsolePlugin::~DebugConsolePlugin()
{
  if(window != NULL)
    delete window;
  window = NULL;
}

void DebugConsolePlugin::init()
{
  //TODO: All
}

QWidget* DebugConsolePlugin::getWidget()
{
  return window;
}

void DebugConsolePlugin::onEntityAdded(QString ID, Entity *entity)
{
  QString line = "PLUGIN (" + ID + ") added the Entity (" +
      entity->getType() + " - " + entity->getUniqueId() +")";
  //    list->insertItem(0, new QListWidgetItem(line));
  if(list->count())
    list->item(0)->setText(line);
  else
    list->addItem(new QListWidgetItem(line));
}

void DebugConsolePlugin::errorMessage(QString error)
{
  //    list->insertItem(0, new QListWidgetItem(error));
  if(list->count())
    list->item(0)->setText(error);
  else
    list->addItem(new QListWidgetItem(error));
}

void DebugConsolePlugin::onEntityChanged(QString ID, Entity * entity)
{
  QString line = "PLUGIN (" + ID + ") changed the Entity (" +
      entity->getType() + " - " + entity->getUniqueId() +")";
//  list->insertItem(0, new QListWidgetItem(line));
  if(list->count())
    list->item(0)->setText(line);
  else
    list->addItem(new QListWidgetItem(line));
}

/*void DebugConsolePlugin::onEntityAboutToRemove(Entity *)
{

}*/

void DebugConsolePlugin::onEntityRemoved(QString ID, QString entityID)
{
  QString line = "PLUGIN (" + ID + ") removed Entity (" +
      entityID + ")";
//  list->insertItem(0, new QListWidgetItem(line));
  if(list->count())
    list->item(0)->setText(line);
  else
    list->addItem(new QListWidgetItem(line));
}

bool DebugConsolePlugin::saveSubsession()
{
  //TODO: All
  return true;
}

void DebugConsolePlugin::sendToAll()
{
  /* Invoker <Result> in;
    in.addArgument<int>(10);
    in.addArgument<int>(20);
    in.addArgument<int>(30);

    in.sendBroadcastMessage("debugHasSendClearAll"); */

  emit sendBroadcastMessage("debugHasSendClearAll", NULL);
}

} } } //end namespace

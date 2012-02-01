/*
 * Copyright 2011-2012 TeleMidia/PUC-Rio.
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
#include "NCLTreeWidget.h"
#include "NCLStructure.h"

using namespace composer::language;

#include <QInputDialog>
#include <QDialog>

NCLTreeWidget::NCLTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
  setContextMenuPolicy(Qt::ActionsContextMenu);

  createActions();
  createMenus();

  QStringList labels;
  labels  << QObject::tr("Element") << QObject::tr("Attributes")
          << QObject::tr("Element Id") << QObject::tr("Tagname");
  setHeaderLabels(labels);

  setHeaderHidden(true);
  setColumnHidden(1, true);
  setColumnHidden(2, true);
  setColumnHidden(3, true);

  isExpandedAll = true;

  /* \todo This defaultFontSize must be configurable. */
  defaultFont = QFont();
}

NCLTreeWidget::~NCLTreeWidget()
{

}

void NCLTreeWidget::createActions ()
{
  insertNodeAct = new QAction( QIcon(":/images/save.png"),
                               tr("&Add child"),
                               this);

  connect( insertNodeAct, SIGNAL(triggered()),
           this, SLOT(userAddNewElement()));
  addAction(insertNodeAct);

  removeNodeAct = new QAction( QIcon(":/images/delete.png"),
                               tr("&Remove Selected element"),
                               this);

  connect( removeNodeAct, SIGNAL(triggered()),
           this, SLOT(userRemoveElement()));
  removeNodeAct->setShortcutContext(Qt::WidgetShortcut);
  removeNodeAct->setShortcut(QKeySequence::Delete);
  addAction(removeNodeAct);

  expandAllAct = new QAction(tr("Expand All"), this);
  expandAllAct->setCheckable(true);
  expandAllAct->setChecked(true);
  connect(expandAllAct, SIGNAL(triggered()), this, SLOT(expandAll()));
  addAction(expandAllAct);

}

void NCLTreeWidget::createMenus()
{
  elementMenu = new QMenu(this);
  elementMenu->addAction(insertNodeAct);
  elementMenu->addAction(removeNodeAct);
  elementMenu->addSeparator();
  elementMenu->addAction(expandAllAct);
}

bool NCLTreeWidget::updateFromText(QString text)
{
  // qDebug() << "MainWindow::updateTreeWidget()";
  QStringList labels;
  labels << QObject::tr("Element") << QObject::tr("Attributes")
         << QObject::tr("id");

  QXmlInputSource inputSource;
  inputSource.setData( text );
  QXmlSimpleReader reader;

  this->setStyleSheet("/*background-color: #ffffff;*/ font-size: 11px;");
  clear();
  setHeaderLabels(labels);
  // header()->setStretchLastSection(false);

  // FIXME: Uncommenting this option brings some performance problems.
  // header()->setResizeMode(QHeaderView::ResizeToContents);

  //TODO: Transform this parser in a puglin
  NCLParser *handler = new NCLParser(this);
  connect (  handler,
             SIGNAL(fatalErrorFound(QString, QString, int, int, int)),
             this,
             SLOT(errorNotification(QString, QString, int, int, int)));

  reader.setContentHandler(handler);
  reader.setErrorHandler(handler);
  bool ret = reader.parse(inputSource);
  delete handler;
  this->expandAll();

  return ret;
}

QTreeWidgetItem* NCLTreeWidget::addElement ( QTreeWidgetItem *father,
                                             int pos,
                                             QString tagname,
                                             QString id,
                                             QMap <QString, QString> &attrs,
                                             int line_in_text,
                                             int column_in_text)
{

  QTreeWidgetItem *child;

  if(father != 0)
  {
    child = new QTreeWidgetItem(0);
    int p = pos;
    if(pos == -1)
      p = father->childCount();

    father->insertChild(p, child);
  }
  else
  {
    child = new QTreeWidgetItem(this);

    if(pos != -1)
      this->insertTopLevelItem(pos, child);
  }

  updateItem(child, tagname, attrs);
  child->setText(2, id);
  child->setText(3, tagname);
  // child->setText(2, QString::number(line_in_text));
  // child->setText(3, QString::number(column_in_text));

  repaint();

  return child;
}

void NCLTreeWidget::userAddNewElement()
{
  bool ok;
  QList<QTreeWidgetItem*> selecteds = this->selectedItems();

  QTreeWidgetItem *item ;
  QString parentId = "", tagname = "";
  QStringList strlist;

  if(selecteds.size()) {
    item = selecteds.at(0);
    parentId = item->text(2);
    tagname = item->text(3);

    map <QString, char> *
        children = NCLStructure::getInstance()->getChildren(tagname);

    if(children != NULL)
    {
      map <QString, char>::iterator it;
      for(it = children->begin(); it != children->end(); ++it)
      {
        strlist << it->first;
      }
    }
  }

  QString element = QInputDialog::getItem( this,
                                           tr("Add child"),
                                           tr("Element name:"),
                                           strlist,
                                           0,
                                           true,
                                           &ok );
  if(ok)
  {
    //Add new Element to OutlineWidget
    QMap<QString,QString> attr;
    emit elementAddedByUser (element, parentId, attr, false);
  }
}

QTreeWidgetItem *NCLTreeWidget::getItemById(QString itemId)
{
  QList <QTreeWidgetItem*> items = findItems( itemId,
                                              Qt::MatchExactly |
                                              Qt::MatchRecursive, 2);

  if(items.size() > 1)
  {
    qDebug() << "NCLTreeWidget::getItemById Warning - You have more than "
             << "one item with id='" << itemId
             << "' - All them will be deleted!";
  }

  for (int i = 0; i < items.size(); i++)
  {
    if(items.at(i)->text(2) == itemId){
      return items.at(i);
    }
  }
  return NULL;
}

void NCLTreeWidget::removeItem(QString itemId)
{
  QRegExp exp("*");
  QList <QTreeWidgetItem*> items = this->findItems(itemId, Qt::MatchExactly
                                                   | Qt::MatchRecursive, 2);
  QTreeWidgetItem *item;

  if(items.size() > 1)
  {
    qDebug() << "NCLTreeWidget::removeItem Warning - You have more than "
             << "one item with id='"<< itemId
             << "' - All them will be deleted!";
  }
  else if(items.size() == 0)
  {
    qDebug() << "NCLTreeWidget::removeItem Warning! Item with id ='"
             << itemId << "' was not found!";
  }

  for (int i = 0; i < items.size(); i++)
  {
    item  = items.at(i);
    if (item->parent() != NULL)
    {
      item->parent()->removeChild(item);
      qDeleteAll(item->takeChildren());
    }
    else
    {
      //            TODO:
      //            this->clear();
      int index = indexOfTopLevelItem(item);
      QTreeWidgetItem *item = takeTopLevelItem(index);
      qDebug() << "index=" << index << "item=" << item;
      //              delete item;
    }
  }
  repaint();
}

void NCLTreeWidget::userRemoveElement()
{
  QList<QTreeWidgetItem*> selecteds = this->selectedItems();
  QTreeWidgetItem *item = selecteds.at (0);

  if(item == NULL)
    return;

  QString id = item->text(2);

  if (item != NULL) {
    int resp = QMessageBox::question(
          this,
          tr("Deleting Element"),
          tr("Do you really want delete the %1 element ?").arg(id),
          QMessageBox::Yes,
          QMessageBox::No );

    if(resp == QMessageBox::Yes) {
      emit elementRemovedByUser(id);
    }
  }
  repaint();
}

void NCLTreeWidget::updateItem(QTreeWidgetItem *item, QString tagname,
                               QMap <QString, QString> &attrs)
{
  QIcon icon;
  /*!
      \todo Create a method to return an Icon to a given element.
     */
  if(tagname == "media")
  {
    if(attrs.contains("type"))
    {
      QString type = attrs.value("type");
      if(type.startsWith("audio"))
        icon = QIcon(":/icon/audio");
      else if(type.startsWith("image"))
        icon = QIcon(":/icon/image");
      else if(type.startsWith("video"))
        icon = QIcon(":/icon/video");
      else if(type.startsWith("text/html"))
        icon = QIcon(":/icon/html");
      else if(type.startsWith("text"))
        icon = QIcon(":/icon/text");
      else if(type.startsWith("application/x-ginga-NCLua"))
        icon = QIcon(":/icon/script");
      else if(type.startsWith("application/x-ncl-settings") ||
              type.startsWith("application/x-ncl-settings"))
        icon = QIcon(":/icon/settings");
      else icon = QIcon (":/icon/media");
    }
    else
      icon = QIcon (":/icon/media");
  }
  else if(tagname == "context" || tagname == "body")
    icon = QIcon (":/icon/context");
  else if(tagname == "meta" || tagname == "metadata")
    icon = QIcon (":/icon/metadata");
  else if(tagname == "switch")
    icon = QIcon (":/icon/switch");
  else if(tagname == "descriptor")
    icon = QIcon (":/icon/descriptor");
  else if(tagname == "link")
    icon = QIcon (":/icon/link");
  else if(tagname == "port")
    icon = QIcon (":/icon/port");
  else if(tagname == "property")
    icon = QIcon (":/icon/property");
  else
    icon = QIcon (":/icon/element");

  QString strAttrList = "";
  QString key;
  QString name;
  foreach (key, attrs.keys())
  {
    strAttrList += " ";
    strAttrList += key + "=\"" + attrs[key] + "\"";
    if(key == "id" || key == "name") {
      name = attrs[key];
    }
  }

  item->setIcon(0, icon);
  item->setText(3, tagname);

  if(name != "")
  {
    item->setText(0, tagname + " (" + name + ")");
  }
  else
    item->setText(0, tagname);

  //item->setText(2, id);
  //item->setText(1, name);

  if(isExpandedAll)
    QTreeWidget::expandAll();

  repaint();
}

void NCLTreeWidget::errorNotification( QString message,
                                       QString filename,
                                       int line,
                                       int column,
                                       int severity)
{
  //this->setStyleSheet("background-color: #aa0000; font-size: 11px;");
  emit parserErrorNotify(message, filename, line, column, severity);
}

void NCLTreeWidget::decreaseFont()
{
  unsigned int newPointSize = font().pointSize()-1;
  QFont newFont(font());
  newFont.setPointSize(newPointSize);
  setFont(newFont);
}

void NCLTreeWidget::increaseFont()
{
  unsigned int newPointSize = font().pointSize()+1;
  QFont newFont(font());
  newFont.setPointSize(newPointSize);
  setFont(newFont);
}

void NCLTreeWidget::resetFont()
{
  setFont(defaultFont);
}

void NCLTreeWidget::setDefaultFont(const QFont &defaultFont)
{
  this->defaultFont = defaultFont;
}

void NCLTreeWidget::wheelEvent(QWheelEvent *event)
{
  if(event->modifiers() == Qt::ControlModifier){
    if(event->delta() > 0)
      zoomIn();
    else
      zoomOut();
    event->accept();
  }
  else {
    QTreeWidget::wheelEvent(event);
  }
}

void NCLTreeWidget::keyPressEvent(QKeyEvent *event)
{
  if(event->modifiers() & Qt::ControlModifier)
  {
    if(event->key() == Qt::Key_Plus) //checks for zoomin event.
    {
      zoomIn();
      event->accept();
    }
    else if(event->key() == Qt::Key_Minus) //checks for zoom out event.
    {
      zoomOut();
      event->accept();
    }
    else if(event->key() == Qt::Key_0) //checks reset zoom event.
    {
      resetZoom();
      event->accept();
    }
  }

  QTreeWidget::keyPressEvent(event);
}

void NCLTreeWidget::expandAll()
{
  if(!isExpandedAll)
  {
    isExpandedAll = true;
    QTreeWidget::expandAll();
  }
  else isExpandedAll = false;
}

void NCLTreeWidget::zoomIn()
{
  increaseFont();
  //IncreaseIconSize();
}

void NCLTreeWidget::zoomOut()
{
  decreaseFont();
  //DecreaseIconSize();
}

void NCLTreeWidget::resetZoom()
{
  resetFont();
}

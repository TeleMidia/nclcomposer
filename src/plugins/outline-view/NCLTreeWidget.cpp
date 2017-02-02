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

#include <QVariant>

#include <util/ComposerSettings.h>
using namespace composer::core::util;

NCLTreeWidget::NCLTreeWidget(QWidget *parent) : QTreeWidget(parent)
{
  setContextMenuPolicy(Qt::ActionsContextMenu);

  createActions();
  createMenus();

  QStringList labels;
  labels  << QObject::tr("Element") << QObject::tr("Attributes")
          << QObject::tr("Element UId") << QObject::tr("Element Id")
          << QObject::tr("Tagname");
  setHeaderLabels(labels);

  setHeaderHidden(true);
  setColumnHidden(1, true);
  setColumnHidden(2, true);
  setColumnHidden(3, true);
  setColumnHidden(4, true);

  _isExpandedAll = true;

  /* \todo This defaultFontSize must be configurable. */
  _defaultFont = QFont();

  setDragEnabled(true);

  // Entities Icon
  _entitiesIcons["media"] = ":/icon/media";
  _entitiesIcons["body"] = ":/icon/body";
  _entitiesIcons["transition"] = ":/icon/transition";
  _entitiesIcons["transitionBase"] = ":/icon/transitionbase";
  _entitiesIcons["rule"] = ":/icon/rule";
  _entitiesIcons["ruleBase"] = ":/icon/rulebase";
  _entitiesIcons["context"] = ":/icon/context";
  _entitiesIcons["switch"] = ":/icon/switch";
  _entitiesIcons["descriptor"] = ":/icon/descriptor";
  _entitiesIcons["descriptorParam"] = ":/icon/descriptorparam";
  _entitiesIcons["descriptorBase"] = ":/icon/descriptorbase";
  _entitiesIcons["connector"] = ":/icon/connector";
  _entitiesIcons["connectorParam"] = ":/icon/connectorparam";
  _entitiesIcons["connectorBase"] = ":/icon/connectorbase";
  _entitiesIcons["importBase"] = ":/icon/importbase";
  _entitiesIcons["region"] = ":/icon/region";
  _entitiesIcons["regionBase"] = ":/icon/regionbase";
  _entitiesIcons["link"] = ":/icon/link";
  _entitiesIcons["linkParam"] = ":/icon/linkparam";
  _entitiesIcons["port"] = ":/icon/port";
  _entitiesIcons["area"] = ":/icon/area";
  _entitiesIcons["switchPort"] = ":/icon/switchport";
  _entitiesIcons["bind"] = ":/icon/bind";
  _entitiesIcons["bindparam"] = ":/icon/bindparam";

  // Media Icons
  _mediaIcons["text/html"] = ":/icon/media-text-html";
  _mediaIcons["text/css"] = ":/icon/media-text";
  _mediaIcons["text/xml"] = ":/icon/media-text";
  _mediaIcons["text/plain"] = ":/icon/media-text";
  _mediaIcons["image/bmp"] = ":/icon/media-image";
  _mediaIcons["image/png"] = ":/icon/media-image";
  _mediaIcons["image/gif"] = ":/icon/media-image";
  _mediaIcons["image/jpeg"] = ":/icon/media-image";
  _mediaIcons["audio/basic"] = ":/icon/media-audio";
  _mediaIcons["audio/x-wav"] = ":/icon/media-audio";
  _mediaIcons["audio/mpeg"] = ":/icon/media-audio";
  _mediaIcons["audio/mp3"] = ":/icon/media-audio";
  _mediaIcons["audio/mp2"] = ":/icon/media-audio";
  _mediaIcons["video/mpeg"] = ":/icon/media-video";
  _mediaIcons["video/mp4"] = ":/icon/media-video";
  _mediaIcons["video/x-mng"] = ":/icon/media-video";
  _mediaIcons["video/quicktime"] = ":/icon/media-video";
  _mediaIcons["video/x-msvideo"] = ":/icon/media-video";
  _mediaIcons["application/x-ginga-NCL"] = ":/icon/media-ncl";
  _mediaIcons["application/x-ncl-NCL"] = ":/icon/media-ncl";
  _mediaIcons["application/x-ginga-NCLua"] = ":/icon/media-nclua";
  _mediaIcons["application/x-ncl-NCLua"] = ":/icon/media-nclua";
  _mediaIcons["application/x-ginga-settings"] = ":/icon/media-settings";
  _mediaIcons["application/x-ncl-settings"] = ":/icon/media-settings";
  _mediaIcons["application/x-ginga-time"] = ":/icon/media-time";
  _mediaIcons["application/x-ncl-time"] = ":/icon/media-time";
  _mediaIcons["application/x-ginga-NCLet"] = ":/icon/media-nclet";

  // Extension Icons
  _extIcons["txt"] = ":/icon/media-text";
  _extIcons["htm"] = ":/icon/media-text-html";
  _extIcons["html"] = ":/icon/media-text-html";
  _extIcons["gif"] = ":/icon/media-image";
  _extIcons["png"] = ":/icon/media-image";
  _extIcons["jpg"] = ":/icon/media-image";
  _extIcons["jpeg"] = ":/icon/media-image";
  _extIcons["gif"] = ":/icon/media-image";
  _extIcons["mp3"] = ":/icon/media-audio";
  _extIcons["wav"] = ":/icon/media-audio";
  _extIcons["ac3"] = ":/icon/media-audio";
  _extIcons["mpa"] = ":/icon/media-audio";
  _extIcons["mp2"] = ":/icon/media-audio";
  _extIcons["mp4"] = ":/icon/media-video";
  _extIcons["avi"] = ":/icon/media-video";
  _extIcons["mpeg4"] = ":/icon/media-video";
  _extIcons["mpeg"] = ":/icon/media-video";
  _extIcons["mpg"] = ":/icon/media-video";
  _extIcons["mov"] = ":/icon/media-video";
  _extIcons["ts"] = ":/icon/media-video";
  _extIcons["ncl"] = ":/icon/media-ncl";
  _extIcons["lua"] = ":/icon/media-nclua";

  // Bind Icons
  _bindIcons["onBegin"] = ":/icon/bind-onbegin";
  _bindIcons["onEnd"] = ":/icon/bind-onend";
  _bindIcons["onPause"] = ":/icon/bind-onpause";
  _bindIcons["onResume"] = ":/icon/bind-onresume";
  _bindIcons["onSelection"] = ":/icon/bind-onselection";
  _bindIcons["pause"] = ":/icon/bind-pause";
  _bindIcons["resume"] = ":/icon/bind-resume";
  _bindIcons["set"] = ":/icon/bind-set";
  _bindIcons["start"] = ":/icon/bind-start";
  _bindIcons["stop"] = ":/icon/bind-stop";

}

NCLTreeWidget::~NCLTreeWidget()
{

}

void NCLTreeWidget::createActions ()
{
  _insertNodeAct = new QAction( QIcon(":/icon/plus"),
                               tr("&Add child"),
                               this);
  connect( _insertNodeAct, SIGNAL(triggered()),
           this, SLOT(userAddNewElement()));
  addAction(_insertNodeAct);

  _removeNodeAct = new QAction( QIcon(":/icon/minus"),
                                tr("&Remove selected element"),
                                this);
  _removeNodeAct->setShortcutContext(Qt::WidgetShortcut);
  _removeNodeAct->setShortcut(QKeySequence::Delete);

  addAction(_removeNodeAct);
  connect( _removeNodeAct, SIGNAL(triggered()),
           this, SLOT(userRemoveElement()) );

  _expandAllAct = new QAction(tr("&Expand All"), this);
  _expandAllAct->setCheckable(true);
  _expandAllAct->setChecked(true);
  connect(_expandAllAct, SIGNAL(triggered()), this, SLOT(expandAll()));
  addAction(_expandAllAct);

}

void NCLTreeWidget::createMenus()
{
  _elementMenu = new QMenu(this);
  _elementMenu->addAction(_insertNodeAct);
  _elementMenu->addAction(_removeNodeAct);
  _elementMenu->addSeparator();
  _elementMenu->addAction(_expandAllAct);
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
                                             int /* line_in_text */,
                                             int /* column_in_text*/)
{

  QTreeWidgetItem *child;

  if(father != 0)
  {
    child = new QTreeWidgetItem(0);

    // Need to find where to insert the child
    int pos_found = false;
    int p = pos;

#ifdef KEEP_ELEMENTS_ORDER
    deque <QString> *elements_ordered =
        NCLStructure::getInstance()->getElementsOrdered();
    if(father->childCount())
    {
      // Find where to insert the element.
      int i = 0;
      p = 0;
      while (!pos_found)
      {
        while ((elements_ordered->at(i) != tagname)
               &&
               (elements_ordered->at(i) != father->child(p)->text(4)))
        {
          i++;
        }

        if(elements_ordered->at(i) == tagname)
          pos_found = true;


        while( (p < father->childCount())
               &&
               (elements_ordered->at(i) == father->child(p)->text(4)) )
        {
          p++;
          qDebug() << p << i << father->childCount();
        }

        if( p == father->childCount() )
          break;
      }
    }

    if(!pos_found)
      p = father->childCount();

#else
    if (p == -1)
      p = father->childCount();
#endif

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
  // child->setText(3, name) // this is done inside update item
  child->setText(4, tagname);
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

  if(selecteds.size())
  {
    item = selecteds.at(0);
    parentId = item->text(2);
    tagname = item->text(4);

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
  else
    strlist << "ncl"; //\fixme this should be loaded from element that has not parents.

  QString element = QInputDialog::getItem( this,
                                           tr("&Add child"),
                                           tr("Element name:"),
                                           strlist,
                                           0,
                                           true,
                                           &ok );
  if(ok)
  {
    //Add new Element to OutlineWidget
    QMap<QString,QString> attr;
    if(element == "ncl")
      attr.insert("id", "myNCLID");

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
    if(items.at(i)->text(2) == itemId)
      return items.at(i);
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
  QString name = item->text(0);

  if (item != NULL) {
    int resp = QMessageBox::question(
          this,
          tr("Deleting Element"),
          tr("Are you sure you want to delete the \"%1\" element ?").arg(name),
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
  QString icon = ":/icon/tag";
  /*!
   *  \todo Create a method to return an Icon to a given element.
   */

  if (_entitiesIcons.contains(tagname))
  {
    icon = _entitiesIcons.value(tagname);

    if (tagname == "media")
    {
      QString src = attrs.value("src");
      QString ext = src.mid(src.lastIndexOf(".") + 1);

      if (_extIcons.contains(ext))
        icon = _extIcons.value(ext);

      QString type = attrs.value("type");

      if (_mediaIcons.contains(type))
        icon = _mediaIcons.value(type);
    }
//    else if (tagname == "bind")
//    {
//      QString role = attrs.value("role");

//      if (_bindIcons.contains(role))
//        icon = _bindIcons.value(role);
//    }
  }

  QString strAttrList = "";
  QString key;
  QString name;
  foreach (key, attrs.keys())
  {
    strAttrList += " ";
    strAttrList += key + "=\"" + attrs[key] + "\"";
    if(key == "id" || key == "name")
    {
      name = attrs[key];
    }
  }

  item->setIcon(0, QIcon(icon));
  item->setText(4, tagname);

  if(name != "")
  {
    item->setText(0, tagname + " (" + name + ")");
    item->setText(3, name);
  }
  else
    item->setText(0, tagname);

  //item->setText(2, id);
  //item->setText(1, name);

  if(_isExpandedAll)
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
  setFont(_defaultFont);
}

void NCLTreeWidget::setDefaultFont(const QFont &defaultFont)
{
  this->_defaultFont = defaultFont;
}

void NCLTreeWidget::wheelEvent(QWheelEvent *event)
{
  if(event->modifiers() == Qt::ControlModifier)
  {
    if(event->delta() > 0)
      zoomIn();
    else
      zoomOut();
    event->accept();
  }
  else
  {
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
  if(!_isExpandedAll)
  {
    _isExpandedAll = true;
    QTreeWidget::expandAll();
  }
  else _isExpandedAll = false;
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

void NCLTreeWidget::mouseMoveEvent(QMouseEvent *event)
{
  QTreeWidgetItem *selectedItem = currentItem();

  // if not left button - return
  if (!(event->buttons() & Qt::LeftButton)) return;

  // If the selected Item exists
  if (selectedItem)
  {
    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;
    mimeData->setColorData(QColor(Qt::green));
    mimeData->setData("nclcomposer/mediaid", currentItem()->text(3).toLatin1());
    mimeData->setData("nclcomposer/qnstuid", currentItem()->text(2).toLatin1());

    drag->setMimeData(mimeData);
    // start drag
    drag->start(Qt::CopyAction | Qt::MoveAction);
  }
}

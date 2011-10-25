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
/*
     treeitem.cpp

     A container for items of data supplied by the simple tree model.
 */

 #include <QStringList>

 #include "TreeItem.h"

 TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent)
 {
     parentItem = parent;
     itemData = data;
 }

 TreeItem::~TreeItem()
 {
     qDeleteAll(childItems);
 }

 void TreeItem::appendChild(TreeItem *item)
 {
     childItems.append(item);
 }

 TreeItem *TreeItem::child(int row)
 {
     return childItems.value(row);
 }

 int TreeItem::childCount() const
 {
     return childItems.count();
 }

 int TreeItem::columnCount() const
 {
     return itemData.count();
 }

 QVariant TreeItem::data(int column) const
 {
     return itemData.value(column);
 }

 TreeItem *TreeItem::parent()
 {
     return parentItem;
 }

 int TreeItem::row() const
 {
     if (parentItem)
         return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

     return 0;
 }

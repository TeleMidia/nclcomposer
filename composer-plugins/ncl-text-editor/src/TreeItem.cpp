/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
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

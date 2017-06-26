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
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

class TreeItem
{
public:
  TreeItem (const QList<QVariant> &data, TreeItem *parent = 0);
  ~TreeItem ();

  void appendChild (TreeItem *child);

  TreeItem *child (int row);
  int childCount () const;
  int columnCount () const;
  QVariant data (int column) const;
  int row () const;
  TreeItem *parent ();

private:
  QList<TreeItem *> childItems;
  QList<QVariant> itemData;
  TreeItem *parentItem;
};

#endif

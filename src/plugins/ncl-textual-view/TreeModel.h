/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  TreeModel (const QString &data, QObject *parent = 0);
  ~TreeModel ();

  QVariant data (const QModelIndex &index, int role) const;
  Qt::ItemFlags flags (const QModelIndex &index) const;
  QVariant headerData (int section, Qt::Orientation orientation,
                       int role = Qt::DisplayRole) const;
  QModelIndex index (int row, int column,
                     const QModelIndex &parent = QModelIndex ()) const;
  QModelIndex parent (const QModelIndex &index) const;
  int rowCount (const QModelIndex &parent = QModelIndex ()) const;
  int columnCount (const QModelIndex &parent = QModelIndex ()) const;

private:
  void setupModelData (const QStringList &lines, TreeItem *parent);

  TreeItem *rootItem;
};

#endif

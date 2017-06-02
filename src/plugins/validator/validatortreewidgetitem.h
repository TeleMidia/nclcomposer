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

#ifndef VALIDATORTREEWIDGETITEM_H
#define VALIDATORTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include <model/Entity.h>

using namespace cpr::core;

class ValidatorTreeWidgetItem : public QTreeWidgetItem
{
public:
  explicit ValidatorTreeWidgetItem(QTreeWidget *view, Entity * entity,
                                   QString message);
  Entity * entity () const { return _entity; }
  QString message () const { return _message; }

signals:


private:
  Entity * _entity;
  QString _message;

};

#endif // VALIDATORTREEWIDGETITEM_H

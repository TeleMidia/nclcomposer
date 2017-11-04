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

#include "ValidatorTreewidgetItem.h"

ValidatorTreeWidgetItem::ValidatorTreeWidgetItem (QTreeWidget *view,
                                                  Entity *entity,
                                                  QString message)
    : QTreeWidgetItem (view)
{
  if (entity)
  {
    _entity = entity;
    _message = message;

    setText (1, entity->type ());
    setText (2, message);

    setToolTip (1, entity->type ());
    setToolTip (2, message);
  }
}

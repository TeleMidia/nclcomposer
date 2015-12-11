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
#ifndef PROPERTYBUTTONS_H
#define PROPERTYBUTTONS_H

#include "LineEditWithButton.h"

class PropertyButtons : public LineEditWithButton
{
  Q_OBJECT

public:
  explicit PropertyButtons(QString propName, QWidget *parent = 0);

  QString key, value;

public slots:
  void openfile();

private:
  QWidget *window;
};

#endif // PROPERTYBUTTONS_H

/*
 * Copyright 2011-2018 TeleMidia/PUC-Rio.
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
#include "TextView.h"

#include<QFile>
#include<QTextStream>
#include<QIODevice>

TextView::TextView(const QString &filename)
{
  QFile inputFile(filename);
  QTextStream in(&inputFile);
  inputFile.open(QIODevice::ReadOnly);
  setText(in.readAll());
  setReadOnly(true);
  setStyleSheet("QTextEdit { background-color : white; color : black; }");
  setAlignment(Qt::AlignCenter);
}

TextView::~TextView()
{

}

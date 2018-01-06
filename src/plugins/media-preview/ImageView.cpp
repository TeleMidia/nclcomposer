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
#include "ImageView.h"

#include<QIcon>
#include<QWidget>
#include <QLabel>

ImageView::ImageView(const QString &filename)
{
  setPixmap(QPixmap(filename));
  setVisible(true);
  setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);
  setAlignment(Qt::AlignCenter);
}

ImageView::~ImageView()
{

}

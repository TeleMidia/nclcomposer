/*
 * Copyright 2011-2013 TeleMidia/PUC-Rio.
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
#ifndef IMAGVIEW_H
#define IMAGVIEW_H
#include<QLabel>
#include<QString>

/*!
 \brief Handles the creation of image objects.
*/

class imagview :public QLabel
{
 Q_OBJECT

public:

    /*!
     \brief Constructor.
     \param Filename is the variable for specifying the media object location.
    */

    imagview(QString filename);
    /*!
      \brief Destructor.
     */
  ~imagview();
};

#endif // IMAGVIEW_H

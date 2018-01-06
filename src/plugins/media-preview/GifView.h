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
#ifndef GIFVIEW_H
#define GIFVIEW_H
#include<QLabel>
#include<QString>
#include<QMovie>
#include<QtTest>

/*!
 * \brief Handles the creation of image objects, but gif type.
 */
class GifView : public QLabel
{
  Q_OBJECT
public: 
  /*!
   * \brief Constructor.
   * \param Filename is the variable for specifying the media object location.
   */
  explicit GifView(const QString &filename);

  /*!
   * \brief Destructor.
   */
  ~GifView();

private:
  QMovie *movie;
};

#endif // GIFVIEW_H

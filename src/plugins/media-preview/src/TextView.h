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
#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include<QString>
#include<QTextEdit>
#include<QFile>

/*!
 * \brief Handles the creation of text objects.
 */
class TextView : public QTextEdit
{
public:
  /*!
   * \brief Constructor.
   * \param Filename is the variable for specifying the media object location.
   */
  explicit TextView(const QString &filename);
  /*!
   * \brief Destructor.
   */
  ~TextView();

private:
  QFile *inputFile;
};

#endif // TEXTVIEW_H

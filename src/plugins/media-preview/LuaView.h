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
#ifndef LUAVIEW_H
#define LUAVIEW_H

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerlua.h>

/*!
 \brief Handles the creation of NCL objects.
*/
class LuaView : public QsciScintilla
{
  Q_OBJECT

public slots:
  /*!
   * \brief saveFile
   * \param fileName
   */
  void saveFile();

public:
  /*!
   * \brief Constructor.
   * \param Filename is the variable for specifying the media object location.
   */
  explicit LuaView(const QString &src = "");

  /*!
   * \brief Destructor.
   */
  ~LuaView();

  /*!
   * \brief loadFile
   * \param fileName
   */
  void loadFile();
private:
  QsciLexerLua *lexerLua;
  QString fileName;

};
#endif // LUAVIEW_H

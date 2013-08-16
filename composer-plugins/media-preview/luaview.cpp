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
#include "luaview.h"

#include <QFile>
#include <QTextStream>
#include <QApplication>

luaview::luaview(const QString &filename)
{
  lexerLua = new QsciLexerLua(this);

  setFolding(QsciScintilla::CircledTreeFoldStyle);
  // setFoldMarginColors(PREF_FOLD_MARGIN_FORE_COLOR, PREF_FOLD_MARGIN_BACK_COLOR);

  setMarginLineNumbers(1, true);
  setMarginWidth(1, 25);
  // setMarginsBackgroundColor(MARGINS_BACKGROUND_COLOR);

  setCaretWidth(20);
  setCaretLineBackgroundColor(QColor("#e6fff0"));
  setCaretLineVisible(true);

  setLexer(lexerLua);

  loadFile(filename);
}


void luaview::loadFile(const QString &fileName)
{
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly))
  {
    setText("-- File not found");
    return;
  }

  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  setText(in.readAll());
  QApplication::restoreOverrideCursor();
}

luaview::~luaview()
{

}

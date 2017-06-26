/*
 * Copyright 2011 TeleMidia/PUC-Rio.
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
#include "QLineEditFilter.h"

QLineEditFilter::QLineEditFilter (QWidget *parent) : QLineEdit (parent)
{
  setStyleSheet ("color: #bbbbbb;");
  setText (tr ("Filter..."));
  _isFilterText = true;

  connect (this, SIGNAL (textEdited (QString)), this,
           SLOT (userTextEdited (QString)));
}

QLineEditFilter::~QLineEditFilter () {}

void
QLineEditFilter::focusInEvent (QFocusEvent *focusEvent)
{
  if (_isFilterText)
  {
    setText ("");
    setStyleSheet ("color: #000000;");
    _isFilterText = false;
  }
  QLineEdit::focusInEvent (focusEvent);
}

void
QLineEditFilter::focusOutEvent (QFocusEvent *focusEvent)
{
  if (text () == "")
  {
    _isFilterText = true;
    setStyleSheet ("color: #bbbbbb;");
    setText (tr ("Filter..."));
  }
  QLineEdit::focusOutEvent (focusEvent);
}

void
QLineEditFilter::userTextEdited (const QString &text)
{
  if (!_isFilterText)
    emit filterTextChanged (text);
  else
    emit filterTextChanged (QString (""));
}

/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#ifndef SEARCHLINEEDIT_H
#define SEARCHLINEEDIT_H

#include <QKeyEvent>
#include <QLineEdit>

class QToolButton;

/*!
 * \todo This class should inherits from LineEditWithButton
 */
class SearchLineEdit : public QLineEdit
{
  Q_OBJECT

public:
  explicit SearchLineEdit (QWidget *parent = nullptr);

signals:
  void escPressed ();
  void shiftReturnPressed ();

protected:
  void resizeEvent (QResizeEvent *event);
  virtual void keyPressEvent (QKeyEvent *keyEvent);

private:
  QString buttonStyleSheetForCurrentState () const;

  QToolButton *_mSearchButton;

private slots:
  void updateSearchButton (const QString &text);
};

#endif // SEARCHLINEEDIT_H

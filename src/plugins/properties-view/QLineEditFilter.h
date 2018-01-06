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
#ifndef QLINEEDITFILTER_H
#define QLINEEDITFILTER_H

#include <QFocusEvent>
#include <QLineEdit>

class QLineEditFilter : public QLineEdit
{
  Q_OBJECT

public:
  QLineEditFilter (QWidget *parent);
  virtual ~QLineEditFilter ();

signals:
  void filterTextChanged (const QString &);

protected:
  void focusInEvent (QFocusEvent *);
  void focusOutEvent (QFocusEvent *);

private slots:
  void userTextEdited (const QString &);

private:
  bool _isFilterText;
};

#endif // QLINEEDITFILTER_H

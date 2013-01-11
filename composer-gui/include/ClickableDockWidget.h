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

#ifndef CLICKABLEDOCKWIDGET_H
#define CLICKABLEDOCKWIDGET_H

#include <QEvent>
#include <QDockWidget>

namespace composer {
  namespace gui {

/*!
* \brief A DockWidget that emit signals when clicked.
*/
class ClickableQDockWidget : public QDockWidget
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor.
   */
  ClickableQDockWidget (const QString &title, QWidget *parent = 0,
                        Qt::WindowFlags flags = 0);

protected:
  bool event(QEvent *event);

private slots:
  void visibilityHasChange(bool visible);
  void topLevelHasChanged(bool a);

signals:
  void clicked();

};

} }

#endif // CLICKABLEDOCKWIDGET_H

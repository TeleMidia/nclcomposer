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

#include "ClickableDockWidget.h"

namespace composer {
  namespace gui {

ClickableQDockWidget::ClickableQDockWidget(const QString &title,
                                           QWidget *parent,
                                           Qt::WindowFlags flags)
  : QDockWidget(title, parent, flags)
{
  setFocusPolicy(Qt::StrongFocus);

  connect(this, SIGNAL(visibilityChanged(bool)),
          this, SLOT(visibilityHasChange(bool)));

  connect(this, SIGNAL(topLevelChanged(bool)),
          this, SLOT(topLevelHasChanged(bool)));
}

bool ClickableQDockWidget::event(QEvent *event)
{
  if(event->type() == QEvent::MouseButtonPress)
  {
    emit clicked();
  }

  return QDockWidget::event(event);
}

void ClickableQDockWidget::visibilityHasChange(bool visible)
{
  if(visible)
    emit clicked();
}

void ClickableQDockWidget::topLevelHasChanged(bool a)
{
  emit clicked();
  raise();
  setFocus();
}

} }

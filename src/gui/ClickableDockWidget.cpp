/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#include "ClickableDockWidget.h"

#include <QLabel>
#include <QHBoxLayout>

namespace composer {
  namespace gui {

ClickableQDockWidget::ClickableQDockWidget( const QString &title,
                                            QWidget *parent,
                                            Qt::WindowFlags flags)
  : QDockWidget(title, parent, flags)
{
  setFocusPolicy(Qt::StrongFocus);

  connect(this, SIGNAL(visibilityChanged(bool)),
          this, SLOT(visibilityHasChange(bool)));

  connect(this, SIGNAL(topLevelChanged(bool)),
          this, SLOT(topLevelHasChanged(bool)));

  createTitlebar (title);
}

void ClickableQDockWidget::createTitlebar(const QString &title)
{
  QFrame *titleBar = new QFrame();
  titleBar->setContentsMargins(0, 0, 0, 0);

  QLabel *titleLabel = new QLabel(title);
  titleLabel->setSizePolicy( QSizePolicy::MinimumExpanding,
                             QSizePolicy::Minimum );

  QHBoxLayout *layout = new QHBoxLayout(titleBar);
  layout->setMargin(0);
  layout->setSpacing(0);

  layout->addWidget(titleLabel);

  titleBar->setStyleSheet(" ");
  setTitleBarWidget(titleBar);

  // Create refresh button
  QPushButton *refresh_btn = new QPushButton(titleBar);
  // refresh_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  refresh_btn->setIcon(QIcon(":/mainwindow/refreshplugin"));
  refresh_btn->setToolTip(tr("Reload View Model"));
  addButtonToDockTitleBar(titleBar, refresh_btn);
  connect(refresh_btn, SIGNAL(pressed()), this, SIGNAL(refreshPressed()));

  // Create close button
  QPushButton *close_btn = new QPushButton(titleBar);
  // close_btn->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
  close_btn->setIcon(QIcon(":/mainwindow/closeplugin"));
  close_btn->setToolTip(tr("Hide View"));
  addButtonToDockTitleBar(titleBar, close_btn);
  connect(close_btn, SIGNAL(pressed()), this, SLOT(close()));

  //layout->addStretch();
}

void ClickableQDockWidget::addButtonToDockTitleBar(QFrame *titleBar,
                                                   QPushButton *button)
{
  button->setIconSize(QSize(18, 18));

  titleBar->layout()->addWidget(button);
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
  Q_UNUSED(a)

  emit clicked();
  raise();
  setFocus();
}

} }

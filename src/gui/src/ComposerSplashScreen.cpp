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

#include "ComposerSplashScreen.h"

#include <QPainter>

CPR_GUI_BEGIN_NAMESPACE

ComposerSplashScreen::ComposerSplashScreen( const QPixmap &pixmap,
                                            Qt::WindowFlags f ):
  QSplashScreen(pixmap, f)
{}

ComposerSplashScreen::ComposerSplashScreen( QWidget *parent,
                                            const QPixmap &pixmap,
                                            Qt::WindowFlags f):
  QSplashScreen(parent, pixmap, f)
{}

ComposerSplashScreen::~ComposerSplashScreen()
{}

void ComposerSplashScreen::drawContents(QPainter *painter)
{
  QSplashScreen::drawContents(painter);

  QRect r;
  r.setRect( 20, 300, 245, 20 );

  QString version("v.");
  version += NCLCOMPOSER_GUI_VERSION;
  painter->drawText(r, version);
}

CPR_GUI_END_NAMESPACE

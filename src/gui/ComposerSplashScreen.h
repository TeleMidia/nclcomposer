/* Copyright (C) 2011-2018 PUC-Rio/TeleMídia Lab.

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

#ifndef COMPOSERSPLASHSCREEN_H
#define COMPOSERSPLASHSCREEN_H

#include <QSplashScreen>
#include <util/ComposerCore_global.h>

CPR_GUI_BEGIN_NAMESPACE

/**
 * \brief The ComposerSplashScreen class defines the initial splash screen of
 *  NCL Composer
 *
 * This class was created mainly to also draw the NCL Composer in the
 * QSplashScreen
 */
class ComposerSplashScreen : public QSplashScreen
{
  Q_OBJECT

public:
  /**
   * @brief constructor
   * @param pixmap
   * @param f
   */
  explicit ComposerSplashScreen (const QPixmap &pixmap = QPixmap (),
                                 Qt::WindowFlags f = 0);

  /**
   * @brief constructor
   * @param parent
   * @param pixmap
   * @param f
   */
  ComposerSplashScreen (QWidget *parent, const QPixmap &pixmap = QPixmap (),
                        Qt::WindowFlags f = 0);

  /**
   * @brief destructor
   */
  virtual ~ComposerSplashScreen ();

protected:
  /**
   * @brief drawContents adds the NCL Composer version to the splash screen
   * @param painter
   */
  virtual void drawContents (QPainter *painter);
};

CPR_GUI_END_NAMESPACE

#endif // COMPOSERSPLASHSCREEN_H

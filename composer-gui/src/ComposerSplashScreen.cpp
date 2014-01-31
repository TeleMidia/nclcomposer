#include "ComposerSplashScreen.h"

#include <QPainter>
#include <QDebug>

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

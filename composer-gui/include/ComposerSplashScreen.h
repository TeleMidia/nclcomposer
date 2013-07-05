#ifndef COMPOSERSPLASHSCREEN_H
#define COMPOSERSPLASHSCREEN_H

#include <QSplashScreen>

/**
 * @brief The ComposerSplashScreen class defines the initial splash screen of NCL Composer
 *
 * This class was created mainly to also draw the NCL Composer in the QSplashScreen
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
  explicit ComposerSplashScreen(const QPixmap &pixmap = QPixmap(), Qt::WindowFlags f = 0);

  /**
   * @brief constructor
   * @param parent
   * @param pixmap
   * @param f
   */
  ComposerSplashScreen(QWidget *parent, const QPixmap &pixmap = QPixmap(), Qt::WindowFlags f = 0);

  /**
   * @brief destructor
   */
  virtual ~ComposerSplashScreen();

protected:
  /**
   * @brief drawContents adds the NCL Composer version to the splash screen
   * @param painter
   */
  virtual void drawContents(QPainter *painter);
};

#endif // COMPOSERSPLASHSCREEN_H

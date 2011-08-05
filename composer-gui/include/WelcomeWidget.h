#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QObject>
#include <QEvent>
#include <QWidget>
#include <QTableWidget>


namespace Ui{
class WelcomeWidget;
}

namespace composer {
    namespace gui {

/* FROM:
http://www.streamreader.org/stackoverflow/questions/2480773/qt-styling-qtabwidget
*/
// Sets the style sheet of the QTabWidget to expand the tabs.
static void expandingTabsStyleSheet(QTabWidget *tw)
{
    tw->setStyleSheet(QString("QTabBar::tab { width: %1px;} ")
                      .arg(tw->size().width()/tw->count()-18));
}

// On resize events, reapply the expanding tabs style sheet
class ResizeFilter : public QObject
{
    QTabWidget *target;
public:
    ResizeFilter(QTabWidget *target) : QObject(target), target(target) {}

    bool eventFilter(QObject *object, QEvent *event)
    {
        if (event->type() == QEvent::Resize)
            expandingTabsStyleSheet(target);
        return false;
    }
};
/************************************************************************/

class WelcomeWidget: public QWidget {
    Q_OBJECT

public:
    /*!
     \brief Constructor.
     \param parent The parent of this class.
    */
    WelcomeWidget(QWidget *parent = 0);
    /*!
     \brief Destructor.
    */
    virtual ~WelcomeWidget();

private:
    Ui::WelcomeWidget *ui;
};

} }
#endif // WELCOMEWIDGET_H

#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QObject>
#include <QDebug>
#include <QEvent>
#include <QWidget>
#include <QTableWidget>

#include <QXmlStreamReader>
#include <QtNetwork>
#include <QUrl>
#include <QDesktopServices>

#define NCL_CLUB_URL "http://club.ncl.org.br/rss.xml"

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

    /*!
      \brief Set the maximum items that will be showed by this WelcomeWidget

      \param maxItems the number of maximum items;
     */
    void setMaximumItems(int maxItems);

    /*!
      \brief Returns the maximum number of items that will be showed by this
        WelcomeWidget
     */
    int getMaximumItems();

private slots:
    void on_commandLinkButton_29_clicked();
    void on_commandLinkButton_6_clicked();
    void on_commandLinkButton_9_clicked();
    void on_commandLinkButton_30_clicked();
    void on_commandLinkButton_10_clicked();
    void on_commandLinkButton_11_clicked();
    void on_commandLinkButton_31_clicked();
    void on_commandLinkButton_27_clicked();
    void on_commandLinkButton_26_clicked();
    void on_commandLinkButton_25_clicked();
    void on_commandLinkButton_28_clicked();
    void on_commandLinkButton_7_clicked();
    void on_commandLinkButton_8_clicked();

private:
    int connectionId;
    QHttp http;
    QXmlStreamReader xmlReader;
    int n_items;
    int maxItems;

    Ui::WelcomeWidget *ui;

    void loadRSS();
    void parseXml();

private slots:
    void readData(const QHttpResponseHeader &resp);
    void finishRSSLoad(int, bool);
};

} }
#endif // WELCOMEWIDGET_H

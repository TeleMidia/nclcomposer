/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef WELCOMEWIDGET_H
#define WELCOMEWIDGET_H

#include <QObject>
#include <QDebug>
#include <QEvent>
#include <QWidget>
#include <QTableWidget>
#include <QListWidgetItem>
#include <QProgressDialog>
#include <QFileDialog>

#include <QXmlStreamReader>
#include <QtNetwork>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>

#ifdef WITH_CLUBNCL
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#endif

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

/*!
 * \brief A filter that reapply the expandingTabsStyle function after a resize.
 *
 * On resize events, reapply the expanding tabs style sheet
 */
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

/*!
  \brief WelcomeWidget shows a friendly message to the user when the NCL
    Composer is open.

    Additionally, this widget allows the user to create new projects based
    on examples from NCL Club and allows the user to install new plugins
    and see the news related to the software.
 */
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

private slots:
    void on_commandLinkButton_29_clicked();
    void on_commandLinkButton_6_clicked();
    void on_commandLinkButton_9_clicked();
    void on_commandLinkButton_30_clicked();
    void on_commandLinkButton_10_clicked();
    void on_commandLinkButton_11_clicked();
    void on_commandLinkButton_31_clicked();
    void on_commandLinkButton_7_clicked();
    void on_commandLinkButton_8_clicked();

private:
    int connectionId;
    QHttp http;
    QXmlStreamReader xmlReader;

    Ui::WelcomeWidget *ui;
    int n_items;

    void loadRSS();
    void parseXml();

    //TODO: Create a class to handle individual RSS items
    QVector <QString> description;
    QVector <QString> imgSrc;
    QVector <QString> downloadUrl;
    bool isImageEnclosure;

    QString currentTag, currentLink, currentTitle, currentDate, currentDesc,
            currentImg, currentDownloadUrl;

#ifdef WITH_CLUBENCL
/* \todo NCL Club download Application (this must be a separated class). */
    QProgressDialog *progressDialog;
    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    bool httpRequestAborted;
    QFile *file;
    int httpGetId;
    int currentNCLClubItem;
    int currentFile;
    QuaZip zip;
    QuaZipFile zipFile;
    QString fileNameToImport;
    QString projectName;
    bool isDownloading;

    void downloadFile();
    void startRequest(const QUrl &url);
    bool extract( const QString & filePath,
                  const QString & extDirPath,
                  const QString & singleFileName = QString(""));

private slots:
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void cancelDownload();

/* END NCL Club download Application */

private slots:
    void changeCurrentItem(int item);

    void readData(const QHttpResponseHeader &resp);
    void finishRSSLoad(int, bool);
    void downloadApplication();


    bool doExtractCurrentFile( QString extDirPath,
                               QString singleFileName,
                               bool stop);

signals:
    void extractNextFile( QString extDirPath,
                          QString singleFileName,
                          bool stop);
#endif

private slots:
    void on_commandLinkButton_pressed();
    void on_commandLinkButton_2_pressed();

signals:
    /*!
     * \brief Signal emitted when the user press the Open Project button.
     */
    void userPressedOpenProject();

    /*!
     * \brief Signal emitted when the user press the New Project button.
     */
    void userPressedNewProject();

    /*!
     * \brief Signal emitted when the user press the See installed plugins
     *   button.
     */
    void userPressedSeeInstalledPlugins();


};

} }
#endif // WELCOMEWIDGET_H

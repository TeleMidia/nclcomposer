/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include <WelcomeWidget.h>
#include <ui_WelcomeWidget.h>
#include <core/modules/ProjectControl.h>

namespace composer {
namespace gui {

WelcomeWidget::WelcomeWidget(QWidget *parent): QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    ui->tabWidget->installEventFilter(new ResizeFilter(ui->tabWidget));

    connect(ui->pushButton_OpenProject, SIGNAL(pressed()),
            this, SIGNAL(userPressedOpenProject()));

    connect(ui->pushButton_NewProject, SIGNAL(pressed()),
            this, SIGNAL(userPressedNewProject()));

    ui->pushButton_DownloadApp->setEnabled(false);

    connect(&httpNotifyMessages, SIGNAL(readyRead(const QHttpResponseHeader &)),
            this, SLOT(notifyMessagesReadData(const QHttpResponseHeader &)));

#ifdef WITH_CLUBENCL
    //Connect the QHttp with
    connect(&http, SIGNAL(readyRead(const QHttpResponseHeader &)),
            this, SLOT(readData(const QHttpResponseHeader &)));

    connect(&http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(finishRSSLoad(int, bool)));

    connect(ui->pushButton_DownloadApp, SIGNAL(pressed()),
            this, SLOT(downloadApplication()));

    connect(ui->listWidget_NCLClub, SIGNAL(currentRowChanged(int)),
            this, SLOT(changeCurrentItem(int)));

    progressDialog = new QProgressDialog(this);
    connect(progressDialog, SIGNAL(canceled()), this, SLOT(cancelDownload()));

    isDownloading = false;

    loadRSS();
#else
    ui->tabWidget->removeTab(2);
#endif

#ifdef WITH_TEST_VERSION_MESSAGE
#define LABEL_BUILD "This is an ALPHA version. Version: " \
    NCLCOMPOSER_GUI_VERSION  " Build Date and time:" BUILD_DATE

    ui->label_BuildMessage->setText(LABEL_BUILD);
    ui->label_BuildMessage->setStyleSheet("color:white; font-size:13px; "
                                          "text-align: right;");
#endif

    //TODO: By now, we have disable GUI to install new plugins
    ui->tabWidget->removeTab(1);

    //TODO: Enable NCL Composer Tips
    ui->frame_Tips->setVisible(false);

    //This will be visible only if there is messages to read
    ui->labelNotifyMessage->setVisible(false);
    updateNotifyMessages();
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::updateRecentProjects(QStringList recentProjects)
{
  foreach (QCommandLinkButton *button,
           ui->frame_RecentProjects->findChildren<QCommandLinkButton*>())
  {
    button->hide();
    button->deleteLater();
  }

  for(int i = 0; i < recentProjects.size() && i < MAX_RECENT_PROJECTS; i++)
  {
    QString file = recentProjects.at(i);
    QCommandLinkButton *button = new QCommandLinkButton(
          file.mid(file.lastIndexOf("/")+1), file, ui->frame_RecentProjects);
    button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    button->setToolTip(file);
    connect(button, SIGNAL(pressed()), this, SLOT(sendRecentProjectClicked()));

    ui->frame_RecentProjects->layout()->addWidget(button);
  }
}

void WelcomeWidget::updateNotifyMessages()
{
  QUrl url = QUrl::fromUserInput(NCL_COMPOSER_NOTIFY_URL);
  httpNotifyMessages.setHost(url.host());
  connectionId = httpNotifyMessages.get(url.path());
}

void WelcomeWidget::notifyMessagesReadData(const QHttpResponseHeader &resp)
{
  if (resp.statusCode() != 200)
  {
    httpNotifyMessages.abort();
  }
  else
  {
    int min_message_id_to_show = DEFAULT_MIN_MESSAGE_ID_TO_SHOW;
    int max_notify_messages = DEFAULT_MAX_NOTIFY_MESSAGES;

    // Get settings values
    ComposerSettings settings;
    settings.beginGroup("notify_system");
    if(settings.contains("min_message_id_to_show"))
        min_message_id_to_show = settings.value("min_message_id_to_show").toInt();
    else
        settings.setValue("min_message_id_to_show", min_message_id_to_show);

    if(settings.contains("max_notify_messages"))
        max_notify_messages = settings.value("max_notify_messages").toInt();
    else
        settings.setValue("max_notify_messages", max_notify_messages);

    QByteArray bytes = httpNotifyMessages.readAll();
    if (bytes.size())
    {
      QStringList messages = QString(bytes).split("\n");
      QString messageToShow = "";

      for(int i = 0; i < messages.size(); i++)
      {
        if( i > max_notify_messages - 1 ) break;
        QStringList msgSplittedId = messages[i].split("\t");

        if(msgSplittedId.size() >= 2)
        {
          if( msgSplittedId[0].toInt() >= min_message_id_to_show )
            messageToShow += msgSplittedId[1];
          else
            break;
        }
      }

      if(messageToShow.size())
      {
        ui->labelNotifyMessage->setText(messageToShow);
        ui->labelNotifyMessage->setVisible(true);
      }
    }
  }
}

void WelcomeWidget::sendRecentProjectClicked()
{
  QCommandLinkButton *button =
      static_cast<QCommandLinkButton*> (QObject::sender());

  if(button != NULL)
  {
    emit userPressedRecentProject(button->description());
  }
}

#ifdef WITH_CLUBENCL
void WelcomeWidget::loadRSS()
{
    xmlReader.clear();
    n_items = 0;
    isImageEnclosure = true;

    description.clear();
    imgSrc.clear();
    downloadUrl.clear();

    QUrl url = QUrl::fromUserInput(NCL_CLUB_URL);

    http.setHost(url.host());
    connectionId = http.get(url.path());
}

void WelcomeWidget::readData(const QHttpResponseHeader &resp)
{
    if (resp.statusCode() != 200)
    {
        http.abort();
    }
    else
    {
        QByteArray bytes = http.readAll();
        xmlReader.addData(bytes);
        parseXml();
    }
}

void WelcomeWidget::parseXml()
{
//    qDebug() << "WelcomeWidget::parseXml()";

    bool readingItem = false;

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "item")
            {
                readingItem = true;

                currentLink.clear();
                currentTitle.clear();
                currentDate.clear();
                currentDesc.clear();
            }
            else if(xmlReader.name() == "enclosure")
            {
                if(isImageEnclosure)
                {
                    currentImg = xmlReader.attributes().value("url").toString();
                    imgSrc.push_back(currentImg);
                    isImageEnclosure = false;
                }
                else
                {
                    isImageEnclosure = true;
                    currentDownloadUrl = xmlReader.attributes().value("url").toString();
                    downloadUrl.push_back(currentDownloadUrl);
                }
            }

            currentTag = xmlReader.name().toString();
        }
        else if (xmlReader.isEndElement())
        {
            if (xmlReader.name() == "item")
            {
//                qDebug() << "#########" << currentTitle
//                         << currentLink << currentDate <<  currentDesc << n_items;

                ui->listWidget_NCLClub->addItem(currentTitle);
                description.push_back(currentDesc);

                n_items++;
                readingItem = false;

                currentTitle.clear();
                currentLink.clear();
                currentDate.clear();
                currentImg.clear();
                currentDesc.clear();
            }

        }
        else if (xmlReader.isCharacters() && !xmlReader.isWhitespace())
        {
            if (currentTag == "title" && readingItem)
                currentTitle += xmlReader.text().toString();
            else if (currentTag == "link")
                currentLink += xmlReader.text().toString();
            else if (currentTag == "pubDate")
                currentDate += xmlReader.text().toString();
            else if(currentTag == "description")
                currentDesc += xmlReader.text().toString();
        }
    }

    if (xmlReader.error() &&
            xmlReader.error() != QXmlStreamReader::PrematureEndOfDocumentError)
    {
        qWarning() << "XML ERROR:"
                   << xmlReader.lineNumber() << ": " << xmlReader.errorString();

        http.abort();
    }
}

void WelcomeWidget::changeCurrentItem(int item)
{
    currentNCLClubItem = item;

    if(!isDownloading)
        ui->pushButton_DownloadApp->setEnabled(true);

    QString html = "<html><body><img src=\"";
    html += imgSrc[item];
    html += "\"/>\n";
    html += description.at(item);
    html += "</body></html>";
    ui->webView->setHtml(html);
}

void WelcomeWidget::finishRSSLoad(int connectionId, bool error)
{
    qDebug() << "finishRSSLoad(" << connectionId << "," << error << ");";
    if(error)
    {
        if(this->connectionId == connectionId)
        {
            QCommandLinkButton *button =
                    new QCommandLinkButton(this);
            button->setIconSize(QSize(0,0));

            ui->listWidget_NCLClub->addItem(tr("Connection to NCL Club has failed."));
        }
    }
}

void WelcomeWidget::downloadApplication()
{
    QString urlStr = downloadUrl[currentNCLClubItem];
    url = QUrl(urlStr);

    ui->pushButton_DownloadApp->setEnabled(false);
    ui->pushButton_DownloadApp->setText(tr("Downloading application..."));
    downloadFile();
    progressDialog->show();
}

void WelcomeWidget::downloadFile()
{
    isDownloading = true;
    QFileInfo fileInfo(url.path());
    QString fileName = QDir::tempPath() + QDir::separator() + fileInfo.fileName();
    if (fileName.isEmpty())
        fileName = "index.html";

    if (QFile::exists(fileName)) {
        if (QMessageBox::question(this, tr("Application from NCL Club"),
                                  tr("There already exists a file called %1 in "
                                     "the current directory. Overwrite?").arg(fileName),
                                  QMessageBox::Yes|QMessageBox::No, QMessageBox::No)
                == QMessageBox::No)
            return;
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Unable to save the file %1: %2.")
                                 .arg(fileName).arg(file->errorString()));
        delete file;
        file = 0;
        return;
    }

    progressDialog->setWindowTitle(tr("Application from NCL Club"));
    progressDialog->setLabelText(tr("Downloading %1.").arg(fileName));
    // downloadButton->setEnabled(false);

    // schedule the request
    httpRequestAborted = false;
    startRequest(url);
}

void WelcomeWidget::startRequest(const QUrl &url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateDataReadProgress(qint64,qint64)));
}

void WelcomeWidget::updateDataReadProgress(qint64 bytesRead, qint64 totalBytes)
{
    if (httpRequestAborted)
        return;

    progressDialog->setMaximum(totalBytes);
    if(totalBytes > bytesRead)
        progressDialog->setValue(bytesRead);
}

void WelcomeWidget::cancelDownload()
{
    // statusLabel->setText(tr("Download canceled."));
    httpRequestAborted = true;
    if(reply != NULL)
        reply->abort();

    isDownloading = false;
    ui->pushButton_DownloadApp->setEnabled(true);
}

void WelcomeWidget::httpFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        progressDialog->hide();
        return;
    }
    file->flush();
    file->close();

    isDownloading = false;
    ui->pushButton_DownloadApp->setEnabled(true);
    ui->pushButton_DownloadApp->setText(tr("Download and Import this application"));

    QFileInfo fileInfo(*file);

    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Choose the name of the new project to be created"),
            QDir::currentPath(),
            tr("NCL Composer Projects (*.cpr)") );

    if( !filename.isNull())
    {
        if(!filename.endsWith(".cpr"))
            filename = filename + QString(".cpr");
    }
    else
    {
        qDebug() << "Filename is empty. A project will not be created!" << endl;
        return;
    }

    QFileInfo fileInfo2(filename);
    projectName = filename;

    extract(fileInfo.absoluteFilePath(), fileInfo2.absoluteDir().absolutePath());
//  file->remove(); //delete the file after unzipping it
//  progressDialog->hide();
    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
}

void WelcomeWidget::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

bool WelcomeWidget::doExtractCurrentFile( QString extDirPath,
                                          QString singleFileName,
                                          bool stop)
{
    if(stop) // this is the last call!
    {
        zip.close();
        if (zip.getZipError() != UNZ_OK)
        {
            qWarning("testRead(): zip.close(): %d", zip.getZipError());
            return false;
        }

        qDebug() << fileNameToImport << projectName;
        ProjectControl::getInstance()->importFromDocument( fileNameToImport,
                                                           projectName);
        progressDialog->hide();

        return true;
    }

    QuaZipFileInfo info;
    QFile out;
    QString name;
    char c;

    zip.setCurrentFile(zipFile.getActualFileName());
    if (!zip.getCurrentFileInfo(&info))
    {
        qWarning("testRead(): getCurrentFileInfo(): %d\n", zip.getZipError());
        return false;
    }

    if (!singleFileName.isEmpty())
        if (!info.name.contains(singleFileName))
            return true;

    if (!zipFile.open(QIODevice::ReadOnly))
    {
        qWarning("testRead(): file.open(): %d", zipFile.getZipError());
        return false;
    }

    name = QString("%1/%2").arg(extDirPath).arg(zipFile.getActualFileName());

    //progressDialog->setLabelText(tr("Unzipping file: %1").arg(name));
    //progressDialog->setMaximum(zip.getEntriesCount());
    //progressDialog->setValue(this->currentFile++);

    if(name.endsWith(QDir::separator()))
    {
        QDir dir(name);
        dir.mkdir(name);
        zipFile.close();
        emit extractNextFile(extDirPath, singleFileName, !zip.goToNextFile());
        return true;
    }

    if (zipFile.getZipError() != UNZ_OK)
    {
        qWarning("testRead(): file.getFileName(): %d", zipFile.getZipError());
        return false;
    }

    out.setFileName(name);

    // this will fail if "name" contains subdirectories, but we don't mind that
    out.open(QIODevice::WriteOnly);

    // Slow like hell (on GNU/Linux at least), but it is not my fault.
    // Not ZIP/UNZIP package's fault either.
    // The slowest thing here is out.putChar(c).
    while (zipFile.getChar(&c)) out.putChar(c);

    out.close();

    //out.setFileName("out/" + name);
    if(name.endsWith(".ncl")) //\fixme This is problematic. It can exists more than one .ncl file.
    {
        fileNameToImport = QFileInfo(out).absoluteFilePath();
    }

    if (zipFile.getZipError() != UNZ_OK)
    {
        qWarning("testRead(): file.getFileName(): %d", zipFile.getZipError());
        return false;
    }

    if (!zipFile.atEnd())
    {
        qWarning("testRead(): read all but not EOF");
        return false;
    }

    zipFile.close();

    if (zipFile.getZipError() != UNZ_OK) {
        qWarning("testRead(): file.close(): %d", zipFile.getZipError());
        return false;
    }

    emit extractNextFile(extDirPath, singleFileName, !zip.goToNextFile());

    return true;
}

bool WelcomeWidget::extract( const QString &filePath,
                             const QString &extDirPath,
                             const QString &singleFileName)
{
    zip.setZipName(filePath);
    zipFile.setZip(&zip);

    connect(this, SIGNAL(extractNextFile(QString,QString,bool)),
            this, SLOT(doExtractCurrentFile(QString,QString,bool)),
            Qt::QueuedConnection);

    if (!zip.open(QuaZip::mdUnzip))
    {
        qWarning() << filePath;
        qWarning("testRead(): zip.open(): %d", zip.getZipError());
        return false;
    }

    zip.setFileNameCodec("IBM866");

    qWarning("%d entries\n", zip.getEntriesCount());
    qWarning("Global comment: %s\n", zip.getComment().toLocal8Bit().constData());

    this->currentFile = 0;

    emit extractNextFile(extDirPath, singleFileName, !zip.goToFirstFile());

    return true;
}
#endif
} }

void composer::gui::WelcomeWidget::on_commandLinkButton_29_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/Introduction%20to%20DTV%20and%20to%20Ginga-NCL.pdf"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_6_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/Curso%20Ginga%20Brasil.zip"));
}

// void composer::gui::WelcomeWidget::on_commandLinkButton_9_clicked()
// {
//     QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/MCAplDeclarativa.pdf"));
// }

// void composer::gui::WelcomeWidget::on_commandLinkButton_30_clicked()
// {
//     QDesktopServices::openUrl(QUrl("http://www.ncl.org.br/en/tutorials"));
// }

void composer::gui::WelcomeWidget::on_commandLinkButton_10_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/Part%209%20-%20NCL3.0-EC.pdf"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_11_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/Part%209%20-%20NCL3.0-EC.pdf"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_31_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.ncl.org.br/en/relatoriostecnicos"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_7_clicked()
{
    QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_8_clicked()
{
    QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br/doku.php/how_to_create_a_plugin_to_ncl_composer"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_pressed()
{
    QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br/doku.php/plugins:index"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_2_pressed()
{
    emit userPressedSeeInstalledPlugins();
}

void composer::gui::WelcomeWidget::on_commandLinkButton_3_pressed()
{
  QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/Programando_em_NCL_30.pdf"));
}

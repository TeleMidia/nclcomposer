/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "ComposerHelpWidget.h"

#include <QBoxLayout>
#include <QSplitter>
#include <QDebug>

#include <QApplication>

namespace composer {
namespace gui {

/************************* HelpBrowser class **********************************/
HelpBrowser::HelpBrowser(QHelpEngine &helpEngine, QWidget *parent)
  : QWebView(parent), helpEngine(helpEngine)
{

  /* In the future, we must change to only use the .qch files:

  settings()->setAttribute(QWebSettings::PluginsEnabled, false);
  settings()->setAttribute(QWebSettings::JavaEnabled, false);

  page()->setNetworkAccessManager(
                              new HelpNetworkAccessManager(&helpEngine, this) );
  */
}

void HelpBrowser::setSource(const QUrl &url)
{
  QUrl baseUrl = QUrl("file://" + QApplication::applicationDirPath()+"/doc/");

  // This should be: load(url);
  setContent(helpEngine.fileData(url), "text/html", baseUrl);
}

ComposerHelpWidget::ComposerHelpWidget(QWidget *parent)
  : helpEngine(QApplication::applicationDirPath() + "/doc/nclcomposer.qhc",
               parent),
    helpBrowser(helpEngine, parent)
{
  helpEngine.setupData();

  QSplitter *splitter = new QSplitter(this);
  splitter->addWidget(helpEngine.contentWidget());
  splitter->addWidget(&helpBrowser);

  QHBoxLayout *boxLayout = new QHBoxLayout(this);
  boxLayout->addWidget(splitter);

  setLayout(boxLayout);
  helpEngine.contentWidget()->expandAll();

  connect(helpEngine.contentWidget(), SIGNAL(linkActivated(const QUrl &)),
          &helpBrowser, SLOT(setSource(const QUrl &)));

  setMinimumSize(800, 600);
  retranslateUi();
}

void ComposerHelpWidget::retranslateUi()
{
  setWindowTitle(tr("NCL Composer Help"));
}


/*
/todo
The current implementation is not the best one. Today, we have to distribute
the .qch and the .html, .css, etc. files. If we reimplement QNetworkReply
we can use only the .qch file. The following commented code was a not
successful attempt to do this.
 */
/*********************** HelpNetworkReply class *******************************/
/*
 * The HelpNetworkReply will handle the
 *
class HelpNetworkReply : public QNetworkReply
{
public:
    HelpNetworkReply(const QNetworkRequest &request, const QByteArray &fileData,
        const QString &mimeType);

    virtual void abort();

    virtual qint64 bytesAvailable() const
        { return data.length() + QNetworkReply::bytesAvailable(); }

protected:
    virtual qint64 readData(char *data, qint64 maxlen);

private:
    QByteArray data;
    qint64 origLen;
};

HelpNetworkReply::HelpNetworkReply(const QNetworkRequest &request,
                                   const QByteArray &fileData,
                                   const QString &mimeType)

  : data(fileData), origLen(fileData.length())
{
  setRequest(request);
  setOpenMode(QIODevice::ReadOnly);

  setHeader(QNetworkRequest::ContentTypeHeader, mimeType);
  setHeader(QNetworkRequest::ContentLengthHeader, QByteArray::number(origLen));

  QTimer::singleShot(0, this, SIGNAL(metaDataChanged()));
  QTimer::singleShot(0, this, SIGNAL(readyRead()));
}

void HelpNetworkReply::abort()
{
  // nothing to do
}

qint64 HelpNetworkReply::readData(char *buffer, qint64 maxlen)
{
  qint64 len = qMin(qint64(data.length()), maxlen);
  if (len)
  {
    qMemCopy(buffer, data.constData(), len);
    data.remove(0, len);
  }

  if (!data.length())
    QTimer::singleShot(0, this, SIGNAL(finished()));

  return len;
}*/

/******************* HelpNetworkAccessManager class ***************************/
/*
class HelpNetworkAccessManager : public QNetworkAccessManager
{
public:
  HelpNetworkAccessManager(QHelpEngine *engine, QObject *parent);

protected:
  virtual QNetworkReply *createRequest(Operation op,
                                       const QNetworkRequest &request,
                                       QIODevice *outgoingData = 0);

private:
  QHelpEngine *helpEngine;
};
HelpNetworkAccessManager::HelpNetworkAccessManager(QHelpEngine *engine,
                                                   QObject *parent)
  : QNetworkAccessManager(parent), helpEngine(engine)
{
}

QNetworkReply *HelpNetworkAccessManager::createRequest(Operation op,
                                                 const QNetworkRequest &request,
                                                       QIODevice *outgoingData)
{
  const QString& scheme = request.url().scheme();
  if (scheme == QLatin1String("qthelp") || scheme == QLatin1String("about"))
  {
    qDebug() << request.url();
    const QUrl& url = request.url();
    QString mimeType = url.toString();
    if (mimeType.endsWith(QLatin1String(".svg"))
        || mimeType.endsWith(QLatin1String(".svgz")))
    {
      mimeType = QLatin1String("image/svg+xml");
    }
    else if (mimeType.endsWith(QLatin1String(".css")))
    {
      mimeType = QLatin1String("text/css");
    }
    else if (mimeType.endsWith(QLatin1String(".png")))
    {
      mimeType = QLatin1String("image/png");
    }
    else if (mimeType.endsWith(QLatin1String(".js")))
    {
      mimeType = QLatin1String("text/javascript");
    }
    else
    {
      mimeType = QLatin1String("text/html");
    }
    return new HelpNetworkReply(request, helpEngine->fileData(url), mimeType);
  }

  return QNetworkAccessManager::createRequest(op, request, outgoingData);
}*/


} } // end namespace

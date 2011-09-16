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

namespace composer {
namespace gui {


WelcomeWidget::WelcomeWidget(QWidget *parent): QWidget(parent),
    ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    ui->tabWidget->installEventFilter(new ResizeFilter(ui->tabWidget));

    this->maxItems = 5;
    this->n_items = 0;

    //Connect the QHttp with
    connect(&http, SIGNAL(readyRead(const QHttpResponseHeader &)),
            this, SLOT(readData(const QHttpResponseHeader &)));

    connect(&http, SIGNAL(requestFinished(int,bool)),
            this, SLOT(finishRSSLoad(int, bool)));

    connect(ui->pushButton_OpenProject, SIGNAL(pressed()),
            this, SIGNAL(userPressedOpenProject()));

    connect(ui->pushButton_NewProject, SIGNAL(pressed()),
            this, SIGNAL(userPressedNewProject()));

    loadRSS();
}

WelcomeWidget::~WelcomeWidget()
{
    delete ui;
}

void WelcomeWidget::loadRSS()
{
    xmlReader.clear();

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
    QString currentTag;
    QString linkString;
    QString titleString;
    QString dateString;

    qDebug() << "WelcomeWidget::parseXml()";

    bool readingItem = false;

    while (!xmlReader.atEnd())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement())
        {
            if (xmlReader.name() == "item")
            {
                readingItem = true;

                linkString.clear();
                titleString.clear();
                dateString.clear();
            }

            currentTag = xmlReader.name().toString();
        }
        else if (xmlReader.isEndElement())
        {
            if (xmlReader.name() == "item")
            {
                if(n_items < this->maxItems && readingItem)
                {

                    qDebug() << "#########" << titleString
                             << linkString << dateString << n_items;
                    //TODO: Update UI
                    QCommandLinkButton *button =
                            new QCommandLinkButton(ui->frame_ClubeNCL);
                    button->setText(titleString);

                    ui->frame_ClubeNCL->layout()->addWidget(button);
                    n_items++;
                    readingItem = false;
                }

                titleString.clear();
                linkString.clear();
                dateString.clear();
            }

        }
        else if (xmlReader.isCharacters() && !xmlReader.isWhitespace())
        {
            if (currentTag == "title" && readingItem)
                titleString += xmlReader.text().toString();
            else if (currentTag == "link")
                linkString += xmlReader.text().toString();
            else if (currentTag == "pubDate")
                dateString += xmlReader.text().toString();
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

void WelcomeWidget::setMaximumItems(int maxItems)
{
    this->maxItems = maxItems;
}

int WelcomeWidget::getMaximumItems()
{
    return maxItems;
}

void WelcomeWidget::finishRSSLoad(int connectionId, bool error)
{
    qDebug() << "finishRSSLoad(" << connectionId << "," << error << ");";
    if(!error)
    {
        if(this->connectionId == connectionId)
        {
            QCommandLinkButton *button =
                    new QCommandLinkButton(this);
            button->setIconSize(QSize(0,0));

            ui->frame_ClubeNCL->layout()->addWidget(button);

            button->setText(tr("More..."));
        }
    }
    else
    {
        QCommandLinkButton *button =
                new QCommandLinkButton(this);
        button->setIconSize(QSize(0,0));

        ui->frame_ClubeNCL->layout()->addWidget(button);
        button->setText(tr("Connection to NCL Club failed."));
    }
}

} }

void composer::gui::WelcomeWidget::on_commandLinkButton_29_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/Introduction%20to%20DTV%20and%20to%20Ginga-NCL.pdf"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_6_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/Curso%20Ginga%20Brasil.zip"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_9_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.telemidia.puc-rio.br/sites/telemidia.puc-rio.br/files/MCAplDeclarativa.pdf"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_30_clicked()
{
    QDesktopServices::openUrl(QUrl("http://www.ncl.org.br/en/tutorials"));
}

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

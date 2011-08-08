#include <WelcomeWidget.h>
#include <ui_WelcomeWidget.h>

#include <QUrl>
#include <QDesktopServices>

namespace composer {
    namespace gui {

WelcomeWidget::WelcomeWidget(QWidget *parent): QWidget(parent),
                                                   ui(new Ui::WelcomeWidget)
{
    ui->setupUi(this);
    ui->tabWidget->installEventFilter(new ResizeFilter(ui->tabWidget));
}

WelcomeWidget::~WelcomeWidget()
{

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

void composer::gui::WelcomeWidget::on_commandLinkButton_27_clicked()
{
    QDesktopServices::openUrl(QUrl("http://club.ncl.org.br/node/48"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_26_clicked()
{
    QDesktopServices::openUrl(QUrl("http://club.ncl.org.br/node/28"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_25_clicked()
{
    QDesktopServices::openUrl(QUrl("http://club.ncl.org.br/node/19"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_28_clicked()
{
    QDesktopServices::openUrl(QUrl("http://club.ncl.org.br"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_7_clicked()
{
    QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br"));
}

void composer::gui::WelcomeWidget::on_commandLinkButton_8_clicked()
{
    QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br/doku.php/how_to_create_a_plugin_to_ncl_composer"));
}

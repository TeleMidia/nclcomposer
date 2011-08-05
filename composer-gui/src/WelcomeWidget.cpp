#include <WelcomeWidget.h>
#include <ui_WelcomeWidget.h>

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

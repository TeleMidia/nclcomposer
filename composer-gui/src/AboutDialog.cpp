#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QMessageBox>
#include <QFile>

namespace composer {
namespace gui {

AboutDialog::AboutDialog(QWidget *parent):
    QDialog(parent, Qt::Dialog),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->label_ProgramName->setText(QString("NCL Composer v.") +
                                   QString(NCLCOMPOSER_GUI_VERSION));

    connect(ui->button_Close, SIGNAL(pressed()), this, SLOT(close()));
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::showLicense()
{
    QMessageBox box(this);
    QFile file(":/LICENSE.LGPL");
    box.setInformativeText(file.readAll());
    box.setTextFormat(Qt::RichText);
    box.exec();
}

} } // end namespace

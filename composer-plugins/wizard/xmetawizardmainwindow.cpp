#include "xmetawizardmainwindow.h"
#include "ui_xmetawizardmainwindow.h"
#include "wizardgenerator.h"
#include "wizardexecutionengine.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

XMetaWizardMainWindow::XMetaWizardMainWindow(QWidget *parent) :
    QMainWindow(parent)
{
  setWindowTitle("Wizard Generator");

  QHBoxLayout *mainLayout = new QHBoxLayout;

  QPushButton *templateBaseButton = new QPushButton("Choose a Template Base");
  QPushButton *wizarButton = new QPushButton("Run Wizard");

  mainLayout->addWidget(templateBaseButton);
  mainLayout->addWidget(wizarButton);

  connect(templateBaseButton, SIGNAL(pressed()), this, SLOT(runWizardGenerator()));
  connect(wizarButton, SIGNAL(pressed()), this,SLOT(runWizardEngine()) );

  QWidget *widget = new QWidget;
  widget->setLayout(mainLayout);

  setCentralWidget(widget);
}

void XMetaWizardMainWindow::runWizardGenerator()
{
  QString filePath = QFileDialog::getOpenFileName(this, "Choose a template base", getenv("HOME"), "*.tal");
  if (filePath != "")
    WizardGenerator::run(filePath);
}

void XMetaWizardMainWindow::runWizardEngine()
{
  QString filePath = QFileDialog::getOpenFileName(this, "Choose a wizard", getenv("HOME"), "*.ws");
  if (filePath != "")
  {
    WizardExecutionEngine engine(filePath);
    engine.run();
  }
}

XMetaWizardMainWindow::~XMetaWizardMainWindow()
{
}

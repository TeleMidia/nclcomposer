#include "XMetaWizardMainWindow.h"
#include "ui_xmetawizardmainwindow.h"
#include "WizardGenerator.h"
#include "WizardExecutionEngine.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QDir>

XMetaWizardMainWindow::XMetaWizardMainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  setWindowTitle("Wizard Generator");

  QHBoxLayout *mainLayout = new QHBoxLayout;

  QPushButton *templateBaseButton = new QPushButton("Choose a Template Base");
  QPushButton *wizarButton = new QPushButton("Run Wizard");

  mainLayout->addWidget(templateBaseButton);
  mainLayout->addWidget(wizarButton);

  connect(templateBaseButton, SIGNAL(pressed()),
          this, SLOT(runWizardGenerator()));
  connect(wizarButton, SIGNAL(pressed()),
          this, SLOT(runWizardEngine()) );

  QWidget *widget = new QWidget;
  widget->setLayout(mainLayout);

  setCentralWidget(widget);
}

void XMetaWizardMainWindow::runWizardGenerator()
{
  QString filePath = QFileDialog::getOpenFileName(this,
                                                  "Choose a template base",
                                                  QDir::homePath(), "*.tal");
  if (filePath != "")
    WizardGenerator::run(filePath);
}

void XMetaWizardMainWindow::runWizardEngine()
{
  QString filePath = QFileDialog::getOpenFileName(this, "Choose a wizard",
                                                  QDir::homePath(), "*.ws");
  if (filePath != "")
  {
    WizardExecutionEngine engine(filePath);
    engine.run();
  }
}

XMetaWizardMainWindow::~XMetaWizardMainWindow()
{
}

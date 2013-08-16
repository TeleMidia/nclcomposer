#include "xmetawizardmainwindow.h"
#include "wizardexecutionengine.h"
#include "wizardchooser.h"
#include "wizardgenerator.h"

#include <QApplication>
#include <QDebug>

#include <iostream>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  if(argc > 0) // Generate the .ws from the template base passed as parameter
  {
    WizardGenerator::run(argv[1]);
    return 0;
  }

  wizardchooser w;
  int resp = w.exec("/home/roberto/XWizardOutput/");

  qDebug() << resp;

  if (resp == QDialog::Accepted)
  {
    WizardExecutionEngine we;
    QString selectedPath = w.getSelectedPath();

    qDebug() << selectedPath;
    we.setWS( selectedPath + selectedPath.mid(selectedPath.lastIndexOf("/")) + ".ws" );
    we.setInputFile(selectedPath + selectedPath.mid(selectedPath.lastIndexOf("/")) + ".ncl" );

    we.run();

    std::cout << (selectedPath + selectedPath.mid(selectedPath.lastIndexOf("/")) + "Final.ncl").toStdString() << endl;
  }

  return 0;
}

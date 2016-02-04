/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include <qglobal.h>
#include <QApplication>
#include <QResource>
#include <QObject>
#include <QStringList>

#include <util/ComposerSettings.h>

#include "ComposerMainWindow.h"
#include "fvupdater.h"

using namespace composer::gui;

// \todo this function should move from here
void loadTranslations()
{
  /* Get the current language code */
  GlobalSettings settings;
  settings.beginGroup("languages");
  QString language_code = settings.value("currentLanguage",
                                         QString("en")).toString();
  settings.endGroup();
  qDebug() << "[GUI] Current Language = " << language_code;

  QLocale locale = QLocale(language_code);
  QLocale::setDefault(locale);

  /* Get all paths were can be translations */
  settings.beginGroup("extensions");
  QStringList extensions_paths = settings.value("path").toStringList();
  settings.endGroup();

  qDebug() << "[GUI]" <<  extensions_paths;

  /* Go in each path and search for files from that language */
  foreach(QString curPath, extensions_paths)
  {
    qDebug() << curPath;
    QDir curDir(curPath);
    //filter only the files for the current language code
    curDir.setNameFilters(QStringList() << "*_" + language_code + ".qm");

    QFileInfoList fileInfoList = curDir.entryInfoList();

    // for each translation file install in the application.
    foreach(QFileInfo fileInfo, fileInfoList)
    {
      qDebug() << "[GUI] translation file = " << fileInfo.absoluteFilePath();
      QTranslator *composerTranslator = new QTranslator(qApp);
      composerTranslator->load(fileInfo.absoluteFilePath());
      QCoreApplication::installTranslator(composerTranslator);
    }
  }
}

int handleArguments (QStringList &args, bool &initGUI)
{
  if ( args.contains ("-version") )
  {
    initGUI = false;
    cout << "NCL Composer v."
         << QCoreApplication::applicationVersion().toStdString()
         << endl
         << "Copyright (C) 2015 "
         << QCoreApplication::organizationName().toStdString() << "." << endl
         << "This is free software; see the source for copying conditions."
         << "  There is NO" << endl
         << "warranty; not even for MERCHANTABILITY or FITNESS FOR "
         << "A PARTICULAR PURPOSE." << endl << endl;

  }
  else if (args.contains("-help"))
  {
    initGUI = false;
    cout << "Usage nclcomposer [options] [files] ..." << endl;
    cout << "Options:" << endl;
  }

  return 1;
}

int main(int argc, char *argv[])
{
  bool initGUI = true;
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("TeleMidia Lab/PUC-Rio");
  QCoreApplication::setOrganizationDomain("telemidia.pucrio.br");
  QCoreApplication::setApplicationName("composer");
  QCoreApplication::setApplicationVersion(NCLCOMPOSER_GUI_VERSION);

  //QApplication::setApplicationVersion(NCLCOMPOSER_GUI_VERSION);
  // Set this to your own appcast URL, of course FERVOR
  FvUpdater::sharedUpdater()->SetFeedURL("http://composer.telemidia.puc-rio.br/downloads/appcast_nclcomposer.xml");
  FvUpdater::sharedUpdater()->CheckForUpdatesSilent();


  QStringList args = a.arguments();
  handleArguments(args, initGUI);

  if (initGUI)
  {
    GlobalSettings settings;
    // We need that to make sure the defaults are in the settings
    settings.updateWithDefaults(DATA_PATH);
    loadTranslations();

    QResource::registerResource("images.qrc");


    //make the library search path include the application dir on windows
    //this is so the plugins can find the dlls they are linked to at run time
    QApplication::addLibraryPath(QApplication::applicationDirPath());

    a.setQuitOnLastWindowClosed(true);
    ComposerMainWindow w;
    w.setWindowIcon(QIcon(":/mainwindow/icon"));

    QStringList dirs =
          GlobalSettings().value("default_stylesheets_dirs").toStringList();

    foreach(QString dir, dirs)
    {
      if (QFile::exists(dir+"/style.qss"))
      {
        QFile style(dir+"/style.qss");
        if (style.open(QFile::ReadOnly))
        {
          w.setStyleSheet(style.readAll());
          style.close();
        }
        break;
      }
    }

    w.init(a);

    QStringList argList = a.arguments();
    QStringList filesToOpen;
    for(int i = 0; i < argList.size(); i++)
    {
      // Take only the arguments ending with .cpr
      if(argList.at(i).endsWith(".cpr"))
        filesToOpen << argList.at(i);
    }

    w.openProjects(filesToOpen);

    return a.exec();
  }

  return 1;
}

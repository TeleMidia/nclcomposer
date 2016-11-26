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
#include <QCommandLineParser>

#include <util/ComposerSettings.h>

#include "ComposerMainWindow.h"

#ifdef FV_GUI
#include "fvupdater.h"
#endif

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

  qDebug() << "[GUI] Looking for extensions in " <<  extensions_paths;

  /* Go in each path and search for files from that language */
  foreach(QString curPath, extensions_paths)
  {
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

int handleArguments (bool &initGUI)
{
  QCommandLineParser parser;
  QString desc;
  desc += "NCL Composer is a flexible authoring tool for interactive \n"
          "multimedia applications.\n";
  desc += "Copyright (C) 2011-2016 by ";
  desc += QCoreApplication::organizationName();
  desc += ".";
  /* desc += "\n\nThis is free software; see the source for copying conditions.\n"
          "There is NO warranty; not even for MERCHANTABILITY or FITNESS \n"
          "FOR PARTICULAR PURPOSE."; */

  parser.setApplicationDescription(desc);

  const QCommandLineOption helpOption = parser.addHelpOption();
  const QCommandLineOption versionOption = parser.addVersionOption();

  if(!parser.parse(QCoreApplication::arguments()))
    return 0;

  if (parser.isSet(helpOption))
  {
    parser.showHelp();
    initGUI = false;
  }

  if (parser.isSet(versionOption))
  {
    parser.showVersion();
    initGUI = false;
  }

  return 1;
}

int main(int argc, char *argv[])
{
  bool initGUI = true;
  QApplication a(argc, argv);
  QCoreApplication::setOrganizationName("TeleMidia Lab/PUC-Rio");
  QCoreApplication::setOrganizationDomain("telemidia.pucrio.br");
  QCoreApplication::setApplicationName("NCL Composer");
  QCoreApplication::setApplicationVersion(NCLCOMPOSER_GUI_VERSION);

#ifdef FV_GUI
  // Set this to your own appcast URL, of course FERVOR
  FvUpdater::sharedUpdater()->SetFeedURL("http://composer.telemidia.puc-rio.br/downloads/appcast_nclcomposer.xml");
  //FvUpdater::sharedUpdater()->setRequiredSslFingerPrint("d7e0f2fc899f5ec4456300c7061ff1da");	// Optional
  //FvUpdater::sharedUpdater()->setHtAuthCredentials("swupdates", "updatepw");	// Optional
    FvUpdater::sharedUpdater()->setSkipVersionAllowed(false);	// Optional
    FvUpdater::sharedUpdater()->setRemindLaterAllowed(true);	// Optional
  // Finish Up old Updates
  //FvUpdater::sharedUpdater()->finishUpdate();

  FvUpdater::sharedUpdater()->CheckForUpdatesSilent();
#endif

  handleArguments(initGUI);

  if (initGUI)
  {
    GlobalSettings settings;

    /* If the settings is empty (i.e. it is the first time we are running, copy
     * all values from the default settings */
    if (settings.allKeys().empty())
    {
#ifdef WIN32
      QSettings defaultSettings( QApplication::applicationDirPath() +
                                 "/data/default.ini",
                                 QSettings::IniFormat );
#else
      QSettings defaultSettings( QString (DATA_PATH) + "/default.ini",
                                 QSettings::IniFormat );
#endif

      QStringList keys = defaultSettings.allKeys();
      for( QStringList::iterator i = keys.begin(); i != keys.end(); i++ )
      {
        settings.setValue( *i, defaultSettings.value( *i ) );
      }
    }

    // We must be sure the defaults are in the settings
#ifdef WIN32
    settings.updateWithDefaults(QApplication::applicationDirPath() + "/data/");
#else
    settings.updateWithDefaults(DATA_PATH);
#endif
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

    foreach (QString dir, dirs)
    {
      if (QFile::exists(dir + "/style.qss"))
      {
        QFile style(dir + "/style.qss");
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

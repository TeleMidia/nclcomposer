/* Copyright (C) 2011-2017 PUC-Rio/TeleMídia Lab.

 NCL Composer is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published
 by the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 NCL Composer is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU General Lesser Public License
 along with NCL Composer.  If not, see <http://www.gnu.org/licenses/>. */

#include <qglobal.h>
#include <QApplication>
#include <QResource>
#include <QObject>
#include <QStringList>
#include <QCommandLineParser>

#include <util/ComposerSettings.h>

#include "ComposerMainWindow.h"
using namespace cpr::gui;

#ifdef FV_GUI
#include "fvupdater.h"
#endif

Q_DECLARE_LOGGING_CATEGORY(CPR_MAIN)
Q_LOGGING_CATEGORY(CPR_MAIN, "cpr.main")

// \todo this function should move from here
void loadTranslations()
{
  /* Get the current language code */
  GlobalSettings settings;
  settings.beginGroup("languages");
  QString language_code = settings.value("currentLanguage",
                                         QString("en")).toString();
  settings.endGroup();
  qCDebug(CPR_MAIN) << "Current Language = " << language_code;

  QLocale locale = QLocale(language_code);
  QLocale::setDefault(locale);

  /* Get all paths were can be translations */
  settings.beginGroup("extensions");
  QStringList extensions_paths = settings.value("path").toStringList();
  settings.endGroup();

  qCDebug(CPR_MAIN) << "Looking for extensions in " <<  extensions_paths;

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
      qCDebug(CPR_MAIN) << "Loading translation file = " << fileInfo.absoluteFilePath();
      QTranslator *composerTranslator = new QTranslator(qApp);
      composerTranslator->load(fileInfo.absoluteFilePath());
      QCoreApplication::installTranslator(composerTranslator);
    }
  }
}

bool handleArguments (QCommandLineParser &cmdParser, bool &initGUI)
{
  QString desc;
  desc += "NCL Composer is a flexible authoring tool for interactive \n"
          "multimedia applications.\n";
  desc += "Copyright (C) 2011-2017 by ";
  desc += QCoreApplication::organizationName();
  desc += ".";
  desc += "\n\nNCL Composer is free software; see the source for copying "
          "conditions.\n"
          "There is NO warranty; not even for MERCHANTABILITY or FITNESS \n"
          "FOR PARTICULAR PURPOSE.";

  cmdParser.setApplicationDescription(desc);

  const QCommandLineOption helpOption = cmdParser.addHelpOption();
  const QCommandLineOption versionOption = cmdParser.addVersionOption();
  cmdParser.addPositionalArgument("projects", "List of projects to open.");

  if(!cmdParser.parse(QCoreApplication::arguments()))
    return false;

  if (cmdParser.isSet(helpOption))
  {
    cmdParser.showHelp();
    initGUI = false;
  }

  if (cmdParser.isSet(versionOption))
  {
    cmdParser.showVersion();
    initGUI = false;
  }

  return true;
}

int main(int argc, char *argv[])
{
  bool initGUI = true;
  QApplication a(argc, argv);

  QCoreApplication::setOrganizationName("TeleMidia Lab/PUC-Rio");
  QCoreApplication::setOrganizationDomain("telemidia.pucrio.br");
  QCoreApplication::setApplicationName("NCL Composer");
  QCoreApplication::setApplicationVersion(NCLCOMPOSER_GUI_VERSION);
  
  QApplication::addLibraryPath(QApplication::applicationDirPath());

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

  QCommandLineParser cmdParser;
  handleArguments(cmdParser, initGUI);

  if (initGUI)
  {
    QResource::registerResource("images.qrc");
    GlobalSettings settings;

    /* If the settings is empty (i.e. it is the first time we are running,
     * copy all values from the default settings */
    if (settings.allKeys().empty())
    {
      const QString defaultIni =
              QApplication::applicationDirPath()+"/../share/nclcomposer/default.ini";

      qCInfo(CPR_MAIN) << "This is the first time you are running "
                        << "NCL Composer.  It will copy the default settings"
                        << "from " << defaultIni;

      settings.loadDefaults(defaultIni);
    }

    settings.addPlatformDefaults();

    loadTranslations();
    a.setQuitOnLastWindowClosed(true);

    ComposerMainWindow w;
    w.init(a);

    // Load files passed as positional arguments
    const QStringList filesToOpen = cmdParser.positionalArguments();
    w.openProjects(filesToOpen);

    return a.exec();
  }

  return 1;
}


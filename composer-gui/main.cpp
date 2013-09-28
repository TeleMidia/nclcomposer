/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include <QApplication>
#include <QResource>
#include <QObject>
#include <QStringList>

#include <core/util/ComposerSettings.h>

#include "ComposerMainWindow.h"

using namespace composer::gui;

/*!
 * \brief Add the default paths to GlobalSettings.
 */
void updateSettingsWithDefaults()
{
/* Defaults plugins paths */
  QStringList defaultPluginsPath;

  // The first path will look for plugins is at user's home.
  defaultPluginsPath << QDir::homePath() + QString("/composer/extensions");

  // After that we will look for plugins in the default system path
#ifdef Q_WS_MAC
  defaultPluginsPath << "/Library/Application Support/Composer/Extensions"
                     << QApplication::applicationDirPath() +
                        "/../PlugIns/composer";
#elif defined(Q_WS_WIN32)
  defaultPluginsPath << QApplication::applicationDirPath() + "/extensions";
  defaultPluginsPath << "C:/Composer/extensions";
#else
  // PREFIX Should be defined by the qmake while compiling the source code.
#ifdef EXT_DEFAULT_PATH
  defaultPluginsPath << QString(EXT_DEFAULT_PATH)
                        + QString("/lib/composer/extensions");
#endif

#endif

  GlobalSettings settings;
  settings.beginGroup("extensions");
    QStringList extensions_path = settings.value("path").toStringList();
    extensions_path << defaultPluginsPath; //add default to extensions path
    extensions_path.removeDuplicates();
    settings.setValue("path", extensions_path); //udpate with the new value
  settings.endGroup();
/* End defaults plugin path */

/* Default language */
  settings.beginGroup("languages");
  if(!settings.contains("currentLanguage"))
    settings.setValue("currentLanguage", "en");

  // Set the defaults supported languages
  if(!settings.contains("supportedLanguages"))
  {
    QStringList list;
    list << "en_US" << "pt_BR" << "es_ES";
    settings.setValue("supportedLanguages", list);
  }
  settings.endGroup();
/* End default language */

/* Import Bases */
  QString defaultConnBaseDir;
  settings.beginGroup("importBases");
		
  if(!settings.contains("default_connector_base"))
  {
    #ifdef Q_WS_MAC
		#ifdef QT_NO_DEBUG
			defaultConnBaseDir = "../PlugIns/composer/";
		#elif
			defaultConnBaseDir = "/Library/Application Support/Composer/Data/";
		#endif
    #elif defined(Q_WS_WIN32)
      defaultConnBaseDir = QApplication::applicationDirPath() + "/data/";
    #else
      // PREFIX Should be defined by the qmake while compiling the source code.
      #ifdef EXT_DEFAULT_PATH
        defaultConnBaseDir = QString(EXT_DEFAULT_PATH)
                            + QString("/share/composer/");
      #endif
    #endif
  }
	
  settings.setValue("default_conn_base",
                    defaultConnBaseDir + "defaultConnBase.ncl");
  settings.endGroup();
/*End Import Bases*/

/* Stylesheets */
  QStringList stylesheetsDirs =
      settings.value("default_stylesheets_dirs").toStringList();

  stylesheetsDirs << QString(DATA_PATH);

#ifdef Q_WS_WIN32
  stylesheetsDirs << QApplication::applicationDirPath() + "/data";
#elif defined(Q_WS_MAC)
  stylesheetsDirs << QApplication::applicationDirPath() + "/../PlugIns/composer";
#endif

  stylesheetsDirs.removeDuplicates();

  settings.setValue("default_stylesheets_dirs", stylesheetsDirs);
/* End Stylesheets */
}

void loadTranslations(QApplication *app)
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


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setQuitOnLastWindowClosed(true);

    updateSettingsWithDefaults();
    loadTranslations(&a);

    QResource::registerResource("images.qrc");
    QCoreApplication::setOrganizationName("Telemidia Lab");
    QCoreApplication::setOrganizationDomain("telemidia.pucrio.br");
    QCoreApplication::setApplicationName("composer");

    //make the library search path include the application dir on windows
    //this is so the plugins can find the dlls they are linked to at run time
    QApplication::addLibraryPath(QApplication::applicationDirPath());

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
      if(argList.at(i).endsWith(".cpr"))
        filesToOpen << argList.at(i);
    }

    w.openProjects(filesToOpen);

    return a.exec();
}

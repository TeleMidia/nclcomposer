/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include <QtGui/QApplication>
#include <QResource>
#include <QObject>
#include <QStringList>

#include <core/util/ComposerSettings.h>

#ifdef Q_WS_X11
#include <QX11EmbedWidget>
#endif

#include "ComposerMainWindow.h"

#ifdef Q_WS_X11
#include <X11/Xlib.h>
#endif

using namespace composer::gui;

/*!
 * \brief Add the default paths to ComposerSettings.
 */
void updateSettingsWithDefaults()
{
/* Defaults plugins paths */
  QStringList defaultPluginsPath;

  // The first path will be at user's home.
  defaultPluginsPath << QDir::homePath() + QString("/composer/extensions");

  // After that we will look for plugins in the default system path
#ifdef Q_WS_MAC
  defaultPluginsPath << "/Library/Application Support/Composer"
                     << QCoreApplication::applicationDirPath() +
                        "/../PlugIns/composer";
#elif defined(Q_WS_WIN32)
  defaultPluginsPath << QApplication::applicationDirPath() + "/lib/composer";
  defaultPluginsPath << "C:/Composer/lib/composer";
#else
  // PREFIX Should be defined by the qmake while compiling the source code.
#ifdef EXT_DEFAULT_PATH
  defaultPluginsPath << QString(EXT_DEFAULT_PATH)
                        + QString("/lib/composer/extensions");
#endif

#endif

  ComposerSettings settings;
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
  settings.endGroup();
/* End default language */
}

void loadTranslations(QApplication *app)
{
  /* Get the current language code */
  ComposerSettings settings;
  settings.beginGroup("languages");
  QString language_code = settings.value("currentLanguage",
                                         QString("en")).toString();
  settings.endGroup();
  qDebug() << "[GUI] Current Language = " << language_code;

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
      app->installTranslator(composerTranslator);
    }
  }
}


int main(int argc, char *argv[])
{
#ifdef Q_WS_X11
XInitThreads();
#endif
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

    ComposerMainWindow w(a);
    w.setWindowIcon(QIcon(":/mainwindow/icon"));
    w.show();

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

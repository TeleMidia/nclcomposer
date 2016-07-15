/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef COMPOSERMAINWINDOW_H
#define COMPOSERMAINWINDOW_H

#include <QtDebug>
#include <QStringList>
#include <QMap>
#include <QFileSystemModel>

#include <QCoreApplication>
#include <QApplication>
#include <QTimer>

#include <QToolButton>
#include <QDockWidget>
#include <QAtomicInt>

#include <QMainWindow>
#include <QTabWidget>
#include <QStatusBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QMenuBar>
#include <QTreeWidget>
#include <QWizardPage>
#include <QWizard>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <QToolBar>
#include <QListWidget>
#include <QDialog>
#include <QBitmap>
#include <QPainter>
#include <QTreeView>

#include <modules/PluginControl.h>
#include <modules/ProjectControl.h>
#include <modules/LanguageControl.h>
using namespace composer::core;

#include <util/ComposerSettings.h>
using namespace composer::core::util;

#include "ComposerSplashScreen.h"

#include "PerspectiveManager.h"
#include "PreferencesDialog.h"
#include "PluginDetailsDialog.h"
#include "WelcomeWidget.h"
#include "AboutDialog.h"
// #include "ComposerHelpWidget.h"
#include "ClickableDockWidget.h"

#include <qtoolwindowmanager.h>
#include "RunGingaConfig.h"

#ifdef WITH_LIBSSH2
#include "RunRemoteGingaVM.h"
#endif

using namespace composer::gui;

namespace Ui {
class ComposerMainWindow;
}

namespace composer {
namespace gui {

class QThreadEx : public QThread
{
protected:
  void run() { exec(); }
};

/*!
 * \brief The main Window of NCL Composer.
 *
 * This class is the main window of NCL Composer.
 *
 * \fixme This class is too big it must be splitted
 */
class ComposerMainWindow : public QMainWindow {
  Q_OBJECT

private:
  static const int maximumRecentProjectsSize = 10;

  Ui::ComposerMainWindow *ui;

  QListWidget *profilesExt; /*!< Shows a list of the current loaded language
                                   profiles. */
  QStringList extensions_paths; /*!< TODO */


  QTabWidget  *_tabProjects; /*!< Each open project is show in a different tab.
                                  The tabProjects variable keeps the list of the
                                  projects open tabs. */

  QToolButton  *_tbPerspectiveDropList; /*!< Action that shows the list of
                                             perspective as a menu. */
  QToolButton  *_tbLanguageDropList; /*!< Action that shows the list of
                                          languages as a menu. */

  QMenu        *_menuPerspective; /*!< The menu containing the list of perspectives. */
  QMenu        *_menuMultidevice; /*!<todo */
  QMenu        *_menuLanguage;    /*!< The menu containing the list of languages.*/

  QAction      *_projectViewAction; /*!< TODO */
  QAction      *_editPreferencesAction; /*!< TODO */
  QAction      *_restorePluginsLayoutAction; /*!< Action to restore a
                                                  previously saved perspective
                                                  as the current
                                                  plugins layout. */

  QMap<QString, QToolWindowManager*> _projectsWidgets; /*!< Keeps a reference
                                                            to each project
                                                            widget. */
  QTreeWidget *pluginsExt; /*!< Shows a list with the current loaded
                                  plugins. */

  QMap <QTreeWidgetItem*, IPluginFactory*> treeWidgetItem2plFactory;

  PreferencesDialog *preferences; /*!< TODO */

  QDialog *aboutPluginsDialog; /*!< TODO */
  QPushButton *detailsButton;


  WelcomeWidget *welcomeWidget; /*!< TODO */

  PerspectiveManager *perspectiveManager;
  PluginDetailsDialog *pluginDetailsDialog;

  QProcess localGingaProcess;
#ifdef WITH_LIBSSH2
  QThreadEx runRemoteGingaVMThread;
  RunRemoteGingaVMAction runRemoteGingaVMAction;
  StopRemoteGingaVMAction stopRemoteGingaVMAction;
#endif
  QProgressDialog *_taskProgressBar;

  QTimer *autoSaveTimer; // auto save timer

  // ComposerHelpWidget composerHelpWidget;

#if WITH_WIZARD
  QProcess wizardProcess;
  QProcess talProcess;
#endif

private:
  /*!
   * \brief Shows a prompt where the user can choose where its plugins are
   *    located.
   *
   * \return QString the path to the choosen directory.
   */
  QString promptChooseExtDirectory();

  void initModules();

  void initGUI();

  void createAboutPlugins();

  void createStatusBar();

  void createMenus();

  void createActions();

  void createFileSystem();

  void readSettings();

  void readExtensions();

  void closeEvent(QCloseEvent *event);

  void cleanUp();

  void updateRecentProjectsMenu(QStringList &recentProjects);

  void updateDockStyle(QDockWidget *titleBar, bool selected=false);  

  void updateTabWithProject(int index, QString newLocation);

  QTranslator m_translator;   /**< contains the translations for this application */
  QTranslator m_translatorQt; /**< contains the translations for qt */
  QString m_currLang;     /**< contains the currently loaded language */
  QString m_langPath;     /**< Path of language files. This is always fixed to
                               /languages. */

  void loadLanguage(const QString& rLanguage);
  void createLanguageMenu(void);
  void switchTranslator(QTranslator& translator, const QString& filename);

protected:
  void changeEvent(QEvent*);
  void keyPressEvent(QKeyEvent *event);

protected slots:
  void slotLanguageChanged(QAction* action);
  void saveLoadPluginData(int);

private slots:

  void about();
  void aboutPlugins();
  void updateViewMenu();
  void showEditPreferencesDialog();
  void tabClosed(int index);
  void closeCurrentTab();
  void showCurrentWidgetFullScreen();
  void closeAllFiles();

  void startOpenProject(QString projectLoc);
  void endOpenProject(QString projectLoc);

  void saveCurrentGeometryAsPerspective();
  void restorePerspective();
  void savePerspective(QString layoutName);
  void saveDefaultPerspective(QString defaultPerspectiveName);
  void restorePerspective(QString layoutName);

  void runNCL();
  void stopNCL();
  void functionRunPassive();
  void functionRunActive();
  bool isRunningNCL();
  void runOnLocalGinga();
  void copyOnRemoteGingaVM (bool autoplay = true);
  void copyHasFinished();
  void updateRunActions();

  void launchProjectWizard();
  void addToRecentProjects(QString projectUrl);
  void userPressedRecentProject(QString src);
  void userPressedRecentProject();
  void clearRecentProjects (void);
  void importFromDocument();

  void selectedAboutCurrentPluginFactory();

  void showPluginDetails();

  void updateMenuPerspectives();
  void updateMenuLanguages();

  void restorePerspectiveFromMenu();

  void currentTabChanged(int n);

  void focusChanged(QWidget *old, QWidget *now);

  void setProjectDirty(QString location, bool isDirty);

  void gotoNCLClubWebsite();

  void autoSaveCurrentProjects();

  // TODO: Remove this function from here.
  void updateLastFileDialogPath(QString filepath);

  // TODO: Remove this function from here.
  QString getLastFileDialogPath();

  void addDefaultStructureToProject(Project *project,
                                    bool shouldCopyDefaultConnBase = true,
                                    bool shouldCreateADefaultRegion = true,
                                    bool save = true);

  void on_actionReport_Bug_triggered();

  void pluginWidgetViewToggled(bool);

#ifdef WITH_SERV_PUB
  void on_actionPublish_triggered();
#endif

#if WITH_WIZARD
  void on_actionProject_from_Wizard_triggered();
  void wizardFinished(int resp);
#endif

public:

  explicit ComposerMainWindow(QWidget *parent = 0);
  virtual ~ComposerMainWindow();

  void init(const QApplication &app);  

public slots:

  void errorDialog(QString);

  void addPluginWidget(IPluginFactory *fac, IPlugin *plugin, Project *project);

  void openProject();
  void onOpenProjectTab(QString location);

  void saveCurrentProject();
  void saveAsCurrentProject();

  /*!
   * \brief
   * \todo Move this function from here!
   */
  void checkTemporaryFileLastModified(QString filename);

  /*!
   * \brief Remove the temporary file related to location.
   * \todo Move this function from here!
   */
  bool removeTemporaryFile(QString location);

  bool showHelp();

  void undo();
  void redo();

  void openProjects(const QStringList &projects);


signals:
  /*!
     \brief Send this signal when must be writed the current settings.
     \deprecated
    */
  void writeSettings();
};

} } //end namespace

#endif // COMPOSERMAINWINDOW_H

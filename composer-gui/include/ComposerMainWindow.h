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
#include <QtGui/QMainWindow>
#include <QtGui/QTabWidget>
#include <QtGui/QStatusBar>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QMessageBox>
#include <QtGui/QMenuBar>
#include <QtGui/QTreeWidget>
#include <QtGui/QWizardPage>
#include <QtGui/QWizard>
#include <QtGui/QVBoxLayout>
#include <QtGui/QFileDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QLineEdit>
#include <QtGui/QToolBar>
#include <QtGui/QSplashScreen>
#include <QtGui/QListWidget>
#include <QtGui/QDialog>
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include <QtGui/QTreeView>
#include <QToolButton>
#include <QDockWidget>
#include <QAtomicInt>

#include <core/modules/PluginControl.h>
#include <core/modules/ProjectControl.h>
#include <core/modules/LanguageControl.h>
using namespace composer::core;

#include <core/util/ComposerSettings.h>
using namespace composer::core::util;

#include "PerspectiveManager.h"
#include "PreferencesDialog.h"
#include "PluginDetailsDialog.h"
#include "WelcomeWidget.h"
#include "AboutDialog.h"
#include "ComposerHelpWidget.h"

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
 * \brief A DockWidget that emit signals when clicked.
 */
class ClickableQDockWidget : public QDockWidget
{
  Q_OBJECT

public:
  ClickableQDockWidget (const QString &title, QWidget *parent = 0, Qt::WindowFlags flags = 0)
    : QDockWidget(title, parent, flags)
  {
    setFocusPolicy(Qt::StrongFocus);

    connect(this, SIGNAL(visibilityChanged(bool)), this, SLOT(visibilityHasChange(bool)));
    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(topLevelHasChanged(bool)));
  }

protected:
  bool event(QEvent *event)
  {
    if(event->type() == QEvent::MouseButtonPress)
    {
      emit clicked();
    }

    return QDockWidget::event(event);
  }

private slots:
  void visibilityHasChange(bool visible)
  {
    if(visible)
      emit clicked();
  }

  void topLevelHasChanged(bool a)
  {
    emit clicked();
    raise();
    setFocus();
  }

signals:
    void clicked();

};

/*!
 * \brief The main Window of NCL Composer.
 *
 * This class is the main window of NCL Composer.
 */
class ComposerMainWindow : public QMainWindow {
  Q_OBJECT

private:
  static const int maximumRecentProjectsSize = 10;

  Ui::ComposerMainWindow *ui; /*!< A reference to Ui class. */
  QTabWidget  *tabProjects; /*!< Each open project is show in a different
                                tab. The tabProjects variable keeps the list of
                                the projects open tabs. */
  QToolButton  *tbPerspectiveDropList; /*!< Action that shows the list of
                                             perspective as a menu. */
  QMenu        *menu_Perspective; /*!< The menu containing the list of
                                  perspectives. */
  QToolButton  *tbLanguageDropList; /*!< Action that shows the list of
                                             languages as a menu. */
  QMenu         *menu_Language; /*!< The menu containing the list of languages.*/

  QMap<QString, QMainWindow*> projectsWidgets; /*!< Keeps a reference to each
                                                      project widget. */
  QMap<QString, QDockWidget*> firstDock; /*!< To each project location
                                                keeps the reference of the
                                                first QDockWidget added.*/
  QList <QDockWidget*> allDocks; /*!< A list with all the QDockWidgets, to
                                        each plugin. */
  QMutex allDocksMutex;

  QAction *fullScreenViewAct; /*!< Action to show Composer in FullScreen. */
  QAction *projectViewAct; /*!< TODO */
  QAction *editPreferencesAct; /*!< TODO */

  QAction *saveCurrentPluginsLayoutAct; /*!< Action to save current plugins
                                                layout as a new perspective. */
  QAction *restorePluginsLayoutAct; /*!< Action to restore a previously saved
                                           perspective as the current plugins
                                           layout. */

  QListWidget *profilesExt; /*!< Shows a list of the current loaded language
                                   profiles. */
  QTreeWidget *pluginsExt; /*!< Shows a list with the current loaded
                                  plugins. */

  QMap <QTreeWidgetItem*, IPluginFactory*> treeWidgetItem2plFactory;

  PreferencesDialog *preferences; /*!< TODO */

  QDialog *aboutPluginsDialog; /*!< TODO */
  QPushButton *detailsButton;

  QStringList extensions_paths; /*!< TODO */

  WelcomeWidget *welcomeWidget; /*!< TODO */

  PerspectiveManager *perspectiveManager;
  PluginDetailsDialog *pluginDetailsDialog;

  QProcess localGingaProcess;
#ifdef WITH_LIBSSH2
  QThreadEx runRemoteGingaVMThread;
  RunRemoteGingaVMAction runRemoteGingaVMAction;
  StopRemoteGingaVMAction stopRemoteGingaVMAction;
#endif
  QProgressDialog *taskProgressBar;

  QTimer *autoSaveTimer; // auto save timer

  ComposerHelpWidget composerHelpWidget;

private:
  /*!
   * \brief Shows a prompt where the user can choose where its plugins are
   *    located.
   *
   * \return QString the path to the choosen directory.
   */
  QString promptChooseExtDirectory();
  /*!
   * \brief
   */
  void initModules();
  /*!
   * \brief
   */
  void initGUI();
  /*!
   * \brief
   */
  void createAboutPlugins();
  /*!
   * \brief
   */
  void createStatusBar();
  /*!
   * \brief
   */
  void createMenus();
  /*!
   * \brief
   */
  void createActions();
  /*!
   * \brief
   */
  void createFileSystem();
  /*!
   * \brief
   */
  void readSettings();
  /*!
   * \brief
   */
  void readExtensions();
  /*!
   * \brief
   *
   * \param event
   */
  void closeEvent(QCloseEvent *event);
  /*!
   * \brief
   */
  void cleanUp();

  void updateRecentProjectsMenu(QStringList &recentProjects);

  void updateDockStyle(QDockWidget *titleBar, bool selected=false);

  void addButtonToDockTitleBar(QFrame *titleBar, QPushButton *button);

  void updateTabWithProject(int index, QString newLocation);

  QTranslator m_translator;   /**< contains the translations for this application */
  QTranslator m_translatorQt; /**< contains the translations for qt */
  QString m_currLang;     /**< contains the currently loaded language */
  QString m_langPath;     /**< Path of language files. This is always fixed to
                               /languages. */

  /*!
   * \brief Loads a language by the given language shortcurt (e.g. de, en, ...)
   */
  void loadLanguage(const QString& rLanguage);
  /*!
   * \brief Creates the language menu dynamically from the content of m_langPath
   */
  void createLanguageMenu(void);

  void switchTranslator(QTranslator& translator, const QString& filename);

protected:
  /*!
   * \brief This event is called, when a new translator is loaded or the system
   *    language is changed.
   */
  void changeEvent(QEvent*);

  virtual void keyPressEvent(QKeyEvent *event);

protected slots:
  /*!
   * \brief This slot is called by the language menu actions
   */
  void slotLanguageChanged(QAction* action);

  /*!
   * \brief TODO
   */
  void saveLoadPluginData(int);

private slots:
  /*!
   * \brief Shows the about dialog.
   */
  void about();
  /*!
   * \brief Shows the about plugins dialog.
   */
  void aboutPlugins();
  /*!
   * \brief
   */
  void updateViewMenu();
  /*!
     * \brief
     */
  void showEditPreferencesDialog();
  /*!
     * \brief
     * \param index
     */
  void tabClosed(int index);
  /*!
     * \brief
     */
  void closeCurrentTab();
  /*!
     * \brief
     */
  void showCurrentWidgetFullScreen();
  /*!
     \brief
    */
  void closeAllFiles();
  /*!
     * \brief
     */
  void startOpenProject(QString projectLoc);
  void endOpenProject(QString projectLoc);

  void saveCurrentGeometryAsPerspective();
  void restorePerspective();
  void savePerspective(QString layoutName);
  void saveDefaultPerspective(QString defaultPerspectiveName);
  void restorePerspective(QString layoutName);

  /*! Run the current open Project.*/
  void runNCL();
  void stopNCL();
  bool isRunningNCL();
  void runOnLocalGinga();
  void runOnRemoteGingaVM();
  void runHasFinished();
  void updateRunActions();

  void launchProjectWizard();
  void addToRecentProjects(QString projectUrl);
  void userPressedRecentProject(QString src);
  void userPressedRecentProject();
  void clearRecentProjects(void);
  void importFromDocument();
  /*!
   *
   */
  void selectedAboutCurrentPluginFactory();
  /*!
   * \brief Shows the details of the current selected plugins.
   */
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
                                    bool save = true);
  void on_actionReport_Bug_triggered();

public:
  /*!
     * \brief Constructs the Composer Main Window with the given parent.
     *
     * \param parent The parent of the Composer Main Window.
     */
  explicit ComposerMainWindow(QWidget *parent = 0);

  /*!
   * \brief You should call this method to initialize the NCL Composer Main Window and
   *    show the SplashScreen
   *
   * \param app QApplication instance
   */
  void init(const QApplication &app);
  /*!
     * \brief
     */
  ~ComposerMainWindow();

public slots:
  /*!
     * \brief
     *
     * \param QString
     */
  void errorDialog(QString);
  /*!
     * \brief Add a plugin Widget an link it to the given project.
     *
     * \param fac
     * \param plugin
     * \param doc
     * \param n
     */
  void addPluginWidget(IPluginFactory *fac, IPlugin *plugin,
                       Project *project, int n);
  /*!
     * \brief Called when a new tab is open.
     *
     * \param location
     */
  void onOpenProjectTab(QString location);

  /*!
   * \brief Save the current project.
   */
  void saveCurrentProject();
  /*!
   * \brief Save the current project.
   */
  void saveAsCurrentProject();

  /*!
   * \brief Called by the user when he/she wants to open an existent project.
   */
  void openProject();

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

  /*!
     * \brief Show the NCL Composer Help.
     */
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

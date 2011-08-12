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
#include <QSettings>
#include <QStringList>
#include <QMap>
#include <QFileSystemModel>

#include <QCoreApplication>
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
#include <QWebView>

#include <core/modules/PluginControl.h>
#include <core/modules/ProjectControl.h>
#include <core/modules/LanguageControl.h>
using namespace composer::core;

#include "wizard/ProjectWizard.h"
#include "wizard/DocumentWizard.h"
#include "PerspectiveManager.h"
#include "PreferencesDialog.h"
#include "PluginDetailsDialog.h"
#include "ComposerQDockWidget.h"
#include "WelcomeWidget.h"

using namespace composer::gui;

namespace Ui {
    class ComposerMainWindow;
}

//#define USE_MDI 1

namespace composer {
    namespace gui {

/*!
 * \brief The main Window of Composer.
 *
 * This class is the main window of Composer.
 */
class ComposerMainWindow : public QMainWindow {
    Q_OBJECT

private:
    static const int maximumRecentProjectsSize = 10;

    QTabWidget *tabProjects; /*!< Each open project is show in a different
                                tab. The tabProjects variable keeps the list of
                                the projects open tabs. */

    QToolButton *tbPerspectiveDropList; /*!< Action that shows the list of
                                             perspective as a menu. */
    QMenu *menu_Perspective; /*!< The menu containing the list of
                                  perspectives. */

    QMap<QString, QMainWindow*> projectsWidgets; /*!< Keeps a reference to each
                                                      project widget. */
    QMap<QString, QDockWidget*> firstDock; /*!< To each project location
                                                keeps the reference of the
                                                first QDockWidget added.*/
    QList <QDockWidget*> allDocks; /*!< A list with all the QDockWidgets, to
                                        each plugin. */

    QAction *aboutComposerAct; /*!< Action to show About Composer. */
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

    ProjectWizard *projectWizard; /*!< TODO */
    DocumentWizard *documentWizard; /*!< TODO */
    PreferencesDialog *preferences; /*!< TODO */

    /* \todo This should be moved to a new Class */
    QDialog *aboutDialog; /*!< TODO */
    QPushButton *detailsButton;

    QString defaultEx; /*!< TODO */
    QString user_directory_ext; /*!< TODO */

    WelcomeWidget *welcomeWidget; /*!< TODO */

    PerspectiveManager *perspectiveManager;
    PluginDetailsDialog *pluginDetailsDialog;

private:
    Ui::ComposerMainWindow *ui; /*!< A reference to  */
    /*!
     \brief Shows a prompt where the user can choose where its plugins are
            located.

     \return QString the path to the choosen directory.
    */
    QString promptChooseExtDirectory();
    /*!
     \brief

    */
    void initModules();
    /*!
     \brief

    */
    void initGUI();
    /*!
     \brief

    */
    void createAbout();
    /*!
     \brief

    */
    void createStatusBar();
    /*!
     \brief

    */
    void createMenus();
    /*!
     \brief

    */
    void createActions();
    /*!
     \brief

    */
    void createFileSystem();
    /*!
     \brief

    */
    void readSettings();
    /*!
     \brief

    */
    void readExtensions();
    /*!
     \brief

     \param event
    */
    void closeEvent(QCloseEvent *event);
    /*!
     \brief

    */
    void cleanUp();

    void updateRecentProjectsMenu(QStringList &recentProjects);

    void updateDockTitleStyle(QFrame *titleBar, bool selected=false);
    void addButtonToDockTitleBar(QFrame *titleBar, QPushButton *button);


private slots:
    /*!
     \brief

    */
    void about();
    /*!
     \brief
    */
    void updateViewMenu();
    /*!
     \brief
    */
    void showEditPreferencesDialog();
    /*!
     \brief

     \param index
    */
    void tabClosed(int index);

    /*!
      \brief
    */
    void closeCurrentTab();
    /*!
     \brief
    */
    void showCurrentWidgetFullScreen();
    /*!
     \brief
    */
    void closeAllFiles();
    /*!
     \brief
    */
    void startOpenProject(QString projectLoc);
    void endOpenProject(QString projectLoc);

    void saveCurrentGeometryAsPerspective();
    void restorePerspective();
    void savePerspective(QString layoutName);
    void saveDefaultPerspective(QString defaultPerspectiveName);
    void restorePerspective(QString layoutName);

    /*! Run the current open Project. */
    void runNCL();

    void launchProjectWizard();

    void addToRecentProjects(QString projectUrl);

    void openRecentProject();

    void clearRecentProjects(void);

    void importFromDocument();
    /*!
     *
     */
    void selectedAboutCurrentFactory();
    /*!
        \brief Shows the details of the current selected plugins.
     */
    void showPluginDetails();

    void updateMenuPerspectives();

    void restorePerspectiveFromMenu();

    void currentTabChanged(int n);

    void focusChanged(QWidget *old, QWidget *now);

public:
    /*!
     \brief Constructs the Composer Main Window with the given parent.

     \param parent The parent of the Composer Main Window.
    */
    explicit ComposerMainWindow(QApplication &app, QWidget *parent = 0);
    /*!
     \brief

    */
    ~ComposerMainWindow();

public slots:
    /*!
     \brief

     \param QString
    */
    void errorDialog(QString);
    /*!
     \brief Add a plugin Widget an link it to the given project.

     \param fac
     \param plugin
     \param doc
     \param n
    */
    void addPluginWidget(IPluginFactory *fac, IPlugin *plugin,
                         Project *project, int n);
    /*!
     \brief Called when a new tab is open.

     \param location
    */
    void onOpenProjectTab(QString location);

    /*!
        \brief Save the current project.
     */
    void saveCurrentProject();

    /*!
      \brief Called by the user when he/she wants to open an existent project.
     */
    void openProject();

signals:
    /*!
     \brief Send this signal when must be writed the current settings.
     \deprecated
    */
    void writeSettings();
};

} } //end namespace

#endif // COMPOSERMAINWINDOW_H

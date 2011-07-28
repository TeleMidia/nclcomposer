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
#include <QtGui/QDockWidget>
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
                                tab. tabProjects contains the list of open
                                tabs. */

    QToolButton *tbPerspectiveDropList; /*!< Action that shows the list of
                                             perspective as a menu. */
    QMenu *menu_Perspective; /*!< The menu containing the list of
                                  perspectives. */

    QMap<QString, QMainWindow*> projectsWidgets; /*!< \deprecated  */
    QMap<QString, QDockWidget*> firstDock; /*!< TODO */

    QAction *aboutComposerAct; /*!< Action to show About Composer. */
    QAction *fullScreenViewAct; /*!< Action to show Composer in FullScreen. */
    QAction *projectViewAct; /*!< TODO */
    QAction *editPreferencesAct; /*!< TODO */
    QAction *switchWS; /*!< TODO */
    QAction *saveCurrentPluginsLayoutAct;
    QAction *restorePluginsLayoutAct;

    QListWidget *profilesExt; /*!< TODO */
    QTreeWidget *pluginsExt; /*!< TODO */
    QMap <QTreeWidgetItem*, IPluginFactory*> treeWidgetItem2plFactory;

    ProjectWizard *projectWizard; /*!< TODO */
    DocumentWizard *documentWizard; /*!< TODO */
    PreferencesDialog *preferences; /*!< TODO */

    /* \todo This should be moved to a new Class */
    QDialog *aboutDialog; /*!< TODO */
    QPushButton *detailsButton;

    QString defaultEx; /*!< TODO */
    QString user_directory_ext; /*!< TODO */

    QWebView *welcomeScreen; /*!< TODO */

    PerspectiveManager *perspectiveManager;
    PluginDetailsDialog *pluginDetailsDialog;

private:
    Ui::ComposerMainWindow *ui; /*!< TODO */
    /*!
     \brief

     \return QString
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

public:
    /*!
     \brief Constructs the Composer Main Window with the given parent.

     \param parent The parent of the Composer Main Window.
    */
    explicit ComposerMainWindow(QWidget *parent = 0);
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

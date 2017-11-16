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

#include "ComposerMainWindow.h"
#include "ui_ComposerMainWindow.h"

#include <QApplication>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QInputDialog>
#include <QPixmap>
#include <QRegExp>
#include <QShortcut>
#include <QToolBar>
#include <QToolButton>

#include "GeneralPreferences.h"
#include "NewProjectWizard.h"

#ifdef USE_MDI
#include <QMdiArea>
#endif

#include <QSimpleUpdater.h>
// static const QString DEFS_URL =
// "https://raw.githubusercontent.com/TeleMidia/nclcomposer/master/updates_test.json";
static const QString DEFS_URL = "https://raw.githubusercontent.com/TeleMidia/"
                                "nclcomposer/master/updates.json";

#define SHOW_PROFILES 1

CPR_GUI_BEGIN_NAMESPACE

const int autoSaveInterval = 1 * 60 * 1000; // ms

/*!
 * \brief Constructs the Composer Main Window with the given parent.
 *
 * \param parent The parent of the Composer Main Window.
 */
ComposerMainWindow::ComposerMainWindow (QWidget *parent)
    : QMainWindow (parent), _ui (new Ui::ComposerMainWindow)
{
  _ui->setupUi (this);

#if WITH_WIZARD
  connect (&wizardProcess, SIGNAL (finished (int)), this,
           SLOT (wizardFinished (int)));
#else
  _ui->actionProject_from_Wizard->setVisible (false);
#endif

  _updater = QSimpleUpdater::getInstance ();
}

/*!
 * \brief You should call this method to initialize the NCL Composer MainWindow
 * and show the SplashScreen.
 *
 * \param app QApplication instance
 */
void
ComposerMainWindow::init (const QApplication &app)
{
  /* The following code could be in another function */
  QPixmap mPix (":/mainwindow/nclcomposer-splash");
  ComposerSplashScreen splash (mPix);
  splash.setMask (mPix.mask ());
  splash.showMessage (tr ("Loading NCL Composer..."), Qt::AlignRight,
                      Qt::gray);

  splash.show ();
  app.processEvents ();

  splash.showMessage (tr ("Starting GUI..."), Qt::AlignRight, Qt::gray);

  initGUI ();
  loadStyleSheets ();
  app.processEvents ();

  splash.showMessage (tr ("Starting Modules and Plugins..."), Qt::AlignRight,
                      Qt::gray);
  initModules ();
  app.processEvents ();

  _autoSaveTimer = new QTimer (this);
  connect (_autoSaveTimer, SIGNAL (timeout ()), this,
           SLOT (autoSaveCurrentProjects ()));

  _autoSaveTimer->start (autoSaveInterval);

  splash.showMessage (tr ("Reloading last session..."), Qt::AlignRight,
                      Qt::gray);
  readSettings ();
  app.processEvents ();

  splash.finish (this);

  show ();

  _updater->setNotifyOnFinish (DEFS_URL, false);
  _updater->checkForUpdates (DEFS_URL);
}

ComposerMainWindow::~ComposerMainWindow () { delete _ui; }

void
ComposerMainWindow::initModules ()
{
  PluginControl *pgControl = PluginControl::instance ();
  LanguageControl *lgControl = LanguageControl::instance ();
  ProjectControl *projControl = ProjectControl::instance ();

  connect (pgControl, SIGNAL (notifyError (QString)),
           SLOT (errorDialog (QString)));

  connect (pgControl, SIGNAL (addPluginWidgetToWindow (IPluginFactory *,
                                                       IPlugin *, Project *)),
           SLOT (addPluginWidget (IPluginFactory *, IPlugin *, Project *)));

  connect (lgControl, SIGNAL (notifyError (QString)),
           SLOT (errorDialog (QString)));

  connect (projControl, SIGNAL (notifyError (QString)),
           SLOT (errorDialog (QString)));

  connect (projControl, SIGNAL (projectAlreadyOpen (QString)),
           SLOT (onOpenProjectTab (QString)));

  connect (projControl, SIGNAL (startOpenProject (QString)), this,
           SLOT (startOpenProject (QString)));

  connect (projControl, SIGNAL (endOpenProject (QString)),
           SLOT (endOpenProject (QString)));

  /* Recent projects */
  connect (projControl, SIGNAL (endOpenProject (QString)),
           SLOT (addToRecentProjects (QString)));

  connect (_welcomeWidget, SIGNAL (userPressedRecentProject (QString)), this,
           SLOT (userPressedRecentProject (QString)));
  /* end recent projects */

  connect (projControl, SIGNAL (projectAlreadyOpen (QString)),
           SLOT (onOpenProjectTab (QString)));

  connect (projControl, SIGNAL (dirtyProject (QString, bool)), this,
           SLOT (setProjectAsDirty (QString, bool)));

  readExtensions ();
}

void
ComposerMainWindow::readExtensions ()
{
  LanguageControl::instance ()->loadProfiles ();
  PluginControl::instance ()->loadPlugins ();

  /* Load the preferences page */
  _preferencesDialog->addPreferencePage (new GeneralPreferences ());
}

QString
ComposerMainWindow::promptChooseExtDirectory ()
{
  QMessageBox mBox;

  mBox.setText (tr ("The extensions directory is not set."));
  mBox.setInformativeText (tr ("Do you want to try the default"
                               "one (%1)?")
                               .arg (QDir::homePath ()));
  mBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
  mBox.setDefaultButton (QMessageBox::Yes);
  mBox.setIcon (QMessageBox::Question);
  if (mBox.exec () == QMessageBox::No)
  {
    QString dirName = QFileDialog::getExistingDirectory (
        this, tr ("Select Directory"), Utilities::getLastFileDialogPath (),
        QFileDialog::ShowDirsOnly);
    return dirName;
  }
  else
  {
    return "";
  }
}

void
ComposerMainWindow::readSettings ()
{
  GlobalSettings stgs;

  stgs.beginGroup ("mainwindow");
  restoreGeometry (stgs.value ("geometry").toByteArray ());
  restoreState (stgs.value ("windowState").toByteArray ());
  stgs.endGroup ();

  QApplication::processEvents ();

  stgs.beginGroup ("openfiles");
  QStringList openfiles = stgs.value ("openfiles").toStringList ();
  stgs.endGroup ();

  openProjects (openfiles);

  updateRecentProjectsWidgets ();
}

void
ComposerMainWindow::openProjects (const QStringList &projects)
{
  qCDebug (CPR_GUI) << "Openning files:" << projects;
  for (int i = 0; i < projects.size (); i++)
  {
    QString src = projects.at (i);
#ifdef WIN32
    src = src.replace (QDir::separator (), "/");
#endif

    QFile file (src);
    bool openCurrentFile = true, createDefaultProjStructure = false;

    if (!file.exists ())
    {
      int resp = QMessageBox::question (
          this, tr ("File does not exists anymore."),
          tr ("The File %1 does not exists, but "
              "the last time you have closed NCL "
              "Composer this files was open. "
              "Do you want to create a new (empty)"
              " project in the same path?")
              .arg (src),
          QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

      if (resp != QMessageBox::Yes)
      {
        openCurrentFile = false;
      }
      else
      {
        createDefaultProjStructure = true;
      }
    }

    if (openCurrentFile)
    {
      checkTemporaryFileLastModified (src);
      ProjectControl::instance ()->launchProject (src);
      if (createDefaultProjStructure)
      {
        addDefaultStructureToProject (
            ProjectControl::instance ()->project (src), true, true,
            true);
      }
    }
  }

  updateRecentProjectsWidgets ();
}

void
ComposerMainWindow::loadStyleSheets ()
{
  GlobalSettings settings;
  settings.beginGroup ("theme");
  QString style_path = settings.value ("stylesheet").toString ();

  if (QFile::exists (style_path))
  {
    QFile style_file (style_path);
    if (style_file.open (QFile::ReadOnly))
    {
      QString style_ini_path = settings.value ("stylesheet_ini").toString ();
      QSettings settings (style_ini_path, QSettings::IniFormat);
      QString style = style_file.readAll ();

      for (const QString &key : settings.allKeys ())
      {
        style = style.replace (key, settings.value (key).toString ());
      }

      setStyleSheet (style);
      style_file.close ();
    }
  }
}

void
ComposerMainWindow::initGUI ()
{
  setWindowIcon (QIcon (":/mainwindow/icon"));
  setWindowTitle (tr ("NCL Composer"));

  _tabProjects = new QTabWidget (0);
  _tabProjects->setObjectName ("TabProjects");

  _ui->frame->layout ()->addWidget (_tabProjects);

  //    _tabProjects->setMovable(true);
  _tabProjects->setTabsClosable (true);

  /* _tbLanguageDropList = new QToolButton(this);
  _tbLanguageDropList->setIcon(QIcon(":/mainwindow/language"));
  _tbLanguageDropList->setToolTip(tr("Change your current language"));
  _tbLanguageDropList->setPopupMode(QToolButton::InstantPopup); */

  connect (_tabProjects, SIGNAL (tabCloseRequested (int)), this,
           SLOT (tabClosed (int)), Qt::DirectConnection);

  connect (_tabProjects, SIGNAL (currentChanged (int)), this,
           SLOT (currentTabChanged (int)));

  createActions ();
  createMenus ();
  // createLanguageMenu();

  _preferencesDialog = new PreferencesDialog (this);
  _perspectiveManager = new PerspectiveManager (this);
  _aboutPluginsDialog = new AboutPluginsDialog (this);
  _preferencesEditor = new PreferencesEditor (0);

  // UNDO/REDO
  // connect(ui->action_Undo, SIGNAL(triggered()), this, SLOT(undo()));
  // connect(ui->action_Redo, SIGNAL(triggered()), this, SLOT(redo()));

  _welcomeWidget = new WelcomeWidget ();
  _tabProjects->addTab (_welcomeWidget, tr ("Welcome"));
  _tabProjects->setTabIcon (0, QIcon ());

  QTabBar *tabBar = _tabProjects->findChild<QTabBar *> ();
  tabBar->setTabButton (0, QTabBar::RightSide, 0);
  tabBar->setTabButton (0, QTabBar::LeftSide, 0);

  connect (_welcomeWidget, SIGNAL (userPressedOpenProject ()), this,
           SLOT (openProject ()));

  connect (_welcomeWidget, SIGNAL (userPressedNewProject ()), this,
           SLOT (launchProjectWizard ()));

  connect (_welcomeWidget, SIGNAL (userPressedSeeInstalledPlugins ()), this,
           SLOT (aboutPlugins ()));
}

void
ComposerMainWindow::keyPressEvent (QKeyEvent *event)
{
  if (event->modifiers () == Qt::ControlModifier)
  {
    if (event->key () == Qt::Key_Z)
    {
      undo ();
      event->accept ();
    }
    else if (event->key () == Qt::Key_Y)
    {
      redo ();
      event->accept ();
    }
    else if ((event->key () >= Qt::Key_1) && (event->key () <= Qt::Key_9))
    {
      GlobalSettings settings;
      settings.beginGroup ("pluginslayout");
      QStringList perspectives = settings.allKeys ();
      if ((event->key () - Qt::Key_1) < perspectives.count ())
      {
        restorePerspective (perspectives.at (event->key () - Qt::Key_1));
      }

      settings.endGroup ();
      event->accept ();
    }
  }
}

/*!
 * \brief Add a plugin Widget an link it to the given project.
 *
 * \param fac
 * \param plugin
 * \param doc
 * \param n
 */
void
ComposerMainWindow::addPluginWidget (IPluginFactory *fac, IPlugin *plugin,
                                     Project *project)
{
  QToolWindowManager *w;
  QString location = project->location ();
  QString projectId = project->attr ("id");

#ifdef USE_MDI
  QMdiArea *mdiArea;
#endif
  if (_projectsWidgets.contains (location))
  {
    w = _projectsWidgets[location];
#ifdef USE_MDI
    mdiArea = (QMdiArea *)w->centralWidget ();
    mdiArea->setBackground (QBrush (QColor ("#FFFFFF")));
#endif
  }
  else
  {
    w = new QToolWindowManager (_tabProjects);
#ifdef USE_MDI
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy (Qt::ScrollBarAsNeeded);
    w->setCentralWidget (mdiArea);
#endif
    int index = _tabProjects->addTab (w, projectId);
    updateTabWithProject (index, location);
    _projectsWidgets[location] = w;
  }

  QWidget *pW = plugin->widget ();

  if (pW != nullptr)
  {
    pW->setObjectName (fac->id ());
#ifdef USE_MDI
    mdiArea->addSubWindow (pW);
    pW->setWindowModified (true);
    pW->setWindowTitle (projectId + " - "
                        + fac->metadata ().value ("name").toString ());
    pW->show ();
#else

    pW->setWindowTitle (fac->metadata ().value ("name").toString ());
    w->addToolWindow (pW, QToolWindowManager::EmptySpaceArea);

    // Add updateFromModel and close button
    QFrame *btn_group = new QFrame (pW);

    // \todo Move this to *.css file
    btn_group->setStyleSheet ("QToolButton { border: none; } \
                                QToolButton::hover {  background: #aaaaaa; } \
                                QToolButton:pressed { \
                                  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                                              top: 0 #dcebfd, \
                                              stop: 1 #c2dcfd); \
                                }");

    QToolButton *btn_UpdateFromModel = new QToolButton (btn_group);
    btn_UpdateFromModel->setIconSize (QSize (12, 12));

    QAction *action_updateFromModel = new QAction (pW);
    action_updateFromModel->setIcon (QIcon (":/mainwindow/refreshplugin"));
    action_updateFromModel->setToolTip (tr ("Reload View's Model"));
    btn_UpdateFromModel->setDefaultAction (action_updateFromModel);

    QShortcut *shortcut
        = new QShortcut (QKeySequence (tr ("F5", "Update from Model")), pW);
    shortcut->setContext (Qt::WidgetWithChildrenShortcut);

    connect (action_updateFromModel, SIGNAL (triggered ()), plugin,
             SLOT (updateFromModel ()));
    connect (shortcut, SIGNAL (activated ()), plugin,
             SLOT (updateFromModel ()));

    QToolButton *btn_close = new QToolButton (btn_group);
    QAction *close_view_action = new QAction (
        QIcon (":/mainwindow/closeplugin"), tr ("Close view"), btn_group);
    btn_close->setIconSize (QSize (12, 12));
    btn_close->setDefaultAction (close_view_action);
    close_view_action->setData (w->toolWindows ().size () - 1);
    connect (close_view_action, SIGNAL (triggered (bool)), this,
             SLOT (pluginWidgetViewToggled (bool)));

    QHBoxLayout *layout = new QHBoxLayout (btn_group);
    layout->addStretch ();
    layout->addWidget (btn_UpdateFromModel);
    layout->addWidget (btn_close);
    layout->setContentsMargins (0, 0, 0, 0);
    layout->setSpacing (0);
    layout->setMargin (0);
    btn_group->setLayout (layout);

    w->setTabButton (pW, QTabBar::RightSide, btn_group);
#endif
  }
}

/*!
 * \brief
 * \param index
 */
void
ComposerMainWindow::tabClosed (int index)
{
  if (index == 0)
    return; // Do nothing

  QString location = _tabProjects->tabToolTip (index);
  Project *project = ProjectControl::instance ()->project (location);

  if (project != nullptr && project->isDirty ())
  {
    int ret = QMessageBox::warning (
        this, project->attr ("id"), tr ("The project has been modified.\n"
                                        "Do you want to save your changes?"),
        QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
        QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
      saveCurrentProject ();
    else if (ret == QMessageBox::Cancel)
      return;
  }

  ProjectControl::instance ()->closeProject (location);

  // Remove temporary file
  removeTemporaryFile (location);

  if (_projectsWidgets.contains (location))
  {
    QToolWindowManager *w = _projectsWidgets[location];

    // Delete QMainWindow
    if (w)
    {
      w->close ();
      w->deleteLater ();
    }

    _projectsWidgets.remove (location);
  }
}

/*!
 * \brief
 */
void
ComposerMainWindow::closeCurrentTab ()
{
  if (_tabProjects->currentIndex ())
  {
    int currentIndex = _tabProjects->currentIndex ();
    tabClosed (currentIndex);
  }
}

/*!
 * \brief
 */
void
ComposerMainWindow::closeAllFiles ()
{
  while (_tabProjects->count () > 1)
  {
    // TODO: Ask to save all projects here!
    //    for (int i = 0; i < _tabProjects->count(); i++)
    //    {
    //      QString location = _tabProjects->tabToolTip(i);
    //      Project *project =
    //      ProjectControl::instance()->project(location);
    //      if (project->isDirty())
    //      {
    //        qCDebug(CPR_GUI) << "ask to save" << location;
    //      }
    //    }

    tabClosed (1);
    _tabProjects->removeTab (1);
  }
}

/*!
 * \brief Called when a new tab is open.
 *
 * \param location
 */
void
ComposerMainWindow::onOpenProjectTab (QString location)
{
  if (!_projectsWidgets.contains (location))
    return;
  QToolWindowManager *w = _projectsWidgets[location];
  _tabProjects->setCurrentWidget (w);
}

void
ComposerMainWindow::createMenus ()
{
#ifdef Q_OS_MAC
  ui->menubar->setNativeMenuBar (true);
#endif

  connect (_ui->menu_View, SIGNAL (aboutToShow ()), this,
           SLOT (updateViewMenu ()));

  connect (_ui->action_CloseProject, SIGNAL (triggered ()), this,
           SLOT (closeCurrentTab ()));

  connect (_ui->action_CloseAll, SIGNAL (triggered ()), this,
           SLOT (closeAllFiles ()));

  connect (_ui->action_Save, SIGNAL (triggered ()), this,
           SLOT (saveCurrentProject ()));

  connect (_ui->action_SaveAs, SIGNAL (triggered ()), this,
           SLOT (saveAsCurrentProject ()));

  connect (_ui->action_NewProject, SIGNAL (triggered ()), this,
           SLOT (launchProjectWizard ()));

  connect (_ui->actionCheck_for_updates, SIGNAL (triggered ()), this,
           SLOT (checkForUpdates ()));

  /* _menuLanguage = new QMenu(0);
  _tbLanguageDropList->setMenu(_menuLanguage);
  ui->toolBar->addWidget(_tbLanguageDropList);*/

  connect (_ui->action_Perspectives_toolbar, SIGNAL (triggered (bool)),
           _ui->toolBar_Perspectives, SLOT (setVisible (bool)));

  _tabProjects->setCornerWidget (_ui->menubar, Qt::TopLeftCorner);

  // updateMenuLanguages();
  updateMenuPerspectives ();
}

/*!
 * \brief Shows the about dialog.
 */
void
ComposerMainWindow::about ()
{
  AboutDialog dialog (this);
  dialog.exec ();
}

/*!
 * \brief Shows the about plugins dialog.
 */
void
ComposerMainWindow::aboutPlugins ()
{
  _aboutPluginsDialog->loadPlugins ();
  _aboutPluginsDialog->show ();
}

/*!
 * \brief
 *
 * \param QString
 */
void
ComposerMainWindow::errorDialog (QString message)
{
  qCWarning (CPR_GUI) << message;
}

void
ComposerMainWindow::createActions ()
{

  connect (_ui->action_About, SIGNAL (triggered ()), this, SLOT (about ()));

  connect (_ui->action_AboutPlugins, SIGNAL (triggered ()), this,
           SLOT (aboutPlugins ()));

#ifdef WITH_SERV_PUB
  ui->actionPublish->setEnabled (true);
#else
  _ui->actionPublish->setEnabled (false);
#endif

  connect (_ui->action_Preferences, SIGNAL (triggered ()), this,
           SLOT (showEditPreferencesDialog ()));

  connect (_ui->action_Exit, SIGNAL (triggered ()), this, SLOT (close ()));

  connect (_ui->action_Fullscreen, SIGNAL (triggered ()), this,
           SLOT (showCurrentWidgetFullScreen ()));

  connect (_ui->action_Save_current_perspective, SIGNAL (triggered ()), this,
           SLOT (saveCurrentGeometryAsPerspective ()));

  connect (_ui->action_Restore_a_perspective, SIGNAL (triggered ()), this,
           SLOT (restorePerspective ()));

  QWidget *spacer = new QWidget ();
  spacer->setSizePolicy (QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect (_ui->action_OpenProject, SIGNAL (triggered ()), this,
           SLOT (openProject ()));

  connect (_ui->action_ImportFromExistingNCL, SIGNAL (triggered ()), this,
           SLOT (importFromDocument ()));

  connect (_ui->action_GoToClubeNCLWebsite, SIGNAL (triggered ()), this,
           SLOT (gotoNCLClubWebsite ()));

  connect (_ui->action_Help, SIGNAL (triggered ()), this, SLOT (showHelp ()));
}

/*!
 * \brief
 */
void
ComposerMainWindow::showCurrentWidgetFullScreen ()
{
  _tabProjects->addAction (_ui->action_Fullscreen);

  if (!_tabProjects->isFullScreen ())
  {
    _tabProjects->setWindowFlags (Qt::Window);
    _tabProjects->showFullScreen ();
  }
  else
  {
    _tabProjects->setParent (_ui->frame, Qt::Widget);
    _tabProjects->show ();
  }
}

/*!
 * \brief
 */
void
ComposerMainWindow::updateViewMenu ()
{
  _ui->menu_Views->clear ();
  if (_tabProjects->count ()) // check if there is an open document
  {
    QToolWindowManager *toolWidgetManager
        = dynamic_cast<QToolWindowManager *> (_tabProjects->currentWidget ());

    if (toolWidgetManager != 0)
    {
      QWidgetList toolWindows = toolWidgetManager->toolWindows ();
      for (int i = 0; i < toolWindows.length (); i++)
      {
        QWidget *w = toolWindows.at (i);
        QAction *act = _ui->menu_Views->addAction (w->windowTitle ());

        act->setData (i);
        act->setCheckable (true);
        qCDebug (CPR_GUI) << w->parent ();
        act->setChecked (w->parent () != 0);

        connect (act, SIGNAL (triggered (bool)), this,
                 SLOT (pluginWidgetViewToggled (bool)));
      }
    }
  }

  if (_ui->menu_Views->isEmpty ())
    _ui->menu_Views->setEnabled (false);
  else
    _ui->menu_Views->setEnabled (true);
}

void
ComposerMainWindow::closeEvent (QCloseEvent *event)
{

  /** Save any dirty project. \todo This must be a function. */
  for (int index = 1; index < _tabProjects->count (); index++)
  {
    QString location = _tabProjects->tabToolTip (index);
    Project *project = ProjectControl::instance ()->project (location);

    qCDebug (CPR_GUI) << location << project;
    if (project != nullptr && project->isDirty ())
    {
      _tabProjects->setCurrentIndex (index);
      int ret = QMessageBox::warning (
          this, project->attr ("id"), tr ("The project %1 has been modified.\n"
                                          "Do you want to save your changes?")
                                          .arg (location),
          QMessageBox::Yes | QMessageBox::Default, QMessageBox::No,
          QMessageBox::Cancel | QMessageBox::Escape);
      if (ret == QMessageBox::Yes)
        saveCurrentProject ();
      else if (ret == QMessageBox::Cancel)
      {
        event->ignore ();
        return;
      }
    }
  }

  GlobalSettings settings;
  settings.beginGroup ("mainwindow");
  settings.setValue ("geometry", saveGeometry ());
  settings.setValue ("windowState", saveState ());
  settings.endGroup ();

  QStringList openfiles;
  QString key;
  foreach (key, _projectsWidgets.keys ())
    openfiles << key;

  settings.beginGroup ("openfiles");
  if (openfiles.size ())
  {
    settings.setValue ("openfiles", openfiles);
  }
  else
  {
    /* If there aren't any openfile, remove this settings, otherwise it will
       try to load the current path */
    settings.remove ("openfiles");
  }
  settings.endGroup ();
  settings.sync ();
  cleanUp ();

  qApp->quit (); // close the application
}

void
ComposerMainWindow::cleanUp ()
{
  LanguageControl::release ();
  ProjectControl::release ();
  PluginControl::release ();
}

/*!
 * \brief TODO
 */
void
ComposerMainWindow::showEditPreferencesDialog ()
{
  _preferencesDialog->show ();
}

/*!
 * \brief
 */
void
ComposerMainWindow::startOpenProject (QString project)
{
  Q_UNUSED (project);

  this->setCursor (QCursor (Qt::WaitCursor));
  update ();
}

void
ComposerMainWindow::endOpenProject (QString project)
{
  this->setCursor (QCursor (Qt::ArrowCursor));

  onOpenProjectTab (project);

  GlobalSettings settings;
  if (settings.contains ("default_perspective"))
  {
    QString defaultPerspec
        = settings.value ("default_perspective").toString ();
    restorePerspective (defaultPerspec);

    update ();
  }

  this->updateGeometry ();
}

/*!
 * \brief Save the current project.
 */
void
ComposerMainWindow::saveCurrentProject ()
{
  int index = _tabProjects->currentIndex ();

  if (index != 0)
  {
    QString location = _tabProjects->tabToolTip (index);
    Project *project = ProjectControl::instance ()->project (location);

    PluginControl::instance ()->savePluginsData (project);
    ProjectControl::instance ()->saveProject (location);
    _ui->action_Save->setEnabled (false);

    // Save ncl document as well
    QString nclfilepath
        = location.mid (0, location.lastIndexOf (".")) + ".ncl";
    QFile file (nclfilepath);
    if (file.open (QFile::WriteOnly | QIODevice::Truncate))
    {
      // Write FILE!!
      if (project->children ().size ())
        file.write (
            project->children ().at (0)->toString (0, false).toLatin1 ());

      file.close ();
    }
  }
  else
  {
    QMessageBox box (QMessageBox::Warning, tr ("Information"),
                     tr ("There aren't a project to be saved."),
                     QMessageBox::Ok);
    box.exec ();
  }
}

/*!
 * \brief Save the current project.
 */
void
ComposerMainWindow::saveAsCurrentProject ()
{
  int index = _tabProjects->currentIndex ();
  bool saveAlsoNCLDocument = true;

  if (index != 0)
  {
    QString location = _tabProjects->tabToolTip (index);

    QString destFileName = QFileDialog::getSaveFileName (
        this, tr ("Save as NCL Composer Project"),
        Utilities::getLastFileDialogPath (),
        tr ("NCL Composer Projects (*.cpr)"));

    if (!destFileName.isNull () && !destFileName.isEmpty ())
    {
      Utilities::updateLastFileDialogPath (destFileName);

      if (!destFileName.endsWith (".cpr"))
        destFileName = destFileName + ".cpr";

      /* Move the location of the current project to destFileName */
      if (location != destFileName)
        ProjectControl::instance ()->moveProject (location, destFileName);

      Project *project
          = ProjectControl::instance ()->project (destFileName);

      PluginControl::instance ()->savePluginsData (project);
      ProjectControl::instance ()->saveProject (destFileName);

      /* Update Tab Text and Index */
      updateTabWithProject (index, destFileName);

      _ui->action_Save->setEnabled (false);

      if (saveAlsoNCLDocument)
      {
        QString nclfilepath
            = location.mid (0, destFileName.lastIndexOf (".")) + ".ncl";

        QFile file (nclfilepath);
        if (file.open (QFile::WriteOnly | QIODevice::Truncate))
        {
          // Write FILE!!
          if (project->children ().size ())
            file.write (
                project->children ().at (0)->toString (0, false).toLatin1 ());

          file.close ();
        }
      }

      addToRecentProjects (destFileName);
    }
  }
  else
  {
    QMessageBox box (QMessageBox::Warning, tr ("Information"),
                     tr ("There aren't a project to be saved."),
                     QMessageBox::Ok);
    box.exec ();
  }
}

void
ComposerMainWindow::saveCurrentGeometryAsPerspective ()
{
  if (_tabProjects->count ()) // If there is a document open
  {
    _perspectiveManager->setBehavior (PERSPEC_SAVE);
    if (_perspectiveManager->exec ())
    {
      savePerspective (_perspectiveManager->getSelectedName ());
      saveDefaultPerspective (_perspectiveManager->getDefaultPerspective ());
    }
  }
  else
  {
    QMessageBox box (QMessageBox::Warning, tr ("Information"),
                     tr ("There aren't a layout open to be saved."),
                     QMessageBox::Ok);
    box.exec ();
  }

  /* Update the elements in MENU PERSPECTIVE*/
  updateMenuPerspectives ();
}

void
ComposerMainWindow::restorePerspective ()
{
  _perspectiveManager->setBehavior (PERSPEC_LOAD);
  if (_perspectiveManager->exec ())
  {
    restorePerspective (_perspectiveManager->getSelectedName ());
  }

  /* Update the elements in MENU PERSPECTIVE*/
  updateMenuPerspectives ();
}

void
ComposerMainWindow::savePerspective (QString layoutName)
{
  if (_tabProjects->count ()) // Check if there is any open document
  {
    GlobalSettings settings;

    QString location
        = _tabProjects->tabToolTip (_tabProjects->currentIndex ());
    QToolWindowManager *window = _projectsWidgets[location];

    settings.beginGroup ("pluginslayout");
    settings.setValue (layoutName, window->saveState ());
    settings.endGroup ();
  }
}

void
ComposerMainWindow::saveDefaultPerspective (QString defaultPerspectiveName)
{
  GlobalSettings settings;
  settings.setValue ("default_perspective", defaultPerspectiveName);
}

void
ComposerMainWindow::restorePerspective (QString layoutName)
{
  if (_tabProjects->count () > 1) // see if there is any open project
  {
    QString location
        = _tabProjects->tabToolTip (_tabProjects->currentIndex ());

    QToolWindowManager *window = _projectsWidgets[location];

    if (window != nullptr)
    {
      GlobalSettings settings;
      settings.beginGroup ("pluginslayout");
#ifndef USE_MDI
      window->restoreState (settings.value (layoutName));
#endif
      settings.endGroup ();
    }
  }
}

void
ComposerMainWindow::launchProjectWizard ()
{
  NewProjectWizard wizard (this);
  wizard.setModal (true);
  wizard.exec ();

  if (wizard.result () == QWizard::Accepted)
  {
    QString filename = wizard.getProjectFullPath ();

    filename.replace ("\\", "/"); // Force the use of "/"

    if (!filename.isNull () && !filename.isEmpty ())
    {

      Utilities::updateLastFileDialogPath (filename);

      if (!filename.endsWith (".cpr"))
        filename = filename + QString (".cpr");

      QFileInfo info (filename);
      // If the file already exist ask the user if it want to overwrite it
      if (info.exists ())
      {
        if (QMessageBox::question (this, tr ("File already exists!"),
                                   tr ("The file \"%1\" already exists. Do "
                                       "you want overwrite it?")
                                       .arg (filename),
                                   QMessageBox::Yes | QMessageBox::No,
                                   QMessageBox::No)
            == QMessageBox::No)

          return; // Do not overwrite!

        QFile::remove (
            filename); // Remove the OLD file (only if the user accept it)
      }

      // We dont need to check this, when creating a new project
      // checkTemporaryFileLastModified(filename);
      if (ProjectControl::instance ()->launchProject (filename))
      {
        // After launch the project we will insert NCL, HEAD and BODY
        // elements
        // by default
        Project *project
            = ProjectControl::instance ()->project (filename);

        addDefaultStructureToProject (project,
                                      wizard.shouldCopyDefaultConnBase (),
                                      wizard.shouldCreateADefaultRegion ());
      }
      else
      {
        // \todo a report to this problem (we should track the error
        // message).
      }
    }
  }
}

void
ComposerMainWindow::addDefaultStructureToProject (
    Project *project, bool shouldCopyDefaultConnBase,
    bool shouldCreateADefaultRegion, bool save)
{
  const QString defaultNCLID = "myNCLDocID";
  const QString defaultBodyID = "myBodyID";
  const QString defaultConnBaseID = "connBaseId";
  const QString defaultRegionBaseID = "regionBase0";
  const QString defaultRegionID = "region0";

  QMap<QString, QString> nclAttrs, headAttrs, bodyAttrs;
  nclAttrs.insert ("id", defaultNCLID);
  nclAttrs.insert ("xmlns", "http://www.ncl.org.br/NCL3.0/EDTVProfile");

  Entity *nclEntity;
  MessageControl *msgControl
      = PluginControl::instance ()->messageControl (project);
  msgControl->anonymousAddEntity ("ncl", project->uid (), nclAttrs);

  nclEntity = project->entitiesByType ("ncl").first ();

  if (nclEntity != nullptr)
  {
    QString nclEntityId = nclEntity->uid ();
    msgControl->anonymousAddEntity ("head", nclEntityId, headAttrs);

    bodyAttrs.insert ("id", defaultBodyID);
    msgControl->anonymousAddEntity ("body", nclEntityId, bodyAttrs);
  }

  // Copy the default connector
  if (shouldCopyDefaultConnBase)
  {
    GlobalSettings settings;
    settings.beginGroup ("importBases");
    QString defaultConnBase = settings.value ("default_conn_base").toString ();
    settings.endGroup ();

    qCDebug (CPR_GUI) << "Default connBase is " << defaultConnBase;

    QFileInfo defaultConnBaseInfo (defaultConnBase);
    if (defaultConnBaseInfo.exists ())
    {
      QString filename = project->location ();
      QString newConnBase = filename.mid (0, filename.lastIndexOf ("/") + 1)
                            + defaultConnBaseInfo.fileName ();

      qCDebug (CPR_GUI) << "Copy " << defaultConnBase << " to " << newConnBase;

      // remove the file if it already exists
      if (QFile::exists (newConnBase))
      {
        QFile::remove (newConnBase);
      }

      // copy the defaultConnBase to the project dir
      if (QFile::copy (defaultConnBase, newConnBase))
      {
        // If everything is OK we import the new defaultConnBase to NCL
        // document.

        QMap<QString, QString> connBaseAttrs, importBaseAttrs;
        connBaseAttrs.insert ("id", defaultConnBaseID);
        importBaseAttrs.insert ("alias", "conn");
        importBaseAttrs.insert ("documentURI",
                                defaultConnBaseInfo.fileName ());

        // add connectorBase element
        Entity *head = project->entitiesByType ("head").at (0);
        msgControl->anonymousAddEntity ("connectorBase", head->uid (),
                                        connBaseAttrs);

        // add importBase element
        Entity *connectorBase
            = project->entitiesByType ("connectorBase").at (0);
        msgControl->anonymousAddEntity ("importBase", connectorBase->uid (),
                                        importBaseAttrs);
      }
      else // error
      {
        QMessageBox::warning (this, tr ("Error!"),
                              tr ("There was an error copying the default"
                                  "Connector Base. You will need to add a "
                                  "Connector Base by hand in your NCL "
                                  "code."),
                              tr ("Ok"));
      }
    }
    else // error
    {
      QMessageBox::warning (this, tr ("Error!"),
                            tr ("The default Connector Base %1 does not "
                                "exists")
                                .arg (defaultConnBase),
                            tr ("Ok"));
    }
  }

  if (shouldCreateADefaultRegion)
  {
    QMap<QString, QString> regionBaseAttrs, regionAttrs;
    QList<Entity *> regionBases = project->entitiesByType ("regionBase");

    // There is no regionBase, so lets create one
    if (!regionBases.size ())
    {
      regionBaseAttrs.insert ("id", defaultRegionBaseID);
      Entity *head = project->entitiesByType ("head").at (0);
      msgControl->anonymousAddEntity ("regionBase", head->uid (),
                                      regionBaseAttrs);
    }

    // Now, its time to add the region itself
    Entity *regionBase = project->entitiesByType ("regionBase").at (0);
    regionAttrs.insert ("id", defaultRegionID);
    regionAttrs.insert ("top", "0");
    regionAttrs.insert ("left", "0");
    regionAttrs.insert ("width", "100%");
    regionAttrs.insert ("height", "100%");
    regionAttrs.insert ("zIndex", "1");
    msgControl->anonymousAddEntity ("region", regionBase->uid (), regionAttrs);
  }

  if (shouldCreateADefaultRegion)
  {
  }

  if (save)
    saveCurrentProject (); // Save the just created basic file!
}

/*!
 * \brief Called by the user when he/she wants to open an existent project.
 */
void
ComposerMainWindow::openProject ()
{
  QString filename
      = QFileDialog::getOpenFileName (this, tr ("Open NCL Composer Project"),
                                      Utilities::getLastFileDialogPath (),
                                      tr ("NCL Composer Projects (*.cpr)"));
  if (filename != "")
  {
#ifdef WIN32
    filename = filename.replace ("\\", "/");
#endif

    checkTemporaryFileLastModified (filename);

    ProjectControl::instance ()->launchProject (filename);

    Utilities::updateLastFileDialogPath (filename);
  }
}

void
ComposerMainWindow::checkTemporaryFileLastModified (const QString &filename)
{
  QFileInfo temporaryFileInfo (filename + "~");
  QFileInfo fileInfo (filename);

  if (temporaryFileInfo.exists ()
      && temporaryFileInfo.lastModified () > fileInfo.lastModified ())
  {
    bool replace = QMessageBox::question (
        this, tr ("Temporary file is newer."),
        tr ("There is a temporary file related to %1 that "
            "is newer. Do you want replace the %1 file "
            "with it?")
            .arg (filename),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (replace)
    {
      QFile file (filename + "~");
      if (!file.copy (filename))
      {
        QFile oldfile (filename);
        oldfile.remove ();
        file.copy (filename);
      }
    }
  }
}

bool
ComposerMainWindow::removeTemporaryFile (QString location)
{
  QFile file (location + "~");
  return file.remove ();
}

void
ComposerMainWindow::importFromDocument ()
{
  QString docFilename = QFileDialog::getOpenFileName (
      this, tr ("Choose the NCL file to be imported"),
      Utilities::getLastFileDialogPath (), tr ("NCL Documents (*.ncl)"));

  if (docFilename != "")
  {
    Utilities::updateLastFileDialogPath (docFilename);

    QString projFilename = QFileDialog::getSaveFileName (
        this,
        tr ("Choose the NCL Composer Project where the NCL document must be "
            "imported"),
        Utilities::getLastFileDialogPath (),
        tr ("NCL Composer Projects (*.cpr)"));

    // Create the file
    QFile f (projFilename);
    f.open (QIODevice::ReadWrite);
    f.close ();

    if (projFilename != "")
    {
#ifdef WIN32
      projFilename = projFilename.replace (QDir::separator (), "/");
#endif
      ProjectControl::instance ()->importFromDocument (docFilename,
                                                       projFilename);

      Utilities::updateLastFileDialogPath (projFilename);
    }
  }
}

void
ComposerMainWindow::addToRecentProjects (QString projectUrl)
{
  GlobalSettings settings;
  QStringList recentProjects
      = settings.value ("recentProjects").toStringList ();

  recentProjects.push_front (projectUrl);
  recentProjects.removeDuplicates ();

  // MAXIMUM SIZE
  while (recentProjects.size () > this->_maximumRecentProjectsSize)
    recentProjects.pop_back ();

  settings.setValue ("recentProjects", recentProjects);

  updateRecentProjectsWidgets ();
}

void
ComposerMainWindow::updateRecentProjectsWidgets ()
{
  GlobalSettings settings;
  QStringList recentProjects
      = settings.value ("recentProjects").toStringList ();

  _ui->menu_Recent_Files->clear ();
  if (recentProjects.size () == 0)
  {
    QAction *act = _ui->menu_Recent_Files->addAction (tr ("empty"));
    act->setEnabled (false);
  }
  else // There is at least one element in the recentProject list
  {
    for (int i = 0; i < recentProjects.size (); i++)
    {
      QAction *act = _ui->menu_Recent_Files->addAction (recentProjects.at (i));
      act->setData (recentProjects.at (i));

      connect (act, SIGNAL (triggered ()), this,
               SLOT (userPressedRecentProject ()));
    }

    _ui->menu_Recent_Files->addSeparator ();

    QAction *clearRecentProjects
        = _ui->menu_Recent_Files->addAction (tr ("Clear Recent Projects"));

    connect (clearRecentProjects, SIGNAL (triggered ()), this,
             SLOT (clearRecentProjects ()));
  }

  _welcomeWidget->updateRecentProjects (recentProjects);
}

void
ComposerMainWindow::userPressedRecentProject (QString src)
{
#ifdef WIN32
  src = src.replace (QDir::separator (), "/");
#endif

  QFile file (src);
  bool openCurrentFile = true, recreateFile = false;
  if (!file.exists ())
  {
    int resp = QMessageBox::question (
        this, tr ("File does not exists anymore."),
        tr ("The File %1 does not exists anymore. "
            "Do you want to create this file "
            "again?")
            .arg (src),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (resp != QMessageBox::Yes)
      openCurrentFile = false;
    else
      recreateFile = true;
  }

  if (openCurrentFile)
  {
    checkTemporaryFileLastModified (src);

    ProjectControl::instance ()->launchProject (src);
    if (recreateFile)
    {
      // Create the directory structure if it does not exist anymore

      QDir dir;
      bool ok = dir.mkpath (QFileInfo (src).absolutePath ());

      if (!ok)
      {
        // error message, could not create the required directory structure!
        QMessageBox::critical (this, tr ("Error!"),
                               tr ("Error creating directory structure"));
        return;
      }

      // \todo Ask for the import or not of the defaultConnBase.
      addDefaultStructureToProject (
          ProjectControl::instance ()->project (src), false, false,
          true);
    }
  }
}

void
ComposerMainWindow::userPressedRecentProject ()
{
  QAction *action = qobject_cast<QAction *> (QObject::sender ());

  QString src = action->data ().toString ();

  userPressedRecentProject (src);
}

void
ComposerMainWindow::clearRecentProjects (void)
{
  GlobalSettings settings;
  settings.remove ("recentProjects");

  updateRecentProjectsWidgets ();
}

void
ComposerMainWindow::restorePerspectiveFromMenu ()
{
  QAction *action = qobject_cast<QAction *> (QObject::sender ());
  restorePerspective (action->data ().toString ());
}

void
ComposerMainWindow::updateMenuPerspectives ()
{
  GlobalSettings settings;
  settings.beginGroup ("pluginslayout");
  QStringList keys = settings.allKeys ();
  settings.endGroup ();

  _ui->toolBar_Perspectives->clear ();

  for (int i = 0; i < keys.size (); i++)
  {
    QAction *act = _ui->toolBar_Perspectives->addAction (
        keys.at (i), this, SLOT (restorePerspectiveFromMenu ()));
    act->setData (keys[i]);
  }
}

void
ComposerMainWindow::updateMenuLanguages ()
{
  QStringList languages;
  languages << tr ("English") << tr ("Portugues (Brasil)");

  for (int i = 0; i < languages.size (); i++)
  {
    QAction *act = _menuLanguage->addAction (languages.at (i), this,
                                             SLOT (changeLanguageFromMenu ()));
    act->setData (languages[i]);
  }
}

void
ComposerMainWindow::currentTabChanged (int n)
{
  if (n)
  {
    _ui->action_Save_current_perspective->setEnabled (true);
    _ui->action_Restore_a_perspective->setEnabled (true);
    _ui->action_CloseProject->setEnabled (true);
    _ui->action_Save->setEnabled (true);
    _ui->action_SaveAs->setEnabled (true);
    _ui->toolBar_Perspectives->setVisible (
        _ui->action_Perspectives_toolbar->isChecked ());
  }
  else
  {
    _ui->action_Save_current_perspective->setEnabled (false);
    _ui->action_Restore_a_perspective->setEnabled (false);
    _ui->action_CloseProject->setEnabled (false);
    _ui->action_Save->setEnabled (false);
    _ui->action_SaveAs->setEnabled (false);
    _ui->toolBar_Perspectives->setVisible (false);
  }
}

void
ComposerMainWindow::pluginWidgetViewToggled (bool state)
{
  QToolWindowManager *toolWidgetManager
      = dynamic_cast<QToolWindowManager *> (_tabProjects->currentWidget ());
  int index = static_cast<QAction *> (sender ())->data ().toInt ();
  QWidget *toolWindow = toolWidgetManager->toolWindows ()[index];
  toolWidgetManager->moveToolWindow (toolWindow,
                                     state ? QToolWindowManager::LastUsedArea
                                           : QToolWindowManager::NoArea);
}

void
ComposerMainWindow::setProjectAsDirty (QString location, bool isDirty)
{
  QToolWindowManager *window = _projectsWidgets[location];

  QString projectId
      = ProjectControl::instance ()->project (location)->attr ("id");

  int index = _tabProjects->indexOf (window);

  _ui->action_Save->setEnabled (true);

  if (index >= 0)
  {
    if (isDirty)
    {
      _tabProjects->setTabText (index, QString ("*") + projectId);
    }
    else
    {
      _tabProjects->setTabText (index, projectId);
    }
  }
}

void
ComposerMainWindow::undo ()
{
  int index = _tabProjects->currentIndex ();

  if (index >= 1)
  {
    QString location = _tabProjects->tabToolTip (index);
    Project *project = ProjectControl::instance ()->project (location);
    MessageControl *msgControl
        = PluginControl::instance ()->messageControl (project);

    if (msgControl != nullptr)
      msgControl->undo ();
  }
}

void
ComposerMainWindow::redo ()
{
  int index = _tabProjects->currentIndex ();

  if (index >= 1)
  {
    QString location = _tabProjects->tabToolTip (index);
    Project *project = ProjectControl::instance ()->project (location);
    MessageControl *msgControl
        = PluginControl::instance ()->messageControl (project);

    if (msgControl != 0)
      msgControl->redo ();
  }
}

void
ComposerMainWindow::gotoNCLClubWebsite ()
{
  QDesktopServices::openUrl (QUrl ("http://club.ncl.org.br"));
}

/*!
   * \brief Show the NCL Composer Help.
   */
bool
ComposerMainWindow::showHelp ()
{
  // composerHelpWidget.show();
  return true;
}

void
ComposerMainWindow::autoSaveCurrentProjects ()
{
  for (int i = 1; i < _tabProjects->count (); i++)
  {
    QString location = _tabProjects->tabToolTip (i);
    Project *project = ProjectControl::instance ()->project (location);
    if (project->isDirty ())
    {
      PluginControl::instance ()->savePluginsData (project);
      ProjectControl::instance ()->saveTempProject (location);
    }
  }
}

/*!
 * \brief Creates the language menu dynamically from the content of
 * _langPath.
 *        We create the menu entries dynamically, dependant on the existing
 *        translations
 */
void
ComposerMainWindow::createLanguageMenu (void)
{
  QActionGroup *langGroup = new QActionGroup (_menuLanguage);
  langGroup->setExclusive (true);

  connect (langGroup, SIGNAL (triggered (QAction *)), this,
           SLOT (slotLanguageChanged (QAction *)));

  // format systems language
  QString defaultLocale = QLocale::system ().name ();       // e.g. "de_DE"
  defaultLocale.truncate (defaultLocale.lastIndexOf ('_')); // e.g. "de"

  _langPath = QApplication::applicationDirPath ();
  _langPath.append ("/languages");
  QDir dir (_langPath);
  QStringList fileNames = dir.entryList (QStringList ("composer_*.qm"));

  qCDebug (CPR_GUI) << fileNames;
  for (int i = 0; i < fileNames.size (); ++i)
  {
    // get locale extracted by filename
    QString locale;
    locale = fileNames[i];                       // "TranslationExample_de.qm"
    locale.truncate (locale.lastIndexOf ('.'));  // "TranslationExample_de"
    locale.remove (0, locale.indexOf ('_') + 1); // "de"

    QString lang = QLocale::languageToString (QLocale (locale).language ());
    QIcon ico (QString ("%1/%2.png").arg (_langPath).arg (locale));

    QAction *action = new QAction (ico, lang, this);
    action->setCheckable (true);
    action->setData (locale);

    // ui->_menuLanguage->addAction(action);
    _menuLanguage->addAction (action);
    langGroup->addAction (action);

    // set default translators and language checked
    if (defaultLocale == locale)
    {
      action->setChecked (true);
    }
  }
}

/*!
 * \brief This slot is called by the language menu actions.
 *
 * Called every time, a menu entry of the language menu is called.
 */
void
ComposerMainWindow::slotLanguageChanged (QAction *action)
{
  if (action)
  {
    // load the language dependant on the action content
    loadLanguage (action->data ().toString ());
    setWindowIcon (action->icon ());
  }
}

void
ComposerMainWindow::switchTranslator (QTranslator &translator,
                                      const QString &filename)
{
  qApp->removeTranslator (&translator); // remove the old translator

  if (translator.load (filename))
    qApp->installTranslator (&translator);
}

/*!
 * \brief Loads a language by the given language shortcurt (e.g. de, en, ...)
 */
void
ComposerMainWindow::loadLanguage (const QString &rLanguage)
{
  if (_currLang != rLanguage)
  {
    _currLang = rLanguage;
    QLocale locale = QLocale (_currLang);
    QLocale::setDefault (locale);

    switchTranslator (_translator,
                      _langPath + "/"
                          + QString ("composer_%1.qm").arg (rLanguage));
    switchTranslator (_translatorQt, QString ("qt_%1.qm").arg (rLanguage));
  }
}

/*!
 * \brief This method is called, when a new translator is loaded or the
 * system
 * language is changed.
 */
void
ComposerMainWindow::changeEvent (QEvent *event)
{
  if (event)
  {
    switch (event->type ())
    {
      // this event is send if a translator is loaded
      case QEvent::LanguageChange:
        _ui->retranslateUi (this);
        break;
      // this event is send, if the system, language changes
      case QEvent::LocaleChange:
      {
        QString locale = QLocale::system ().name ();
        locale.truncate (locale.lastIndexOf ('_'));
        loadLanguage (locale);
      }
      break;
      default:
        break;
    }
  }

  QMainWindow::changeEvent (event);
}

void
ComposerMainWindow::updateTabWithProject (int index, QString newLocation)
{
  QString oldLocation = _tabProjects->tabToolTip (index);

  if (oldLocation == newLocation)
    return; /* do nothing */

  /* Already had a project in this tab */
  if (!oldLocation.isNull () && !oldLocation.isEmpty ())
  {
    // Update _projectsWidgets
    if (_projectsWidgets.contains (oldLocation))
    {
      _projectsWidgets.insert (newLocation,
                               _projectsWidgets.value (oldLocation));
      _projectsWidgets.remove (oldLocation);
    }
  }

  _tabProjects->setTabToolTip (index, newLocation);
  Project *project = ProjectControl::instance ()->project (newLocation);
  if (project != nullptr)
  {
    QString projectId = project->attr ("id");
    _tabProjects->setTabText (index, projectId);
  }
}

void
ComposerMainWindow::on_actionReport_Bug_triggered ()
{
  QDesktopServices::openUrl (
      QUrl ("https://github.com/telemidia/nclcomposer/issues"));
}

#if WITH_WIZARD
void
ComposerMainWindow::on_actionProject_from_Wizard_triggered ()
{
  GlobalSettings settings;
  settings.beginGroup ("wizard");
  wizardProcess.start (settings.value ("wizard-engine").toByteArray ());
  settings.endGroup ();
}

void
ComposerMainWindow::wizardFinished (int resp)
{
  qCDebug (CPR_GUI) << "Wizard process finished with ret=" << resp;
  if (!resp)
  {
    QString filename = wizardProcess.readLine ();
    filename = (filename.mid (0, filename.size () - 1));

    qCDebug (CPR_GUI) << "Wizard process finished with ret=" << resp;
    qCDebug (CPR_GUI) << filename;

    QStringList args;
    args << filename;
    args << "/tmp/a.ncl";

    GlobalSettings settings;

    settings.beginGroup ("wizard");
    talProcess.start (settings.value ("tal-processor").toByteArray (), args);
    settings.endGroup ();
    talProcess.waitForFinished ();

    qCDebug (CPR_GUI) << talProcess.readAllStandardError ();
    qCDebug (CPR_GUI) << talProcess.readAllStandardOutput ();

    QFileInfo fInfo ("/tmp/a.ncl");
    if (fInfo.exists ())
    {
      ProjectControl::instance ()->importFromDocument ("/tmp/a.ncl",
                                                       "/tmp/a.cpr");
      QFile::remove ("/tmp/a.ncl");
    }
    else
    {
      QMessageBox::warning (
          this, tr ("Error"),
          tr ("It was not possible to create the file from the template!"));
    }
  }
}
#endif

void
ComposerMainWindow::checkForUpdates ()
{
  _updater->setNotifyOnFinish (DEFS_URL, true);
  _updater->checkForUpdates (DEFS_URL);
}

CPR_GUI_END_NAMESPACE

void
cpr::gui::ComposerMainWindow::on_actionKeyboard_shortcuts_triggered ()
{
  ShortcutsDialog dialog (this);
  dialog.exec ();
}

void
cpr::gui::ComposerMainWindow::on_action_Preferences_2_triggered ()
{
  _preferencesEditor->show (); // TEST
}

void
cpr::gui::ComposerMainWindow::on_action_Help_triggered ()
{
  QDesktopServices::openUrl (
      QUrl ("http://ncl-composer-manual.readthedocs.io/en/latest/"));
}

/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "ComposerMainWindow.h"
#include "ui_ComposerMainWindow.h"

#include <QPixmap>
#include <QCloseEvent>
#include <QDialogButtonBox>
#include <QToolBar>
#include <QToolButton>
#include <QApplication>
#include <QRegExp>
#include <QDesktopServices>
#include <QInputDialog>
#include <QShortcut>

#include "GeneralPreferences.h"
#include "NewProjectWizard.h"

#ifdef USE_MDI
#include <QMdiArea>
#endif

#ifdef FV_GUI
#include "fvupdater.h"
#endif

#define SHOW_PROFILES 1

namespace composer {
namespace gui {

const int autoSaveInterval = 1 * 60 * 1000; //ms

/*!
 * \brief Constructs the Composer Main Window with the given parent.
 *
 * \param parent The parent of the Composer Main Window.
 */
ComposerMainWindow::ComposerMainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::ComposerMainWindow),
    localGingaProcess(this)
{
  ui->setupUi(this);

  // Local Ginga Run
  connect( &localGingaProcess, SIGNAL(finished(int)),
           this, SLOT(copyHasFinished()) );

  // Remote Ginga Run
#ifdef WITH_LIBSSH2
  runRemoteGingaVMAction.moveToThread(&runRemoteGingaVMThread);

  connect(&runRemoteGingaVMThread, SIGNAL(started()),
          &runRemoteGingaVMAction, SLOT(copyCurrentProject()));

  connect(&runRemoteGingaVMAction, SIGNAL(finished()),
          &runRemoteGingaVMThread, SLOT(quit()));

  connect(&runRemoteGingaVMThread, SIGNAL(finished()),
          this, SLOT(copyHasFinished()));

  connect(&runRemoteGingaVMThread, SIGNAL(terminated()),
          this, SLOT(copyHasFinished()));
#endif

#if WITH_WIZARD
  connect( &wizardProcess, SIGNAL(finished(int)),
           this, SLOT(wizardFinished(int)) );
#else
  ui->actionProject_from_Wizard->setVisible(false);
#endif
  // setWindowFlags(Qt::Window | Qt::WindowTitleHint);
}

/*!
 * \brief You should call this method to initialize the NCL Composer Main
 *        Window and show the SplashScreen.
 *
 * \param app QApplication instance
 */
void ComposerMainWindow::init(const QApplication &app)
{
  /* The following code could be in another function */
  QPixmap mPix(":/mainwindow/nclcomposer-splash");
  ComposerSplashScreen splash(mPix);
  splash.setMask(mPix.mask());
  splash.showMessage(tr("Loading NCL Composer..."),
                     Qt::AlignRight, Qt::gray);

  //splash.blockSignals(true);
  splash.show();
  app.processEvents();

  splash.showMessage(tr("Starting GUI..."),
                     Qt::AlignRight, Qt::gray);

  initGUI();
  app.processEvents();

  splash.showMessage(tr("Starting Modules and Plugins..."),
                     Qt::AlignRight, Qt::gray);
  initModules();
  app.processEvents();

#ifdef WITH_LIBSSH2
  SimpleSSHClient::init(); // Initializes the libssh2 library
#endif

  autoSaveTimer = new QTimer(this);
  connect(autoSaveTimer, SIGNAL(timeout()),
          this, SLOT(autoSaveCurrentProjects()));

  autoSaveTimer->start(autoSaveInterval);

  splash.showMessage(tr("Reloading last session..."),
                     Qt::AlignRight, Qt::gray);
  readSettings();
  app.processEvents();

  splash.finish(this);
  connect(&app, SIGNAL(focusChanged(QWidget *, QWidget *)),
          this, SLOT(focusChanged(QWidget *, QWidget *)),
          Qt::DirectConnection);

  show();
}

ComposerMainWindow::~ComposerMainWindow()
{
  delete ui;
#ifdef WITH_LIBSSH2
  SimpleSSHClient::exit();
#endif
}

void ComposerMainWindow::initModules()
{
  PluginControl  *pgControl = PluginControl::getInstance();
  LanguageControl *lgControl = LanguageControl::getInstance();
  ProjectControl *projectControl = ProjectControl::getInstance();

  connect( pgControl, SIGNAL(notifyError(QString)),
           SLOT(errorDialog(QString)) );

  connect( pgControl, SIGNAL(addPluginWidgetToWindow(IPluginFactory*,
                                                    IPlugin*, Project*)),
           SLOT(addPluginWidget(IPluginFactory*, IPlugin*, Project*)) );

  connect( lgControl, SIGNAL(notifyError(QString)),
           SLOT(errorDialog(QString)));

  connect( projectControl, SIGNAL(notifyError(QString)),
           SLOT(errorDialog(QString)) );

  connect( projectControl,SIGNAL(projectAlreadyOpen(QString)),
           SLOT(onOpenProjectTab(QString)) );

  connect( projectControl, SIGNAL(startOpenProject(QString)),
           this, SLOT(startOpenProject(QString)) );

  connect( projectControl, SIGNAL(endOpenProject(QString)),
           SLOT(endOpenProject(QString)) );

  /* Recent projects */
  connect( projectControl, SIGNAL(endOpenProject(QString)),
           SLOT(addToRecentProjects(QString)) );

  connect( welcomeWidget, SIGNAL(userPressedRecentProject(QString)),
           this, SLOT(userPressedRecentProject(QString)) );
  /* end recent projects */

  connect( projectControl,SIGNAL(projectAlreadyOpen(QString)),
           SLOT(onOpenProjectTab(QString)) );

  connect( projectControl, SIGNAL(dirtyProject(QString, bool)),
           this, SLOT(setProjectDirty(QString, bool)) );

  readExtensions();
}

void ComposerMainWindow::readExtensions()
{
  GlobalSettings settings;

  settings.beginGroup("extensions");
  extensions_paths.clear();

  //Remember: The default paths are been added in main.cpp
  if (settings.contains("path"))
    extensions_paths << settings.value("path").toStringList();

  extensions_paths.removeDuplicates(); // Remove duplicate paths

  // add all the paths to LibraryPath, i.e., plugins are allowed to install
  // dll dependencies in the extensions path.
  for (int i = 0; i < extensions_paths.size(); i++)
  {
    QApplication::addLibraryPath(extensions_paths.at(i) + "/");
  }

  // foreach path where extensions can be installed, try to load profiles.
  for (int i = 0; i < extensions_paths.size(); i++)
  {
    LanguageControl::getInstance()->loadProfiles(extensions_paths.at(i));
  }

  // foreach path where extensions can be installed, try to load plugins.
  for(int i = 0; i < extensions_paths.size(); i++)
  {
    PluginControl::getInstance()->loadPlugins(extensions_paths.at(i));
  }
  settings.endGroup();

  /* Load the preferences page */
  preferences->addPreferencePage(new GeneralPreferences());
  // preferences->addPreferencePage(new RunGingaConfig());

  // preferences->addPreferencePage(new ImportBasePreferences());

  /* Load PreferencesPages from Plugins */
  QList<IPluginFactory*> list =
      PluginControl::getInstance()->getLoadedPlugins();

  IPluginFactory *currentFactory;
  foreach(currentFactory, list)
  {
    preferences->addPreferencePage(currentFactory);
  }
}

QString ComposerMainWindow::promptChooseExtDirectory()
{
  QMessageBox mBox;

  mBox.setText(tr("The Extension Directory is not set"));
  mBox.setInformativeText(tr("Do you want to try the default"
                             "directory (%1)?").arg(QDir::homePath()));
  mBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  mBox.setDefaultButton(QMessageBox::Yes);
  mBox.setIcon(QMessageBox::Question);
  if (mBox.exec() == QMessageBox::No)
  {
    QString dirName = QFileDialog::getExistingDirectory( this,
                                                        tr("Select Directory"),
                                                       getLastFileDialogPath(),
                                                    QFileDialog::ShowDirsOnly);
    return dirName;
  }
  else
  {
    return "";
  }
}

void ComposerMainWindow::readSettings()
{
  GlobalSettings settings;

  settings.beginGroup("mainwindow");
  restoreGeometry(settings.value("geometry").toByteArray());
  restoreState(settings.value("windowState").toByteArray());
  settings.endGroup();

  QApplication::processEvents();

  settings.beginGroup("openfiles");
  QStringList openfiles = settings.value("openfiles").toStringList();
  settings.endGroup();

  openProjects(openfiles);

  updateRecentProjectsWidgets();
}

void ComposerMainWindow::openProjects(const QStringList &projects)
{
  qDebug() << "Openning files:" << projects;
  for(int i = 0; i < projects.size(); i++)
  {
    QString src = projects.at(i);
#ifdef WIN32
    src = src.replace(QDir::separator(), "/");
#endif

    QFile file(src);
    bool openCurrentFile = true;

    if(!file.exists())
    {
      int resp =
          QMessageBox::question(this,
                                tr("File does not exists anymore."),
                                tr("The File %1 does not exists, but "
                                   "the last time you have closed NCL"
                                   " Composer this files was open. "
                                   "Do you want to create this file "
                                   "again?").arg(src),
                                QMessageBox::Yes | QMessageBox::No,
                                QMessageBox::No);

      if(resp != QMessageBox::Yes) openCurrentFile = false;
    }

    if (openCurrentFile)
    {
      checkTemporaryFileLastModified(src);
      ProjectControl::getInstance()->launchProject(src);
    }
  }

  updateRecentProjectsWidgets();
}

void ComposerMainWindow::initGUI()
{
#ifndef Q_OS_MAC
  setWindowIcon(QIcon(":/mainwindow/icon"));
#endif
  setWindowTitle(tr("NCL Composer"));
  _tabProjects = new QTabWidget(0);

  ui->frame->layout()->addWidget(_tabProjects);

  //    _tabProjects->setMovable(true);
  _tabProjects->setTabsClosable(true);

  /* _tbLanguageDropList = new QToolButton(this);
  _tbLanguageDropList->setIcon(QIcon(":/mainwindow/language"));
  _tbLanguageDropList->setToolTip(tr("Change your current language"));
  _tbLanguageDropList->setPopupMode(QToolButton::InstantPopup); */

  connect( _tabProjects, SIGNAL(tabCloseRequested(int)),
          this, SLOT(tabClosed(int)), Qt::DirectConnection);

  connect(_tabProjects, SIGNAL(currentChanged(int)),
          this, SLOT(currentTabChanged(int)));

  createActions();
  createMenus();
  createAboutPlugins();
// createLanguageMenu();

  preferences = new PreferencesDialog(this);
  perspectiveManager = new PerspectiveManager(this);
  pluginDetailsDialog = new PluginDetailsDialog(aboutPluginsDialog);

  connect(ui->action_RunNCL, SIGNAL(triggered()), this, SLOT(runNCL()));
  connect(ui->action_StopNCL, SIGNAL(triggered()), this, SLOT(stopNCL()));
  connect(ui->action_runPassiveNCL, SIGNAL(triggered()), this, SLOT(functionRunPassive()));
  connect(ui->action_runActiveNCL, SIGNAL(triggered()), this, SLOT(functionRunActive()));

  ui->action_RunNCL->setEnabled(true);

// UNDO/REDO
  // connect(ui->action_Undo, SIGNAL(triggered()), this, SLOT(undo()));
  // connect(ui->action_Redo, SIGNAL(triggered()), this, SLOT(redo()));

  welcomeWidget = new WelcomeWidget();
  _tabProjects->addTab(welcomeWidget, tr("Welcome"));
  _tabProjects->setTabIcon(0, QIcon());

  QTabBar *tabBar = _tabProjects->findChild<QTabBar *>();
  tabBar->setTabButton(0, QTabBar::RightSide, 0);
  tabBar->setTabButton(0, QTabBar::LeftSide, 0);

  connect(welcomeWidget, SIGNAL(userPressedOpenProject()),
          this, SLOT(openProject()));

  connect(welcomeWidget, SIGNAL(userPressedNewProject()),
          this, SLOT(launchProjectWizard()));

  connect(welcomeWidget, SIGNAL(userPressedSeeInstalledPlugins()),
          this, SLOT(aboutPlugins()));


  //Task Progress Bar
#ifdef WITH_LIBSSH2
  _taskProgressBar = new QProgressDialog(this);
  _taskProgressBar->setWindowTitle(tr("Copy content to Ginga VM."));
  _taskProgressBar->setModal(true);

  // start _taskProgressBar
  connect( &runRemoteGingaVMAction, SIGNAL(startTask()),
           _taskProgressBar, SLOT(show()) );

  connect( &runRemoteGingaVMAction, SIGNAL(taskDescription(QString)),
           _taskProgressBar, SLOT(setLabelText(QString)) );

  connect( &runRemoteGingaVMAction, SIGNAL(taskMaximumValue(int)),
           _taskProgressBar, SLOT(setMaximum(int)) );

  connect( &runRemoteGingaVMAction, SIGNAL(taskValue(int)),
           _taskProgressBar, SLOT(setValue(int)) );

  connect(&runRemoteGingaVMAction, SIGNAL(copyFinished()),
          _taskProgressBar, SLOT(hide()) );

  connect( &runRemoteGingaVMAction, SIGNAL(finished()),
           _taskProgressBar, SLOT(hide()));

  connect( _taskProgressBar, SIGNAL(canceled()),
           &runRemoteGingaVMAction, SLOT(stopExecution()),
           Qt::DirectConnection );

  disconnect( _taskProgressBar, SIGNAL(canceled()),
              _taskProgressBar, SLOT(cancel()) );

  connect( &runRemoteGingaVMAction, SIGNAL(finished()),
           _taskProgressBar, SLOT(hide()) );

// This shows the taskBar inside the toolBar. In the future, this can
//   _taskProgressBarAction = ui->toolBar->insertWidget( ui->action_Save,
//                                                       _taskProgressBar);
// taskProgressBarAction->setVisible(false);
#endif

}

void ComposerMainWindow::keyPressEvent(QKeyEvent *event)
{
  if(event->modifiers() == Qt::ControlModifier)
  {
    if(event->key() == Qt::Key_Z)
    {
        undo();
        event->accept();
    }
    else if(event->key() == Qt::Key_Y)
    {
        redo();
        event->accept();
    }
    else if ( (event->key() >= Qt::Key_1)
              && (event->key() <= Qt::Key_9) )
    {
      GlobalSettings settings;
      settings.beginGroup("pluginslayout");
      QStringList perspectives = settings.allKeys();
      if ( (event->key() - Qt::Key_1) < perspectives.count() )
      {
        restorePerspective( perspectives.at(event->key() - Qt::Key_1) );
      }

      settings.endGroup();
      event->accept();
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
void ComposerMainWindow::addPluginWidget( IPluginFactory *fac,
                                          IPlugin *plugin,
                                          Project *project )
{
  QToolWindowManager *w;
  QString location = project->getLocation();
  QString projectId = project->getAttribute("id");

#ifdef USE_MDI
  QMdiArea *mdiArea;
#endif
  if (_projectsWidgets.contains(location))
  {
    w = _projectsWidgets[location];
#ifdef USE_MDI
    mdiArea = (QMdiArea *)w->centralWidget();
    mdiArea->setBackground(QBrush(QColor("#FFFFFF")));
#endif
  } else {
    w = new QToolWindowManager(_tabProjects);
#ifdef USE_MDI
    mdiArea = new QMdiArea;
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    w->setCentralWidget(mdiArea);
#else
    // w->setDockNestingEnabled(true);
    // w->setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::West);
#endif
    int index = _tabProjects->addTab(w, projectId);
    updateTabWithProject(index, location);
    _projectsWidgets[location] = w;
  }

  QWidget *pW = plugin->getWidget();
  pW->setObjectName(fac->id());

  if (pW != NULL)
  {
    #ifdef USE_MDI
      mdiArea->addSubWindow(pW);
      pW->setWindowModified(true);
        pW->setWindowTitle(projectId + " - " + fac->metadata().value("name").toString());
      pW->show();
    #else
      // ClickableQDockWidget *dock;
      //dock = new ClickableQDockWidget(fac->metadata().value("name").toString());
      pW->setWindowTitle(fac->metadata().value("name").toString());

      w->addToolWindow(pW, QToolWindowManager::EmptySpaceArea);

      // Add updateFromModel and close button
      QFrame *btn_group = new QFrame(pW);

      // \todo Move this to *.css file
      btn_group->setStyleSheet("QToolButton { border: none; } \
                                QToolButton::hover {  background: #aaaaaa; } \
                                QToolButton:pressed { \
                                  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, \
                                              top: 0 #dcebfd, \
                                              stop: 1 #c2dcfd); \
                                }");

      QToolButton *btn_UpdateFromModel = new QToolButton(btn_group);
      btn_UpdateFromModel->setIconSize(QSize(12, 12));

      QAction *action_updateFromModel = new QAction(pW);
      action_updateFromModel->setIcon(QIcon(":/mainwindow/refreshplugin"));
      action_updateFromModel->setToolTip(tr("Reload View's Model"));
      btn_UpdateFromModel->setDefaultAction(action_updateFromModel);

      QShortcut *shortcut = new QShortcut(
                                  QKeySequence(tr("F5", "Update from Model")),
                                  pW);
      shortcut->setContext(Qt::WidgetWithChildrenShortcut);

      connect(action_updateFromModel, SIGNAL(triggered()), plugin, SLOT(updateFromModel()));
      connect(shortcut, SIGNAL(activated()), plugin, SLOT(updateFromModel()));

      QToolButton *btn_close = new QToolButton(btn_group);
      QAction* close_view_action = new QAction(QIcon(":/mainwindow/closeplugin"), tr("Close view"), btn_group);
      btn_close->setIconSize(QSize(12, 12));
      btn_close->setDefaultAction(close_view_action);
      close_view_action->setData(w->toolWindows().size()-1);
      connect(close_view_action, SIGNAL(triggered(bool)), this, SLOT(pluginWidgetViewToggled(bool)));

      QHBoxLayout *layout = new QHBoxLayout(btn_group);
      layout->addStretch();
      layout->addWidget(btn_UpdateFromModel);
      layout->addWidget(btn_close);
      layout->setContentsMargins(0,0,0,0);
      layout->setSpacing(0);
      layout->setMargin(0);
      btn_group->setLayout(layout);

      w->setTabButton(pW, QTabBar::RightSide, btn_group);
  }
#endif
}

void ComposerMainWindow::updateDockStyle(QDockWidget *dock, bool selected)
{
  qWarning() << "ComposerMainWindows::updateDockStyle ( " << dock << selected << ")";

  QList <QTabBar*> tabBars = this->findChildren <QTabBar *>();

  // \todo We can improve this foreach
  foreach (QTabBar *tabBar, tabBars)
  {
    for(int index = 0; index < tabBar->count(); index++)
    {
      QVariant tmp = tabBar->tabData(index);
      QDockWidget * dockWidget = reinterpret_cast<QDockWidget *>(tmp.toULongLong());
      if(dockWidget == dock)
      {
        if(!tabBar->property("activePlugin").isValid())
        {
          tabBar->setProperty("activePlugin", "false");
          tabBar->setStyleSheet(styleSheet());
        }

        bool activePlugin = tabBar->property("activePlugin").toBool();
        if(selected)
        {
          if(!activePlugin)
          {
            tabBar->setProperty("activePlugin", "true");
            tabBar->setStyleSheet(styleSheet());
          }
        }
        else
        {
          if(activePlugin)
          {
            tabBar->setProperty("activePlugin", "false");
            tabBar->setStyleSheet(styleSheet());
          }
        }
      }
    }
  }

  QFrame *titleBar = (QFrame*) dock->titleBarWidget();
  if(!titleBar->property("activePluginTitleBar").isValid())
  {
    titleBar->setProperty("activePluginTitleBar", "false");
    dock->setProperty("activePluginBorder", "false");
    dock->setStyleSheet(styleSheet());
  }

  bool activePluginTitleBar = titleBar->property("activePluginTitleBar").toBool();
  if(!selected)
  {
    if(activePluginTitleBar)
    {
      titleBar->setProperty("activePluginTitleBar", "false");
      dock->setProperty("activePluginBorder", "false");
      dock->setStyleSheet(styleSheet());
    }
  }
  else
  {
    if(!activePluginTitleBar)
    {
      titleBar->setProperty("activePluginTitleBar", "true");
      dock->setProperty("activePluginBorder", "true");
      dock->setStyleSheet(styleSheet());
    }
  }
}

/*!
 * \brief
 * \param index
 */
void ComposerMainWindow::tabClosed(int index)
{
  if(index == 0)
    return; // Do nothing

  QString location = _tabProjects->tabToolTip(index);
  Project *project = ProjectControl::getInstance()->getOpenProject(location);

  if( project != NULL &&
      project->isDirty() )
  {
    int ret = QMessageBox::warning(this, project->getAttribute("id"),
                                   tr("The project has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::No,
                                   QMessageBox::Cancel|QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
      saveCurrentProject();
    else if (ret == QMessageBox::Cancel)
      return;
  }

  ProjectControl::getInstance()->closeProject(location);

  //Remove temporary file
  removeTemporaryFile(location);

  if(_projectsWidgets.contains(location))
  {
    QToolWindowManager *w = _projectsWidgets[location];

    // Delete QMainWindow
    if (w)
    {
      w->close();
      w->deleteLater();
    }

    _projectsWidgets.remove(location);
  }  
}

/*!
 * \brief
 */
void ComposerMainWindow::closeCurrentTab()
{
  if(_tabProjects->currentIndex())
  {
    int currentIndex = _tabProjects->currentIndex();
    tabClosed(currentIndex);
  }
}

/*!
 * \brief
 */
void ComposerMainWindow::closeAllFiles()
{
  while(_tabProjects->count() > 1)
  {
    // TODO: Ask to save all projects here!
//    for (int i = 0; i < _tabProjects->count(); i++)
//    {
//      QString location = _tabProjects->tabToolTip(i);
//      Project *project = ProjectControl::getInstance()->getOpenProject(location);
//      if (project->isDirty())
//      {
//        qDebug() << "ask to save" << location;
//      }
//    }

    tabClosed(1);
    _tabProjects->removeTab(1);
  }
}

/*!
 * \brief Called when a new tab is open.
 *
 * \param location
 */
void ComposerMainWindow::onOpenProjectTab(QString location)
{
  if (!_projectsWidgets.contains(location)) return;
  QToolWindowManager *w = _projectsWidgets[location];
  _tabProjects->setCurrentWidget(w);
}

void ComposerMainWindow::createMenus()
{
#ifdef Q_OS_MAC
  ui->menubar->setNativeMenuBar(true);
#endif

#ifdef FV_GUI
  QAction *action_checkForUpdate = new QAction("Check for updates",this);
  connect (action_checkForUpdate, SIGNAL(triggered()),
           FvUpdater::sharedUpdater(), SLOT(CheckForUpdatesNotSilent()));
  ui->menu_Help->addAction(action_checkForUpdate);

#endif

  ui->menu_Edit->addAction(_editPreferencesAction);

  connect( ui->menu_View, SIGNAL(aboutToShow()),
           this, SLOT(updateViewMenu()) );

  connect( ui->action_CloseProject, SIGNAL(triggered()),
           this, SLOT(closeCurrentTab()) );

  connect( ui->action_CloseAll, SIGNAL(triggered()),
           this, SLOT(closeAllFiles()) );

  connect( ui->action_Save, SIGNAL(triggered()),
           this, SLOT(saveCurrentProject()) );

  connect( ui->action_SaveAs, SIGNAL(triggered()),
           this, SLOT(saveAsCurrentProject()) );

  connect ( ui->action_NewProject, SIGNAL(triggered()),
            this, SLOT(launchProjectWizard()) );

  /* _menuLanguage = new QMenu(0);
  _tbLanguageDropList->setMenu(_menuLanguage);
  ui->toolBar->addWidget(_tbLanguageDropList);*/

  QToolButton *button_Run = new QToolButton(0); //create a run_NCL button(It used to be created in design mode)
  _menuMultidevice = new QMenu(0); // assign a dropdown menu to the button
  _menuMultidevice->addAction(ui->action_runPassiveNCL);
  _menuMultidevice->addAction(ui->action_runActiveNCL);
  button_Run->setMenu(_menuMultidevice);
  button_Run->setDefaultAction(ui->action_RunNCL);
  button_Run->setPopupMode(QToolButton::MenuButtonPopup);
  ui->toolBar->addWidget(button_Run); //put button_run in toolbar
  ui->toolBar->addSeparator();

  ui->action_Main_toolbar->setChecked(ui->toolBar->isVisible());
  ui->action_Perspectives_toolbar->setChecked(ui->toolBar_Perspectives->isVisible());

  connect(ui->action_Main_toolbar, SIGNAL(triggered(bool)), ui->toolBar, SLOT(setVisible(bool)));
  connect(ui->action_Perspectives_toolbar, SIGNAL(triggered(bool)), ui->toolBar_Perspectives, SLOT(setVisible(bool)));

  _tabProjects->setCornerWidget(ui->menubar, Qt::TopLeftCorner);


  // updateMenuLanguages();
  updateMenuPerspectives();
}

void ComposerMainWindow::createAboutPlugins()
{
  aboutPluginsDialog = new QDialog(this);
  aboutPluginsDialog->setWindowTitle(tr("Installed Plugins"));

#ifdef SHOW_PROFILES
  profilesExt = new QListWidget(aboutPluginsDialog);
  profilesExt->setAlternatingRowColors(true);
#endif

  /* This should be a new Widget and change some code for there */
  pluginsExt = new QTreeWidget(aboutPluginsDialog);
  pluginsExt->setAlternatingRowColors(true);

  connect(pluginsExt, SIGNAL(itemSelectionChanged()),
          this, SLOT(selectedAboutCurrentPluginFactory()));

  QStringList header;
  header << tr("Name") << tr("Load") << tr("Version") << tr("Vendor");
  pluginsExt->setHeaderLabels(header);

  QDialogButtonBox *bOk = new QDialogButtonBox(QDialogButtonBox::Ok |
                                               QDialogButtonBox::Close,
                                               Qt::Horizontal,
                                               aboutPluginsDialog);

  detailsButton = bOk->button(QDialogButtonBox::Ok);
  detailsButton->setText(tr("Details"));
  detailsButton->setIcon(QIcon());
  detailsButton->setEnabled(false);

  connect( bOk, SIGNAL(rejected()),
           aboutPluginsDialog, SLOT(close()) );

  connect( detailsButton, SIGNAL(pressed()),
           this, SLOT(showPluginDetails()) );

  QGridLayout *gLayout = new QGridLayout(aboutPluginsDialog);
  gLayout->addWidget(new QLabel(tr("The <b>Composer</b> is an IDE for"
                                   " Declarative Multimedia languages."),
                                aboutPluginsDialog));

#ifdef SHOW_PROFILES
  gLayout->addWidget(new QLabel(tr("<b>Installed Language Profiles</b>"),
                                aboutPluginsDialog));
  gLayout->addWidget(profilesExt);
#endif
  gLayout->addWidget(new QLabel(tr("<b>Installed Plug-ins</b>")));
  gLayout->addWidget(pluginsExt);
  gLayout->addWidget(bOk);
  aboutPluginsDialog->setLayout(gLayout);

  aboutPluginsDialog->setModal(true);

  connect( aboutPluginsDialog, SIGNAL(finished(int)),
           this, SLOT(saveLoadPluginData(int)) );
}

/*!
 * \brief Shows the about dialog.
 */
void ComposerMainWindow::about()
{
  AboutDialog dialog(this);
  dialog.exec();
}

/*!
 * \brief Shows the about plugins dialog.
 */
void ComposerMainWindow::aboutPlugins()
{
  QList<IPluginFactory*>::iterator it;
  QList<IPluginFactory*> pList = PluginControl::getInstance()->
      getLoadedPlugins();
  pluginsExt->clear();

  //search for categories
  QTreeWidgetItem *treeWidgetItem;
  QMap <QString, QTreeWidgetItem*> categories;
  for (it = pList.begin(); it != pList.end(); it++) {
    IPluginFactory *pF = *it;
    QString category = pF->metadata().value("category").toString();

    if(!categories.contains(category)){
      treeWidgetItem = new QTreeWidgetItem(pluginsExt);
      categories.insert(category, treeWidgetItem);
      treeWidgetItem->setText(0, category);
      treeWidgetItem->setTextColor(0, QColor("#0000FF"));
    }
  }

  treeWidgetItem2plFactory.clear();
  for (it = pList.begin(); it != pList.end(); it++) {
    IPluginFactory *pF = *it;
#if QT_VERSION < 0x050000
    treeWidgetItem = new QTreeWidgetItem(categories.value(pF->category()));
#else
    QString category = pF->metadata().value("category").toString();
    treeWidgetItem = new QTreeWidgetItem ( categories.value(category) );
#endif
    treeWidgetItem2plFactory.insert(treeWidgetItem, pF);
#if QT_VERSION < 0x050000
    treeWidgetItem->setText(0, pF->name());
#else
    treeWidgetItem->setText(0, pF->metadata().value("name").toString());
#endif

    // Set checked (or not) based on the settings
    GlobalSettings settings;
    settings.beginGroup("loadPlugins");
    if(!settings.contains(pF->id()) || settings.value(pF->id()).toBool())
      treeWidgetItem->setCheckState(1, Qt::Checked);
    else
      treeWidgetItem->setCheckState(1, Qt::Unchecked);

    settings.endGroup();
#if QT_VERSION < 0x050000
    treeWidgetItem->setText(2, pF->version());
    treeWidgetItem->setText(3, pF->vendor());
#else
    treeWidgetItem->setText(2, pF->metadata().value("version").toString());
    treeWidgetItem->setText(3, pF->metadata().value("vendor").toString());
#endif
  }

  pluginsExt->expandAll();

  pluginsExt->setColumnWidth(0, 150);
  pluginsExt->resizeColumnToContents(1);
  pluginsExt->resizeColumnToContents(2);
  pluginsExt->resizeColumnToContents(3);

  /* PROFILE LANGUAGE */
#ifdef SHOW_PROFILES
  QList<ILanguageProfile*>::iterator itL;
  QList<ILanguageProfile*> lList = LanguageControl::getInstance()->
      getLoadedProfiles();
  profilesExt->clear();

  for(itL = lList.begin(); itL != lList.end(); itL++)
  {
    ILanguageProfile *lg = *itL;
    profilesExt->addItem(new QListWidgetItem(lg->getProfileName()));
  }
#endif

  detailsButton->setEnabled(false);
  aboutPluginsDialog->show();
}

/*!
 * \brief
 *
 * \param QString
 */
void ComposerMainWindow::errorDialog(QString message)
{
  //QMessageBox::warning(this,tr("Error!"),message);
  qWarning() << message;
}

void ComposerMainWindow::createActions() {

  connect( ui->action_About, SIGNAL(triggered()), this, SLOT(about()));

  connect( ui->action_AboutPlugins, SIGNAL(triggered()),
           this, SLOT(aboutPlugins()) );

#ifdef WITH_SERV_PUB
  ui->actionPublish->setEnabled(true);
#else
  ui->actionPublish->setEnabled(false);
#endif

  _editPreferencesAction = new QAction(tr("&Preferences"), this);
  _editPreferencesAction->setStatusTip(tr("Edit preferences"));
  connect (_editPreferencesAction, SIGNAL(triggered()), this,
           SLOT(showEditPreferencesDialog()));

  connect(ui->action_Preferences, SIGNAL(triggered()),
          this, SLOT(showEditPreferencesDialog()));

  connect( ui->action_Exit, SIGNAL(triggered()),
           this, SLOT(close()) );

  connect ( ui->action_Fullscreen, SIGNAL(triggered()),
            this, SLOT(showCurrentWidgetFullScreen()) );

  connect( ui->action_Save_current_perspective, SIGNAL(triggered()),
           this, SLOT(saveCurrentGeometryAsPerspective()));

  connect( ui->action_Restore_a_perspective, SIGNAL(triggered()),
           this, SLOT(restorePerspective()) );

  QWidget* spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  //Add a separator to the toolbar. All actions after preferences (including
  // it will be aligned in the bottom.
  ui->toolBar->insertWidget(ui->action_Preferences, spacer);

  connect (ui->action_OpenProject, SIGNAL(triggered()),
           this, SLOT(openProject()));

  connect (ui->action_ImportFromExistingNCL, SIGNAL(triggered()),
           this, SLOT(importFromDocument()));

  connect (ui->action_GoToClubeNCLWebsite, SIGNAL(triggered()),
           this, SLOT(gotoNCLClubWebsite()));

  connect (ui->action_Help, SIGNAL(triggered()), this, SLOT(showHelp()));

}

/*!
 * \brief
 */
void ComposerMainWindow::showCurrentWidgetFullScreen()
{
  _tabProjects->addAction(ui->action_Fullscreen);

  if(!_tabProjects->isFullScreen())
  {
    _tabProjects->setWindowFlags(Qt::Window);
    _tabProjects->showFullScreen();
  }
  else
  {
    _tabProjects->setParent(ui->frame, Qt::Widget);
    _tabProjects->show();
  }
}

/*!
 * \brief
 */
void ComposerMainWindow::updateViewMenu ()
{
  //Update menu Views.
  ui->menu_Views->clear();
  if(_tabProjects->count()) //see if there is any open document
  {
    QToolWindowManager *toolWidgetManager =
        dynamic_cast<QToolWindowManager *> (_tabProjects->currentWidget());

    if(toolWidgetManager != 0)
    {
      QWidgetList toolWindows = toolWidgetManager->toolWindows();
      for (int i = 0; i < toolWindows.length(); i++)
      {
        QWidget *w = toolWindows.at(i);
        QAction *act = ui->menu_Views->addAction(w->windowTitle());

        act->setData(i);
        act->setCheckable(true);
        qDebug() << w->parent();
        act->setChecked(w->parent() != 0);

        connect( act, SIGNAL(triggered(bool)),
                 this, SLOT(pluginWidgetViewToggled(bool)) );
      }
    }
  }

  if(ui->menu_Views->isEmpty())
    ui->menu_Views->setEnabled(false);
  else
    ui->menu_Views->setEnabled(true);
}

void ComposerMainWindow::closeEvent(QCloseEvent *event)
{

  /** Save any dirty project. \todo This must be a function. */
  for(int index = 1; index < _tabProjects->count(); index++)
  {
    QString location = _tabProjects->tabToolTip(index);
    Project *project = ProjectControl::getInstance()->getOpenProject(location);

    qDebug() << location << project;
    if(project != NULL && project->isDirty())
    {
      _tabProjects->setCurrentIndex(index);
      int ret = QMessageBox::warning(this, project->getAttribute("id"),
                                     tr("The project %1 has been modified.\n"
                                        "Do you want to save your changes?").
                                     arg(location),
                                     QMessageBox::Yes | QMessageBox::Default,
                                     QMessageBox::No,
                                     QMessageBox::Cancel|QMessageBox::Escape);
      if (ret == QMessageBox::Yes)
        saveCurrentProject();
      else if (ret == QMessageBox::Cancel)
      {
        event->ignore();
        return;
      }
    }
  }

  GlobalSettings settings;
  settings.beginGroup("extensions");
  settings.setValue("path", extensions_paths);
  settings.endGroup();

  settings.beginGroup("mainwindow");
  settings.setValue("geometry", saveGeometry());
  settings.setValue("windowState", saveState());
  settings.endGroup();

  QStringList openfiles;
  QString key;
  foreach (key, _projectsWidgets.keys())
    openfiles << key;

  settings.beginGroup("openfiles");
  if(openfiles.size())
  {
    settings.setValue("openfiles", openfiles);
  }
  else {
    /* If there aren't any openfile, remove this settings, otherwise it will
       try to load the current path */
    settings.remove("openfiles");
  }
  settings.endGroup();
  settings.sync();
  cleanUp();

  qApp->quit(); // close the application
}

void ComposerMainWindow::cleanUp()
{
  LanguageControl::releaseInstance();
  ProjectControl::releaseInstance();
  PluginControl::releaseInstance();
}

/*!
 * \brief TODO
 */
void ComposerMainWindow::showEditPreferencesDialog()
{
  preferences->show();
}

/*!
 * \brief
 */
void ComposerMainWindow::startOpenProject(QString project)
{
  (void) project;

  this->setCursor(QCursor(Qt::WaitCursor));
  update();
}

void ComposerMainWindow::endOpenProject(QString project)
{
  this->setCursor(QCursor(Qt::ArrowCursor));

  onOpenProjectTab(project);

  GlobalSettings settings;
  if(settings.contains("default_perspective"))
  {
    QString defaultPerspective =
        settings.value("default_perspective").toString();

    // Ok! There is a default perspective, so use it
    restorePerspective(defaultPerspective);

    update();
  }
  else
  {
    /*
    // There is no a default perspective, so let use the system default.
    QToolWindowManager *window = _projectsWidgets[project];
    QList <QDockWidget *> docks = window->findChildren <QDockWidget* >(QRegExp("br.*"));
    QMap <QString, QDockWidget*> dockByPluginId;
    QList <QDockWidget *> leftDocks, rightDocks, bottomDocks;
    for(int i = 0; i < docks.size(); i++)
    {
      dockByPluginId[docks[i]->objectName()] = docks[i];
      if(docks[i]->objectName() == "br.puc-rio.telemidia.OutlineView" ||
              docks[i]->objectName() == "br.puc-rio.telemidia.PropertiesView")
      {
        leftDocks.append(docks[i]);
      }
      else if(docks[i]->objectName() == "br.puc-rio.telemidia.DebugConsole" ||
              docks[i]->objectName() == "br.ufma.deinf.laws.validator" ||
              docks[i]->objectName() == "br.puc-rio.telemidia.RulesView")
      {
        bottomDocks.append(docks[i]);
      }
      else
        rightDocks.append(docks[i]);
    }

    for(int i = leftDocks.size()-1; i >= 0; i--)
    {
      leftDocks[i]->widget()->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
      leftDocks[i]->widget()->setMinimumWidth(200);
      leftDocks[i]->widget()->setMinimumHeight(100);
      if(!i)
        window->addDockWidget(Qt::LeftDockWidgetArea, leftDocks[i]);
      else
        window->addDockWidget(Qt::LeftDockWidgetArea, leftDocks[i], Qt::Vertical);

      window->adjustSize();
    }

    for(int i = 0; i < rightDocks.size(); i++)
    {
      rightDocks[i]->widget()->setSizePolicy(QSizePolicy::Expanding,
                                             QSizePolicy::Expanding);

      if(!i)
        window->addDockWidget(Qt::RightDockWidgetArea, rightDocks[i]);
      else
        window->tabifyDockWidget(rightDocks[0], rightDocks[i]);
    }

    for(int i = 0; i < bottomDocks.size(); i++)
    {
      bottomDocks[i]->widget()->setSizePolicy(QSizePolicy::MinimumExpanding,
                                              QSizePolicy::MinimumExpanding);
      bottomDocks[i]->widget()->setMaximumHeight(200);
      bottomDocks[i]->widget()->setMinimumHeight(100);
      if(!i)
        window->addDockWidget(Qt::RightDockWidgetArea, bottomDocks[i], Qt::Vertical);
      else
        window->tabifyDockWidget(bottomDocks[0], bottomDocks[i]);
    }*/
  }

  this->updateGeometry();
}

/*!
 * \brief Save the current project.
 */
void ComposerMainWindow::saveCurrentProject()
{
  int index = _tabProjects->currentIndex();
  bool saveAlsoNCLDocument = true;

  if(index != 0)
  {
    QString location = _tabProjects->tabToolTip(index);
    Project *project = ProjectControl::getInstance()->getOpenProject(location);

    PluginControl::getInstance()->savePluginsData(project);
    ProjectControl::getInstance()->saveProject(location);
    ui->action_Save->setEnabled(false);

    if(saveAlsoNCLDocument)
    {
      QString nclfilepath = location.mid(0, location.lastIndexOf(".")) + ".ncl";
      QFile file(nclfilepath);
      if(file.open(QFile::WriteOnly | QIODevice::Truncate))
      {
        // Write FILE!!
        if(project->getChildren().size())
          file.write(project->getChildren().at(0)->toString(0, false).toLatin1());

        file.close();
      }
    }
  }
  else
  {
    QMessageBox box(QMessageBox::Warning,
                    tr("Information"),
                    tr("There aren't a project to be saved."),
                    QMessageBox::Ok
                    );
    box.exec();
  }
}

/*!
 * \brief Save the current project.
 */
void ComposerMainWindow::saveAsCurrentProject()
{
  int index = _tabProjects->currentIndex();
  bool saveAlsoNCLDocument = true;

  if(index != 0)
  {
    QString location = _tabProjects->tabToolTip(index);

    QString destFileName = QFileDialog::getSaveFileName(
          this,
          tr("Save as NCL Composer Project"),
          getLastFileDialogPath(),
          tr("NCL Composer Projects (*.cpr)") );

    if(!destFileName.isNull() && !destFileName.isEmpty())
    {
      updateLastFileDialogPath(destFileName);

      if(!destFileName.endsWith(".cpr"))
        destFileName  = destFileName + ".cpr";

      /* Move the location of the current project to destFileName */
      if (location != destFileName)
        ProjectControl::getInstance()->moveProject(location, destFileName);

      /* Get the project */
      Project *project =
          ProjectControl::getInstance()->getOpenProject(destFileName);

      PluginControl::getInstance()->savePluginsData(project);
      ProjectControl::getInstance()->saveProject(destFileName);

      /* Update Tab Text and Index */
      updateTabWithProject(index, destFileName);

      ui->action_Save->setEnabled(false);

      if(saveAlsoNCLDocument)
      {
        QString nclfilepath =
            location.mid(0, destFileName.lastIndexOf(".")) + ".ncl";

        QFile file(nclfilepath);
        if(file.open(QFile::WriteOnly | QIODevice::Truncate))
        {
          // Write FILE!!
          if(project->getChildren().size())
            file.write(project->getChildren().at(0)->toString(0, false).toLatin1());

          file.close();
        }
      }

      addToRecentProjects(destFileName);
    }
  }
  else
  {
    QMessageBox box(QMessageBox::Warning,
                    tr("Information"),
                    tr("There aren't a project to be saved."),
                    QMessageBox::Ok
                    );
    box.exec();
  }
}

void ComposerMainWindow::saveCurrentGeometryAsPerspective()
{
  if(_tabProjects->count()) // If there is a document open
  {
    perspectiveManager->setBehavior(PERSPEC_SAVE);
    if(perspectiveManager->exec())
    {
      savePerspective(perspectiveManager->getSelectedName());
      saveDefaultPerspective(perspectiveManager->getDefaultPerspective());
    }
  }
  else
  {
    QMessageBox box( QMessageBox::Warning,
                     tr("Information"),
                     tr("There aren't a layout open to be saved."),
                     QMessageBox::Ok );
    box.exec();
  }

  /* Update the elements in MENU PERSPECTIVE*/
  updateMenuPerspectives();
}

void ComposerMainWindow::restorePerspective()
{
  perspectiveManager->setBehavior(PERSPEC_LOAD);
  if(perspectiveManager->exec())
  {
    restorePerspective(perspectiveManager->getSelectedName());
  }

  /* Update the elements in MENU PERSPECTIVE*/
  updateMenuPerspectives();
}

void ComposerMainWindow::savePerspective (QString layoutName)
{
  if(_tabProjects->count()) //see if there is any open document
  {
    QString location = _tabProjects->tabToolTip(_tabProjects->currentIndex());

    QToolWindowManager *window = _projectsWidgets[location];
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");

    settings.beginGroup("pluginslayout");
    settings.setValue(layoutName, window->saveState());
    settings.endGroup();
  }
}

void ComposerMainWindow::saveDefaultPerspective(QString defaultPerspectiveName)
{
  GlobalSettings settings;
  settings.setValue("default_perspective", defaultPerspectiveName);
}

void ComposerMainWindow::restorePerspective(QString layoutName)
{
  if(_tabProjects->count() > 1) // see if there is any open project
  {
    QString location = _tabProjects->tabToolTip(_tabProjects->currentIndex());

    QToolWindowManager *window = _projectsWidgets[location];

    if(window != NULL)
    {
      GlobalSettings settings;
      settings.beginGroup("pluginslayout");
  #ifndef USE_MDI
      window->restoreState(settings.value(layoutName));
  #endif
      settings.endGroup();
    }

  }
}

/*!
 * \brief Run the current open Project.
 */
void ComposerMainWindow::runNCL()
{
  // check if there is other instance already running
  if(localGingaProcess.state() == QProcess::Running
#ifdef WITH_LIBSSH2
        || runRemoteGingaVMThread.isRunning()
#endif
        )
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning( NULL, tr("Warning!"),
                                  tr("You already have an NCL application "
                                     "running. Please, stop it before you start "
                                     "a new one."),
                                  QMessageBox::Ok);
    Q_UNUSED(reply)

    return;
  }

  ui->action_RunNCL->setEnabled(false);

  // There is no other instance running, so let's run
  bool runRemote = false;
  GlobalSettings settings;
  settings.beginGroup("runginga");
  if(settings.contains("run_remote") && settings.value("run_remote").toBool())
    runRemote = true;
  else
    runRemote = false;
  settings.endGroup();

  if(runRemote)
    copyOnRemoteGingaVM();
  else
    runOnLocalGinga();

  ui->action_StopNCL->setEnabled(true);
}

void ComposerMainWindow::runOnLocalGinga()
{
  GlobalSettings settings;
  QString command, args;
  settings.beginGroup("runginga");
  command = settings.value("local_ginga_cmd").toString();
  args = settings.value("local_ginga_args").toString();
  settings.endGroup();

  // \todo: Ask to Save current project before send it to Ginga VM.

  QString location = _tabProjects->tabToolTip(_tabProjects->currentIndex());

  if(location.isEmpty())
  {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(this, tr("Warning!"),
                                 tr("There aren't a current NCL project."),
                                 QMessageBox::Ok);
    Q_UNUSED(reply)
    return;
  }

  Project *project = ProjectControl::getInstance()->getOpenProject(location);
  QString nclpath = location.mid(0, location.lastIndexOf("/")) +  "/tmp.ncl";
  qDebug() << "Running NCL File: " << nclpath;

  QFile file(nclpath);
  if(file.open(QFile::WriteOnly | QIODevice::Truncate))
  {
    /* Write FILE!! */
    if(project->getChildren().size())
      file.write(project->getChildren().at(0)->toString(0, false).toLatin1());

    file.close();

    /* PARAMETERS */
    //\todo Other parameters
    QStringList args_list = Utilities::splitParams(args);
    args_list.replaceInStrings("${nclpath}", nclpath);

    /* RUNNING GINGA */
    qDebug() << command << args_list;
    localGingaProcess.start(command, args_list);
    QByteArray result = localGingaProcess.readAll();
  }
  else
  {
    qWarning() << "Error trying to running NCL. Could not create : "
               << nclpath << " !";
  }
}

void ComposerMainWindow::functionRunPassive()
{
  int i;
  bool ok;
  GlobalSettings settings;
  settings.beginGroup("runginga");
  QString command = settings.value("local_ginga_cmd").toString();
  QString args = settings.value("local_ginga_passive_args").toString();
  settings.endGroup();

  /* PARAMETERS */
  //\todo Other parameters
  QStringList args_list = Utilities::splitParams(args);
  // args_list.replaceInStrings("${nclpath}", nclpath);

  int value = QInputDialog::getInt(
        this,
        tr("Multidevices"),//title
        tr("Passive"), //label
        1, //start
        1, //min
        5, //max
        1, //step
        &ok );

  if( ok )
  {
    qDebug() << command << args_list;
    for(i = 0; i < value; i++)
      QProcess::startDetached(command, args_list);
  }
}

void ComposerMainWindow::functionRunActive()
{
  int i;
  bool ok;
  GlobalSettings settings;
  settings.beginGroup("runginga");
  QString command = settings.value("local_ginga_cmd").toString();
  QString args = settings.value("local_ginga_active_args").toString();
  settings.endGroup();

  /* PARAMETERS */
  //\todo Other parameters
  QStringList args_list = Utilities::splitParams(args);
  // args_list.replaceInStrings("${nclpath}", nclpath);

  int value = QInputDialog::getInt(
        this,
        tr("Multidevices"),
        tr("Active"),
        1,
        1,
        5,
        1,
        &ok );

  if( ok )
  {
    qDebug() << command << args_list;
    for(i = 0; i < value; i++)
      QProcess::startDetached(command, args_list);
  }
}

void ComposerMainWindow::copyOnRemoteGingaVM(bool autoplay)
{

#ifdef WITH_LIBSSH2
  int currentTab = _tabProjects->currentIndex();
  if(currentTab != 0)
  {
    //Disable a future Run (while the current one does not finish)!!
    ui->action_RunNCL->setEnabled(false);

    QString location = _tabProjects->tabToolTip(currentTab);
    Project *currentProject = ProjectControl::getInstance()->
                                                      getOpenProject(location);

    if(currentProject->isDirty())
    {
      /*QMessageBox::StandardButton reply;
      reply = QMessageBox::warning(this, tr("Warning!"),
                                     tr("Your document is not saved. "
                                        "Do you want to save it now?"),
                                     QMessageBox::Yes, QMessageBox::No);*/

      int reply;
      reply = QMessageBox::warning(NULL, tr("Warning!"),
                                   tr("Your document is not saved."
                                      "Do you want to save it now?"),
                                   QMessageBox::Yes, QMessageBox::No);

      if(reply == QMessageBox::Yes)
      {
        // save the current project before send it to Ginga VM
        saveCurrentProject();
      }
    }

    runRemoteGingaVMAction.setCurrentProject(currentProject);
    runRemoteGingaVMAction.setAutoplay(autoplay);

    runRemoteGingaVMThread.start();
  }
  else
  {
    // There aren't a current project.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::warning(NULL, tr("Warning!"),
                                 tr("There aren't a current NCL project."),
                                 QMessageBox::Ok);
    Q_UNUSED(reply)
  }

#else
  Q_UNUSED(autoplay)

  QMessageBox::StandardButton reply;
  reply = QMessageBox::warning(NULL, tr("Warning!"),
                               tr("Your NCL Composer was not build with Remote "
                                  "Run support."),
                               QMessageBox::Ok);
  Q_UNUSED (reply)
#endif
}

void ComposerMainWindow::stopNCL()
{
  if(localGingaProcess.state() == QProcess::Running)
      localGingaProcess.close();

#ifdef WITH_LIBSSH2
  if(runRemoteGingaVMThread.isRunning())
    stopRemoteGingaVMAction.stopRunningApplication();
#endif

  updateRunActions();
}


bool ComposerMainWindow::isRunningNCL()
{
  // check if there is other instance already running
  if(localGingaProcess.state() == QProcess::Running
#ifdef WITH_LIBSSH2
        || runRemoteGingaVMThread.isRunning()
#endif
    )
  {
      return true;
  }
  return false;
}

void ComposerMainWindow::updateRunActions()
{
  if(isRunningNCL())
  {
    ui->action_RunNCL->setEnabled(false);
    ui->action_StopNCL->setEnabled(true);
  }
  else
  {
    ui->action_RunNCL->setEnabled(true);
    ui->action_StopNCL->setEnabled(false);
  }
}

void ComposerMainWindow::copyHasFinished()
{
  qDebug() << "ComposerMainWindow::runHasFinished()";
  updateRunActions();
}

void ComposerMainWindow::launchProjectWizard()
{
  NewProjectWizard wizard (this);
  wizard.setModal(true);
  wizard.exec();

  if(wizard.result() == QWizard::Accepted)
  {
    QString filename = wizard.getProjectFullPath();

    filename.replace("\\", "/"); //Force the use of "/"

    if( !filename.isNull() && !filename.isEmpty())
    {

      updateLastFileDialogPath(filename);

      if(!filename.endsWith(".cpr"))
        filename = filename + QString(".cpr");

      QFileInfo info(filename);
      // If the file already exist ask the user if it want to overwrite it
      if(info.exists())
      {
          if(QMessageBox::question(this, tr("File already exists!"),
                                    tr("The file \"%1\" already exists. Do you want overwrite it?").arg(filename),
                                    QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            return; // Do not overwrite!

          QFile::remove(filename); // Remove the OLD file (only if the user accept it)
      }

      // We dont need to check this, when creating a new project
      // checkTemporaryFileLastModified(filename);


      if(ProjectControl::getInstance()->launchProject(filename))
      {
        // After launch the project we will insert NCL, HEAD and BODY elements
        // by default
        Project *project = ProjectControl::getInstance()
                                               ->getOpenProject(filename);

        addDefaultStructureToProject( project,
                                      wizard.shouldCopyDefaultConnBase(),
                                      wizard.shouldCreateADefaultRegion());
      }
      else
      {
        // \todo a report to this problem (we should track the error message).
      }
    }
  }
}

void ComposerMainWindow::addDefaultStructureToProject(Project *project,
                                                 bool shouldCopyDefaultConnBase,
                                                 bool shouldCreateADefaultRegion,
                                                 bool save)
{
  const QString defaultNCLID = "myNCLDocID";
  const QString defaultBodyID = "myBodyID";
  const QString defaultConnBaseID = "connBaseId";
  const QString defaultRegionBaseID = "regionBase0";
  const QString defaultRegionID = "region0";

  QMap <QString, QString> nclAttrs, headAttrs, bodyAttrs;
  nclAttrs.insert("id", defaultNCLID);
  nclAttrs.insert("xmlns", "http://www.ncl.org.br/NCL3.0/EDTVProfile");

  Entity *nclEntity;
  MessageControl *msgControl = PluginControl::getInstance()
      ->getMessageControl(project);
  msgControl->anonymousAddEntity("ncl", project->getUniqueId(), nclAttrs);

  nclEntity = project->getEntitiesbyType("ncl").first();

  if(nclEntity != NULL)
  {
    QString nclEntityId = nclEntity->getUniqueId();
    msgControl->anonymousAddEntity("head", nclEntityId, headAttrs);

    bodyAttrs.insert("id", defaultBodyID);
    msgControl->anonymousAddEntity("body", nclEntityId, bodyAttrs);
  }

  // Copy the default connector
  if(shouldCopyDefaultConnBase)
  {
    GlobalSettings settings;
    settings.beginGroup("importBases");
    QString defaultConnBase =
        settings.value("default_conn_base").toString();
    settings.endGroup();

    qDebug() << "[GUI] DefaultConnBase " << defaultConnBase;

    QFileInfo defaultConnBaseInfo(defaultConnBase);
    if(defaultConnBaseInfo.exists())
    {
      QString filename = project->getLocation();
      QString newConnBase = filename. mid(0, filename.lastIndexOf("/")+1) +
                            defaultConnBaseInfo.fileName();

      qDebug() << "[GUI] Copy " << defaultConnBase << " to "
               << newConnBase;

      //remove the file if it already exists
      if(QFile::exists(newConnBase))
      {
        QFile::remove(newConnBase);
      }

      //copy the defaultConnBase to the project dir
      if(QFile::copy(defaultConnBase, newConnBase))
      {
        //If everything is OK we import the new defaultConnBase to NCL
        // document.

        QMap <QString, QString> connBaseAttrs, importBaseAttrs;
        connBaseAttrs.insert("id", defaultConnBaseID);
        importBaseAttrs.insert("alias", "conn");
        importBaseAttrs.insert("documentURI",
                               defaultConnBaseInfo.fileName());

        //add connectorBase element
        Entity *head = project->getEntitiesbyType("head").at(0);
        msgControl->anonymousAddEntity("connectorBase",
                                       head->getUniqueId(),
                                       connBaseAttrs);

        //add importBase element
        Entity *connectorBase =
            project->getEntitiesbyType("connectorBase").at(0);
        msgControl->anonymousAddEntity("importBase",
                                       connectorBase->getUniqueId(),
                                       importBaseAttrs);
      }
      else //error
      {
        QMessageBox::warning(this, tr("Error!"),
                             tr("There was an error copying the default"
                                "Connector Base. You will need to add a "
                                "Connector Base by hand in your NCL "
                                "code."),
                             tr("Ok"));
      }
    }
    else //error
    {
      QMessageBox::warning(this, tr("Error!"),
                           tr("The default Connector Base %1 does not "
                              "exists").arg(defaultConnBase),
                           tr("Ok"));
    }
  }

  if(shouldCreateADefaultRegion)
  {
    QMap <QString, QString> regionBaseAttrs, regionAttrs;
    QList<Entity*> regionBases = project->getEntitiesbyType("regionBase");

    // There is no regionBase, so lets create one
    if(!regionBases.size())
    {
      regionBaseAttrs.insert("id", defaultRegionBaseID);
      Entity *head = project->getEntitiesbyType("head").at(0);
      msgControl->anonymousAddEntity("regionBase",
                                     head->getUniqueId(),
                                     regionBaseAttrs);
    }

    //Now, its time to add the region itself
    Entity *regionBase = project->getEntitiesbyType("regionBase").at(0);
    regionAttrs.insert("id", defaultRegionID);
    regionAttrs.insert("top", "0");
    regionAttrs.insert("left", "0");
    regionAttrs.insert("width", "100%");
    regionAttrs.insert("height", "100%");
    regionAttrs.insert("zIndex", "1");
    msgControl->anonymousAddEntity("region",
                                   regionBase->getUniqueId(),
                                   regionAttrs);
  }

  if(shouldCreateADefaultRegion)
  {

  }

  if(save)
    saveCurrentProject(); //Save the just created basic file!
}

/*!
 * \brief Called by the user when he/she wants to open an existent project.
 */
void ComposerMainWindow::openProject()
{
  QString filename = QFileDialog::getOpenFileName(this,
                                               tr("Open NCL Composer Project"),
                                               getLastFileDialogPath(),
                                           tr("NCL Composer Projects (*.cpr)"));
  if(filename != "")
  {
#ifdef WIN32
    filename = filename.replace("\\", "/");
#endif

    checkTemporaryFileLastModified(filename);

    ProjectControl::getInstance()->launchProject(filename);

    updateLastFileDialogPath(filename);
  }
}

void ComposerMainWindow::checkTemporaryFileLastModified(QString filename)
{
  QFileInfo temporaryFileInfo(filename + "~");
  QFileInfo fileInfo(filename);

  if( temporaryFileInfo.exists() &&
      temporaryFileInfo.lastModified() > fileInfo.lastModified() )
  {
      bool replace = QMessageBox::question(this,
                            tr("Temporary file is newer."),
                            tr("There is a temporary file related to %1 that is"
                               " newer. Do you want replace the %1 file with "
                               " this temporary one?").arg(filename),
                            QMessageBox::Yes | QMessageBox::No,
                            QMessageBox::No);

      if(replace)
      {
          QFile file (filename + "~");
          if(!file.copy(filename))
          {
              QFile oldfile (filename);
              oldfile.remove();
              file.copy(filename);
          }
      }
  }
}

bool ComposerMainWindow::removeTemporaryFile(QString location)
{
  QFile file(location + "~");
  return file.remove();
}

void ComposerMainWindow::updateLastFileDialogPath(QString filepath)
{
  Utilities::updateLastFileDialogPath(filepath);
}

QString ComposerMainWindow::getLastFileDialogPath()
{
  return Utilities::getLastFileDialogPath();
}

void ComposerMainWindow::importFromDocument()
{
  QString docFilename = QFileDialog::getOpenFileName(
        this,
        tr("Choose the NCL file to be imported"),
        getLastFileDialogPath(),
        tr("NCL Documents (*.ncl)") );

  if(docFilename != "")
  {
    updateLastFileDialogPath(docFilename);

    QString projFilename = QFileDialog::getSaveFileName(
          this,
          tr("Choose the NCL Composer Project where the NCL document must be "
             "imported"),
             getLastFileDialogPath(),
             tr("NCL Composer Projects (*.cpr)") );

    //Create the file
    QFile f(projFilename);
    f.open(QIODevice::ReadWrite);
    f.close();

    if(projFilename != "")
    {
#ifdef WIN32
      projFilename = projFilename.replace(QDir::separator(), "/");
#endif
      ProjectControl::getInstance()->importFromDocument(docFilename,
                                                        projFilename);

      updateLastFileDialogPath(projFilename);
    }
  }
}

void ComposerMainWindow::addToRecentProjects(QString projectUrl)
{
  GlobalSettings settings;
  QStringList recentProjects = settings.value("recentProjects").toStringList();

  recentProjects.push_front(projectUrl);
  recentProjects.removeDuplicates();

  //MAXIMUM SIZE
  while(recentProjects.size() > this->maximumRecentProjectsSize)
    recentProjects.pop_back();

  settings.setValue("recentProjects", recentProjects);

  updateRecentProjectsWidgets();
}

void ComposerMainWindow::updateRecentProjectsWidgets()
{
  GlobalSettings settings;
  QStringList recentProjects = settings.value("recentProjects").toStringList();

  ui->menu_Recent_Files->clear();
  if(recentProjects.size() == 0 )
  {
    QAction *act = ui->menu_Recent_Files->addAction(tr("empty"));
    act->setEnabled(false);
  }
  else /* There are at least one element in the recentProject list */
  {
    for (int i = 0; i < recentProjects.size(); i++)
    {
      QAction *act = ui->menu_Recent_Files->addAction(recentProjects.at(i));
      act->setData(recentProjects.at(i));

      connect( act, SIGNAL(triggered()),
               this, SLOT(userPressedRecentProject()) );
    }

    ui->menu_Recent_Files->addSeparator();

    QAction *clearRecentProjects =
        ui->menu_Recent_Files->addAction(tr("Clear Recent Projects"));

    connect(clearRecentProjects, SIGNAL(triggered()),
            this, SLOT(clearRecentProjects()));
  }

  welcomeWidget->updateRecentProjects(recentProjects);
}

void ComposerMainWindow::userPressedRecentProject(QString src)
{
#ifdef WIN32
  src = src.replace(QDir::separator(), "/");
#endif

  QFile file(src);
  bool openCurrentFile = true, recreateFile = false;
  if(!file.exists())
  {
    int resp =
        QMessageBox::question(this,
                              tr("File does not exists anymore."),
                              tr("The File %1 does not exists anymore. "
                                 "Do you want to create this file "
                                 "again?").arg(src),
                              QMessageBox::Yes | QMessageBox::No,
                              QMessageBox::No);

    if(resp != QMessageBox::Yes)
      openCurrentFile = false;
    else
      recreateFile = true;
  }

  if(openCurrentFile)
  {
    checkTemporaryFileLastModified(src);

    ProjectControl::getInstance()->launchProject(src);
    if(recreateFile)
    {
      // Create the directory structure if it does not exist anymore

      QDir dir;
      bool ok = dir.mkpath(QFileInfo(src).absolutePath()); // this function creates the path, with all its necessary parents;

      if(!ok)
      {
        //error message, could not create the required directory structure!
        QMessageBox::critical(this, tr("Error!"), tr("Error creating directory structure"));
        return;
      }

      // \todo Ask for the import or not of the defaultConnBase.
      addDefaultStructureToProject(
            ProjectControl::getInstance()->getOpenProject(src),
            false,
            false,
            true);
    }
  }
}

void ComposerMainWindow::userPressedRecentProject()
{
  QAction *action = qobject_cast<QAction *> (QObject::sender());

  QString src = action->data().toString();

  userPressedRecentProject(src);
}

void ComposerMainWindow::clearRecentProjects(void)
{
  GlobalSettings settings;
  settings.remove("recentProjects");

  updateRecentProjectsWidgets();
}

void ComposerMainWindow::selectedAboutCurrentPluginFactory()
{
  QList<QTreeWidgetItem*> selectedPlugins = pluginsExt->selectedItems();
  if(selectedPlugins.size())
  {
    if(treeWidgetItem2plFactory.value(selectedPlugins.at(0)) != NULL)
    {
      pluginDetailsDialog->setCurrentPlugin(
            treeWidgetItem2plFactory.value(selectedPlugins.at(0)));
      detailsButton->setEnabled(true);
    }
    else
      detailsButton->setEnabled(false);
  }
}

/*!
 * \brief Shows the details of the current selected plugins.
 */
void ComposerMainWindow::showPluginDetails()
{
  pluginDetailsDialog->show();
}

void ComposerMainWindow::restorePerspectiveFromMenu()
{
  QAction *action = qobject_cast<QAction*>(QObject::sender());
  restorePerspective(action->data().toString());
}

void ComposerMainWindow::updateMenuPerspectives()
{
  GlobalSettings settings;
  settings.beginGroup("pluginslayout");
  QStringList keys = settings.allKeys();
  settings.endGroup();

  ui->toolBar_Perspectives->clear();

  for(int i = 0; i < keys.size(); i++)
  {
    QAction *act = ui->toolBar_Perspectives->addAction( keys.at(i),
                                                        this,
                                                        SLOT(restorePerspectiveFromMenu()) );
    act->setData(keys[i]);
  }

  // Add Option to save current Perspective
  /* _menuPerspective->addSeparator();
  _menuPerspective->addAction(_saveCurrentPluginsLayoutAction); */
}

void ComposerMainWindow::updateMenuLanguages()
{
  QStringList languages;
  languages << tr("English") << tr("Portugues (Brasil)");

  for(int i = 0; i < languages.size(); i++)
  {
    QAction *act = _menuLanguage->addAction(languages.at(i),
                                            this,
                                            SLOT(changeLanguageFromMenu()));
    act->setData(languages[i]);
  }
}

void ComposerMainWindow::currentTabChanged(int n)
{
  if(n)
  {
    ui->action_Save_current_perspective->setEnabled(true);
    ui->action_Restore_a_perspective->setEnabled(true);
    ui->action_CloseProject->setEnabled(true);
    ui->action_Save->setEnabled(true);
    ui->action_SaveAs->setEnabled(true);

    // \todo: This should check if there is already running application
    updateRunActions();
  }
  else
  {
    ui->action_Save_current_perspective->setEnabled(false);
    ui->action_Restore_a_perspective->setEnabled(false);
    ui->action_CloseProject->setEnabled(false);
    ui->action_Save->setEnabled(false);
    ui->action_SaveAs->setEnabled(false);
    ui->action_RunNCL->setEnabled(false);
    ui->action_StopNCL->setEnabled(false);
  }
}

void ComposerMainWindow::pluginWidgetViewToggled(bool state)
{
  QToolWindowManager *toolWidgetManager =
      dynamic_cast<QToolWindowManager *> (_tabProjects->currentWidget());
  int index = static_cast<QAction*>(sender())->data().toInt();
  QWidget *toolWindow = toolWidgetManager->toolWindows()[index];
  toolWidgetManager->moveToolWindow( toolWindow, state ?
                                     QToolWindowManager::LastUsedArea :
                                     QToolWindowManager::NoArea );
}

void ComposerMainWindow::focusChanged(QWidget *old, QWidget *now)
{
  Q_UNUSED(old)

  /*
  if(now == NULL)
    return; // Do nothing!!

  if(qApp->activeModalWidget() != NULL)
    return; // Do nothing!! (Wait for the popup to end)

  // qDebug() << "Locking allDocksMutex 1";
  allDocksMutex.lock();
  for(int i = 0; i < allDocks.size(); i++)
  {
    // if(old != NULL && allDocks.at(i)->isAncestorOf(old))
    updateDockStyle(allDocks.at(i), false);
  }
  allDocksMutex.unlock();
  // qDebug() << "Unlocked allDocksMutex 1";

  // qDebug() << "Locking allDocksMutex 2";
  allDocksMutex.lock();

  if(now != NULL)
  {
    for(int i = 0 ; i < allDocks.size(); i++)
    {
      bool isAncestor = false;
      QWidget *child = now;
//      qDebug() << "Start" << i << allDocks.size();
      while (child && child != this)
      {
//        qDebug() << "child pointer" << child;
//        qDebug() << child->metaObject()->className();
        if (child == allDocks.at(i))
        {
          isAncestor = true;
          break;
        }
        child = child->parentWidget();
      }
//      qDebug() << "End";

      if(isAncestor)
        updateDockStyle(allDocks.at(i), true);
    }
  }
  allDocksMutex.unlock();
//  qDebug() << "Unlocked allDocksMutex 2";
  */
}

void ComposerMainWindow::setProjectDirty(QString location, bool isDirty)
{
  QToolWindowManager *window = _projectsWidgets[location];

  QString projectId =
      ProjectControl::getInstance()->
      getOpenProject(location)->getAttribute("id");

  int index = _tabProjects->indexOf(window);

  ui->action_Save->setEnabled(true);

  if(index >= 0) {
    if(isDirty)
      _tabProjects->setTabText(index, QString("*")+projectId);
    else
      _tabProjects->setTabText(index, projectId);
  }
}

void ComposerMainWindow::undo()
{
  int index = _tabProjects->currentIndex();

  if(index >= 1)
  {
    QString location = _tabProjects->tabToolTip(index);
    Project *project = ProjectControl::getInstance()->getOpenProject(location);
    MessageControl *msgControl =
        PluginControl::getInstance()->getMessageControl(project);

    if(msgControl != NULL)
      msgControl->undo();
  }
}

void ComposerMainWindow::redo()
{
  int index = _tabProjects->currentIndex();

  if(index >= 1)
  {
    QString location = _tabProjects->tabToolTip(index);
    Project *project = ProjectControl::getInstance()->getOpenProject(location);
    MessageControl *msgControl =
        PluginControl::getInstance()->getMessageControl(project);

    if(msgControl != 0)
      msgControl->redo();
  }

}

void ComposerMainWindow::gotoNCLClubWebsite()
{
  QDesktopServices::openUrl(QUrl("http://club.ncl.org.br"));
}

/*!
   * \brief Show the NCL Composer Help.
   */
bool ComposerMainWindow::showHelp()
{
  // composerHelpWidget.show();
  return true;

  // Old implementation based on Assistant
  /*if (!proc)
    proc = new QProcess();

  if (proc->state() != QProcess::Running)
  {
    QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath) +
        QDir::separator();
#if !defined(Q_OS_MAC)
    app += QLatin1String("assistant");
#else
    app += QLatin1String("Assistant.app/Contents/MacOS/Assistant");
#endif

    QStringList args;
//  args << QLatin1String("-collectionFile")
//       << QLatin1String("help/composerhelp.qhc")
//       << QLatin1String("-enableRemoteControl");

    proc->start(app, args);

    if (!proc->waitForStarted()) {
      QMessageBox::critical(0, QObject::tr("Simple Text Viewer"),
                    QObject::tr("Unable to launch Qt Assistant (%1)").arg(app));
      return false;
    }
  }
  return true; */
}

void ComposerMainWindow::autoSaveCurrentProjects()
{
  for(int i = 1; i < _tabProjects->count(); i++)
  {
    QString location = _tabProjects->tabToolTip(i);
    Project *project = ProjectControl::getInstance()->getOpenProject(location);
    if(project->isDirty())
    {
      PluginControl::getInstance()->savePluginsData(project);
      ProjectControl::getInstance()->saveTemporaryProject(location);
    }
  }
}

/*!
 * \brief Creates the language menu dynamically from the content of m_langPath.
 *        We create the menu entries dynamically, dependant on the existing
 *        translations
 */
void ComposerMainWindow::createLanguageMenu(void)
{
  QActionGroup* langGroup = new QActionGroup( _menuLanguage );
  langGroup->setExclusive(true);

  connect( langGroup, SIGNAL(triggered(QAction *)),
           this, SLOT(slotLanguageChanged(QAction *)) );

  // format systems language
  QString defaultLocale = QLocale::system().name();       // e.g. "de_DE"
  defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

  m_langPath = QApplication::applicationDirPath();
  m_langPath.append("/languages");
  QDir dir(m_langPath);
  QStringList fileNames = dir.entryList(QStringList("composer_*.qm"));

  qDebug() << fileNames;
  for (int i = 0; i < fileNames.size(); ++i)
  {
    // get locale extracted by filename
    QString locale;
    locale = fileNames[i];                     // "TranslationExample_de.qm"
    locale.truncate(locale.lastIndexOf('.'));  // "TranslationExample_de"
    locale.remove(0, locale.indexOf('_') + 1); // "de"

    QString lang = QLocale::languageToString(QLocale(locale).language());
    QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

    QAction *action = new QAction(ico, lang, this);
    action->setCheckable(true);
    action->setData(locale);

    // ui->_menuLanguage->addAction(action);
    _menuLanguage->addAction(action);
    langGroup->addAction(action);

    // set default translators and language checked
    if (defaultLocale == locale)
    {
      action->setChecked(true);
    }
  }
}

/*!
 * \brief This slot is called by the language menu actions.
 *
 * Called every time, a menu entry of the language menu is called.
 */
void ComposerMainWindow::slotLanguageChanged(QAction* action)
{
  if(0 != action)
  {
    // load the language dependant on the action content
    loadLanguage(action->data().toString());
    setWindowIcon(action->icon());
  }
}

void ComposerMainWindow::switchTranslator(QTranslator& translator,
                                          const QString& filename)
{
  // remove the old translator
  qApp->removeTranslator(&translator);

  // load the new translator
  if(translator.load(filename))
    qApp->installTranslator(&translator);
}

/*!
 * \brief Loads a language by the given language shortcurt (e.g. de, en, ...)
 */
void ComposerMainWindow::loadLanguage(const QString& rLanguage)
{
  qDebug() << rLanguage;
  if(m_currLang != rLanguage)
  {
    m_currLang = rLanguage;
    QLocale locale = QLocale(m_currLang);
    QLocale::setDefault(locale);
    QString languageName = QLocale::languageToString(locale.language());
    switchTranslator(m_translator,
                     m_langPath + "/" + QString("composer_%1.qm").arg(rLanguage));
    switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));

  }
}

/*!
 * \brief This event is called, when a new translator is loaded or the system
 * language is changed.
 */
void ComposerMainWindow::changeEvent(QEvent* event)
{
  if(0 != event)
  {
    switch(event->type())
    {
    // this event is send if a translator is loaded
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
      // this event is send, if the system, language changes
    case QEvent::LocaleChange:
    {
      QString locale = QLocale::system().name();
      locale.truncate(locale.lastIndexOf('_'));
      loadLanguage(locale);
    }
    break;
    default:
      break;
    }
  }

  QMainWindow::changeEvent(event);
}

void ComposerMainWindow::updateTabWithProject(int index, QString newLocation)
{
  QString oldLocation = _tabProjects->tabToolTip(index);

  if(oldLocation == newLocation) return; /* do nothing */

  /* Already had a project in this tab */
  if(!oldLocation.isNull() && !oldLocation.isEmpty())
  {
    // Update _projectsWidgets
    if(_projectsWidgets.contains(oldLocation))
    {
      _projectsWidgets.insert(newLocation, _projectsWidgets.value(oldLocation));
      _projectsWidgets.remove(oldLocation);
    }
  }

  _tabProjects->setTabToolTip(index, newLocation);
  Project *project = ProjectControl::getInstance()->getOpenProject(newLocation);
  if(project != NULL)
  {
    QString projectId = project->getAttribute("id");
    _tabProjects->setTabText(index, projectId);
  }
}

/*!
 * \brief TODO
 */
void ComposerMainWindow::saveLoadPluginData(int)
{
  GlobalSettings settings;
  settings.beginGroup("loadPlugins");
  QTreeWidgetItem *item;
  qDebug() << treeWidgetItem2plFactory.keys();
  foreach(item, treeWidgetItem2plFactory.keys())
  {
    if(item->checkState(1))
    {
      settings.setValue(treeWidgetItem2plFactory.value(item)->id(), true);
    }
    else
    {
      qDebug() << treeWidgetItem2plFactory.value(item) << "2";
      settings.setValue(treeWidgetItem2plFactory.value(item)->id(), false);
    }
  }
  settings.endGroup();
}

void ComposerMainWindow::on_actionReport_Bug_triggered()
{
  QDesktopServices::openUrl(QUrl("http://composer.telemidia.puc-rio.br/en/contact"));
}

#ifdef WITH_SERV_PUB
void ComposerMainWindow::on_actionPublish_triggered()
{
  int index = _tabProjects->currentIndex();

  if (index != 0)
  {
    QString location = _tabProjects->tabToolTip(index);

    Project *currentProject =
        ProjectControl::getInstance()->getOpenProject(location);

    QString projectName = currentProject->getLocation();

    GlobalSettings settings;
    settings.beginGroup("runginga");
    QString remoteIp = settings.value("remote_ip").toString();
    QString remoteUser = settings.value("remote_user").toString();
    QString remotePath = settings.value("remote_path").toString();
    settings.endGroup();

    QString  remoteLocation = remoteUser+"@"+remoteIp+":"+remotePath;

    int r = QMessageBox::question(this,
                    tr("Publish"), tr("Are you sure you want to publish"
    " the project <b>'%1'</b> to <b>'%2'</b>").arg(projectName, remoteLocation),
                    QMessageBox::Yes | QMessageBox::No,
                    QMessageBox::No);

    if (r == QMessageBox::Yes)
      copyOnRemoteGingaVM(false);
  }
}
#endif

#if WITH_WIZARD
void ComposerMainWindow::on_actionProject_from_Wizard_triggered()
{
  GlobalSettings settings;
  settings.beginGroup("wizard");
  wizardProcess.start(settings.value("wizard-engine").toByteArray());
  settings.endGroup();
}

void ComposerMainWindow::wizardFinished(int resp)
{
  qDebug() << "Wizard process finished with ret=" << resp;
  if(!resp)
  {
    QString filename = wizardProcess.readLine();
    filename = (filename.mid(0, filename.size()-1));

    qDebug() << "Wizard process finished with ret=" << resp;
    qDebug() << filename;

    QStringList args;
    args << filename;
    args << "/tmp/a.ncl";

    GlobalSettings settings;

    settings.beginGroup("wizard");
    talProcess.start( settings.value("tal-processor").toByteArray(), args );
    settings.endGroup();
    talProcess.waitForFinished();

    qDebug() << talProcess.readAllStandardError();
    qDebug() << talProcess.readAllStandardOutput();

    QFileInfo fInfo("/tmp/a.ncl");
    if(fInfo.exists())
    {
      ProjectControl::getInstance()->importFromDocument("/tmp/a.ncl",
                                                        "/tmp/a.cpr");
      QFile::remove("/tmp/a.ncl");
    }
    else
    {
      QMessageBox::warning(this, tr("Error"), tr("It was not possible to create the file from the template!"));
    }
  }
}
#endif

} } //end namespace

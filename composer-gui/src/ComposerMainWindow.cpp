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

#ifdef USE_MDI
#include <QMdiArea>
#endif

namespace composer {
    namespace gui {

ComposerMainWindow::ComposerMainWindow(QApplication &app, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ComposerMainWindow)
{
    ui->setupUi(this);

#ifdef Q_WS_MAC
    defaultPluginsPath << "/Library/Application Support/Composer"
                       << QCoreApplication::applicationDirPath() +
                          "/../PlugIns/composer";;

#elif defined(Q_WS_WIN32)
    defaultPluginsPath << "C:/Composer/lib/composer";
#else
    // PREFIX Should be defined by the qmake while compiling the source code.

#ifdef EXT_DEFAULT_PATH
    defaultPluginsPath << QString(EXT_DEFAULT_PATH)
                          + QString("/lib/composer/extensions");
#endif

    defaultPluginsPath << QDir::homePath() + QString("/composer/extensions");
#endif

    initGUI();
    initModules();
    readSettings();

    connect(&app, SIGNAL(focusChanged(QWidget *, QWidget *)),
            this, SLOT(focusChanged(QWidget *, QWidget *)));
}

ComposerMainWindow::~ComposerMainWindow()
{
    delete ui;
    delete menu_Perspective;
}

void ComposerMainWindow::initModules()
{
    PluginControl  *pgControl = PluginControl::getInstance();
    LanguageControl *lgControl = LanguageControl::getInstance();
    ProjectControl *projectControl = ProjectControl::getInstance();

    connect(pgControl,SIGNAL(notifyError(QString)),
            SLOT(errorDialog(QString)));

    connect(pgControl,SIGNAL(addPluginWidgetToWindow(IPluginFactory*,
                                                     IPlugin*, Project*, int)),
            SLOT(addPluginWidget(IPluginFactory*, IPlugin*, Project*, int)));

    connect(lgControl,SIGNAL(notifyError(QString)),
            SLOT(errorDialog(QString)));

    connect(projectControl, SIGNAL(notifyError(QString)),
            SLOT(errorDialog(QString)));

    connect(projectControl,SIGNAL(projectAlreadyOpen(QString)),
            SLOT(onOpenProjectTab(QString)));

    connect(projectControl, SIGNAL(startOpenProject(QString)),
            this, SLOT(startOpenProject(QString)));

    connect(projectControl, SIGNAL(endOpenProject(QString)), this,
            SLOT(endOpenProject(QString)));

    connect(projectControl,SIGNAL(endOpenProject(QString)),
            SLOT(addToRecentProjects(QString)));

    connect(projectControl,SIGNAL(projectAlreadyOpen(QString)),
                 SLOT(onOpenProjectTab(QString)));

    connect(projectControl, SIGNAL(dirtyProject(QString, bool)),
            this, SLOT(setProjectDirty(QString, bool)));

    readExtensions();
}

void ComposerMainWindow::readExtensions()
{
#ifdef Q_WS_MAC
    QSettings settings("br.puc-rio.telemidia", "Composer");
#else
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");
#endif

    settings.beginGroup("extension");

    if (settings.contains("path"))
        extensions_paths = settings.value("path").toStringList();

    extensions_paths << defaultPluginsPath; //Add default location to extensions
    extensions_paths.removeDuplicates(); // Remove duplicate paths

    // foreach path where extensions can be installed, try to load profiles.
    for(int i = 0; i < extensions_paths.size(); i++)
    {
        LanguageControl::getInstance()->loadProfiles(extensions_paths.at(i));
    }

    // foreach path where extensions can be installed, try to load plugins.
    for(int i = 0; i < extensions_paths.size(); i++)
    {
        PluginControl::getInstance()->loadPlugins(extensions_paths.at(i));
    }
    settings.endGroup();
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
        QString dirName = QFileDialog::getExistingDirectory(
                this, tr("Select Directory"),
                QDir::homePath(), QFileDialog::ShowDirsOnly);
        return dirName;
    } else {
        return "";
    }
}

void ComposerMainWindow::readSettings()
{
#ifdef Q_WS_MAC
    QSettings settings("telemidia.pucrio.br", "composer");
#else
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");
#endif
    settings.beginGroup("mainwindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();

    settings.beginGroup("openfiles");
    QStringList openfiles = settings.value("openfiles").toStringList();
    settings.endGroup();

    qDebug() << "Openning files:";
    for(int i = 0; i < openfiles.size(); i++)
    {
        qDebug() << openfiles.at(i);
        QFile file(openfiles.at(i));
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
                                          "again?").arg(openfiles.at(i)),
                                          QMessageBox::Yes | QMessageBox::No,
                                          QMessageBox::No);

            if(resp != QMessageBox::Yes) openCurrentFile = false;
        }
        if (openCurrentFile)
        {
            ProjectControl::getInstance()->launchProject(openfiles.at(i));
        }
    }

    /* Update Recent Projects on Menu */
    QStringList recentProjects =
            settings.value("recentprojects").toStringList();
    updateRecentProjectsMenu(recentProjects);
}

void ComposerMainWindow::initGUI()
{
#ifndef Q_WS_MAC
    setWindowIcon(QIcon(":/mainwindow/icon"));
#endif
    setWindowTitle(tr("Composer NCL"));
    tabProjects = new QTabWidget(0);

    ui->frame->layout()->addWidget(tabProjects);

//    tabProjects->setMovable(true);
    tabProjects->setTabsClosable(true);

    tbPerspectiveDropList = new QToolButton(this);
    tbPerspectiveDropList->setIcon(QIcon(":/mainwindow/webcam"));
    tbPerspectiveDropList->setPopupMode(QToolButton::InstantPopup);

    connect( tabProjects, SIGNAL(tabCloseRequested(int)),
             this, SLOT(tabClosed(int)));

    connect(tabProjects, SIGNAL(currentChanged(int)),
            this, SLOT(currentTabChanged(int)));

//  createStatusBar();
    createActions();
    createMenus();
    createAboutPlugins();

    preferences = new PreferencesDialog(this);
    perspectiveManager = new PerspectiveManager(this);
    pluginDetailsDialog = new PluginDetailsDialog(aboutPluginsDialog);
    projectWizard = new ProjectWizard(this);

    connect(ui->action_RunNCL, SIGNAL(triggered()), this, SLOT(runNCL()));

    welcomeWidget = new WelcomeWidget(this);
    welcomeWidget->show();
    tabProjects->addTab(welcomeWidget, "Welcome");
    tabProjects->setTabIcon(0, QIcon());

    connect(welcomeWidget, SIGNAL(userPressedOpenProject()),
            this, SLOT(openProject()));

    connect(welcomeWidget, SIGNAL(userPressedNewProject()),
            this, SLOT(launchProjectWizard()));

    connect(welcomeWidget, SIGNAL(userPressedSeeInstalledPlugins()),
            this, SLOT(aboutPlugins()));
}

void ComposerMainWindow::addPluginWidget(IPluginFactory *fac, IPlugin *plugin,
                                 Project *project, int n)
{
    QMainWindow *w;
    QString location = project->getLocation();
    QString projectId = project->getAttribute("id");

#ifdef USE_MDI
    QMdiArea *mdiArea;
#endif
    if (projectsWidgets.contains(location))
    {
        w = projectsWidgets[location];
#ifdef USE_MDI
        mdiArea = (QMdiArea *)w->centralWidget();
#endif
    } else {
        w = new QMainWindow(tabProjects);
#ifdef USE_MDI
        mdiArea = new QMdiArea;
        mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        w->setCentralWidget(mdiArea);
#else
        w->setDockNestingEnabled(true);
        w->setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::West);
#endif
        int index = tabProjects->addTab(w, projectId);
        tabProjects->setTabToolTip(index, location);
        projectsWidgets[location] = w;
    }
    QWidget *pW = plugin->getWidget();

#ifdef USE_MDI
    mdiArea->addSubWindow(pW);
    pW->setWindowModified(true);
    pW->setWindowTitle(projectId + " - " + fac->name());
    pW->show();
    pW->setObjectName(fac->id());
#else
    QDockWidget *dock = new QDockWidget(fac->name());

    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(QDockWidget::AllDockWidgetFeatures);

    dock->setWidget(pW);
    dock->setObjectName(fac->id());
    if (n%2)
        w->addDockWidget(Qt::RightDockWidgetArea, dock, Qt::Vertical);
    else
        w->addDockWidget(Qt::LeftDockWidgetArea, dock, Qt::Vertical);

    dock->setMinimumSize(150, 150);
    tabProjects->setCurrentWidget(w);

    if(firstDock.contains(location)) {
        // w->tabifyDockWidget(firstDock[location], dock);
    }
    else firstDock[location] = dock;

    QFrame *titleBar = new QFrame();
    titleBar->setContentsMargins(0,0,0,0);

    QLabel *titleLabel = new QLabel(fac->name());
    titleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *layout = new QHBoxLayout(titleBar);
    layout->setMargin(0);
    layout->setSpacing(0);

    layout->addWidget(titleLabel);

    titleBar->setStyleSheet(" ");

    dock->setTitleBarWidget(titleBar);
    allDocks.insert(0, dock);
    QPushButton *refresh = new QPushButton(titleBar);
    refresh->setIcon(QIcon(":/mainwindow/refreshplugin"));
    addButtonToDockTitleBar(titleBar, refresh);

    QPushButton *hide = new QPushButton(titleBar);
    connect(hide, SIGNAL(pressed()), dock, SLOT(close()));
    hide->setIcon(QIcon(":/mainwindow/closeplugin"));
    addButtonToDockTitleBar(titleBar, hide);

    updateDockTitleStyle(titleBar, false);
#endif
}

void ComposerMainWindow::updateDockTitleStyle(QFrame *titleBar, bool selected)
{
    if(!selected)
    {
        titleBar->setStyleSheet(" \
                QFrame { border: none; \
                    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                    stop: 0 #a6a6a6, stop: 0.08 #7f7f7f,\
                    stop: 0.39999 #717171, stop: 0.4 #626262,\
                    stop: 0.9 #4c4c4c, stop: 1 #333333);\
                    color: white;\
                    padding-left: 2px;} \
                QPushButton { \
                    background-color: transparent; \
                    border: none;\
                  } \
                QPushButton:hover { \
                    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                    stop: 0 #d6d6d6, stop: 0.08 #afafaf,\
                    stop: 0.79999 #717171, stop: 0.4 #a2a2a2,\
                    stop: 0.9 #8c8c8c, stop: 1 #777777);\
                } \
                QPushButton:pressed { \
                    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                    stop: 0 #868686, stop: 0.08 #5f5f5f,\
                    stop: 0.19999 #717171, stop: 0.4 #424242,\
                    stop: 0.9 #2c2c2c, stop: 1 #111111);\
                } \
                QPushButton:flat { \
                    border: none; \
                }");
    }
    else
    {
        titleBar->setStyleSheet(" \
                QFrame { border: none; \
                     background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                         stop: 0 #dcebfd, \
                         stop: 1 #c2dcfd); \
                         color: black;\
                         font-style: bold;\
                         padding-left: 2px; \
                }\
                QPushButton { \
                    background-color: transparent; \
                    border: none;\
                  } \
                QPushButton:hover { \
                    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                        stop: 0 #edfcfe, \
                        stop: 1 #d3edfe); \
                } \
                QPushButton:pressed { \
                    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,\
                        stop: 0 #dcebfd, \
                        stop: 1 #c2dcfd); \
                } \
                QPushButton:flat { \
                    border: none; \
                }");
    }

}
void ComposerMainWindow::addButtonToDockTitleBar(QFrame *titleBar,
                                                 QPushButton *button)
{
    button->setIconSize(QSize(22, 22));

    titleBar->layout()->addWidget(button);
}

void ComposerMainWindow::tabClosed(int index)
{
    if(index == 0)
        return; // Do nothing

    QString location = tabProjects->tabToolTip(index);

    Project *project = ProjectControl::getInstance()->getOpenProject(location);
    if(project->isDirty())
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
    QMainWindow *w = projectsWidgets[location];

    //Remove from allDocks
    int i = 0;
    while(i < allDocks.size())
    {
        if(w->isAncestorOf(allDocks.at(i)))
            allDocks.removeAt(i);
        else
            i++;
    }

    // Delete QMainWindow
    if (w)
    {
        delete w;
        w = NULL;
    }
    projectsWidgets.remove(location);
    firstDock.remove(location);
}

void ComposerMainWindow::closeCurrentTab()
{
    if(tabProjects->currentIndex())
    {
        int currentIndex = tabProjects->currentIndex();
        tabClosed(currentIndex);
    }
}
void ComposerMainWindow::closeAllFiles()
{
    while(tabProjects->count() > 1)
    {
        tabProjects->removeTab(1);
        tabClosed(1);
    }
}

void ComposerMainWindow::onOpenProjectTab(QString location)
{
    if (!projectsWidgets.contains(location)) return;
    QMainWindow *w = projectsWidgets[location];
    tabProjects->setCurrentWidget(w);
}

void ComposerMainWindow::createMenus()
{
    ui->menu_Edit->addAction(editPreferencesAct);

    menuBar()->addSeparator();

    connect( ui->menu_Window, SIGNAL(aboutToShow()),
             this, SLOT(updateViewMenu()));

    connect(ui->action_Close_Project, SIGNAL(triggered()),
            this, SLOT(closeCurrentTab()));

    connect(ui->action_Close_All, SIGNAL(triggered()),
            this, SLOT(closeAllFiles()));

    connect( ui->action_Save, SIGNAL(triggered()),
             this, SLOT(saveCurrentProject()));

    connect ( ui->action_New_Project, SIGNAL(triggered()),
              this, SLOT(launchProjectWizard()));


    menu_Perspective = new QMenu(0);

    // assing menu_Perspective to tbPerspectiveDropList
    tbPerspectiveDropList->setMenu(menu_Perspective);
    tabProjects->setCornerWidget(tbPerspectiveDropList, Qt::TopRightCorner);
//  tabProjects->setCornerWidget(ui->menu_Window, Qt::TopLeftCorner);

    updateMenuPerspectives();
}

void ComposerMainWindow::createAboutPlugins()
{
    aboutPluginsDialog = new QDialog(this);
    aboutPluginsDialog->setWindowTitle(tr("Installed Plugins"));

    profilesExt = new QListWidget(aboutPluginsDialog);
    profilesExt->setAlternatingRowColors(true);

    /* This should be a new Widget and change some code for there */
    pluginsExt = new QTreeWidget(aboutPluginsDialog);
    pluginsExt->setAlternatingRowColors(true);

    connect(pluginsExt, SIGNAL(itemSelectionChanged()),
            this, SLOT(selectedAboutCurrentPluginFactory()));

    QStringList header;
    header << "Name" << "Load" << "Version" << "Vendor";
    pluginsExt->setHeaderLabels(header);

    QDialogButtonBox *bOk = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                 QDialogButtonBox::Close,
                                                 Qt::Horizontal,
                                                 aboutPluginsDialog);

    detailsButton = bOk->button(QDialogButtonBox::Ok);
    detailsButton->setText(tr("Details"));
    detailsButton->setIcon(QIcon());
    detailsButton->setEnabled(false);

    connect(bOk, SIGNAL(rejected()), aboutPluginsDialog, SLOT(close()));

    connect( detailsButton, SIGNAL(pressed()),
             this, SLOT(showPluginDetails()) );

    QGridLayout *gLayout = new QGridLayout(aboutPluginsDialog);
    gLayout->addWidget(new QLabel(tr("The <b>Composer</b> is an IDE for"
                                     " Declarative Multimedia languages."),
                                  aboutPluginsDialog));

    gLayout->addWidget(new QLabel(tr("<b>Installed Language Profiles</b>"),
                                  aboutPluginsDialog));

    gLayout->addWidget(profilesExt);
    gLayout->addWidget(new QLabel(tr("<b>Installed Plug-ins</b>")));
    gLayout->addWidget(pluginsExt);
    gLayout->addWidget(bOk);
    aboutPluginsDialog->setLayout(gLayout);

    aboutPluginsDialog->setModal(true);
}

void ComposerMainWindow::about()
{
    AboutDialog dialog(this);
    dialog.exec();
}

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
        QString category = pF->category();
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
        treeWidgetItem = new QTreeWidgetItem(categories.value(pF->category()));
        treeWidgetItem2plFactory.insert(treeWidgetItem, pF);

        treeWidgetItem->setText(0, pF->name());
        treeWidgetItem->setCheckState(1, Qt::Checked);
        treeWidgetItem->setText(2, pF->version());
        treeWidgetItem->setText(3, pF->vendor());
    }

    pluginsExt->expandAll();

    pluginsExt->setColumnWidth(0, 150);
    pluginsExt->resizeColumnToContents(1);
    pluginsExt->resizeColumnToContents(2);
    pluginsExt->resizeColumnToContents(3);

    /* PROFILE LANGUAGE */
    QList<ILanguageProfile*>::iterator itL;
    QList<ILanguageProfile*> lList = LanguageControl::getInstance()->
                                     getLoadedProfiles();
    profilesExt->clear();

    for(itL = lList.begin(); itL != lList.end(); itL++)
    {
        ILanguageProfile *lg = *itL;
        profilesExt->addItem(new QListWidgetItem(lg->getProfileName()));
    }

    detailsButton->setEnabled(false);
    aboutPluginsDialog->show();
}

void ComposerMainWindow::errorDialog(QString message)
{
    //QMessageBox::warning(this,tr("Error!"),message);
    qWarning() << message;
}

void ComposerMainWindow::createActions() {

    connect(ui->action_About, SIGNAL(triggered()),
            this, SLOT(about()));

    connect( ui->action_About_Plugins, SIGNAL(triggered()),
             this, SLOT(aboutPlugins()));

    fullScreenViewAct = new QAction(tr("&FullScreen"),this);
    fullScreenViewAct->setShortcut(tr("F11"));

    connect(fullScreenViewAct,SIGNAL(triggered()),this,
            SLOT(showCurrentWidgetFullScreen()));

    editPreferencesAct = new QAction(tr("&Preferences"), this);
    editPreferencesAct->setStatusTip(tr("Edit preferences"));
    connect (editPreferencesAct, SIGNAL(triggered()), this,
             SLOT(showEditPreferencesDialog()));

    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));
    saveCurrentPluginsLayoutAct = new QAction(tr("Save current perspective"),
                                              this);

    connect(    saveCurrentPluginsLayoutAct, SIGNAL(triggered()),
                this, SLOT(saveCurrentGeometryAsPerspective()));

    restorePluginsLayoutAct = new QAction(tr("Restore a perspective"), this);
    connect(restorePluginsLayoutAct, SIGNAL(triggered()),
                this, SLOT(restorePerspective()));

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    //Add a separator to the toolbar. All actions after preferences (including
    // it will be aligned in the bottom.
    ui->toolBar->insertWidget(ui->action_Preferences, spacer);

    connect (ui->actionOpen_Project, SIGNAL(triggered()),
             this, SLOT(openProject()));

    connect (ui->actionImport_from_existing_NCL, SIGNAL(triggered()),
             this, SLOT(importFromDocument()));
}

void ComposerMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void ComposerMainWindow::showCurrentWidgetFullScreen()
{
    tabProjects->addAction(fullScreenViewAct);

    if(!tabProjects->isFullScreen())
    {
        tabProjects->setWindowFlags(Qt::Window);
        tabProjects->showFullScreen();
    }
    else
    {
        tabProjects->setParent(ui->frame, Qt::Widget);
        tabProjects->show();
    }
}

void ComposerMainWindow::updateViewMenu()
{
    ui->menu_Window->clear();
    ui->menu_Window->addAction(fullScreenViewAct);
    ui->menu_Window->addSeparator();
    ui->menu_Window->addAction(saveCurrentPluginsLayoutAct);
    ui->menu_Window->addAction(restorePluginsLayoutAct);
}

void ComposerMainWindow::closeEvent(QCloseEvent *event)
{

#ifdef Q_WS_MAC
    QSettings settings("telemidia.pucrio.br", "composer");
#else
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");
#endif
    settings.beginGroup("extension");
    settings.setValue("path", extensions_paths);
    settings.endGroup();

    settings.beginGroup("mainwindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();

    QStringList openfiles;
    QString key;
    foreach (key, projectsWidgets.keys())
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
    event->accept();
}

void ComposerMainWindow::cleanUp()
{
    LanguageControl::releaseInstance();
    ProjectControl::releaseInstance();
    PluginControl::releaseInstance();
}

void ComposerMainWindow::showEditPreferencesDialog()
{
    preferences->show();
}

void ComposerMainWindow::startOpenProject(QString project)
{
    this->setCursor(QCursor(Qt::WaitCursor));
    update();
}

void ComposerMainWindow::endOpenProject(QString project)
{
    this->setCursor(QCursor(Qt::ArrowCursor));

    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");

    if(settings.contains("default_perspective"))
    {
        QString defaultPerspective =
                settings.value("default_perspective").toString();
        restorePerspective(defaultPerspective);
        update();
    }
}

void ComposerMainWindow::saveCurrentProject()
{
    int index = tabProjects->currentIndex();

    if(index != -1)
    {
        QString location = tabProjects->tabToolTip(index);
        PluginControl::getInstance()->savePluginsData(location);
        ProjectControl::getInstance()->saveProject(location);
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
    if(tabProjects->count()) // If there is a document open
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
        QMessageBox box(QMessageBox::Warning,
                        tr("Information"),
                        tr("There aren't a layout open to be saved."),
                        QMessageBox::Ok
                     );
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

void ComposerMainWindow::savePerspective(QString layoutName)
{
    if(tabProjects->count()) //see if there is any open document
    {
        QString location = tabProjects->tabToolTip(
                tabProjects->currentIndex());

        QMainWindow *window = projectsWidgets[location];
        QSettings settings(QSettings::IniFormat,
                           QSettings::UserScope,
                           "telemidia",
                           "composer");

        settings.beginGroup("pluginslayout");
        settings.setValue(layoutName, window->saveState(0));
        settings.endGroup();
    }
}

void ComposerMainWindow::saveDefaultPerspective(QString defaultPerspectiveName)
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");
    settings.setValue("default_perspective", defaultPerspectiveName);
}

void ComposerMainWindow::restorePerspective(QString layoutName)
{
    if(tabProjects->count()) //see if there is any open project
    {
        QString location = tabProjects->tabToolTip(
                tabProjects->currentIndex());

        QMainWindow *window = projectsWidgets[location];
        QSettings settings(QSettings::IniFormat,
                           QSettings::UserScope,
                           "telemidia",
                           "composer");

        settings.beginGroup("pluginslayout");
#ifndef USE_MDI
        window->restoreState(settings.value(layoutName).toByteArray());
#endif
        settings.endGroup();
    }
}

void ComposerMainWindow::runNCL()
{
    QProcess *ginga = new QProcess(this);
    QStringList arguments;
    QString location = tabProjects->tabToolTip(tabProjects->currentIndex());

    if(location.isEmpty())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning!"),
                              tr("There aren't a current NCL project."),
                          QMessageBox::Ok);
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
            file.write(project->getChildren().at(0)->toString(0).toAscii());

        file.close();

        /* RUNNING GINA */
        arguments << "--ncl"<< nclpath;
        ginga->start("ginga", arguments);
        QByteArray result = ginga->readAll();
    }
    else
    {
        qWarning() << "Error trying to running NCL. Could not create : "
                << nclpath << " !";
    }
}

void ComposerMainWindow::launchProjectWizard()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Creating a new Composer Project"),
            QDir::currentPath(),
            tr("Composer Projects (*.cpr)") );

    if( !filename.isNull() )
    {
         ProjectControl::getInstance()->launchProject(filename);
    }
}

void ComposerMainWindow::openProject()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                tr("Open Composer Project"),
                                                QDir::homePath(),
                                                tr("Composer Project (*.cpr)"));
    if(filename != "") {
        ProjectControl::getInstance()->launchProject(filename);
    }
}

void ComposerMainWindow::importFromDocument()
{
    QString docFilename = QFileDialog::getOpenFileName(
            this,
            tr("Choose the NCL file to be imported"),
            QDir::currentPath(),
            tr("NCL Documents (*.ncl)") );

    if(docFilename != "")
    {
        QString projFilename = QFileDialog::getSaveFileName(
                this,
                tr("Choose the Composer Project where the NCL document must be \
                    imported"),
                QDir::currentPath(),
                tr("Composer Projects (*.cpr)") );

        //Create the file
        QFile f(projFilename);
        f.open(QIODevice::ReadWrite);
        f.close();

        if(projFilename != "")
        {
            ProjectControl::getInstance()->importFromDocument(docFilename,
                                                           projFilename);
        }
    }
}

void ComposerMainWindow::addToRecentProjects(QString projectUrl)
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");

    QStringList recentprojects = settings.value("recentProjects").toStringList();

    recentprojects.push_front(projectUrl);
    recentprojects.removeDuplicates();

    //MAXIMUM SIZE
    while(recentprojects.size() > this->maximumRecentProjectsSize)
        recentprojects.pop_back();

    settings.setValue("recentProjects", recentprojects);

    updateRecentProjectsMenu(recentprojects);
}

void ComposerMainWindow::updateRecentProjectsMenu(QStringList &recentProjects)
{
    ui->menu_Recent_Files->clear();
    if(recentProjects.size() == 0 )
    {
        QAction *act = ui->menu_Recent_Files->addAction(tr("empty"));
        act->setEnabled(false);
    }
    else /* There are at least one element in the recentProject list */
    {
        for(int i = 0; i < recentProjects.size(); i++)
        {
            QAction *act = ui->menu_Recent_Files->addAction(
                                                        recentProjects.at(i));
            act->setData(recentProjects.at(i));
            connect(act, SIGNAL(triggered()), this,
                    SLOT(openRecentProject()));
        }
        ui->menu_Recent_Files->addSeparator();
        QAction *clearRecentProjects =
                ui->menu_Recent_Files->addAction(tr("Clear Recent Projects"));

        connect(clearRecentProjects, SIGNAL(triggered()),
                this, SLOT(clearRecentProjects()));

    }
}

void ComposerMainWindow::clearRecentProjects(void)
{
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");

    settings.remove("recentProjects");
    QStringList empty;
    updateRecentProjectsMenu(empty);
}

void ComposerMainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *> (QObject::sender());
    ProjectControl::getInstance()->launchProject(action->data().toString());
}

void ComposerMainWindow::selectedAboutCurrentPluginFactory()
{
    QList<QTreeWidgetItem*> selectedPlugins = pluginsExt->selectedItems();
    if(selectedPlugins.size())
    {
        if(treeWidgetItem2plFactory[selectedPlugins.at(0)] != NULL)
        {
            pluginDetailsDialog->setCurrentPlugin(
                treeWidgetItem2plFactory[selectedPlugins.at(0)]);
            detailsButton->setEnabled(true);
        }
        else
            detailsButton->setEnabled(false);
    }
}

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
    QSettings settings(QSettings::IniFormat,
                       QSettings::UserScope,
                       "telemidia",
                       "composer");

    settings.beginGroup("pluginslayout");
    QStringList keys = settings.allKeys();
    settings.endGroup();

    menu_Perspective->clear();

    for(int i = 0; i < keys.size(); i++)
    {
        QAction *act = menu_Perspective->addAction(keys.at(i),
                                            this,
                                            SLOT(restorePerspectiveFromMenu()));
        act->setData(keys[i]);
    }
}

void ComposerMainWindow::currentTabChanged(int n)
{
    if(n)
    {
        tbPerspectiveDropList->setEnabled(true);
        saveCurrentPluginsLayoutAct->setEnabled(true);
        restorePluginsLayoutAct->setEnabled(true);
        ui->action_Close_Project->setEnabled(true);
        ui->action_Save->setEnabled(true);
    }
    else
    {
        tbPerspectiveDropList->setEnabled(false);
        saveCurrentPluginsLayoutAct->setEnabled(false);
        restorePluginsLayoutAct->setEnabled(false);
        ui->action_Close_Project->setEnabled(false);
        ui->action_Save->setEnabled(false);
    }
}

void ComposerMainWindow::focusChanged(QWidget *old, QWidget *now)
{
    for(int i = 0; i < allDocks.size(); i++)
    {
        if(old != NULL && allDocks.at(i)->isAncestorOf(old))
            updateDockTitleStyle((QFrame*)allDocks.at(i)->titleBarWidget(),
                                 false);

        if(now != NULL && allDocks.at(i)->isAncestorOf(now))
            updateDockTitleStyle((QFrame*)allDocks.at(i)->titleBarWidget(),
                                 true);
    }

}

void ComposerMainWindow::setProjectDirty(QString location, bool isDirty)
{
    QMainWindow *window = projectsWidgets[location];
    QString projectId =
            ProjectControl::getInstance()->
                                getOpenProject(location)->getAttribute("id");

    int index = tabProjects->indexOf(window);

    if(index >= 0) {
        if(isDirty)
            tabProjects->setTabText(index, QString("*")+projectId);
        else
            tabProjects->setTabText(index, projectId);
    }
}

} } //end namespace

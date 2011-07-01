#include "ComposerMainWindow.h"
#include "ui_ComposerMainWindow.h"

#include <QPixmap>
#include <QCloseEvent>
#include <QDialogButtonBox>

#ifdef USE_MDI
#include <QMdiArea>
#endif

namespace composer {
    namespace gui {

ComposerMainWindow::ComposerMainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ComposerMainWindow)
{
    ui->setupUi(this);

    user_directory_ext = "";

#ifdef Q_WS_MAC
    defaultEx = "/Library/Application Support/Composer";
#elif defined(Q_WS_WIN32)
    defaultEx = "C:/Composer/lib/composer";
#else
    defaultEx = "/usr/local/lib/composer/extension";
#endif

    initGUI();

    initModules();

    readSettings();

    preferences = new PreferencesDialog(this);
    perspectiveManager = new PerspectiveManager(this);

    projectWizard = new ProjectWizard(this);

    connect(ui->action_RunNCL, SIGNAL(triggered()), this, SLOT(runNCL()));
}

ComposerMainWindow::~ComposerMainWindow() {
    delete ui;
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

    connect(tabProjects,SIGNAL(tabCloseRequested(int)),
                this,SLOT(tabClosed(int)));

    readExtensions();

}

void ComposerMainWindow::readExtensions()
{

#ifdef Q_WS_MAC
    QSettings settings("telemidia.pucrio.br", "composer");
#else
    QSettings settings("telemidia", "composer");
#endif

    settings.beginGroup("extension");
#ifdef Q_WS_WIN32
    user_directory_ext = defaultEx;
#else
    if (settings.contains("path"))
    {
        user_directory_ext = settings.value("path").toString();
    } else {
        user_directory_ext = promptChooseExtDirectory();
        if (user_directory_ext == "") user_directory_ext = defaultEx;
    }
#endif
    // qDebug() << "MainWindow::readExtensions(" << user_directory_ext
    //          << ")";

    LanguageControl::getInstance()->
            loadProfiles(user_directory_ext);

    PluginControl::getInstance()->loadPlugins(user_directory_ext);
    settings.endGroup();
}

QString ComposerMainWindow::promptChooseExtDirectory()
{

    QMessageBox mBox;
    mBox.setText(tr("The Extension Directory is not set"));
    mBox.setInformativeText(tr("Do you want to try the default"
                               "directory (%1)?").arg(defaultEx));
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

void ComposerMainWindow::readSettings() {
#ifdef Q_WS_MAC
    QSettings settings("telemidia.pucrio.br", "composer");
#else
    QSettings settings("telemidia", "composer");
#endif
    settings.beginGroup("mainwindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();

    settings.beginGroup("openfiles");
    QStringList openfiles = settings.value("openfiles").toStringList();
    settings.endGroup();

    for(int i = 0; i < openfiles.size(); i++)
    {
        qDebug() << openfiles.at(i);
        ProjectControl::getInstance()->launchProject(openfiles.at(i));
    }

    /* Update Recent Projects on Menu */
    QStringList recentProjects =
            settings.value("recentprojects").toStringList();
    updateRecentProjectsMenu(recentProjects);
}

void ComposerMainWindow::initGUI() {
#ifndef Q_WS_MAC
    setWindowIcon(QIcon(":/mainwindow/icon"));
#endif
    setWindowTitle(tr("Composer NCL 3.0"));
    // tabDocuments = new QTabWidget(ui->frame);
    tabProjects = ui->tabWidget;

    tabProjects->setMovable(true);
    tabProjects->setTabsClosable(true);
    connect(tabProjects,SIGNAL(tabCloseRequested(int)),
            this,SLOT(tabClosed(int)));

    // createStatusBar();
    createActions();
    createMenus();
    createAbout();
    showMaximized();

//    welcomeScreen = new QWebView(this);
//    welcomeScreen->setStyleSheet("background-color:rgb(150,147,88); padding: 7px ; color:rgb(255,255,255)");
//    welcomeScreen->load(QUrl("http://www.ncl.org.br"));
//    welcomeScreen->showMaximized();
//    tabDocuments->addTab(welcomeScreen, "Welcome");

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
    pW->setWindowTitle(fac->name()+"[*]");
    pW->show();
    pW->setObjectName(fac->id());
#else
    QDockWidget *dock = new QDockWidget(fac->name());
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(QDockWidget::DockWidgetClosable |
                      QDockWidget::DockWidgetMovable);

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
#endif
}

void ComposerMainWindow::tabClosed(int index)
{
    QString location = tabProjects->tabToolTip(index);
    ProjectControl::getInstance()->closeProject(location);
    QMainWindow *w = projectsWidgets[location];
    if (w)
    {
        delete w;
        w = NULL;
    }
    projectsWidgets.remove(location);
    firstDock.remove(location);
}

void ComposerMainWindow::closeAllFiles()
{
    while(tabProjects->count())
    {
        tabProjects->removeTab(0);
        tabClosed(0);
    }
}

void ComposerMainWindow::onOpenProjectTab(QString location)
{
    if (!projectsWidgets.contains(location)) return;
    QMainWindow *w = projectsWidgets[location];
    tabProjects->setCurrentWidget(w);
}

void ComposerMainWindow::createMenus() {

    ui->menu_Edit->addAction(editPreferencesAct);

    menuBar()->addSeparator();

    connect( ui->menu_Window, SIGNAL(aboutToShow()),
             this, SLOT(updateViewMenu()));

    connect(ui->action_Close_All, SIGNAL(triggered()),
            this, SLOT(closeAllFiles()));

    connect( ui->action_Save, SIGNAL(triggered()),
             this, SLOT(saveCurrentProject()));

    connect ( ui->action_New_Project, SIGNAL(triggered()),
              this, SLOT(launchProjectWizard()));
}


void ComposerMainWindow::createAbout()
{
    aboutDialog = new QDialog(this);
    aboutDialog->setWindowTitle(tr("About Composer"));

    profilesExt = new QListWidget(aboutDialog);
    profilesExt->setAlternatingRowColors(true);

    pluginsExt = new QTreeWidget(aboutDialog);
    pluginsExt->setAlternatingRowColors(true);
    QStringList header;
    header << "Name" << "Load" << "Version" << "Vendor";
    pluginsExt->setHeaderLabels(header);

    QDialogButtonBox *bOk = new QDialogButtonBox(QDialogButtonBox::Ok |
                                                 QDialogButtonBox::Close,
                                                 Qt::Horizontal,
                                                 aboutDialog);

    QPushButton *tmp = bOk->button(QDialogButtonBox::Ok);
    tmp->setText(tr("Details"));
    tmp->setIcon(QIcon());

    connect(bOk, SIGNAL(rejected()), aboutDialog, SLOT(close()));

    QGridLayout *gLayout = new QGridLayout(aboutDialog);
    gLayout->addWidget(new QLabel(tr("The <b>Composer</b> is an IDE for"
                                     " Declarative Multimedia languages."),
                                  aboutDialog));

    gLayout->addWidget(new QLabel(tr("<b>Installed Language Profiles</b>"),
                                  aboutDialog));

    gLayout->addWidget(profilesExt);
    gLayout->addWidget(new QLabel(tr("<b>Installed Plug-ins</b>")));
    gLayout->addWidget(pluginsExt);
    gLayout->addWidget(bOk);
    aboutDialog->setLayout(gLayout);

}

void ComposerMainWindow::about() {
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

    for (it = pList.begin(); it != pList.end(); it++) {
        IPluginFactory *pF = *it;
        treeWidgetItem = new QTreeWidgetItem(categories.value(pF->category()));
        treeWidgetItem->setText(0, pF->name());
        treeWidgetItem->setCheckState(1, Qt::Checked);
        treeWidgetItem->setText(2, pF->vendor());
        treeWidgetItem->setText(3, pF->version());
    }

    pluginsExt->expandAll();

    pluginsExt->setColumnWidth(0, 150);
    pluginsExt->resizeColumnToContents(1);
    pluginsExt->resizeColumnToContents(2);
    pluginsExt->resizeColumnToContents(3);

    QList<ILanguageProfile*>::iterator itL;
    QList<ILanguageProfile*> lList = LanguageControl::getInstance()->
                                     getLoadedProfiles();
    profilesExt->clear();

    for(itL = lList.begin(); itL != lList.end(); itL++)
    {
        ILanguageProfile *lg = *itL;
        profilesExt->addItem(new QListWidgetItem(lg->getProfileName()));
    }

    aboutDialog->setModal(true);
    aboutDialog->show();
}

void ComposerMainWindow::errorDialog(QString message) {
    QMessageBox::warning(this,tr("Error!"),message);

}

void ComposerMainWindow::createActions() {

    aboutComposerAct = new QAction(tr("&About"), this);
    aboutComposerAct->setStatusTip(tr("Show the application's About box"));

    connect(aboutComposerAct, SIGNAL(triggered()), this, SLOT(about()));

    connect(ui->action_About_Plugins, SIGNAL(triggered()), this, SLOT(about()));

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
    spacer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding);

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
    QSettings settings("telemidia", "composer");
#endif
    if (user_directory_ext != "")
    {
        settings.beginGroup("extension");
        settings.setValue("path",user_directory_ext);
        settings.endGroup();
    }

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

    QSettings settings("telemidia", "composer");
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
    if(tabProjects->count()) { // If there is a document open
        perspectiveManager->setBehavior(PERSPEC_SAVE);
        if(perspectiveManager->exec())
        {
            savePerspective(perspectiveManager->getSelectedName());
            saveDefaultPerspective(perspectiveManager->getDefaultPerspective());
        }
    }
    else {
        QMessageBox box(QMessageBox::Warning,
                        tr("Information"),
                        tr("There aren't a layout open to be saved."),
                        QMessageBox::Ok
                     );
        box.exec();
    }
}

void ComposerMainWindow::restorePerspective()
{
    perspectiveManager->setBehavior(PERSPEC_LOAD);
    if(perspectiveManager->exec())
    {
        restorePerspective(perspectiveManager->getSelectedName());
    }
}

void ComposerMainWindow::savePerspective(QString layoutName)
{
    if(tabProjects->count()) //see if there is any open document
    {
        QString location = tabProjects->tabToolTip(
                tabProjects->currentIndex());

        QMainWindow *window = projectsWidgets[location];
        QSettings settings("telemidia", "composer");
        settings.beginGroup("pluginslayout");
        settings.setValue(layoutName, window->saveState(0));
        settings.endGroup();
    }
}

void ComposerMainWindow::saveDefaultPerspective(QString defaultPerspectiveName)
{
    QSettings settings("telemidia", "composer");
    settings.setValue("default_perspective", defaultPerspectiveName);
}

void ComposerMainWindow::restorePerspective(QString layoutName)
{
    if(tabProjects->count()) //see if there is any open project
    {
        QString location = tabProjects->tabToolTip(
                tabProjects->currentIndex());

        QMainWindow *window = projectsWidgets[location];
        QSettings settings("telemidia", "composer");
        settings.beginGroup("pluginslayout");
        window->restoreState(settings.value(layoutName).toByteArray());
        settings.endGroup();
    }
}

void ComposerMainWindow::runNCL()
{
    QProcess *ginga = new QProcess(this);
    QStringList arguments;
    QString location = tabProjects->tabToolTip(
            tabProjects->currentIndex());

    if(!location.isEmpty())
    {
        arguments << "--ncl"<< location;
        ginga->start("ginga", arguments);
        QByteArray result = ginga->readAll();
    }
    else
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, tr("Warning!"),
                                     tr("There aren't a current NCL project."),
                                     QMessageBox::Ok);
    }
}

void ComposerMainWindow::launchProjectWizard()
{
    QString filename = QFileDialog::getSaveFileName(
            this,
            tr("Creating a new Project"),
            QDir::currentPath(),
            tr("Composer Projects (*.ncl)") );

    if( !filename.isNull() )
    {
         ProjectControl::getInstance()->launchProject(filename);
    }
}

void ComposerMainWindow::openProject()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    tr("Open NCL Document"),
                                                    QDir::homePath(),
                                                    tr("NCL Document (*.ncl)"));
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
                tr("Composer Projects (*.ncl)") );
        if(projFilename != "")
        {
            ProjectControl::getInstance()->importFromDocument( docFilename,
                                                           projFilename);
        }
    }
}

void ComposerMainWindow::addToRecentProjects(QString projectUrl)
{
    QSettings settings("telemidia", "composer");
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
            QAction *act = ui->menu_Recent_Files->addAction(recentProjects.at(i));
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
    QSettings settings("telemidia", "composer");
    settings.remove("recentProjects");
    QStringList empty;
    updateRecentProjectsMenu(empty);
}

void ComposerMainWindow::openRecentProject()
{
    QAction *action = qobject_cast<QAction *> (QObject::sender());
    ProjectControl::getInstance()->launchProject(action->data().toString());
}

} } //end namespace

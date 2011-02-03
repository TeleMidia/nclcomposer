#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

    user_directory_ext = "";
    work_space_path = QDir::homePath();

    fileSystemModel = NULL;

#ifdef Q_WS_MAC
    defaultEx = "/Library/Application Support/Composer";
#elif defined(Q_WS_WIN32)
    defaultEx = "C:/Composer/lib/composer";
#else
    defaultEx = "/usr/local/lib/composer";
#endif

    wsSwitch = new WorkspaceSwitch(this);
    connect(wsSwitch,SIGNAL(accepted()),SLOT(switchWorkspace()));
    initGUI();
    initModules();
    readSettings();
    preferences = new PreferencesDialog(this);
}

MainWindow::~MainWindow() {
    if(fileSystemModel)
    {
        delete fileSystemModel;
        fileSystemModel = NULL;
    }
}

void MainWindow::initModules()
{
    PluginControl  *pgControl = PluginControl::getInstance();
    LanguageControl *lgControl = LanguageControl::getInstance();
    DocumentControl *docControl = DocumentControl::getInstance();

    connect(pgControl,SIGNAL(notifyError(QString)),
                      SLOT(errorDialog(QString)));
    connect(pgControl,SIGNAL(addPluginWidgetToWindow(IPluginFactory*,
                                                     IPlugin*,Document*)),
                SLOT(addPluginWidget(IPluginFactory*,IPlugin*,Document*)));

    connect(lgControl,SIGNAL(notifyError(QString)),
                      SLOT(errorDialog(QString)));

    connect(docControl, SIGNAL(notifyError(QString)),
            SLOT(errorDialog(QString)));
    connect(docControl,SIGNAL(openDocumentTab(QString)),
            SLOT(onOpenDocumentTab(QString)));

    readExtensions();

}

void MainWindow::readExtensions()
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
//    qDebug() << "MainWindow::readExtensions(" << user_directory_ext
//            << ")";

    LanguageControl::getInstance()->
            loadProfiles(user_directory_ext);

    PluginControl::getInstance()->loadPlugins(user_directory_ext);
    settings.endGroup();
}

QString MainWindow::promptChooseExtDirectory()
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

void MainWindow::readSettings() {
#ifdef Q_WS_MAC
    QSettings settings("telemidia.pucrio.br", "composer");
#else
    QSettings settings("telemidia", "composer");
#endif
    settings.beginGroup("mainwindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("windowState").toByteArray());
    settings.endGroup();

    settings.beginGroup("workspace");
    if (!settings.contains("path"))
    {
        wsSwitch->exec();
        if (wsSwitch->result() == QDialog::Accepted)
            work_space_path = wsSwitch->getWorspacePath();
    } else {
        work_space_path = settings.value("path").toString();
        QDir dir(work_space_path);
        if (!dir.exists())
        {
            if (wsSwitch->result() == QDialog::Accepted)
                work_space_path = wsSwitch->getWorspacePath();
        }
    }
    createTreeProject();
    settings.endGroup();
}

void MainWindow::initGUI() {
#ifndef Q_WS_MAC
    setWindowIcon(QIcon(":/mainwindow/icon"));
#endif
    setWindowTitle(tr("Composer NCL 3.0"));
    tabDocuments = new QTabWidget(this);
    tabDocuments->setMovable(true);
    tabDocuments->setTabsClosable(true);
    connect(tabDocuments,SIGNAL(tabCloseRequested(int)),
            this,SLOT(tabClosed(int)));
    setCentralWidget(tabDocuments);
    createStatusBar();
    createActions();
    createMenus();
    createAbout();
    showMaximized();

}

void MainWindow::addPluginWidget(IPluginFactory *fac, IPlugin *plugin,
                                 Document *doc)
{
    QMainWindow *w;
    QString location = doc->getLocation();
    QString documentId = doc->getAttribute("id");
    QString projectId = doc->getProjectId();

    if (documentsWidgets.contains(location))
    {
        w = documentsWidgets[location];
    } else {
        w = new QMainWindow(tabDocuments);
        w->setDockNestingEnabled(true);
        w->setTabPosition(Qt::AllDockWidgetAreas,QTabWidget::West);
        int index = tabDocuments->addTab(w,projectId + "(" + documentId + ")");
        tabDocuments->setTabToolTip(index, location);
        documentsWidgets[location] = w;
    }

    QDockWidget *dock = new QDockWidget(fac->getPluginName());
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(QDockWidget::DockWidgetClosable |
                      QDockWidget::DockWidgetMovable);
    QWidget *pW = plugin->getWidget();
    dock->setWidget(pW);
    w->addDockWidget(Qt::BottomDockWidgetArea, dock, Qt::Horizontal);
    tabDocuments->setCurrentWidget(w);

    if(firstDock.contains(location)) {
        w->tabifyDockWidget(firstDock[location], dock);
    }
    else firstDock[location] = dock;
}

void MainWindow::tabClosed(int index)
{
    QString location = tabDocuments->tabToolTip(index);
    DocumentControl::getInstance()->closeDocument(location);
    QMainWindow *w = documentsWidgets[location];
    if (w)
    {
        delete w;
        w = NULL;
        documentsWidgets.remove(location);
        firstDock.remove(location);
    }
}

void MainWindow::onOpenDocumentTab(QString location)
{
    if (!documentsWidgets.contains(location)) return;
    QMainWindow *w = documentsWidgets[location];
    tabDocuments->setCurrentWidget(w);
}

void MainWindow::switchWorkspace()
{

    if (fileSystemModel == NULL) return;

    if (wsSwitch->getWorspacePath() != "")
        work_space_path = wsSwitch->getWorspacePath();

    QModelIndex index = fileSystemModel->setRootPath(work_space_path);

    workspace_model->setSourceModel(fileSystemModel);
    workspace_model->setTopIndex(index);

    fileSystemView->setModel(workspace_model);

    fileSystemView->setRootIndex(workspace_model->mapFromSource
                                 (index));
}

void MainWindow::createTreeProject()
{
    fileSystemDock = new QDockWidget(tr("Projects"), this);
    fileSystemDock->setObjectName("projectTree");

    fileSystemView = new ProjectTreeView(this);
    workspace_model = new WorkspaceModel(fileSystemView);

    fileSystemModel = new QFileSystemModel();

    switchWorkspace();

    fileSystemView->setSortingEnabled(false);
    fileSystemView->setHeaderHidden(true);
    fileSystemView->setColumnHidden(1,true);
    fileSystemView->setColumnHidden(2,true);
    fileSystemView->setColumnHidden(3,true);
    fileSystemDock->setWidget(fileSystemView);

    addDockWidget(Qt::LeftDockWidgetArea, fileSystemDock);

    projectWindowAct = new QAction(QIcon(":/mainwindow/projectTree"),
                                   tr("Projects"),this);
    projectWindowAct->setCheckable(true);
    projectWindowAct->setChecked(fileSystemDock->isVisible());

    connect(fileSystemDock,SIGNAL(visibilityChanged(bool)),this,
            SLOT(updateWindowMenu()));
    connect(projectWindowAct,SIGNAL(triggered(bool)),fileSystemDock,
            SLOT(setVisible(bool)));

}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newProjectAct);
    fileMenu->addAction(newDocumentAct);
    fileMenu->addSeparator();
    fileMenu->addAction(switchWS);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(editPreferencesAct);

    windowMenu = menuBar()->addMenu(tr("&Window"));

    menuBar()->addSeparator();
    connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutComposerAct);

}


void MainWindow::createAbout()
{
    aboutDialog = new QDialog(this);
    aboutDialog->setWindowTitle(tr("About Composer"));

    profilesExt = new QListWidget(aboutDialog);
    profilesExt->setAlternatingRowColors(true);
    pluginsExt = new QListWidget(aboutDialog);
    pluginsExt->setAlternatingRowColors(true);


    QPushButton *bOk = new QPushButton(tr("OK"),aboutDialog);
    connect(bOk,SIGNAL(clicked()),aboutDialog,SLOT(close()));

    QGridLayout *gLayout = new QGridLayout(aboutDialog);
    gLayout->addWidget(new QLabel(tr("The <b>Composer</b> is an IDE for"
                                     " Declarative Multimedia languages."),
                                  aboutDialog));
    gLayout->addWidget(new QLabel(tr("<b>Profile Languages Loaded</b>"),
                                  aboutDialog));
    gLayout->addWidget(profilesExt);
    gLayout->addWidget(new QLabel(tr("<b>Plug-ins Loaded</b>")));
    gLayout->addWidget(pluginsExt);
    gLayout->addWidget(bOk, 6, 2);
    aboutDialog->setLayout(gLayout);

}

void MainWindow::about() {
    QList<IPluginFactory*>::iterator it;
    QList<IPluginFactory*> pList = PluginControl::getInstance()->
                                   getLoadedPlugins();

    pluginsExt->clear();
    for (it = pList.begin(); it != pList.end(); it++) {
        IPluginFactory *pF = *it;
        pluginsExt->addItem(new QListWidgetItem(pF->getPluginIcon(),
                                                pF->getPluginName()));
    }

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

void MainWindow::errorDialog(QString message) {
    QMessageBox::warning(this,tr("Error!"),message);

}

void MainWindow::createActions() {

    newProjectAct = new QAction(QIcon(":/mainwindow/openProject"),
                                tr("New &Project"), this);

    newProjectAct->setStatusTip(tr("Create a new Project"));
//    connect(newProjectAct, SIGNAL(triggered()), this,
//            SLOT(launchProjectWizard()));

    newDocumentAct = new QAction(QIcon(":/mainwindow/newDocument"),
                                 tr("New &Document"), this);
    newDocumentAct->setShortcuts(QKeySequence::New);
    newDocumentAct->setStatusTip(tr("Create a new NCL Document"));
//    connect(newDocumentAct, SIGNAL(triggered()), this,
//            SLOT(launchAddDocumentWizard()));

    aboutComposerAct = new QAction(tr("&About"), this);
    aboutComposerAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutComposerAct, SIGNAL(triggered()), this, SLOT(about()));

    separatorWindowAct = new QAction(this);
    separatorWindowAct->setSeparator(true);

    editPreferencesAct = new QAction(tr("&Preferences"), this);
    editPreferencesAct->setStatusTip(tr("Edit preferences"));
    connect (editPreferencesAct, SIGNAL(triggered()), this,
             SLOT(showEditPreferencesDialog()));

    switchWS = new QAction(tr("Switch Workspace"),this);
    connect(switchWS,SIGNAL(triggered()),SLOT(showSwitchWorkspaceDialog()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit Composer application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::updateWindowMenu()
{
    windowMenu->clear();
    windowMenu->addAction(projectWindowAct);
    windowMenu->addAction(separatorWindowAct);
    projectWindowAct->setChecked(fileSystemDock->isVisible());
}

void MainWindow::closeEvent(QCloseEvent *event)
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

    settings.beginGroup("workspace");
    if (work_space_path!= "")
    {
        settings.setValue("path", work_space_path);
    }
    settings.endGroup();
    settings.beginGroup("mainwindow");
    settings.setValue("geometry", saveGeometry());
    settings.setValue("windowState", saveState());
    settings.endGroup();
    settings.sync();

    cleanUp();
    event->accept();
}

void MainWindow::cleanUp()
{
    LanguageControl::releaseInstance();
    DocumentControl::releaseInstance();
    PluginControl::releaseInstance();
}

void MainWindow::showEditPreferencesDialog()
{
    preferences->show();
}

void MainWindow::showSwitchWorkspaceDialog()
{
    wsSwitch->show();
}


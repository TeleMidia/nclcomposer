#include "include/MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    QSplashScreen splash(QPixmap(":/mainwindow/icon"));
    splash.show();

    user_directory_ext = "";

    #ifdef Q_WS_MAC
        setUnifiedTitleAndToolBarOnMac(true);
        defaultEx = "/Library/Application Support/Composer";
    #else
        defaultEx = "/usr/local/lib";
    #endif

    initGUI();

    initModules();

    splash.finish(this);
    preferences = new PreferencesWidget(this);
    readSettings();
}

MainWindow::~MainWindow() {

}

void MainWindow::initModules()
{
    ProjectControl *pjControl = ProjectControl::getInstance();
    PluginControl  *pgControl = PluginControl::getInstance();
    LanguageControl *lgControl = LanguageControl::getInstance();

    connect(pjControl,SIGNAL(projectCreated(QString,QString)),
            this,SLOT(createProjectInTree(QString,QString)));
    connect(pjControl,SIGNAL(notifyError(QString)),
            this,SLOT(errorDialog(QString)));
    connect(pgControl,SIGNAL(notifyError(QString)),
            this,SLOT(errorDialog(QString)));
    connect(pgControl,SIGNAL(newDocumentLaunchedAndCreated(QString,QString)),
            this,SLOT(createDocumentInTree(QString,QString)));
    connect(lgControl,SIGNAL(notifyLoadedProfile(QString,QString)),
            this,SLOT(addProfileLoaded(QString,QString)));
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
            if (settings.contains("language"))
            {
                user_directory_ext = settings.value("language").toString();
            } else {
                user_directory_ext = promptChooseDirectory();
            }
            qDebug() << "MainWindow::readExtensions(" << user_directory_ext
                     << ")";
            LanguageControl::getInstance()->
                   loadProfiles(user_directory_ext);
       //TODO - loadPlugins
       settings.endGroup();
}

QString MainWindow::promptChooseDirectory()
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
        return defaultEx;
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
    settings.beginGroup("projects");
        QStringList projects = settings.childKeys();
        QStringListIterator it(projects);
        while (it.hasNext()) {
            QString projectId = it.next();
            QString location  = settings.value(projectId).toString();
            qDebug() << "MainWindow::readSettings" <<
                        "READ projectId: " << projectId <<
                        "location: " << location;
            ProjectControl::getInstance()->addProject(projectId,location);
        }
    settings.endGroup();
}

void MainWindow::initGUI() {
    #ifndef Q_WS_MAC
        setWindowIcon(QIcon(":/mainwindow/icon"));
    #endif
    setWindowTitle(tr("Composer NCL 3.0"));
    mdiArea = new QMdiArea(this);
    mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(mdiArea);
    setUnifiedTitleAndToolBarOnMac(true);
    createStatusBar();
    createTreeProject();
    createActions();
    createMenus();
    createToolBar();
    createAbout();
    showMaximized();


}

void MainWindow::launchProjectWizard() {
    projectWizard = new ProjectWizard(this);

    connect(projectWizard,SIGNAL(infoReceived(QString,QString)),
            ProjectControl::getInstance(),SLOT(addProject(QString,QString)));
    projectWizard->init();
}

void MainWindow::launchAddDocumentWizard(QString projectId) {
    //TODO criar somente um ponteiro wizard que vai recebendo os diferentes
    //wizards
    documentWizard = new DocumentWizard(this);
    documentWizard->setProjectId(projectId);
    qDebug() << "MainWindow::launchAddDocumentWizard (" << projectId << ")";
    connect(documentWizard,SIGNAL(infoReceived(QString,QString,QString,bool)),
            ProjectControl::getInstance(),
            SLOT(addDocument(QString,QString,QString,bool)));
    documentWizard->init();
}



void MainWindow::createProjectInTree(QString name,QString location) {
    QTreeWidgetItem *item = new QTreeWidgetItem(projectTree);
    item->setIcon(0,QIcon(":/mainwindow/folderEmpty"));
    item->setText(1,name);
    item->setToolTip(1,location);
    projectTree->setCurrentItem(item); //TODO verificar se funciona

    /* The wizard is no long required
    if(projectWizard != NULL) {
        delete projectWizard;
        projectWizard = NULL;
    }*/

}

void MainWindow::createDocumentInTree(QString name,
                          QString location) {

    qDebug() << "MainWindow::createDocumentInTree (" << name << ", "
             << location << ")";
    QTreeWidgetItem *parent = projectTree->currentItem();
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setIcon(0,QIcon(":/mainwindow/document"));
    item->setText(1,name);
    item->setToolTip(1,location+QDir::separator()+name);
    projectTree->setCurrentItem(parent);
    parent->setExpanded(true);
    parent->setIcon(0,QIcon(":/mainwindow/folder"));
}


void MainWindow::createTreeProject() {
    dockTree = new QDockWidget(tr("Projects"), this);
    dockTree->setObjectName("treeProject");
    dockTree->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    projectTree = new ProjectTreeWidget(this);
    projectTree->setColumnCount(2);
    projectTree->setSortingEnabled(false);
    //projectTree->setHeaderLabel(tr("Name"));
    projectTree->headerItem()->setHidden(true);
    projectTree->setIconSize(QSize(35,35));
    projectTree->setContextMenuPolicy(Qt::DefaultContextMenu);
    dockTree->setWidget(projectTree);
    addDockWidget(Qt::LeftDockWidgetArea,dockTree,Qt::Horizontal);
    connect(projectTree,SIGNAL(addDocument(QString)), this,
            SLOT(launchAddDocumentWizard(QString)));
    connect(projectTree,SIGNAL(newProject()), this,
            SLOT(launchProjectWizard()));

}

void MainWindow::addProfileLoaded(QString name, QString fileName)
{
    qDebug() << "MainWindow::addProfileLoaded(" << name << ")";
    profilesExt->addItem(new QListWidgetItem(name+"("+fileName+")"));
}

void MainWindow::createMenus() {
     fileMenu = menuBar()->addMenu(tr("&File"));
     fileMenu->addAction(newProjectAct);
     fileMenu->addAction(newDocumentAct);

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
    profilesExt->setEnabled(false);

    QPushButton *bOk = new QPushButton(tr("OK"),aboutDialog);
    connect(bOk,SIGNAL(clicked()),aboutDialog,SLOT(close()));

    QGridLayout *gLayout = new QGridLayout(aboutDialog);
    gLayout->addWidget(new QLabel(tr("The <b>Composer</b> is an IDE for"
                                     " Declarative Multimedia languages."),
                                     aboutDialog));
    gLayout->addWidget(new QLabel(tr("<b>Profile Languages Loaded</b>"),
                                  aboutDialog));
    gLayout->addWidget(profilesExt);
    gLayout->addWidget(bOk);
    aboutDialog->setLayout(gLayout);



}

void MainWindow::about() {
    aboutDialog->exec();

 }

void MainWindow::errorDialog(QString message) {
    QMessageBox::warning(this,tr("Error!"),
                         tr(message.toStdString().c_str()));

 }

void MainWindow::createActions() {

    newProjectAct = new QAction(QIcon(":/mainwindow/openProject"),
                                tr("New &Project"), this);



     //newProjectAct->setShortcuts(QKeySequence("CTRL+P"));
     newProjectAct->setStatusTip(tr("Create a new Project"));
     connect(newProjectAct, SIGNAL(triggered()), this,
             SLOT(launchProjectWizard()));

     newDocumentAct = new QAction(QIcon(":/mainwindow/newDocument"),
                                  tr("New &Document"), this);
     newDocumentAct->setShortcuts(QKeySequence::New);
     newDocumentAct->setStatusTip(tr("Create a new NCL Document"));
//     connect(newDocumentAct, SIGNAL(triggered()), this,
//             SLOT(launchAddDocumentWizard(QString)));

     aboutComposerAct = new QAction(tr("&About"), this);
     aboutComposerAct->setStatusTip(tr("Show the application's About box"));
     connect(aboutComposerAct, SIGNAL(triggered()), this, SLOT(about()));

     separatorWindowAct = new QAction(this);
     separatorWindowAct->setSeparator(true);

     projectWindowAct = new QAction(QIcon(":/mainwindow/projectTree"),
                                    tr("Projects"),this);
     projectWindowAct->setCheckable(true);
     projectWindowAct->setChecked(dockTree->isVisible());

    connect(dockTree,SIGNAL(visibilityChanged(bool)),this,
             SLOT(updateWindowMenu()));
     connect(projectWindowAct,SIGNAL(triggered(bool)),dockTree,
             SLOT(setVisible(bool)));


     editPreferencesAct = new QAction(tr("&Preferences"), this);
     editPreferencesAct->setStatusTip(tr("Edit preferences"));
     connect (editPreferencesAct, SIGNAL(triggered()), this, SLOT(showEditPreferencesDialog()));
 }

void MainWindow::createStatusBar() {
     statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createToolBar() {
    fileTool = addToolBar("File");
    fileTool->addAction(newProjectAct);
    fileTool->addAction(newDocumentAct);
}

void MainWindow::updateWindowMenu()
 {
     windowMenu->clear();
     windowMenu->addAction(projectWindowAct);
     windowMenu->addAction(separatorWindowAct);
     projectWindowAct->setChecked(dockTree->isVisible());

     QList<QMdiSubWindow *> windows = mdiArea->subWindowList();
     separatorWindowAct->setVisible(!windows.isEmpty());

 }

void MainWindow::closeEvent(QCloseEvent *event)
 {

#ifdef Q_WS_MAC
    QSettings settings("telemidia.pucrio.br", "composer");
 #else
    QSettings settings("telemidia", "composer");
 #endif
    if (user_directory_ext != "") {
        settings.beginGroup("extension");
            settings.setValue("language",user_directory_ext);
        settings.endGroup();
    }
    if(ProjectControl::getInstance()->saveProjects()) {
     settings.beginGroup("mainwindow");
        settings.setValue("geometry", saveGeometry());
        settings.setValue("windowState", saveState());
     settings.endGroup();
     event->accept();
     } else {
        if ( QMessageBox::question(this,tr("Could not save some projects."),
                               tr("Do you want to exit anyway ?"))
            == QMessageBox::Ok) {
            event->accept();
        } else  {
            event->ignore();
        }
     }
    settings.sync();
 }

void MainWindow::showEditPreferencesDialog()
{
    preferences->show();
}


#include "ComposerMainWindow.h"
#include "ui_ComposerMainWindow.h"

#include <QPixmap>
#include <QDialogButtonBox>

using namespace composer::gui;

namespace composer {
    namespace gui {

ComposerMainWindow::ComposerMainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::ComposerMainWindow)
{
    ui->setupUi(this);

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
    connect(wsSwitch,SIGNAL(accepted()), SLOT(switchWorkspace()));
    initGUI();
    initModules();
    readSettings();
    preferences = new PreferencesDialog(this);

    ui->progressBar->setVisible(false);
    ui->progressBar->setRange(0, 99);

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(slotTimeout()));
}

ComposerMainWindow::~ComposerMainWindow() {
    if(fileSystemModel)
    {
        delete fileSystemModel;
        fileSystemModel = NULL;
    }
    delete ui;
}

void ComposerMainWindow::initModules()
{
    PluginControl  *pgControl = PluginControl::getInstance();
    LanguageControl *lgControl = LanguageControl::getInstance();
    DocumentControl *docControl = DocumentControl::getInstance();

    connect(pgControl,SIGNAL(notifyError(QString)),
            SLOT(errorDialog(QString)));
    connect(pgControl,SIGNAL(addPluginWidgetToWindow(IPluginFactory*,
                                                     IPlugin*,Document*, int)),
            SLOT(addPluginWidget(IPluginFactory*,IPlugin*,Document*, int)));

    connect(lgControl,SIGNAL(notifyError(QString)),
            SLOT(errorDialog(QString)));

    connect(docControl, SIGNAL(notifyError(QString)),
            SLOT(errorDialog(QString)));
    connect(docControl,SIGNAL(openDocumentTab(QString)),
            SLOT(onOpenDocumentTab(QString)));

    connect(docControl, SIGNAL(beginOpenDocument()),
            this, SLOT(beginOpenDocument()));

    connect(docControl, SIGNAL(endOpenDocument()), this,
            SLOT(endOpenDocument()));

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
    //    qDebug() << "MainWindow::readExtensions(" << user_directory_ext
    //            << ")";

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

void ComposerMainWindow::initGUI() {
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
                                 Document *doc, int n)
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
        w->setTabPosition(Qt::AllDockWidgetAreas, QTabWidget::West);
        int index = tabDocuments->addTab(w,projectId + "(" + documentId + ")");
        tabDocuments->setTabToolTip(index, location);
        documentsWidgets[location] = w;
    }

    QDockWidget *dock = new QDockWidget(fac->name());
    dock->setAllowedAreas(Qt::AllDockWidgetAreas);
    dock->setFeatures(QDockWidget::DockWidgetClosable |
                      QDockWidget::DockWidgetMovable);
    QWidget *pW = plugin->getWidget();
    dock->setWidget(pW);
    qDebug() << n;
    if (n%2)
        w->addDockWidget(Qt::RightDockWidgetArea, dock, Qt::Vertical);
    else
        w->addDockWidget(Qt::LeftDockWidgetArea, dock, Qt::Vertical);
    tabDocuments->setCurrentWidget(w);

    if(firstDock.contains(location)) {
        // w->tabifyDockWidget(firstDock[location], dock);
    }
    else firstDock[location] = dock;
}

void ComposerMainWindow::tabClosed(int index)
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

void ComposerMainWindow::closeAllFiles()
{
    while(tabDocuments->count())
    {
        tabDocuments->removeTab(0);
        tabClosed(0);
    }
}

void ComposerMainWindow::onOpenDocumentTab(QString location)
{
    if (!documentsWidgets.contains(location)) return;
    QMainWindow *w = documentsWidgets[location];
    tabDocuments->setCurrentWidget(w);
}

void ComposerMainWindow::switchWorkspace()
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

void ComposerMainWindow::createTreeProject()
{
    fileSystemDock = new QDockWidget(tr("Projects View"), this);
    fileSystemDock->setObjectName("projectTree");

    fileSystemView = new ProjectTreeView(this);
    connect(fileSystemView,
            SIGNAL(launchDocument(QString,QString)),
            DocumentControl::getInstance(),
            SLOT(launchDocument(QString,QString)));

    workspace_model = new WorkspaceModel(fileSystemView);

    fileSystemModel = new QFileSystemModel();

    switchWorkspace();

    fileSystemView->setSortingEnabled(false);
    fileSystemView->setHeaderHidden(true);
    fileSystemView->setColumnHidden(1,true);
    fileSystemView->setColumnHidden(2,true);
    fileSystemView->setColumnHidden(3,true);
    fileSystemDock->setWidget(fileSystemView);

    addDockWidget(Qt::LeftDockWidgetArea, fileSystemDock, Qt::Horizontal);

    projectViewAct = new QAction(QIcon(":/mainwindow/projectTree"),
                                 tr("Projects"),this);
    projectViewAct->setCheckable(true);
    projectViewAct->setChecked(fileSystemDock->isVisible());

    connect(fileSystemDock,SIGNAL(visibilityChanged(bool)),this,
            SLOT(updateViewMenu()));
    connect(projectViewAct,SIGNAL(triggered(bool)),fileSystemDock,
            SLOT(setVisible(bool)));

}

void ComposerMainWindow::createMenus() {
    ui->menu_File->addAction(newProjectAct);
    ui->menu_File->addAction(newDocumentAct);
    ui->menu_File->addSeparator();
    ui->menu_File->addAction(switchWS);
    ui->menu_File->addSeparator();
    // ui->menu_File->addAction(exitAct);

    ui->menu_Edit->addAction(editPreferencesAct);

    menuBar()->addSeparator();
    connect( ui->menu_Window, SIGNAL(aboutToShow()),
             this, SLOT(updateViewMenu()));

    connect(ui->action_Close_All, SIGNAL(triggered()),
            this, SLOT(closeAllFiles()));

    connect( ui->actionSave, SIGNAL(triggered()),
             this, SLOT(saveCurrentDocument()));
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

    connect(bOk,SIGNAL(rejected()),aboutDialog,SLOT(close()));

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
        /** FIXME: This should cause memory leaks */
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

    connect(ui->action_About_Plugins, SIGNAL(triggered()), this, SLOT(about()));

    fullScreenViewAct = new QAction(tr("&FullScreen"),this);
    //fullScreenViewAct->setShortcut();
    connect(fullScreenViewAct,SIGNAL(triggered()),this,
            SLOT(showCurrentWidgetFullScreen()));

    separatorViewAct = new QAction(this);
    separatorViewAct->setSeparator(true);

    editPreferencesAct = new QAction(tr("&Preferences"), this);
    editPreferencesAct->setStatusTip(tr("Edit preferences"));
    connect (editPreferencesAct, SIGNAL(triggered()), this,
             SLOT(showEditPreferencesDialog()));

    switchWS = new QAction(tr("Switch Workspace"),this);
    connect(switchWS,SIGNAL(triggered()),SLOT(showSwitchWorkspaceDialog()));

    connect(ui->toolButton_3, SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->action_Exit, SIGNAL(triggered()), this, SLOT(close()));
}

void ComposerMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void ComposerMainWindow::showCurrentWidgetFullScreen()
{
    //     tabDocuments->setWindowFlags(Qt::Window);
    //    tabDocuments->setWindowState(Qt::WindowFullScreen);
    tabDocuments->show();
    //    QWidget *w = tabDocuments->currentWidget();
    //    if (w != NULL)
    //    {
    //        w->setWindowFlags(Qt::Window);
    //        w->setWindowState(Qt::WindowFullScreen);
    //        w->show();
    //    }
}

void ComposerMainWindow::updateViewMenu()
{
    ui->menu_Window->clear();
    ui->menu_Window->addAction(projectViewAct);
    ui->menu_Window->addAction(separatorViewAct);
    ui->menu_Window->addAction(fullScreenViewAct);
    projectViewAct->setChecked(fileSystemDock->isVisible());
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

void ComposerMainWindow::cleanUp()
{
    LanguageControl::releaseInstance();
    DocumentControl::releaseInstance();
    PluginControl::releaseInstance();
}

void ComposerMainWindow::showEditPreferencesDialog()
{
    preferences->show();
}

void ComposerMainWindow::showSwitchWorkspaceDialog()
{
    wsSwitch->show();
}

void ComposerMainWindow::beginOpenDocument()
{
    qDebug() << "MainWindow::beginOpenDocument";
    ui->progressBar->setVisible(true);
    openingDocument = true;
    timer->start(10);
}

void ComposerMainWindow::endOpenDocument()
{
    openingDocument = false;
    timer->stop();
    ui->progressBar->setVisible(false);
}

void ComposerMainWindow::slotTimeout()
{
    qDebug() << "MainWindow::slotTimeout (out)" ;
    if(openingDocument){
        int v = ui->progressBar->value();
        qDebug() << "MainWindow::slotTimeout";
        v %= 100;
        ui->progressBar->setValue(v);
    }
}

void ComposerMainWindow::saveCurrentDocument()
{
     QString location = tabDocuments->tabToolTip(tabDocuments->currentIndex());
     DocumentControl::getInstance()->saveDocument(location);
     PluginControl::getInstance()->saveDocument(location);
}

} } //end namespace

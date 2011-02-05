#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtDebug>
#include <QSettings>
#include <QStringList>
#include <QMap>
#include <QFileSystemModel>


#include <QCoreApplication>
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

#include <core/modules/PluginControl.h>
#include <core/modules/DocumentControl.h>
#include <core/modules/LanguageControl.h>
using namespace composer::core::module;

#include "wizard/ProjectWizard.h"
#include "wizard/DocumentWizard.h"
#include "WorkspaceSwitch.h"
#include "PreferencesDialog.h"
#include "WorkspaceModel.h"
#include "ProjectTreeView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        QTabWidget *tabDocuments;
        QMap<QString,QMainWindow*> documentsWidgets;
        QMap<QString,QDockWidget*> firstDock;

        QMenu *fileMenu;
        QMenu *helpMenu;
        QMenu *windowMenu;
        QMenu *editMenu;

        //QToolBar *fileTool;

        QAction *newProjectAct;
        QAction *newDocumentAct;
        QAction *aboutComposerAct;
        QAction *aboutComposerPluginsAct;
        QAction *separatorWindowAct;
        QAction *projectWindowAct;
        QAction *editPreferencesAct;
        QAction *exitAct;
        QAction *switchWS;

        QListWidget *profilesExt;
        QListWidget *pluginsExt;

        ProjectWizard *projectWizard;
        DocumentWizard *documentWizard;
        PreferencesDialog *preferences;
        WorkspaceSwitch *wsSwitch;

        QFileSystemModel *fileSystemModel;
        WorkspaceModel *workspace_model;
        ProjectTreeView  *fileSystemView;
        QDockWidget      *fileSystemDock;


        QDialog *aboutDialog;

        QString defaultEx;
        QString user_directory_ext;
        QString work_space_path;

    private:
        QString promptChooseExtDirectory();
        void initModules();
        void initGUI();
        void createAbout();
        void createStatusBar();
        void createMenus();
        void createActions();
        void createTreeProject();
        void createFileSystem();
        void readSettings();
        void readExtensions();
        void closeEvent(QCloseEvent *event);
        void cleanUp();


    private slots:
        void about();
        void updateWindowMenu();
        void showEditPreferencesDialog();
        void showSwitchWorkspaceDialog();
        void tabClosed(int index);

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void errorDialog(QString);
        void addPluginWidget(IPluginFactory *fac, IPlugin *plugin,
                             Document *doc);
        void onOpenDocumentTab(QString location);
        void switchWorkspace();

    signals:
        void writeSettings();

};

#endif // MAINWINDOW_H

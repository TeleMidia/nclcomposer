#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtDebug>
#include <QCoreApplication>
#include <QSettings>
#include <QStringList>
#include <QtGui/QMainWindow>
#include <QtGui/QMdiArea>
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

#include <control/modules/ProjectControl.h>
#include <control/modules/PluginControl.h>
using namespace composer::core::module;

#include "wizard/ProjectWizard.h"
#include "wizard/DocumentWizard.h"
#include "ProjectTreeWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        QMdiArea *mdiArea;

        QMenu *fileMenu;
        QMenu *helpMenu;
        QMenu *windowMenu;

        QToolBar *fileTool;

        QAction *newProjectAct;
        QAction *newDocumentAct;
        QAction *aboutComposerAct;
        QAction *separatorWindowAct;
        QAction *projectWindowAct;

        ProjectTreeWidget *projectTree;
        QDockWidget *dockTree;

        QListWidget *profilesExt;
        //QListWidget *pluginsExt;

        ProjectWizard *projectWizard;
        DocumentWizard *documentWizard;

        QString defaultEx;
        QString user_directory_ext;

    private:
        QString promptChooseDirectory();
        void initModules();
        void initGUI();
        void createStatusBar();
        void createToolBar();
        void createMenus();
        void createActions();
        void createTreeProject();
        void readSettings();
        void readExtensions();
        void closeEvent(QCloseEvent *event);


    private slots:
        void about();
        void launchProjectWizard();
        void updateWindowMenu();

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();
    public slots:
        void createProjectInTree(QString name,QString location);
        void createDocumentInTree(QString name,
                                  QString location);
        void errorDialog(QString);
        void launchAddDocumentWizard(QString projectId);

    signals:
        void writeSettings();

};

#endif // MAINWINDOW_H

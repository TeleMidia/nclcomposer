#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtDebug>
#include <QSettings>
#include <QStringList>
#include <QMap>

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

#include <core/modules/ProjectControl.h>
#include <core/modules/PluginControl.h>
using namespace composer::core::module;

#include "wizard/ProjectWizard.h"
#include "wizard/DocumentWizard.h"
#include "ProjectTreeWidget.h"
#include "PreferencesDialog.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        QTabWidget *tabDocuments;
        QMap<QString,QMainWindow*> documentsWidgets;

        QMenu *fileMenu;
        QMenu *helpMenu;
        QMenu *windowMenu;
        QMenu *editMenu;

        QToolBar *fileTool;

        QAction *newProjectAct;
        QAction *newDocumentAct;
        QAction *aboutComposerAct;
        QAction *separatorWindowAct;
        QAction *projectWindowAct;
        QAction *editPreferencesAct;

        ProjectTreeWidget *projectTree;
        QDockWidget *dockTree;

        QListWidget *profilesExt;
        QListWidget *pluginsExt;

        ProjectWizard *projectWizard;
        DocumentWizard *documentWizard;
        PreferencesDialog *preferences;

        QDialog *aboutDialog;

        QString defaultEx;
        QString user_directory_ext;

    private:
        QString promptChooseDirectory();
        void initModules();
        void initGUI();
        void createAbout();
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
        void showEditPreferencesDialog();
        void addProfileLoaded(QString name, QString fileName);

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void createProjectInTree(QString name,QString location);
        void createDocumentInTree(QString name,
                                  QString location);
        void errorDialog(QString);
        void launchAddDocumentWizard();
        void addPluginWidget(IPlugin *plugin, QString documentId);

    signals:
        void writeSettings();

};

#endif // MAINWINDOW_H

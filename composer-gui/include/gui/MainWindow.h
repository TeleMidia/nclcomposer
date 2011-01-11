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

#include <control/util/CoreManager.h>
using namespace composer::core::util;

#include "wizard/MainWizard.h"
#include "wizard/ProjectWizard.h"
#include "wizard/DocumentWizard.h"
#include "ProjectTreeWidget.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

    private:
        QMdiArea *mdiArea;
        QMenu *fileMenu;
        QMenu *newMenu;
        QMenu *helpMenu;
        QMenu *windowMenu;
        QAction *newProjectAct;
        QAction *newDocumentAct;
        QAction *aboutComposerAct;
        QAction *separatorWindowAct;
        QAction *projectWindowAct;
        ProjectTreeWidget *projectTree;
        QDockWidget *dockTree;
        MainWizard *wizard;
        ProjectWizard *projectWizard;
        DocumentWizard *documentWizard;

    private:
         CoreManager *cManager;
    private:
        void initGUI();
        void createStatusBar();
        void createMenus();
        void createActions();
        void createTreeProject();
        void readSettings();
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
        void launchNewDocumentWizard(QString projectLocation);
        void launchAddDocumentWizard(QString projectId);

    signals:
        void writeSettings();

};

#endif // MAINWINDOW_H

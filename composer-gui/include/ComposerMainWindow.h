#ifndef COMPOSERMAINWINDOW_H
#define COMPOSERMAINWINDOW_H

#include <QtDebug>
#include <QSettings>
#include <QStringList>
#include <QMap>
#include <QFileSystemModel>


#include <QCoreApplication>
#include <QTimer>
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
#include <QWebView>

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

namespace Ui {
    class ComposerMainWindow;
}


namespace composer {
    namespace ui {

class ComposerMainWindow : public QMainWindow {
    Q_OBJECT

private:
    QTabWidget *tabDocuments;
    QMap<QString,QMainWindow*> documentsWidgets;
    QMap<QString,QDockWidget*> firstDock;

    QAction *newProjectAct;
    QAction *newDocumentAct;
    QAction *aboutComposerAct;
    QAction *fullScreenViewAct;
    QAction *separatorViewAct;
    QAction *projectViewAct;
    QAction *editPreferencesAct;
    QAction *switchWS;

    QListWidget *profilesExt;
    QTreeWidget *pluginsExt;

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

    QWebView *welcomeScreen;

    bool openingDocument;
    QTimer *timer;

private:
    Ui::ComposerMainWindow *ui;
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
    void updateViewMenu();
    void showEditPreferencesDialog();
    void showSwitchWorkspaceDialog();
    void tabClosed(int index);
    void showCurrentWidgetFullScreen();
    void closeAllFiles();
    void beginOpenDocument();
    void endOpenDocument();
    void slotTimeout();

public:
    explicit ComposerMainWindow(QWidget *parent = 0);
    ~ComposerMainWindow();

public slots:
    void errorDialog(QString);
    void addPluginWidget(IPluginFactory *fac, IPlugin *plugin,
                         Document *doc, int n);
    void onOpenDocumentTab(QString location);
    void switchWorkspace();

signals:
    void writeSettings();
    void progressBarValueChanged(int);

};

} } //end namespace

#endif // COMPOSERMAINWINDOW_H

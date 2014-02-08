#ifndef QNSTMAINWINDOW_H
#define QNSTMAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QPushButton>

#include <QDebug>

#include "View.h"

class QnstMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  QnstMainWindow(QWidget* parent = 0);
  virtual ~QnstMainWindow();

protected slots:
  void performNew();
  void performOpen();
  void performSave();
  void performSaveAs();
  void performExport();
  void performImport();
  void performClose();
  void performQuit();

  void performUndo();
  void performRedo();
  void performCut();
  void performCopy();
  void performPaste();

  void performPointer();
  void performLink();

  void performBody();
  void performContext();
  void performSwitch();
  void performMedia();

  void performPreferences();

  void performReport();
  void performAbout();

private:
  void createMenus();
  void createActions();
  void createToolbar();
  void createWidgets();
  void createStatusbar();
  void createConnections();

  QMenu* fileMenu;
  QMenu* editMenu;
  QMenu* windowMenu;
  QMenu* helpMenu;

  QToolBar* fileToolbar;
  QToolBar* editToolbar;
  QToolBar* drawToolbar;
  QToolBar* insertToolbar;

  QAction* newAction;
  QAction* openAction;
  QAction* saveAction;
  QAction* saveAsAction;
  QAction* importAction;
  QAction* exportAction;
  QAction* closeAction;
  QAction* quitAction;

  QAction* undoAction;
  QAction* redoAction;
  QAction* cutAction;
  QAction* copyAction;
  QAction* pasteAction;

  QAction* pointerAction;
  QAction* linkAction;

  QAction* mediaAction;
  QAction* contextAction;
  QAction* switchAction;
  QAction* bodyAction;
  QAction* areaAction;
  QAction* propertyAction;
  QAction* portAction;
  QAction* switchPortAction;

  QAction* preferencesAction;

  QAction* reportAction;
  QAction* aboutAction;

  QActionGroup* drawActionGroup;

  QnstView* view;
};

#endif // QNSTMAINWINDOW_H

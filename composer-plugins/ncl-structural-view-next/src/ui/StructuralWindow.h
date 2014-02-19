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

#include "StructuralView.h"

class StructuralWindow : public QMainWindow
{
  Q_OBJECT

public:
  StructuralWindow(QWidget* parent = 0);
  virtual ~StructuralWindow();

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
  void createActions();
  void createToolbar();
  void createWidgets();
  void createStatusbar();
  void createConnections();

  QToolBar* fileToolbar;
  QToolBar* editToolbar;
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

  QAction* selectionAction;
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

  QActionGroup* insertActionGroup;

  StructuralView* view;
};

#endif // QNSTMAINWINDOW_H

#ifndef QNSTMAINWINDOW_H
#define QNSTMAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QPushButton>

#include "StructuralView.h"
#include "StructuralUtil.h"

class StructuralWindow : public QMainWindow
{
  Q_OBJECT

public:
  StructuralWindow(QWidget* parent = 0);
  virtual ~StructuralWindow();

  StructuralView* getView();
  void setView(StructuralView* view);

protected slots:
  void performUndo();
  void performRedo();
  void performCut();
  void performCopy();
  void performPaste();
  void performZoomIn();
  void performZoomOut();
  void performPointer();
  void performLink();
  void performBody();
  void performContext();
  void performSwitch();
  void performMedia();
  void performPort();
  void performArea();
  void performSwitchPort();
  void performProperty();
  void performMinimap();
  void performSnapshot();

  void changeUndoState(bool enable);
  void changeRedoState(bool enable);
  void changeCutState(bool enable);
  void changeCopyState(bool enable);
  void changePasteState(bool enable);
  void changeBodyState(bool enable);
  void changeZoomInState(bool enable);

  void changeLinkState(bool enable);

  void changeSelect(QString uid);

  void insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void remove(QString uid, QMap<QString, QString> settings);
  void change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void select(QString uid, QMap<QString, QString> settings);

private:
  void createActions();
  void createToolbar();
  void createWidgets();
  void createStatusbar();
  void createConnections();

  QMenu* _editMenu;
  QMenu* _insertMenu;
  QMenu* _windowMenu;
  QMenu* _helpMenu;

  QToolBar* _editToolbar;
  QToolBar* _insertToolbar;
  QToolBar* _windowToolbar;

  QAction* _undoAction;
  QAction* _redoAction;
  QAction* _cutAction;
  QAction* _copyAction;
  QAction* _pasteAction;
  QAction* _zoominAction;
  QAction* _zoomoutAction;
  QAction* _pointerAction;
  QAction* _linkAction;
  QAction* _mediaAction;
  QAction* _contextAction;
  QAction* _switchAction;
  QAction* _bodyAction;
  QAction* _areaAction;
  QAction* _propertyAction;
  QAction* _portAction;
  QAction* _switchportAction;
  QAction* _minimapAction;
  QAction* _snapshotAction;
  QAction* _preferencesAction;
  QAction* _reportAction;
  QAction* _aboutAction;
  QActionGroup* _insertActionGroup;

  StructuralView* _view;
};

#endif // QNSTMAINWINDOW_H

#ifndef QNSTMAINWINDOW_H
#define QNSTMAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QAction>
#include <QActionGroup>

#include "StructuralView.h"
#include "StructuralUtil.h"

class StructuralWindow : public QMainWindow
{
  Q_OBJECT

public:
  StructuralWindow(QWidget* parent = 0);
  virtual ~StructuralWindow();

  StructuralView* getView() const;
  void setView(StructuralView* view);

protected slots:
  void switchUndo(bool enable);
  void switchRedo(bool enable);
  void switchCut(bool enable);
  void switchCopy(bool enable);
  void switchPaste(bool enable);
  void switchZoomIn(bool enable);
  void switchZoomOut(bool enable);
  void switchPointer(bool enable);
  void switchLink(bool enable);
  void switchBody(bool enable);

  void select(QString uid, QMap<QString, QString> settings);

private:
  void createActions();
  void createToolbar();
  void createWidgets();
  void createStatusbar();
  void createConnections();

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
  QAction* _snapshotAction;
  QAction* _minimapAction;
  QAction* _mediaAction;
  QAction* _contextAction;
  QAction* _switchAction;
  QAction* _bodyAction;
  QAction* _areaAction;
  QAction* _propertyAction;
  QAction* _portAction;
  QAction* _switchportAction;
  QAction* _preferencesAction;

  QActionGroup* _insertActionGroup;

  StructuralView* _view;
};

#endif // QNSTMAINWINDOW_H

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
  void switchUndo(bool state);
  void switchRedo(bool state);
  void switchCut(bool state);
  void switchCopy(bool state);
  void switchPaste(bool state);
  void switchDelete(bool state);
  void switchSnapshot(bool state);
  void handleZoomChanged(int zoom);
  void switchPointer(bool state);
  void switchLink(bool state);
  void switchBody(bool state);

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
  QAction* _deleteAction;
  QAction* _snapshotAction;
  QAction* _zoominAction;
  QAction* _zoomoutAction;
  QAction* _zoomresetAction;
  QAction* _pointerAction;
  QAction* _linkAction;
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

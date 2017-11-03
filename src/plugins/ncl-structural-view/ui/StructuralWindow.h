#ifndef QNSTMAINWINDOW_H
#define QNSTMAINWINDOW_H

#include <QAction>
#include <QActionGroup>
#include <QMainWindow>
#include <QToolBar>

#include "StructuralUtil.h"
#include "StructuralView.h"

class StructuralWindow : public QMainWindow
{
  Q_OBJECT

public:
  StructuralWindow (QWidget *parent = 0);
  virtual ~StructuralWindow () {}

  StructuralView *getView () const;
  void setView (StructuralView *view);

protected slots:
  void handleZoomChange (int zoom);
  void handleInteractionModeChange (StructuralInteractionMode state);

  void select (QString uid, QStrMap stgs);

private:
  void createActions ();
  void createToolbar ();
  void createWidgets ();
  void createConnections ();

  QToolBar *_editToolbar, *_insertToolbar, *_windowToolbar;

  QAction *_undoAction, *_redoAction;
  QAction *_cutAction, *_copyAction, *_pasteAction, *_deleteAction;

  QAction *_snapshotAction, *_zoominAction, *_zoomoutAction, *_zoomresetAction,
      *_minimapAction;

  QAction *_pointerAction, *_linkAction;

  QAction *_mediaAction, *_contextAction, *_switchAction, *_bodyAction,
      *_areaAction, *_propertyAction, *_portAction, *_switchportAction;

  QAction *_autostartAction, *_propertiesAction;

#ifdef WITH_GRAPHVIZ
  QAction *_autoadjustAction;
#endif

  QAction *_preferencesAction;

  QActionGroup *_insertActionGroup;

  StructuralView *_view;
};

#endif // QNSTMAINWINDOW_H

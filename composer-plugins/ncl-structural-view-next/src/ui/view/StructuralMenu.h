#ifndef QNSTMENU_H
#define QNSTMENU_H

#include <QMenu>

#include "Structural.h"
#include "StructuralUtil.h"

class StructuralMenu : public QMenu
{
  Q_OBJECT

public:
  StructuralMenu(QWidget* parent = 0);

  ~StructuralMenu();

private slots:
  void dispatchAddAction();

signals:
  void helpRequested();

  // void undoRequested();

  // void redoRequested();

  void cutRequested();

  void copyRequested();

  void pasteRequested();

  void deleteRequested();

  void exportRequested();

  void zoominRequested();

  void zoomoutRequested();

  void zoomresetRequested();

  void fullscreenRequested();

  void bringfrontRequested();

  void bringforwardRequested();

  void sendbackwardRequested();

  void sendbackRequested();

  void hideRequested();

  void propertiesRequested();

  void autoRequested();

  /*!
   * This signal says that the user wants to add an entity of the type passed
   * as parameter.
   */
  void menuAddEntityTriggered(QnstSubtype);

private:
  void createActions();

  void createMenus();

  void createConnections();

public:
  QMenu* menuView;

  QMenu* menuInsert;

  QMenu* menuShow;

  QMenu* menuArrange;

  QMenu* menuAdjust;

  QAction* actionHelp;

  // QAction* actionUndo;

  QAction* actionRedo;

  QAction* actionCut;

  QAction* actionCopy;

  QAction* actionPaste;

  QAction* actionDelete;

  QAction* actionExport;

  QAction* actionZoomIn;

  QAction* actionZoomOut;

  QAction* actionZoomReset;

  QAction* actionFullscreen;

  QAction* actionBringfront;

  QAction* actionBringforward;

  QAction* actionSendbackward;

  QAction* actionSendback;

  QAction* actionHide;

  QAction* actionAuto;

  QAction* actionProperties;

  /* Add actions */
  QAction* actionAddMedia;

  QAction* actionAddContext;

  QAction* actionAddSwitch;

  QAction* actionAddBody;

  QAction* actionAddPort;

  QAction* actionAddArea;

  QAction* actionAddProperty;

  QAction* actionAddSwitchPort;

  QAction* actionAddAggregator;
};

#endif // QNSTMENU_H

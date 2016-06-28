#ifndef QNSTMENU_H
#define QNSTMENU_H

#include <QMenu>

#include "Structural.h"
#include "StructuralUtil.h"

class StructuralMenu : public QMenu
{
  Q_OBJECT

public:
  StructuralMenu(QWidget* _parent = 0);
  virtual ~StructuralMenu();

  void updateInsertAction(StructuralType type, bool enableBody = false);

signals:
  void insert(StructuralType);

private:
  void createActions();
  void createMenus();
  void createConnections();

private slots:
  void performBody();
  void performContext();
  void performSwitch();
  void performMedia();
  void performPort();
  void performArea();
  void performSwitchPort();
  void performProperty();

public slots:
  void changeUndoState(bool enable);
  void changeRedoState(bool enable);
  void changeCutState(bool enable);
  void changeCopyState(bool enable);
  void changePasteState(bool enable);

public:
  QMenu* insertMenu;

  QAction* helpAction;
  QAction* undoAction;
  QAction* redoAction;
  QAction* cutAction;
  QAction* copyAction;
  QAction* pasteAction;
  QAction* deleteAction;
  QAction* snapshotAction;
  QAction* mediaAction;
  QAction* contextAction;
  QAction* switchAction;
  QAction* bodyAction;
  QAction* portAction;
  QAction* areaAction;
  QAction* propertyAction;
  QAction* switchPortAction;
  QAction* propertiesAction;
};

#endif // QNSTMENU_H

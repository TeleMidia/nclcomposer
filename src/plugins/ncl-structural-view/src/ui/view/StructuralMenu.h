#ifndef STRUCTURALMENU_H
#define STRUCTURALMENU_H

#include <QMenu>

#include "StructuralUtil.h"
#include "StructuralEntity.h"

class StructuralEntity;

class StructuralMenu : public QMenu
{
  Q_OBJECT

public:
  StructuralMenu(QWidget* parent = 0);
  virtual ~StructuralMenu();

  qreal getInsertTop() const;
  void setInsertTop(qreal insertTop);

  qreal getInsertLeft() const;
  void setInsertLeft(qreal insertLeft);

  void adjust(StructuralType type = Structural::NoType);

public slots:
  void switchHelp(bool state);
  void switchAutostart(bool state);
  void switchAutostartProperty(bool state);
  void switchUndo(bool state);
  void switchRedo(bool state);
  void switchCut(bool state);
  void switchCopy(bool state);
  void switchPaste(bool state);
  void switchDelete(bool state);
  void switchSnapshot(bool state);
  void switchMedia(bool state);
  void switchContext(bool state);
  void switchSwitch(bool state);
  void switchBody(bool state);
  void switchArea(bool state);
  void switchProperty(bool state);
  void switchPort(bool state);
  void switchSwitchPort(bool state);
  void switchProperties(bool state);

#ifdef WITH_GRAPHVIZ
  void switchAutoAdjust(bool state);
#endif

signals:
  void performedHelp();
  void performedAutostart();
  void performedUndo();
  void performedRedo();
  void performedCut();
  void performedCopy();
  void performedPaste();
  void performedDelete();
  void performedSnapshot();  
  void performedMedia();
  void performedContext();
  void performedSwitch();
  void performedBody();
  void performedArea();
  void performedProperty();
  void performedPort();
  void performedSwitchPort();
  void performedProperties();

#ifdef WITH_GRAPHVIZ
  void performedAutoAdjust();
#endif

  void performedInsert(StructuralType type, QMap<QString, QString> properties);

private slots:
  void performMedia();
  void performContext();
  void performSwitch();
  void performBody();
  void performArea();
  void performProperty();
  void performPort();
  void performSwitchPort();

private:
  void createActions();
  void createMenus();
  void createConnections();

  QMenu* _insertMenu;

  QAction* _helpAction;
  QAction* _undoAction;
  QAction* _redoAction;
  QAction* _cutAction;
  QAction* _copyAction;
  QAction* _pasteAction;
  QAction* _deleteAction;
  QAction* _snapshotAction;
  QAction* _autostartAction;
  QAction* _mediaAction;
  QAction* _contextAction;
  QAction* _switchAction;
  QAction* _bodyAction;
  QAction* _portAction;
  QAction* _areaAction;
  QAction* _propertyAction;
  QAction* _switchPortAction;
  QAction* _propertiesAction;

#ifdef WITH_GRAPHVIZ
  QAction* _autoadjustAction;
#endif

  qreal _insertTop;
  qreal _insertLeft;
};

#endif // STRUCTURALMENU_H

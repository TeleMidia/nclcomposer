#ifndef QNSTVIEW_H
#define QNSTVIEW_H

#include <QGraphicsView>
#include <QVector>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>
#include <QWheelEvent>
#include <QSet>
#include <QUndoStack>
#include <QDir>
#include <QGraphicsScene>

#include <QDebug>

#include "StructuralScene.h"
#include "StructuralViewLink.h"
#include "StructuralEntity.h"
#include "StructuralMedia.h"
#include "StructuralInterface.h"
#include "StructuralReference.h"
#include "StructuralLink.h"
#include "StructuralBind.h"
#include "StructuralComposition.h"

#include "Remove.h"
#include "Change.h"
#include "Insert.h"

class QnstAddCommand;
class StructuralScene;

class StructuralView : public QGraphicsView
{
  Q_OBJECT

public:
  StructuralView(QWidget* parent = 0);
  virtual ~StructuralView();

public:
  bool hasEntity(QString uid);
  StructuralEntity* getEntity(QString uid);
  QMap<QString, StructuralEntity*> entities;

  void setEnableLink(bool status);

  void setAction(QString action);
  void setCondition(QString condition);

  void serialize(QString &data);
  void exportDataFromEntity(StructuralEntity* entity, QDomDocument* doc, QDomElement parent);

public slots:
  void insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void remove(QString uid, QMap<QString, QString> settings);
  void change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void select(QString uid, QMap<QString, QString> settings);

  void create(QnstSubtype subtype, QMap<QString, QString> properties, QMap<QString, QString> settings);

  void performHelp();

  void performCut();
  void performCopy();
  void performPaste();

  void performUndo();
  void performRedo();

  void performSnapshot();

  void performDelete();

  void performZoomIn();
  void performZoomOut();
  void performZoomReset();
  void performFullscreen();

  void performBringfront();
  void performBringforward();
  void performSendback();
  void performSendbackward();

  void performHide();

  void performProperties();



signals:
  void inserted(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void removed(QString uid, QMap<QString, QString> settings);
  void changed(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void selected(QString uid, QMap<QString, QString> settings);

  void viewChanged();

protected:

  bool updateEntityWithUniqueNstId(StructuralEntity *entity);

  void resizeEvent(QResizeEvent *event);

  virtual void mouseMoveEvent(QMouseEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);

  virtual void mouseReleaseEvent(QMouseEvent*event);

  virtual void keyPressEvent(QKeyEvent *event);

  virtual void keyReleaseEvent(QKeyEvent *event);

  virtual void focusOutEvent(QFocusEvent *event);

  void wheelEvent(QWheelEvent * event);

public slots:
  void clearAllData();

private:
  void performPaste(StructuralEntity* entity, StructuralEntity* parent, QString CODE);

  bool isChild(StructuralEntity* e , StructuralEntity* p);
  void createLink(StructuralEntity* a, StructuralEntity* b);
  void createBind(StructuralEntity* a, StructuralEntity* b,QString type = "", QString code = "");
  void createReference(StructuralEntity* a, StructuralEntity* b);
  Command* rmcmd(StructuralEntity* entity, Command* cmdparent, QMap<QString, QString> settings);

  void createObjects();

  void createConnection();

  void collapseCompositions(QDomElement element, QDomElement parent);

  void deletePendingEntities();

  bool modified;

  bool linking;

  int zoomStep;

  bool hasCutted;

  QAction* undoAct;

  QAction* redoAct;

  StructuralScene* scene;

  StructuralViewLink* link;

  StructuralEntity* _selected;

  StructuralEntity* clipboard;

  QString clip;

  QSet<QString> linkWriterAux;

  QMap<QString, StructuralLink*> links;

  QMap<QString, StructuralBind*> binds;

  QMap<QString, QString> refers;

  QMap<QString, QString> importBases; // importBaseUid - ConnUid

  QMap<QString, QString> bindParamUIDToBindUID;

  //QMap<QString, QnstConnector*> connectors2; // uid - conn

  //QMap<QString, QnstConnector*> connectors; // id - conn

  QVector<StructuralEntity*> toDelete;

  QUndoStack commnads;

  StructuralEntity* lastLinkMouseOver;

  std::map < Structural::EntitySubtype, int > entityCounter;

  static std::map <Structural::EntitySubtype, QString> mediaTypeToXMLStr;

  //  MiniMap *minimap;

  QString action;
  QString condition;
};

#endif // QNSTVIEW_H

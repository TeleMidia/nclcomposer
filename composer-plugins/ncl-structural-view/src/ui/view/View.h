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

#include "Scene.h"
#include "ViewLink.h"
#include "Entity.h"
#include "Media.h"
#include "Interface.h"
#include "Reference.h"
#include "Link.h"
#include "Bind.h"
#include "Composition.h"

#include "RemoveCommand.h"
#include "ChangeCommand.h"
#include "InsertCommand.h"

class QnstAddCommand;

class QnstView : public QGraphicsView
{
  Q_OBJECT

public:
  QnstView(QWidget* parent = 0);
  ~QnstView();

public:
  bool hasEntity(QString uid);
  QnstEntity* getEntity(QString uid);
  QMap<QString, QnstEntity*> entities;

  void setEnableLink(bool status);

  void setAction(QString action);
  void setCondition(QString condition);


public slots:
  void insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void remove(QString uid, QMap<QString, QString> settings);
  void change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void select(QString uid, QMap<QString, QString> settings);

  void create(QnstSubtype subtype);

  void performHelp();

  void performCut();
  void performCopy();
  void performPaste();

  void performUndo();
  void performRedo();

  void performExport();

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

  bool updateEntityWithUniqueNstId(QnstEntity *entity);

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
  void performPaste(QnstEntity* entity, QnstEntity* parent, QString CODE);

  bool isChild(QnstEntity* e , QnstEntity* p);
  void createLink(QnstEntity* a, QnstEntity* b);
  void createBind(QnstEntity* a, QnstEntity* b,QString type = "", QString code = "");
  void createReference(QnstEntity* a, QnstEntity* b);
  QnstCommand* rmcmd(QnstEntity* entity, QnstCommand* cmdparent, QMap<QString, QString> settings);

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

  QnstScene* scene;

  QnstViewLink* link;

  QnstEntity* _selected;

  QnstEntity* clipboard;

  QString clip;

  QSet<QString> linkWriterAux;

  QMap<QString, QnstLink*> links;

  QMap<QString, QnstBind*> binds;

  QMap<QString, QString> refers;

  QMap<QString, QString> importBases; // importBaseUid - ConnUid

  QMap<QString, QString> bindParamUIDToBindUID;

  //QMap<QString, QnstConnector*> connectors2; // uid - conn

  //QMap<QString, QnstConnector*> connectors; // id - conn

  QVector<QnstEntity*> toDelete;

  QUndoStack commnads;

  QnstEntity* lastLinkMouseOver;

  std::map < Qnst::EntitySubtype, int > entityCounter;

  static std::map <Qnst::EntitySubtype, QString> mediaTypeToXMLStr;

//  MiniMap *minimap;

  QString action;
  QString condition;
};

#endif // QNSTVIEW_H

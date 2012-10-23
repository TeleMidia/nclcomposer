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

#include <QDebug>

#include "qnstscene.h"
#include "qnstviewlink.h"
#include "qnstgraphicsentity.h"
#include "qnstgraphicsbody.h"
#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicsreference.h"
#include "qnstgraphicslink.h"
#include "qnstgraphicslinkdialog.h"
#include "qnstgraphicsconditiondialog.h"
#include "qnstgraphicsactiondialog.h"
#include "qnstconnector.h"
#include "qnstlink.h"
#include "qnstbind.h"
#include "qnstgraphicsproperty.h"
#include "qnstgraphicsbind.h"
#include "qnstgraphicsarea.h"
#include "qnstaddcommand.h"
#include "qnstchangecommand.h"
#include "qnstremovecommand.h"
#include "qnstgraphicsmapping.h"

class QnstAddCommand;

//
// ATTENTION: This code needs a refactoring.
//

class QnstView : public QGraphicsView
{
  Q_OBJECT

public:
  QnstView(QWidget* parent = 0);

  ~QnstView();

  void load(QString data);

  QString serialize();

  void read(QDomElement element, QDomElement parent);

  void readLink(QDomElement element, QDomElement parent);

  void write(QDomElement element, QDomDocument* dom, QnstGraphicsEntity* entity);

  void writeLink(QDomElement element, QDomDocument* dom, QnstGraphicsEntity* entity);

public:
  void addEntity(const QString uid, const QString parent,
                 const QMap<QString, QString> &properties, bool undo = false,
                 bool adjust = false);

  void removeEntity(const QString uid, bool undo = false, bool rmRef = true);

  void changeEntity(const QString uid, const QMap<QString, QString> &properties,
                    bool adjust=true);

  void selectEntity(const QString uid);

  bool hasEntity(QString uid);

  QnstEntity* getEntity(QString uid);

  QMap<QString, QnstGraphicsEntity*> entities;

public slots:
  void performHelp();

  void performUndo();

  void performRedo();

  void performCut();

  void performCopy();

  void performPaste();

  void performDelete();

  void performExport();

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
  void entityAdded(const QString uid, const QString parent, const QMap<QString, QString> properties);

  void entityChanged(const QString uid, const QMap<QString, QString> properties);

  void entityRemoved(const QString uid);

  void entitySelected(const QString uid);

  void viewChanged();

protected:
  bool updateEntityWithUniqueNstId(QnstGraphicsEntity *entity);

  virtual void mouseMoveEvent(QMouseEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);

  virtual void mouseReleaseEvent(QMouseEvent*event);

  virtual void keyPressEvent(QKeyEvent *event);

  virtual void keyReleaseEvent(QKeyEvent *event);

  virtual void focusOutEvent(QFocusEvent *event);

  void wheelEvent(QWheelEvent * event);

public slots:
  void requestEntityAddition(QnstGraphicsEntity* entity, bool undo = false);

  void requestEntityRemotion(QnstGraphicsEntity* entity, bool undo = false, bool rmRefs = true);

  void requestEntityChange(QnstGraphicsEntity* entity);

  void requestEntityPreparation(QnstGraphicsEntity* entity, QMap<QString, QString> properties);

  void requestEntitySelection(QnstGraphicsEntity* entity);

  void requestBindParamAdjust(QString uid, QString parent, QMap<QString, QString> properties);

  void updateBindParams(QString bindUID,QMap<QString, QString> params, QMap<QString, QString> name_uids);

  void markError(QString uid);

  void clearValidationErrors();

private:
  void addImportBase(QString uid, const QMap<QString, QString> &properties);

  void changeImportBase(QString uid, const QMap<QString, QString> &properties);

  void adjustContext(QnstGraphicsContext* entity);

  void adjustMedia(QnstGraphicsMedia* entity);

  void adjustPort(QnstGraphicsPort* entity);

  void adjustMapping(QnstGraphicsMapping* entity);

  void changeLink(QnstLink* entity, const QMap<QString, QString> &properties);

  void adjustLink(QnstLink* entity);

  void changeBind(QnstBind* entity, const QMap<QString, QString> &properties);

  void adjustBind(QnstBind* entity);

  void addConnector(const QString uid, const QString parent, const QMap<QString, QString> &properties);

  void changeConnector(QnstConnector* entity, const QMap<QString, QString> &properties);

  void addCondition(const QString uid, const QString parent, const QMap<QString, QString> &properties);

  void changeCondition(QString condition, const QMap<QString, QString> &properties);

  void addAction(const QString uid, const QString parent, const QMap<QString, QString> &properties);

  void changeAction(QString condition, const QMap<QString, QString> &properties);

  void adjustConnector(QnstConnector* entity);

  void changeBindParam(const QString uid, const QMap<QString, QString> &properties);

  void addConnectorParam(const QString uid, const QString parent, const QMap<QString, QString> &properties);

  void changeConnectorParam(const QString uid, const QMap<QString, QString> &properties);

  void performCopy(QnstGraphicsEntity* entity, QnstGraphicsEntity* parent);

  void performPaste(QnstGraphicsEntity* copy, QnstGraphicsEntity* parent,
                    bool makeRefer = false, bool keepOriginalPos = false);

  void performReference(QnstGraphicsEntity* entity, QnstGraphicsEntity* referred);

  void createObjects();

  void createConnection();

  void readImportBase(QString uid, QDomElement e, const QString alias);

  void readConnector(QDomElement e, QnstConnector* conn);

  void addNodetoNodeEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb);

  void addNodetoInterfaceEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb);

  void addInterfacetoNodeEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb);

  void addInterfacetoNodeLink(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb);

  void addInterfacetoInterfaceEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb);

  void adjustAngle(QnstGraphicsEdge* edge, QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb);

  void collapseCompositions(QDomElement element, QDomElement parent);

  QString createNewConnector(QString condition, QString action);

  void createReference(QnstGraphicsEntity* entitya,
                       QnstGraphicsEntity* entityb);

  void createMapping(QnstGraphicsEntity* entitya,
                     QnstGraphicsEntity* entityb);

  QnstGraphicsAggregator* createAggregator(QnstGraphicsEntity* entitya,
                                           QnstGraphicsEntity* entityb);

  void createActionWithDialog(QnstGraphicsEntity* entitya,
                              QnstGraphicsEntity* entityb);

  void createConditionWithDialog(QnstGraphicsEntity* entitya,
                                 QnstGraphicsEntity* entityb);

  void createLinkWithDialog(QnstGraphicsEntity* entitya,
                            QnstGraphicsEntity* entityb);

  void createNoGraphicalLink(QnstGraphicsEntity* entitya,
                             QnstGraphicsEntity* entityb,
                             QnstGraphicsAggregator* aggregator,
                             QnstGraphicsBind* condition,
                             QnstGraphicsBind* actionDialog,
                             QString connector);

  QnstGraphicsBind* createBind(QnstGraphicsEntity* entitya,
                               QnstGraphicsEntity* entityb,
                               QString condition,
                               QString action);

  QnstBind* createNoGraphicalBind(QnstGraphicsEntity* entitya,
                             QnstGraphicsEntity* entityb,
                             QnstLink* link,
                             QnstGraphicsBind* bind,
                             QString type);

  bool modified;

  int nlink;

  bool linking;

  int zoomStep;

  bool hasCutted;

  QAction* undoAct;

  QAction* redoAct;

  QnstScene* scene;

  QnstViewLink* link;

  QnstGraphicsLinkDialog* linkDialog;

  QnstGraphicsConditionDialog* conditionDialog;

  QnstGraphicsActionDialog* actionDialog;

  QnstGraphicsEntity* selected;

  QnstGraphicsEntity* clipboard;

  QSet<QString> linkWriterAux;

  QMap<QString, QnstLink*> links;

  QMap<QString, QnstBind*> binds;

  QMap<QString, QString> brelations;

  QMap<QString, QString> link2conn;

  QMap<QString, QString> refers;

  QMap<QString, QString> importBases; // importBaseUid - ConnUid

  QMap<QString, QString> interfaceRefers;

  QMap<QString, QString> bindParamUIDToBindUID;

  QMap<QString, QnstConnector*> connectors2; // uid - conn

  QMap<QString, QnstConnector*> connectors; // id - conn

  QUndoStack history;

  QnstGraphicsEntity* lastLinkMouseOver;

  std::map < Qnst::EntityType, int > entityCounter;

  static std::map <Qnst::EntityType, QString> mediaTypeToXMLStr;
};

#endif // QNSTVIEW_H

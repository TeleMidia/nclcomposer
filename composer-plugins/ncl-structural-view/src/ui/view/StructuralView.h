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

#include "StructuralScene.h"
#include "StructuralViewLink.h"
#include "StructuralGraphicsEntity.h"
#include "StructuralGraphicsBody.h"
#include "StructuralGraphicsContext.h"
#include "StructuralGraphicsSwitch.h"
#include "StructuralGraphicsMedia.h"
#include "StructuralGraphicsPort.h"
#include "StructuralGraphicsReference.h"
#include "StructuralGraphicsLink.h"
#include "StructuralGraphicsLinkDialog.h"
#include "StructuralGraphicsConditionDialog.h"
#include "StructuralGraphicsActionDialog.h"
#include "StructuralConnector.h"
#include "StructuralGraphicsProperty.h"
#include "StructuralGraphicsBind.h"
#include "StructuralGraphicsArea.h"
#include "Insert.h"
#include "Change.h"
#include "Remove.h"
#include "StructuralGraphicsMapping.h"

#include "StructuralMiniMap.h"

class Insert;
class StructuralMiniMap;

// #define SMOOTH_ZOOM 1
//
// ATTENTION: This code needs a refactoring.
//

class StructuralView : public QGraphicsView
{
  Q_OBJECT

public:
  StructuralView(QWidget* parent = 0);

  ~StructuralView();

  void load(QString data);

  QString serialize();

  void read(QDomElement element, QDomElement parent);

  void readLink(QDomElement element, QDomElement parent);

  void write(QDomElement element, QDomDocument* dom, StructuralGraphicsEntity* entity);

  void writeLink(QDomElement element, QDomDocument* dom, StructuralGraphicsEntity* entity);

  inline StructuralScene *getScene() { return this->scene; }

public:
  void addEntity(const QString uid, const QString parent,
                 const QMap<QString, QString> &properties, bool undo = false,
                 bool adjust = true);

  void removeEntity(const QString uid, bool undo = false, bool rmRef = true);

  void changeEntity(const QString uid, const QMap<QString, QString> &properties,
                    bool adjust=true);

  void selectEntity(const QString uid);

  bool hasEntity(QString uid);

  StructuralEntity* getEntity(QString uid);

  QMap<QString, StructuralGraphicsEntity*> entities;

public slots:
  void performHelp();

  /* void performUndo();

  void performRedo(); */

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

  void setMinimapVisible(bool v = true);

signals:
  void entityAdded(const QString uid, const QString parent, const QMap<QString, QString> properties);

  void entityChanged(const QString uid, const QMap<QString, QString> properties);

  void entityRemoved(const QString uid);

  void entitySelected(const QString uid);

  void viewChanged();

protected:

  bool updateEntityWithUniqueNstId(StructuralGraphicsEntity *entity);

  void resizeEvent(QResizeEvent *event);

  virtual void mouseMoveEvent(QMouseEvent* event);

  virtual void mousePressEvent(QMouseEvent* event);

  virtual void mouseReleaseEvent(QMouseEvent*event);

  virtual void keyPressEvent(QKeyEvent *event);

  virtual void keyReleaseEvent(QKeyEvent *event);

  virtual void focusOutEvent(QFocusEvent *event);

  void wheelEvent(QWheelEvent * event);

#ifdef SMOOTH_ZOOM
  int _numScheduledScalings;
#endif

public slots:
#ifdef SMOOTH_ZOOM
  void scalingTime(qreal x);
  void animFinished();
#endif

  void requestEntityAddition(StructuralGraphicsEntity* entity, bool undo = false);

  void requestEntityRemotion(StructuralGraphicsEntity* entity, bool undo = false,
                             bool rmRefs = true, bool notify = true);

  void requestEntityChange(StructuralGraphicsEntity* entity);

  void requestEntityPreparation(StructuralGraphicsEntity* entity,
                                QMap<QString, QString> properties);

  void requestEntitySelection(StructuralGraphicsEntity* entity);

  void requestBindParamAdjust(QString uid, QString parent,
                              QMap<QString, QString> properties);

  void updateBindParams(QString bindUID, QMap<QString, QString> params,
                        QMap<QString, QString> name_uids);

  void clearAllData();

  void markError(QString uid, QString msg);

  void clearValidationErrors();

  void adjustAll();

private:
  void addImportBase(const QString &uid,
                     const QMap<QString, QString> &properties);

  void changeImportBase(const QString &uid,
                        const QMap<QString, QString> &properties);

  void adjustEntity(StructuralGraphicsEntity *entity);

  void adjustContext(StructuralGraphicsContext* entity);

  void adjustMedia(StructuralGraphicsMedia* entity);

  void adjustPort(StructuralGraphicsPort* entity);

  void adjustMapping(StructuralGraphicsMapping* entity);

  void changeLink(StructuralGraphicsLink* entity,
                  const QMap<QString, QString> &properties);

  void adjustLink(StructuralGraphicsLink* entity);

  void adjustBind(StructuralGraphicsBind* entity);

  void addConnector(const QString &uid,
                    const QMap<QString, QString> &properties);

  void changeConnector(StructuralConnector* entity,
                       const QMap<QString, QString> &properties);

  void addCondition(const QString &uid,
                    const QMap<QString, QString> &properties);

  void changeCondition(const QString &condition,
                       const QMap<QString, QString> &properties);

  void addAction(const QString &uid,
                 const QMap<QString, QString> &properties);

  void changeAction(const QString &uid,
                    const QMap<QString, QString> &properties);

  void adjustConnector(StructuralConnector* entity);

  void changeBindParam(const QString &uid,
                       const QMap<QString, QString> &properties);

  void addConnectorParam(const QString &uid,
                         const QString &parent,
                         const QMap<QString, QString> &properties);

  void changeConnectorParam(const QString &uid,
                            const QMap<QString, QString> &properties);

  void performCopy(StructuralGraphicsEntity* entity, StructuralGraphicsEntity* parent);

  void performPaste(StructuralGraphicsEntity* copy, StructuralGraphicsEntity* parent,
                    bool makeRefer = false, bool keepOriginalPos = false);

  void performReference(StructuralGraphicsEntity* entity,
                        StructuralGraphicsEntity* referred);

  void createObjects();

  void createConnection();

  void readImportBase(const QString &uid, QDomElement e, const QString &alias);

  void readConnector(QDomElement e, StructuralConnector* conn);

  void addNodetoNodeEdge(StructuralGraphicsEntity* entitya,
                         StructuralGraphicsEntity* entityb);

  void addNodetoInterfaceEdge(StructuralGraphicsEntity* entitya,
                              StructuralGraphicsEntity* entityb);

  void addInterfacetoNodeEdge(StructuralGraphicsEntity* entitya,
                              StructuralGraphicsEntity* entityb);

  void addInterfacetoNodeLink(StructuralGraphicsEntity* entitya,
                              StructuralGraphicsEntity* entityb);

  void addInterfacetoInterfaceEdge(StructuralGraphicsEntity* entitya,
                                   StructuralGraphicsEntity* entityb);

  void adjustAngles(StructuralGraphicsEdge* edge);

  void collapseCompositions(QDomElement element, QDomElement parent);

  QString createNewConnector(QString condition, QString action);

  void createReference(StructuralGraphicsEntity* entitya,
                       StructuralGraphicsEntity* entityb);

  void createMapping(StructuralGraphicsEntity* entitya,
                     StructuralGraphicsEntity* entityb);

  StructuralGraphicsLink* createLink(StructuralGraphicsEntity* entitya,
                               StructuralGraphicsEntity* entityb,
                               QString connName);

  void createActionWithDialog(StructuralGraphicsEntity* entitya,
                              StructuralGraphicsEntity* entityb);

  void createConditionWithDialog(StructuralGraphicsEntity* entitya,
                                 StructuralGraphicsEntity* entityb);

  void createLinkWithDialog(StructuralGraphicsEntity* entitya,
                            StructuralGraphicsEntity* entityb);

  void createNoGraphicalLink(StructuralGraphicsEntity* entitya,
                             StructuralGraphicsEntity* entityb,
                             StructuralGraphicsLink* aggregator,
                             StructuralGraphicsBind* condition,
                             StructuralGraphicsBind* actionDialog,
                             QString connector);

  StructuralGraphicsBind* createBind(StructuralGraphicsLink* link,
                               StructuralGraphicsEntity* target,
                               QString condition,
                               QString action,
                               bool notifyCreation = true);

  void removeEdgesOfEntity (QnstGraphicsEntityWithEdges *entity, bool notify=true);

  void removeEdge (StructuralGraphicsEdge *edge, bool notify=true);

  void traceRefers();

  void traceEntities();

  void traceConnectors();
  void traceConnectors2();

  void deletePendingEntities();

  StructuralConnector* getConnectorByRoleUid(QString uid);

  bool modified;

  bool linking;

  int zoomStep;

  bool hasCutted;

  QAction* undoAct;

  QAction* redoAct;

  StructuralScene* scene;

  StructuralViewLink* link;

  StructuralGraphicsLinkDialog* linkDialog;

  StructuralGraphicsConditionDialog* conditionDialog;

  StructuralGraphicsActionDialog* actionDialog;

  StructuralGraphicsEntity* selected;

  StructuralGraphicsEntity* clipboard;

  QSet<QString> linkWriterAux;

  QMap<QString, StructuralGraphicsLink*> links;

  QMap<QString, StructuralGraphicsBind*> binds;

  QMap<QString, QString> refers;

  QMap<QString, QString> importBases; // importBaseUid - ConnUid

  QMap<QString, QString> bindParamUIDToBindUID;

  QMap<QString, StructuralConnector*> connectors2; // uid - conn

  QMap<QString, StructuralConnector*> connectors; // id - conn

  QVector<StructuralGraphicsEntity*> toDelete;

  QUndoStack history;

  StructuralGraphicsEntity* lastLinkMouseOver;

  std::map < Structural::EntityType, int > entityCounter;

  static std::map <Structural::EntityType, QString> mediaTypeToXMLStr;

  StructuralMiniMap *minimap;
};

#endif // QNSTVIEW_H

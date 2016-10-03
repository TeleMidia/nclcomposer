#ifndef STRUCTURALVIEW_H
#define STRUCTURALVIEW_H

#include <QGraphicsView>

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

#include "StructuralScene.h"
#include "StructuralEntity.h"
#include "StructuralContent.h"
#include "StructuralInterface.h"
#include "StructuralComposition.h"
#include "StructuralBind.h"
#include "StructuralLink.h"
#include "StructuralLinkDialog.h"

#include "Remove.h"
#include "Change.h"
#include "Insert.h"

#include "StructuralMinimap.h"
#include "StructuralLinkTool.h"

class StructuralScene;
class StructuralMinimap;

class StructuralView : public QGraphicsView
{
  Q_OBJECT

public:
  StructuralView(QWidget* parent = 0);
  virtual ~StructuralView();

  StructuralMode getMode() const;
  void setMode(StructuralMode mode);

public slots:
  void insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void remove(QString uid, QMap<QString, QString> settings);
  void change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void select(QString uid, QMap<QString, QString> settings);

  void move(QString uid, QString parent);

  void create(StructuralType type);
  void create(StructuralType type, QMap<QString, QString> properties, QMap<QString, QString> settings);

  void performHelp();
  void performUndo();
  void performRedo();
  void performCut();
  void performCopy();
  void performPaste();
  void performZoomIn();
  void performZoomOut();
  void performPointer();
  void performLink();
  void performDelete();
  void performSnapshot();
  void performMinimap();
  void performMedia();
  void performContext();
  void performSwitch();
  void performBody();
  void performArea();
  void performProperty();
  void performPort();
  void performSwitchPort();

#ifdef WITH_GRAPHVIZ
  void performAutoAdjust();
#endif

  void performProperties();

  void performInsert(StructuralType type, QMap<QString, QString> properties);

  void showEditLinkDialog(StructuralLink* entity);
  void showEditBindDialog(StructuralBind* entity);

signals:
  void inserted(QString uid, QString _parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void removed(QString uid, QMap<QString, QString> settings);
  void changed(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void selected(QString uid, QMap<QString, QString> settings);

  void switchedUndo(bool state);
  void switchedRedo(bool state);
  void switchedCut(bool state);
  void switchedCopy(bool state);
  void switchedPaste(bool state);
  void switchedPointer(bool state);
  void switchedLink(bool state);
  void switchedZoomIn(bool state);
  void switchedZoomOut(bool state);
  void switchedBody(bool state);


  void selectChange(QString uid);
  void viewChanged();
  void requestLinkDialogUpdate();

private:
#ifdef WITH_GRAPHVIZ
   void autoadjust();
   void autoadjust(StructuralEntity* entity, const QString &code);
#endif

   int _zoomStep;

   QString _selected;

   StructuralEntity* _clipboard;
   QMap<QString, QString> _clipboardReferences;

   StructuralMenu* _menu;
   StructuralMinimap* _minimap;

   StructuralScene* _scene;

   StructuralMode _mode;

   QUndoStack _commnads;

   StructuralLinkDialog* _linkDialog;

   QMap<QString, StructuralEntity*> _entities;

   /************ */
public:
  void setMod(bool mod);

public:
  StructuralScene* getScene();

  bool hasEntity(QString uid);
  StructuralEntity* getEntity(const QString &uid);
  QMap<QString, StructuralEntity*> getEntities();

  void switchMinimapVis();
  void cleanUndoRedo();


  void updateLinkDialog(QMap<QString, QVector<QString> > conditions,
                        QMap<QString, QVector<QString> > actions,
                        QMap<QString, QVector<QString> > params);


public:
  void load(const QString &data);
  void load(QDomElement entity, QDomElement parent);

  QString save();
  void createDocument(StructuralEntity* entity, QDomDocument* document, QDomElement parent);

  void unselect();

   StructuralEntity* getBody();
   QVector<StructuralEntity*> getRoots();

   void clearErrors();
   void markError(const QString &uid, const QString &msg);

   void setMiniMapVisible(bool enable);

   void adjustReferences(StructuralEntity* entity);


protected:

  QString getNewId(StructuralEntity *entity);

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
  StructuralEntity* clone(StructuralEntity* entity, StructuralEntity * parent = NULL);

  void paste(StructuralEntity* entity, StructuralEntity* parent);
  void paste(StructuralEntity* entity, StructuralEntity* parent, const QString &code, bool adjust);

  void adjustProperties(StructuralEntity* entity);
  qreal getNewAngle(StructuralBind* edge);
  void adjustAngles(StructuralBind* edge);

  bool isChild(StructuralEntity* e , StructuralEntity* p);
  void createLink(StructuralEntity* a, StructuralEntity* b);
  void createBind(StructuralEntity* a, StructuralEntity* b,StructuralRole type = Structural::NoRole, QString code = "");
  void createReference(StructuralEntity* a, StructuralEntity* b);


  void createObjects();

  void createConnection();

  void collapseCompositions(QDomElement element, QDomElement _parent);

  bool modified;

  bool linking;


  bool hasCutted;



  QAction* undoAct;

  QAction* redoAct;


  QMap<StructuralType, int> entityCount;



  QString clip_cut;
  QString clip_copy;

  QSet<QString> linkWriterAux;

  QMap<QString, QString> importBases; // importBaseUid - ConnUid

  QMap<QString, QString> bindParamUIDToBindUID;

  StructuralLinkTool* link;

  StructuralEntity* lastLinkMouseOver;
};

#endif // STRUCTURALVIEW_H

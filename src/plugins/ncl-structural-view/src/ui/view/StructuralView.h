#ifndef STRUCTURALVIEW_H
#define STRUCTURALVIEW_H

#include <QGraphicsView>

#include <QDomDocument>
#include <QDomElement>

#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include <QResizeEvent>

#include <QUndoStack>
#include <QDir>

#include "Remove.h"
#include "Change.h"
#include "Insert.h"

#include "StructuralScene.h"
#include "StructuralEntity.h"
#include "StructuralContent.h"
#include "StructuralInterface.h"
#include "StructuralComposition.h"
#include "StructuralBind.h"
#include "StructuralLink.h"
#include "StructuralLinkDialog.h"
#include "StructuralMinimap.h"
#include "StructuralLinkTool.h"

class StructuralMinimap;

class StructuralView : public QGraphicsView
{
  Q_OBJECT

public:
  // Zoom values are in %
  constexpr static int ZOOM_MIN = 25;
  constexpr static int ZOOM_MAX  = 150;
  constexpr static int ZOOM_ORIGINAL  = 100;
  constexpr static int ZOOM_STEP = 5;

  StructuralView(QWidget* parent = 0);
  virtual ~StructuralView();

  StructuralScene* getScene();
  StructuralMenu* getMenu();
  StructuralEntity* getBody();
  StructuralLinkDialog* getDialog();

  bool hasEntity(const QString &uid);
  StructuralEntity* getEntity(const QString &uid);
  QMap<QString, StructuralEntity*> getEntities();

  StructuralMode getMode() const;
  void setMode(StructuralMode mode);

  QString getError(const QString &uid) const;
  void setError(const QString &uid, const QString &error);

  void cleanErrors();

  void load(const QString &data);
  void load(QDomElement entity, QDomElement parent);

  QString save();

public slots:
  void insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void remove(QString uid, QMap<QString, QString> settings);
  void change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings);
  void select(QString uid, QMap<QString, QString> settings);

  void move(QString uid, QString parent);

  void performHelp();
  void performAutostart();
  void performUndo();
  void performRedo();
  void performCut();
  void performCopy();
  void performPaste();
  void performZoomIn();
  void performZoomOut();
  void performZoomOriginal();
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

  void performDialog(StructuralLink* entity);
  void performDialog(StructuralBind* entity);

  void clean();

  void adjustReferences(StructuralEntity* entity);

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
  void switchedDelete(bool state);
  void switchedSnapshot(bool state);
  void switchedPointer(bool state);
  void switchedLink(bool state);
  void zoomChanged(int zoom);
  void switchedBody(bool state);

  void requestedUpdate();

protected:
  virtual void mouseMoveEvent(QMouseEvent* event);
  virtual void mousePressEvent(QMouseEvent* event);
  virtual void mouseReleaseEvent(QMouseEvent*event);

  virtual void keyPressEvent(QKeyEvent *event);
  virtual void keyReleaseEvent(QKeyEvent *event);

  virtual void wheelEvent(QWheelEvent * event);

  virtual void resizeEvent(QResizeEvent *event);

private:
  void createObjects();
  void createConnection();

  void createEntity(StructuralType type);
  void createEntity(StructuralType type, QMap<QString, QString> properties, QMap<QString, QString> settings);
  void createDocument(StructuralEntity* entity, QDomDocument* document, QDomElement parent);
  void createLink(StructuralEntity* tail, StructuralEntity* head);
  void createBind(StructuralEntity* tail, StructuralEntity* head, StructuralRole role = Structural::NoRole, const QString &code = "");
  void createReference(StructuralEntity* tail, StructuralEntity* head);

  QString getNewId(StructuralEntity *entity);
  qreal getNewAngle(StructuralBind* edge);

  void adjustProperties(StructuralEntity* entity);

#ifdef WITH_GRAPHVIZ
  void adjustLayout();
  void adjustLayout(StructuralEntity* entity, const QString &code);
#endif

  bool isChild(StructuralEntity* entity , StructuralEntity* parent);

  void paste(StructuralEntity* entity, StructuralEntity* parent);
  void paste(StructuralEntity* entity, StructuralEntity* parent, const QString &code, bool adjust);

  StructuralEntity* clone(StructuralEntity* entity, StructuralEntity* parent = NULL);

  StructuralMode _mode;

  int _zoom; // an integer in %  (varing from ZOOM_MIN to ZOOM_MAX)

  bool _linking;
  StructuralEntity* _linkingTail;
  StructuralEntity* _linkingHead;

  QString _selected;

  StructuralEntity* _clipboard;
  QMap<QString, QString> _clipboardReferences;

  QUndoStack _commnads;

  StructuralMenu* _menu;
  StructuralScene* _scene;
  StructuralLinkTool* _tool;
  StructuralLinkDialog* _dialog;
  StructuralMinimap* _minimap;

  QMap<StructuralType, int> _counter;

  QMap<QString, QString> _references;

  QMap<QString, StructuralEntity*> _entities;
};

#endif // STRUCTURALVIEW_H

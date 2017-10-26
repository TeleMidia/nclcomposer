#ifndef STRUCTURALVIEW_H
#define STRUCTURALVIEW_H

#include <QGraphicsView>

#include <QDomDocument>
#include <QDomElement>

#include <QKeyEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QWheelEvent>

#include <QDir>
#include <QUndoStack>

#include "Commands.h"

#include "StructuralBind.h"
#include "StructuralComposition.h"
#include "StructuralContent.h"
#include "StructuralEntity.h"
#include "StructuralInterface.h"
#include "StructuralLink.h"
#include "StructuralLinkDialog.h"
#include "StructuralLinkTool.h"
#include "StructuralMinimap.h"
#include "StructuralScene.h"

class StructuralMinimap;

class StructuralView : public QGraphicsView
{
  Q_OBJECT

public:
  constexpr static int ZOOM_MIN = ST_DEFAULT_SCENE_ZOOM_MIN;
  constexpr static int ZOOM_MAX = ST_DEFAULT_SCENE_ZOOM_MAX;
  constexpr static int ZOOM_ORIGINAL = ST_DEFAULT_SCENE_ZOOM_ORIGINAL;
  constexpr static int ZOOM_STEP = ST_DEFAULT_SCENE_ZOOM_STEP;

  StructuralView (QWidget *parent = 0);
  virtual ~StructuralView () {}

  StructuralScene *getScene ();
  StructuralEntity *getBody ();
  StructuralLinkDialog *getDialog ();

  bool hasEntity (const QString &uid);
  StructuralEntity *getEntity (const QString &uid);
  QMap<QString, StructuralEntity *> getEntities ();

  StructuralMode getMode () const;
  void setMode (StructuralMode mode);

  void setError (const QString &uid, const QString &error);

  void cleanErrors ();

  void load (const QString &data);
  void load (QDomElement entity, QDomElement parent);

  QString save ();

  StructuralMenu *
  getMenu ()
  {
    return _menu;
  }

public slots:
  void createEntity (StructuralType type,
                     QMap<QString, QString> prop = QMap<QString, QString> (),
                     QMap<QString, QString> stgs = QMap<QString, QString> ());

  void changeEntity (QString uid, QMap<QString, QString> props,
                     QMap<QString, QString> prev, QMap<QString, QString> stgs);

  void deleteEntity (const QString &uid);

  void insert (QString uid, QString parent, QMap<QString, QString> properties,
               QMap<QString, QString> settings);
  void remove (QString uid, QMap<QString, QString> settings);
  void change (QString uid, QMap<QString, QString> properties,
               QMap<QString, QString> previous,
               QMap<QString, QString> settings);
  void select (QString uid, QMap<QString, QString> settings);

  void move (QString uid, QString parent, QMap<QString, QString> properties,
             QMap<QString, QString> settings);

  void zoomIn ();
  void zoomOut ();
  void zoomOriginal ();
  void snapshot ();
  void toggleMinimapVisibility ();

  // FIXME: These methods should not be part of StructuralView
  void performAutostart ();
  void performUndo ();
  void performRedo ();
  void performPointer ();
  void performLink ();
  void performDelete ();

  void performCut ();
  void performCopy ();
  void performPaste ();

#ifdef WITH_GRAPHVIZ
  void performAutoAdjust ();
#endif

  void performProperties ();

  void performInsert (StructuralType type, QMap<QString, QString> properties);

  void performLinkDialog (StructuralLink *entity);
  void performBindDialog (StructuralBind *entity);

  void clean ();

  void adjustReferences (StructuralEntity *entity);

signals:
  void inserted (QString uid, QString _parent,
                 QMap<QString, QString> properties,
                 QMap<QString, QString> settings);
  void removed (QString uid, QMap<QString, QString> settings);
  void changed (QString uid, QMap<QString, QString> properties,
                QMap<QString, QString> previous,
                QMap<QString, QString> settings);
  void selected (QString uid, QMap<QString, QString> settings);

  void switchedUndo (bool state);
  void switchedRedo (bool state);
  void switchedCut (bool state);
  void switchedCopy (bool state);
  void switchedPaste (bool state);
  void switchedDelete (bool state);
  void switchedSnapshot (bool state);
  void switchedPointer (bool state);
  void switchedLink (bool state);
  void zoomChanged (int zoom);
  void switchedBody (bool state);

  void updateRequested ();

protected:
  virtual void mouseMoveEvent (QMouseEvent *event);
  virtual void mousePressEvent (QMouseEvent *event);
  virtual void mouseReleaseEvent (QMouseEvent *event);

  virtual void keyPressEvent (QKeyEvent *event);
  virtual void keyReleaseEvent (QKeyEvent *event);

  virtual void wheelEvent (QWheelEvent *event);

  virtual void dragEnterEvent (QDragEnterEvent *event);
  virtual void dragMoveEvent (QDragMoveEvent *event);
  virtual void dropEvent (QDropEvent *event);

  virtual void resizeEvent (QResizeEvent *event);

private:
  StructuralEntity *createEntity (StructuralType type,
                                  StructuralEntity *parent);

  void createDocument (StructuralEntity *entity, QDomDocument *document,
                       QDomElement parent);
  void createLink (StructuralEntity *tail, StructuralEntity *head);
  void createBind (StructuralEntity *tail, StructuralEntity *head,
                   const QString &role = "", const QString &code = "");
  void createReference (StructuralEntity *tail, StructuralEntity *head);

  QString getNewId (StructuralEntity *entity);
  qreal getNewAngle (StructuralBind *edge);

  void adjustProperties (StructuralEntity *entity);

#ifdef WITH_GRAPHVIZ
  void adjustLayout ();
  void adjustLayout (StructuralEntity *entity, const QString &code);
#endif

  bool isChild (StructuralEntity *entity, StructuralEntity *parent);

  void paste (StructuralEntity *entity, StructuralEntity *parent);
  void paste (StructuralEntity *entity, StructuralEntity *parent,
              const QString &code, bool adjust);

  StructuralEntity *clone (StructuralEntity *entity,
                           StructuralEntity *parent = NULL);

  StructuralMode _mode;

  int _zoom; // an integer in %  (varing from ZOOM_MIN to ZOOM_MAX)
  bool _linking;

  StructuralEntity *_linkingTail;
  StructuralEntity *_linkingHead;

  QString _selected;

  StructuralEntity *_clipboard;
  QMap<QString, QString> _clipboardReferences;

  QUndoStack _commands;

  StructuralMinimap *_minimap;
  StructuralMenu *_menu;
  StructuralLinkDialog *_dialog;
  StructuralScene *_scene;
  StructuralLinkTool *_tool;

  QMap<StructuralType, int> _counter;
  QMap<QString, QString> _references;
  QMap<QString, StructuralEntity *> _entities;
};

#endif // STRUCTURALVIEW_H

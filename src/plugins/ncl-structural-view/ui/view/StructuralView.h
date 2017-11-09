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

namespace util = StructuralUtil;
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

  StructuralScene *scene ();
  StructuralLinkDialog *dialog ();

  StructuralInteractionMode mode () const;
  void setMode (StructuralInteractionMode mode, bool notify = false);
  void toogleMode (bool notify = false);

  void setError (const QString &uid, const QString &error);

  void clearErrors ();

  StructuralMenu *
  menu ()
  {
    return _menu;
  }

  QString selectedUid () { return _selected; }
  void adjustEntity (StructuralEntity *e, const QStrMap &props,
                     const QStrMap &stgs);

public slots:
  // External methods
  void createEntity (StructuralType type, QStrMap prop = QStrMap (),
                     QStrMap stgs = QStrMap ());

  void changeEntity (QString uid, QStrMap props, QStrMap prev, QStrMap stgs);

  void removeEntity (QString uid, QStrMap settings);

  // "Internal" methods
  void insert (QString uid, QString parent, QStrMap props, QStrMap stgs);
  void remove (QString uid, QStrMap stgs);
  void change (QString uid, QStrMap props, QStrMap stgs);
  void select (QString uid, QStrMap stgs);
  void unselect ();
  void move (QString uid, QString parent, QStrMap props, QStrMap stgs);

  void zoomIn ();
  void zoomOut ();
  void zoomOriginal ();
  void snapshot ();
  void toggleMinimapVisibility ();

  // FIXME: These methods should not be part of StructuralView
  void deleteSelected ();

  void cut ();
  void copy ();
  void paste ();

  void undo ();
  void redo ();

  void performAutostart ();

#ifdef WITH_GRAPHVIZ
  void performAutoAdjust ();
#endif

  void performLinkDialog (StructuralLink *entity);
  void performBindDialog (StructuralBind *entity);

  void clean ();

  void adjustAllReferences ();
  void adjustReferences (StructuralEntity *entity);

signals:
  void inserted (QString uid, QString _parent, QStrMap properties,
                 QStrMap settings);
  void removed (QString uid, QStrMap settings);
  void changed (QString uid, QStrMap properties, QStrMap previous,
                QStrMap settings);
  void entitySelected (QString uid);

  void canUndoChanged (bool state);
  void canRedoChanged (bool state);

  void canCutChanged (bool state);
  void canCopyChanged (bool state);
  void canPasteChanged (bool state);
  void canDeleteChanged (bool state);

  void interactionModeChanged (StructuralInteractionMode mode);

  void zoomChanged (int zoom);
  void canAddBody (bool state);

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
  void createLink (StructuralEntity *tail, StructuralEntity *head);
  void createBind (StructuralEntity *tail, StructuralEntity *head,
                   const QString &role = "", const QString &code = "");
  void createReference (StructuralEntity *tail, StructuralEntity *head);

  qreal calcNewAngle (StructuralBind *edge);

  void adjustProperties (StructuralEntity *entity);

#ifdef WITH_GRAPHVIZ
  void adjustLayout ();
  void adjustLayout (StructuralEntity *entity, const QString &code);
#endif

  bool isChild (StructuralEntity *entity, StructuralEntity *parent);

  void pasteEntity (StructuralEntity *entity, StructuralEntity *parent,
                    const QString &code, bool adjust);

  StructuralInteractionMode _mode;

  int _zoom; // an integer in %  (varing from ZOOM_MIN to ZOOM_MAX)
  bool _linking;

  StructuralEntity *_linkingOrig;
  StructuralEntity *_linkingDest;

  QString _selected;

  StructuralEntity *_clipboard;
  QStrMap _clipboardRefs;

  QUndoStack _commands;

  StructuralMinimap *_minimap;
  StructuralMenu *_menu;
  StructuralLinkDialog *_dialog;
  StructuralScene *_scene;
  StructuralLinkTool *_tool;
};

#endif // STRUCTURALVIEW_H

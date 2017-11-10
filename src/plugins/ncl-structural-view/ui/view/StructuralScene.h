#ifndef STRUCTURALSCENE_H
#define STRUCTURALSCENE_H

#include <QDomDocument>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QObject>

#include "StructuralMenu.h"
#include "StructuralEntity.h"
#include "StructuralEdge.h"

class StructuralView;

class StructuralScene : public QGraphicsScene
{
  Q_OBJECT

public:
  StructuralScene (StructuralView *view, StructuralMenu *menu,
                   QObject *parent = 0);
  virtual ~StructuralScene () {}

  bool hasBody ();
  StructuralEntity *body ();

  bool hasEntity (const QString &uid);
  StructuralEntity *entity (const QString &uid);
  QMap<QString, StructuralEntity *> entities ();
  QList<StructuralEntity *> entitiesByAttrId (const QString &id);

  const QMap<QString, StructuralEntity *> &nodes ();
  const QMap<QString, StructuralEdge *> &edges (void);
  QList <StructuralEdge *> edges (const QString &nodeuid);
  QList <StructuralEdge *> parallelEdges (StructuralEdge *edge);
  QStrMap &refs ();

  QString createId (StructuralType type, const QString &customPrefix = "");

  void load (const QString &data);
  QString save ();

  StructuralEntity *createEntity (StructuralType type);
  StructuralEntity *clone (const StructuralEntity *ent,
                           StructuralEntity *parent = nullptr);
  void updateWithDefaultProperties (StructuralEntity *e);

  void insertIntoMap (const QString &uid, StructuralEntity *ent);
  void removeFromMap (const QString &uid);

  // Edit scene.
  void insert (QString uid, QString parent, QStrMap props, QStrMap stgs);
  void remove (QString uid, QStrMap stgs);
  void change (QString uid, QStrMap props, QStrMap stgs);

public slots:
  void clear (); // override?

signals:
  void inserted (QString uid, QString parent, QStrMap props, QStrMap stngs);
  void removed (QString uid, QStrMap stngs);
  void changed (QString uid, QStrMap props, QStrMap previous, QStrMap stngs);

protected:
  virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

private:
  StructuralView *_view;
  StructuralMenu *_menu;

  QMap<QString, StructuralEntity *> _nodes;
  QMap<QString, StructuralEdge *> _edges;
  QStrMap _refs;

  void load (QDomElement ent, QDomElement parent);
  void createXmlElement (StructuralEntity *ent, QDomDocument *doc,
                         QDomElement parent);

  void removeReferences (StructuralEntity *ent, const QStrMap &stgs);
  void removeEdges (StructuralEntity *ent, const QStrMap &stgs);
  void removeChildren (StructuralEntity *ent, const QStrMap &stgs);
  void removeParams (StructuralEntity *ent, const QStrMap &stgs);
};

#endif // STRUCTURALSCENE_H

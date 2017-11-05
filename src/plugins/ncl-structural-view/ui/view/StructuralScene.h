#ifndef STRUCTURALSCENE_H
#define STRUCTURALSCENE_H

#include <QDomDocument>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

#include "StructuralMenu.h"

class StructuralView;

class StructuralScene : public QGraphicsScene
{
public:
  StructuralScene (StructuralView *view, StructuralMenu *menu,
                   QObject *parent = 0);
  virtual ~StructuralScene () {}

  StructuralEntity *body ();
  bool hasEntity (const QString &uid);
  StructuralEntity *entity (const QString &uid);
  QMap<QString, StructuralEntity *> &entities ();
  QList<StructuralEntity *> entitiesByAttrId (const QString &id);
  QStrMap &refs ();

  QString createNewId (StructuralType type, const QString &customPrefix = "");

  void load (const QString &data);
  QString save ();

  StructuralEntity *createEntity (StructuralType type);
  StructuralEntity *clone (const StructuralEntity *ent,
                           StructuralEntity *parent = nullptr);
  void updateWithDefaultProperties (StructuralEntity *e);

public slots:
  void clear (); // override?

protected:
  virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

private:
  StructuralView *_view;
  StructuralMenu *_menu;

  QMap<QString, StructuralEntity *> _entities;
  QStrMap _refs;

  void load (QDomElement ent, QDomElement parent);
  void createXmlElement (StructuralEntity *ent, QDomDocument *doc,
                         QDomElement parent);
};

#endif // STRUCTURALSCENE_H

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

  StructuralEntity *getBody ();
  bool hasEntity (const QString &uid);
  StructuralEntity *getEntity (const QString &uid);
  QList<StructuralEntity *> getEntitiesByAttrId (const QString &id);
  QMap<QString, StructuralEntity *> &getEntities ();
  QMap<QString, QString> &getRefs ();

  QString createNewId (StructuralType type);

  void load (const QString &data);
  QString save ();

  StructuralEntity *createEntity (StructuralType type);
  StructuralEntity *clone (const StructuralEntity *ent,
                           StructuralEntity *parent = nullptr);


public slots:
  void clear (); // override?

protected:
  virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

private:
  StructuralView *_view;
  StructuralMenu *_menu;
  QMap<QString, StructuralEntity *> _entities;
  QMap<QString, QString> _refs;

  void load (QDomElement ent, QDomElement parent);
  void createXmlElement (StructuralEntity *ent, QDomDocument *doc,
                         QDomElement parent);
};

#endif // STRUCTURALSCENE_H

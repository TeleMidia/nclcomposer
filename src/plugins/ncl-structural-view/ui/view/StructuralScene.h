#ifndef STRUCTURALSCENE_H
#define STRUCTURALSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>

#include "StructuralMenu.h"

class StructuralScene : public QGraphicsScene
{
public:
  StructuralScene (StructuralMenu *menu, QObject *parent = 0);
  virtual ~StructuralScene () {}

  StructuralEntity *getBody ();
  bool hasEntity (const QString &uid);
  StructuralEntity *getEntity (const QString &uid);
  QMap<QString, StructuralEntity *> &getEntities ();
  QMap<QString, QString> &getRefs ();

public slots:
  void clear (); // override?

protected:
  virtual void contextMenuEvent (QGraphicsSceneContextMenuEvent *event);

private:
  StructuralMenu *_menu;
  QMap<QString, StructuralEntity *> _entities;
  QMap<QString, QString> _refs;
};

#endif // STRUCTURALSCENE_H

#include "StructuralNode.h"

#include "StructuralEdge.h"

StructuralNode::StructuralNode (StructuralEntity *parent)
    : StructuralEntity (parent)
{
  setCategory (Structural::Node);
  setType (Structural::NoType);
}

StructuralNode::~StructuralNode () {}

void
StructuralNode::inside ()
{
  StructuralEntity *parent = getParent ();

  if (parent != NULL)
  {
    QLineF line
        = QLineF (getLeft () + getWidth () / 2, getTop () + getHeight () / 2,
                  parent->getWidth () / 2, parent->getHeight () / 2);

    int max = 1000;
    int n = 0;

    qreal current = 0.0;

    setSelectable (false);

    while (!collidesWithItem (parent, Qt::ContainsItemShape))
    {
      current += 0.01;

      setTop (getTop () + line.pointAt (current).y () - line.p1 ().y ());
      setLeft (getLeft () + line.pointAt (current).x () - line.p1 ().x ());

      if (++n > max)
      {
        n = -1;
        break;
      }
    }

    if (n < 0)
    {
      setTop (line.p2 ().y () - getHeight () / 2);
      setLeft (line.p2 ().x () - getWidth () / 2);
    }

    setSelectable (true);
  }
}

void
StructuralNode::adjust (bool collision, bool recursion)
{
  StructuralEntity::adjust (collision, recursion);

  setSelectable (false);

  if (recursion)
  {
    for (StructuralEntity *ent : getChildren ())
      if (ent->getCategory () != Structural::Edge)
        ent->adjust (true, false);
  }

  StructuralEntity *parent = getParent ();

  if (parent != NULL || !ST_DEFAULT_WITH_BODY)
  {

    if (!collision)
    {
      // Tries (10x) to find a position where there is no collision
      // with others relatives
      for (int i = 0; i < 10; i++)
      {
        bool colliding = false;

        for (StructuralEntity *ent : StructuralUtil::getNeighbors (this))
        {
          if (ent != this)
          {

            int max = 1000;
            int n = 0;

            qreal current = 0.0;

            ent->setSelectable (false);

            while (collidesWithItem (ent, Qt::IntersectsItemBoundingRect))
            {
              QLineF line = QLineF (
                  getLeft () + getWidth () / 2, getTop () + getHeight () / 2,
                  ent->getWidth () / 2, ent->getHeight () / 2);

              line.setAngle (qrand () % 360);

              current += (double)(qrand () % 100) / 1000.0;

              setTop (getTop () + line.pointAt (current / 2).y ()
                      - line.p1 ().y ());
              setLeft (getLeft () + line.pointAt (current / 2).x ()
                       - line.p1 ().x ());

              if (++n > max)
                break;
            }

            inside ();

            ent->setSelectable (true);
          }
        }

        for (StructuralEntity *ent : StructuralUtil::getNeighbors (this))
          if (collidesWithItem (ent, Qt::IntersectsItemBoundingRect))
            colliding = true;

        if (!colliding)
          break;
      }
    }

    inside ();

    StructuralUtil::adjustEdges (this);
  }

  setSelectable (true);

  if (scene () != NULL)
    scene ()->update ();
}

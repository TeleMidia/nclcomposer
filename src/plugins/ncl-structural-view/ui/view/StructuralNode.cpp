#include "StructuralNode.h"

#include "StructuralEdge.h"

StructuralNode::StructuralNode (StructuralEntity *parent)
    : StructuralEntity (parent)
{
  setCategory (Structural::Node);
  setStructuralType (Structural::NoType);
}

StructuralNode::~StructuralNode () {}

void
StructuralNode::inside ()
{
  StructuralEntity *parent = structuralParent ();

  if (parent)
  {
    QLineF line = QLineF (left () + width () / 2, top () + height () / 2,
                          parent->width () / 2, parent->height () / 2);

    int max = 1000;
    int n = 0;

    qreal current = 0.0;

    setSelectable (false);

    while (!collidesWithItem (parent, Qt::ContainsItemShape))
    {
      current += 0.01;

      setTop (top () + line.pointAt (current).y () - line.p1 ().y ());
      setLeft (left () + line.pointAt (current).x () - line.p1 ().x ());

      if (++n > max)
      {
        n = -1;
        break;
      }
    }

    if (n < 0)
    {
      setTop (line.p2 ().y () - height () / 2);
      setLeft (line.p2 ().x () - width () / 2);
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
    for (StructuralEntity *ent : children ())
      if (ent->category () != Structural::Edge)
        ent->adjust (true, false);
  }

  StructuralEntity *parent = structuralParent ();
  if (parent || !ST_OPT_WITH_BODY)
  {
    if (!collision)
    {
      // Tries (10x) to find a position where there is no collision
      // with others relatives
      for (int i = 0; i < 10; i++)
      {
        bool colliding = false;

        for (StructuralEntity *ent : StructuralUtil::neighbors (this))
        {
          if (ent != this)
          {

            int max = 1000;
            int n = 0;

            qreal current = 0.0;

            ent->setSelectable (false);

            while (collidesWithItem (ent, Qt::IntersectsItemBoundingRect))
            {
              QLineF line
                  = QLineF (left () + width () / 2, top () + height () / 2,
                            ent->width () / 2, ent->height () / 2);

              line.setAngle (qrand () % 360);

              current += (double)(qrand () % 100) / 1000.0;

              setTop (top () + line.pointAt (current / 2).y ()
                      - line.p1 ().y ());
              setLeft (left () + line.pointAt (current / 2).x ()
                       - line.p1 ().x ());

              if (++n > max)
                break;
            }

            inside ();

            ent->setSelectable (true);
          }
        }

        for (StructuralEntity *ent : StructuralUtil::neighbors (this))
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

  if (scene ())
    scene ()->update ();
}

#include "qncggraphicsnode.h"

QncgGraphicsNode::QncgGraphicsNode(QncgGraphicsEntity* parent)
  : QncgGraphicsEntity(parent)
{
  setncgType(Qncg::Node);
}

QncgGraphicsNode::~QncgGraphicsNode()
{

}

void QncgGraphicsNode::adjust(bool avoidCollision)
{
  Q_UNUSED(avoidCollision)
  if (scene() != NULL)
  {
    scene()->update();
  }
}

void QncgGraphicsNode::move(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();

  // setting delta
  qreal dx = event->pos().x() - getPressLeft(); // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();  // (y1 - y0)

  // setting next position
  qreal nextx = x + dx;
  qreal nexty = y + dy;

  // moving
  setMoveTop(nexty);
  setMoveLeft(nextx);

  scene()->update();
}

void QncgGraphicsNode::resize(QGraphicsSceneMouseEvent* event)
{
  // setting
  qreal x = getLeft();
  qreal y = getTop();
  qreal w = getWidth();
  qreal h = getHeight();

  // setting delta
  qreal dx = event->pos().x() - getPressLeft();    // (x1 - x0)
  qreal dy = event->pos().y() - getPressTop();     // (y1 - y0)
  qreal dw = -dx;
  qreal dh = -dy;

  // setting next bounds
  qreal nextx = x + dx;
  qreal nexty = y + dy;
  qreal nextw = w + dw;
  qreal nexth = h + dh;

  // adjusting
  switch(getncgResize())
  {
    case Qncg::TopLeft:
    {
      break;
    }

    case Qncg::Top:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width
      break;
    }

    case Qncg::TopRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;
      break;
    }

    case Qncg::Right:
    {
      nextx = x; // fixed x
      nextw = w - dw;
      nexty = y; // fixed y
      nexth = h; // fixed height
      break;
    }

    case Qncg::BottomRight:
    {
      nextx = x; // fixed x
      nextw = w - dw;
      nexty = y; // fixed y
      nexth = h - dh;
      break;
    }

    case Qncg::Bottom:
    {
      nextx = x; // fixed x
      nextw = w; // fixed width
      nexty = y; // fixed y
      nexth = h - dh;
      break;
    }

    case Qncg::BottomLeft:
    {
      nexty = y; // fixed y
      nexth = h - dh;
      break;
    }

    case Qncg::Left:
    {
      nexty = y; // fixed y
      nexth = h; // fixed height
      break;
    }

    default:
      //do nothing?
      break;
  }

  // resizing
  setResizeTop(nexty);
  setResizeLeft(nextx);
  setResizeWidth(nextw);
  setResizeHeight(nexth);

  scene()->update();
}

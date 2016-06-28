#include "StructuralMinimap.h"

StructuralMiniMap::StructuralMiniMap(QWidget *parent)
  : QGraphicsView(parent)
  , mEditorView(NULL)
  , mMode(None)
{
  setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);

  setAttribute(Qt::WA_TranslucentBackground);

  setFrameShape(QFrame::NoFrame);

  setStyleSheet("background: transparent");

  viewport()->setAutoFillBackground(false);
}

void StructuralMiniMap::init(StructuralView *mEditorView)
{
  this->mEditorView = mEditorView;

  setRenderHint(QPainter::Antialiasing, true);

  setInteractive(false);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  StructuralScene *editorViewScene = mEditorView->getScene();
  if (editorViewScene != NULL)
  {
    setScene(editorViewScene);
    // can affect zoom - need to change it if we make another desision about it
    connect(editorViewScene,
            SIGNAL(sceneRectChanged(QRectF)),
            this, SLOT(showScene()));
  }
}

void StructuralMiniMap::setScene(QGraphicsScene *scene)
{
  QGraphicsView::setScene(scene);
  showScene();
}

void StructuralMiniMap::showScene()
{
  if (scene() != NULL)
  {
    setSceneRect(scene()->sceneRect());
    fitInView(sceneRect(), Qt::KeepAspectRatio);
  }
}

void StructuralMiniMap::ensureVisible(QList<QRectF> region)
{
  foreach (QRectF rect, region)
  {
    fitInView(rect, Qt::KeepAspectRatio);
  }
}

void StructuralMiniMap::clear()
{
  mEditorViewRect = QRectF();
  mEditorView = NULL;
}

QRectF StructuralMiniMap::getNewRect()
{
  QRect visibleRect = mEditorView->viewport()->rect();
  QRectF newRect = mEditorView->mapToScene(visibleRect).boundingRect();

  newRect.setTopLeft(newRect.topLeft() - QPointF(2, 2));
  newRect.setBottomRight(newRect.bottomRight() + QPointF(2, 2));

  return newRect;
}

void StructuralMiniMap::wheelEvent(QWheelEvent *event)
{
  setInteractive(true);
  QGraphicsView::wheelEvent(event);
  setInteractive(false);
}

void StructuralMiniMap::mousePressEvent(QMouseEvent *event)
{
  if (mEditorView != NULL)
  {
    mMode = Drag;
    mEditorView->centerOn(mapToScene(event->pos()));
  }
  QGraphicsView::mousePressEvent(event);
}

void StructuralMiniMap::mouseMoveEvent(QMouseEvent *event)
{
  if (mEditorView != NULL && mMode == Drag)
  {
    mEditorView->centerOn(mapToScene(event->pos()));
  }

  QGraphicsView::mouseMoveEvent(event);
}

void StructuralMiniMap::mouseReleaseEvent(QMouseEvent *event)
{
  mMode = None;
  QGraphicsView::mouseReleaseEvent(event);
}

void StructuralMiniMap::resizeEvent(QResizeEvent *event)
{
  showScene();
  QGraphicsView::resizeEvent(event);
}

void StructuralMiniMap::drawForeground(QPainter *painter, QRectF const &rect)
{
  setWindowOpacity(0.5);
  painter->setOpacity(0.5);
  QGraphicsView::drawForeground(painter, rect);
  painter->setPen(Qt::red);

  if (mEditorView != NULL)
      mEditorViewRect = getNewRect();

  drawNonExistentAreas(painter, rect);
  painter->drawRect(mEditorViewRect);
}

void StructuralMiniMap::drawNonExistentAreas(QPainter *painter, QRectF const &rect)
{
  QList<QRectF> areas = getNonExistentAreas(rect);
  foreach (QRectF area, areas)
  {
    painter->fillRect(area, Qt::lightGray);
  }
}

QList<QRectF> StructuralMiniMap::getNonExistentAreas(QRectF const &rect)
{
  QRectF existent = rect.intersected(sceneRect());

  QList<QRectF> areas;
  areas << QRectF(rect.topLeft(), existent.bottomLeft())
        << QRectF(rect.topLeft(), QPointF(rect.right(), existent.top()))
        << QRectF(existent.topRight(), rect.bottomRight())
        << QRectF(QPointF(rect.left(), existent.bottom()), rect.bottomRight());

  return areas;
}

#include "StructuralComposition.h"

#include <QMimeData>
#include <QFileInfo>
#include <QUrl>

#include "StructuralContent.h"
#include "StructuralUtil.h"

StructuralComposition::StructuralComposition(StructuralEntity* parent)
  : StructuralNode(parent)
{
  setWidth(STR_DEFAULT_COMPOSITION_W);
  setHeight(STR_DEFAULT_COMPOSITION_H);

  setHoverable(false);
  setUncollapsed(true);

  setAcceptDrops(true);
}

StructuralComposition::~StructuralComposition()
{

}

void StructuralComposition::collapse(bool notify)
{
  QMap<QString,QString> previous = getStructuralProperties();

  if (!isUncollapsed())
  {
    setHovering(false);

    setTop(getTop() - (getUncollapedHeight()/2 - STR_DEFAULT_CONTENT_H/2));
    setLeft(getLeft() - (getUncollapedWidth()/2 - STR_DEFAULT_CONTENT_W/2));
    setWidth(getUncollapedWidth());
    setHeight(getUncollapedHeight());

    foreach(StructuralEntity* entity, getStructuralEntities()) {
      if (entity->getStructuralCategory() == Structural::Interface) {
        entity->setTop(((entity->getTop()*getUncollapedHeight())/STR_DEFAULT_CONTENT_H));
        entity->setLeft(((entity->getLeft()*getUncollapedWidth())/STR_DEFAULT_CONTENT_W));

      } else {
        entity->setHidden(false);

        entity->setTop(entity->getUncollapedTop());
        entity->setLeft(entity->getUncollapedLeft());

        if (entity->isUncollapsed()) {
          entity->setWidth(entity->getUncollapedWidth());
          entity->setHeight(entity->getUncollapedHeight());
        }
      }

      entity->adjust(true);
    }

    setResizable(true);

  } else {
    setHovering(true);

    setUncollapedWidth(getWidth());
    setUncollapedHeight(getHeight());

    setTop(getTop() + getUncollapedHeight()/2 - STR_DEFAULT_CONTENT_H/2);
    setLeft(getLeft() + getUncollapedWidth()/2 - STR_DEFAULT_CONTENT_W/2);
    setWidth(STR_DEFAULT_CONTENT_W);
    setHeight(STR_DEFAULT_CONTENT_H);

    foreach(StructuralEntity* entity, getStructuralEntities()) {
      if (entity->getStructuralCategory() == Structural::Interface) {
        entity->setTop(((entity->getTop()*STR_DEFAULT_CONTENT_H)/getUncollapedHeight()));
        entity->setLeft(((entity->getLeft()*STR_DEFAULT_CONTENT_W)/getUncollapedWidth()));

      } else {
        entity->setHidden(true);

        entity->setUncollapedTop(entity->getTop());
        entity->setUncollapedLeft(entity->getLeft());

        if (entity->isUncollapsed()) {
          entity->setUncollapedWidth(entity->getWidth());
          entity->setUncollapedHeight(entity->getHeight());
        }
      }

      entity->adjust(true);
    }

    setResizable(false);
  }

  StructuralEntity* parent = getStructuralParent();

  if (parent != NULL)
    parent->adjust(true);

  setUncollapsed(!isUncollapsed());

  if (notify)
    emit changed(getStructuralUid(),getStructuralProperties(),previous,StructuralUtil::createSettings(true,false));
}

void StructuralComposition::draw(QPainter* painter)
{
  if (isUncollapsed()) {
    painter->setRenderHint(QPainter::Antialiasing,true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);

    QColor drawColor = QColor(StructuralUtil::getColor(getStructuralType()));

    painter->setBrush(drawColor);
    painter->setPen(QPen(drawColor.darker(), 0));

    painter->drawEllipse(STR_DEFAULT_ENTITY_PADDING,
                         STR_DEFAULT_ENTITY_PADDING,
                         getWidth(),
                         getHeight());

    painter->setBrush(Qt::NoBrush);

    if (isMoving())
      painter->drawEllipse(getMoveLeft()+STR_DEFAULT_ENTITY_PADDING-getLeft(),
                           getMoveTop()+STR_DEFAULT_ENTITY_PADDING-getTop(),
                           getWidth(),
                           getHeight());

    else if (isResizing())
      painter->drawEllipse(getResizeLeft()+STR_DEFAULT_ENTITY_PADDING-getLeft(),
                           getResizeTop()+STR_DEFAULT_ENTITY_PADDING-getTop(),
                           getResizeWidth(),
                           getResizeHeight());

  } else {
    painter->setRenderHint(QPainter::Antialiasing,false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->drawPixmap(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                        STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                        getWidth() - 2*STR_DEFAULT_CONTENT_PADDING,
                        getHeight() - 2*STR_DEFAULT_CONTENT_PADDING - 4*STR_DEFAULT_CONTENT_PADDING,
                        QPixmap(StructuralUtil::getIcon(getStructuralType())));

    painter->setBrush(Qt::NoBrush);
    painter->setPen(QPen(QBrush(Qt::black),0));

    QString text = getStructuralId();

    if (text.isEmpty())
      text = "(?)";

    if (text.length() > 5)
      text = text.replace(3, text.length()-3, "...");

    painter->drawText(STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING,
                      STR_DEFAULT_ENTITY_PADDING + STR_DEFAULT_CONTENT_PADDING + getHeight() - STR_DEFAULT_CONTENT_TEXT_H - 2*STR_DEFAULT_CONTENT_PADDING,
                      getWidth() - 2*STR_DEFAULT_CONTENT_PADDING,
                      STR_DEFAULT_CONTENT_TEXT_H,
                      Qt::AlignCenter, text);

    if (isMoving())
      painter->drawRect(getMoveLeft() + STR_DEFAULT_ENTITY_PADDING - getLeft(),
                        getMoveTop() + STR_DEFAULT_ENTITY_PADDING - getTop(),
                        getWidth(),
                        getHeight());
  }
}

void StructuralComposition::delineate(QPainterPath* painter) const
{
  painter->addEllipse(STR_DEFAULT_ENTITY_PADDING,
                      STR_DEFAULT_ENTITY_PADDING,
                      getWidth(),
                      getHeight());
}

void StructuralComposition::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  StructuralNode::dragEnterEvent(event);

  QList<QUrl> list = event->mimeData()->urls();

  if (!list.isEmpty())
    event->acceptProposedAction();
}

void StructuralComposition::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  StructuralNode::dropEvent(event);

  QList<QUrl> list = event->mimeData()->urls();

  if (!list.isEmpty()) {
    event->acceptProposedAction();

    foreach(QUrl url, list) {
      QString filename = url.toLocalFile();
      QString suffix = QFileInfo(filename).suffix().toLower();

      QMap<QString,QString> properties;
      properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Media);
      properties[STR_PROPERTY_ENTITY_ID] = StructuralUtil::formatId(QFileInfo(filename).baseName());
      properties[STR_PROPERTY_CONTENT_MIMETYPE] = StructuralUtil::translateMimeTypeToString(StructuralUtil::getMimeTypeByExtension(suffix));
      properties[STR_PROPERTY_CONTENT_LOCATION] = filename;

      inserted(StructuralUtil::createUid(), getStructuralUid(), properties, StructuralUtil::createSettings());
    }
  }
}

void StructuralComposition::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  StructuralNode::mouseDoubleClickEvent(event);

  collapse(true);
}

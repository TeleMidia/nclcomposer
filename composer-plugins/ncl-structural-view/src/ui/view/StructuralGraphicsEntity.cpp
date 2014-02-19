#include "StructuralGraphicsEntity.h"
#include "StructuralGraphicsMedia.h"
#include "StructuralGraphicsComposition.h"

//
// ATTENTION: This code needs a refactoring.
//

StructuralGraphicsEntity::StructuralGraphicsEntity(StructuralGraphicsEntity* parent)
  : QncgGraphicsEntity(parent), StructuralEntity(parent)
{
  setnstType(Structural::NoType);

  setnstGraphicsParent(parent);

  if(parent)
    parent->addnstGraphicsEntity(this);

  connect(this, SIGNAL(entitySelected()), SLOT(requestEntitySelection()));
  connect(this, SIGNAL(entityAboutToChange(QMap<QString,QString>)),
                SLOT(requestEntityPreparation(QMap<QString,QString>)));

  hover = false;
  menu = NULL;
  draggable = false;
  hasError = false;
  enableMouseHoverHighlight = true;
}


StructuralGraphicsEntity::~StructuralGraphicsEntity()
{

}

void StructuralGraphicsEntity::setnstId(const QString &id)
{
  StructuralEntity::setnstId(id);
  updateToolTip();
}

StructuralGraphicsEntity* StructuralGraphicsEntity::getnstGraphicsParent() const
{
  return parent;
}

void StructuralGraphicsEntity::setnstGraphicsParent(StructuralGraphicsEntity* parent)
{
  this->parent = parent;

  setnstParent(parent);
  setncgGraphicsParent(parent);
}

QSet<StructuralGraphicsEntity*> StructuralGraphicsEntity::getnstGraphicsEntities()
{
  return entities;
}

bool StructuralGraphicsEntity::isDraggable()
{
  return draggable;
}

void StructuralGraphicsEntity::setDraggable(bool isDraggable)
{
  this->draggable = isDraggable;
}

void StructuralGraphicsEntity::addnstGraphicsEntity(StructuralGraphicsEntity* entity)
{
  if (entity != NULL)
  {
    // \fixme This is only an additional check. Maybe, the better way to do
    // that could be using a set instead of a vector to entities.
    if(!entities.contains(entity))
    {
      entities.insert(entity);

      // connect(entity, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
      // connect(entity, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

      connect(entity, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
      connect(entity, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
      connect(entity, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

      connect(entity, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

      connect(entity, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

      connect(entity, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
      connect(entity, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
      connect(entity, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
      connect(entity, SIGNAL(fullscreenRequested()),
                      SIGNAL(fullscreenRequested()));

      connect(entity, SIGNAL(entityAdded(StructuralGraphicsEntity*)),
                      SIGNAL(entityAdded(StructuralGraphicsEntity*)));
      connect(entity, SIGNAL(entityChanged(StructuralGraphicsEntity*)),
                      SIGNAL(entityChanged(StructuralGraphicsEntity*)));

      connect(entity, SIGNAL(entityAboutToChange(StructuralGraphicsEntity*,
                                                 QMap<QString,QString>)),
                      SIGNAL(entityAboutToChange(StructuralGraphicsEntity*,
                                                 QMap<QString,QString>)));
      connect(entity, SIGNAL(entityRemoved(StructuralGraphicsEntity*)),
                      SIGNAL(entityRemoved(StructuralGraphicsEntity*)));

      connect(entity, SIGNAL(entitySelected(StructuralGraphicsEntity*)),
                      SIGNAL(entitySelected(StructuralGraphicsEntity*)));

      addnstEntity(entity);
      addncgGraphicsEntity(entity);

      if(entity)
        entity->setnstGraphicsParent(this);
    }
    else
    {
      qWarning() << "[QNST] Warning! You are adding the same entity twice as \
        child of " << QString().sprintf("%p", this) << __FILE__ << __LINE__;
    }
  }
}

void StructuralGraphicsEntity::removenstGraphicsEntity(StructuralGraphicsEntity* entity)
{
  if (entity != NULL)
  {
    entities.remove(entity);

    removenstEntity(entity);
    removencgGraphicsEntity(entity);

    entity->setnstGraphicsParent(NULL);
  }
}

void StructuralGraphicsEntity::requestEntityChange()
{
  emit entityChanged(this);
}

void StructuralGraphicsEntity::requestEntityPreparation(QMap<QString, QString> props)
{
  emit entityAboutToChange(this, props);
}

void StructuralGraphicsEntity::requestEntitySelection()
{
  emit entitySelected(this);
}

bool StructuralGraphicsEntity::createEntity(Structural::EntityType type)
{
  // \todo Check if type is an media type allowed to me!
  StructuralGraphicsEntity *entity = StructuralUtil::makeGraphicsEntity(type, this);

  if(entity == NULL) return false;

  StructuralGraphicsMedia *content  = dynamic_cast<StructuralGraphicsMedia*>(entity);

  if(content != NULL) // If the Entity is a Media content
  {
    content->adjust();

    if (dropsrc != "") //if it is a drop we will keep the baseName as id
    {
      content->setSource(dropsrc);
      QFileInfo file = QFileInfo(dropsrc);
      QString nstId = file.baseName();
      entity->setnstId(nstId);
      dropsrc = "";
    }
  }
  else
  {
    StructuralGraphicsComposition *composition =
        dynamic_cast<StructuralGraphicsComposition*>(entity);

    //If the Entity is a Composition (i.e. Body, Context or Switch)
    if(composition != NULL)
    {
      composition->adjust();

      composition->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());
    }
  }

  entity->adjust();

  emit entityAdded(entity);
}

void StructuralGraphicsEntity::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
  QncgGraphicsEntity::contextMenuEvent(event);

  if (!event->isAccepted())
  {
    if (!isSelected())
    {
      setSelected(true);
      emit entitySelected(this);
    }

    if (menu != NULL)
      menu->exec(event->screenPos());

    event->accept();
  }
}

bool StructuralGraphicsEntity::hasMouseHover()
{
  return hover;
}

void StructuralGraphicsEntity::setMouseHover(bool hover)
{
  this->hover = hover;
}

void StructuralGraphicsEntity::setMouseHoverHighlight(bool enable)
{
  this->enableMouseHoverHighlight = enable;
}

void StructuralGraphicsEntity::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  QncgGraphicsEntity::hoverEnterEvent(event);

  hover = true;
}

void StructuralGraphicsEntity::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  QncgGraphicsEntity::hoverLeaveEvent(event);

  hover = false;
}

void StructuralGraphicsEntity::setError(bool hasError)
{
  this->hasError = hasError;
}

void StructuralGraphicsEntity::setErrorMsg(QString erroMsg)
{
  this->erroMsg = erroMsg;
}

void StructuralGraphicsEntity::setProperties(const QMap<QString, QString> &props)
{
  if(props.contains("id"))
    setnstId(props["id"]);

  if (props["top"] != "")
    setTop(props["top"].toDouble());

  if (props["left"] != "")
    setLeft(props["left"].toDouble());

  if (props["width"] != "")
    setWidth(props["width"].toDouble());

  if (props["height"] != "")
    setHeight(props["height"].toDouble());
}

void StructuralGraphicsEntity::getProperties(QMap <QString, QString> &props)
{
  props["id"] = getnstId();

  props["top"] = QString::number(getTop());
  props["left"] = QString::number(getLeft());
  props["width"] = QString::number(getWidth());
  props["height"] = QString::number(getHeight());
  props["zIndex"] = QString::number(getzIndex());
}

void StructuralGraphicsEntity::drawMouseHoverHighlight(QPainter *painter)
{
  if(enableMouseHoverHighlight)
  {
    // Draw MouseHover rectangle
    if (!isSelected() && hasMouseHover())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

      painter->drawRect(4, 4, getWidth(), getHeight());
    }
  }
}

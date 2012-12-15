#include "qnstgraphicsentity.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicscomposition.h"

//
// ATTENTION: This code needs a refactoring.
//

QnstGraphicsEntity::QnstGraphicsEntity(QnstGraphicsEntity* parent)
  : QncgGraphicsEntity(parent), QnstEntity(parent)
{
  setnstType(Qnst::NoType);

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
}


QnstGraphicsEntity::~QnstGraphicsEntity()
{

}

void QnstGraphicsEntity::setnstId(const QString &id)
{
  QnstEntity::setnstId(id);
  updateToolTip();
}

QnstGraphicsEntity* QnstGraphicsEntity::getnstGraphicsParent() const
{
  return parent;
}

void QnstGraphicsEntity::setnstGraphicsParent(QnstGraphicsEntity* parent)
{
  this->parent = parent;

  setnstParent(parent);
  setncgGraphicsParent(parent);
}

QSet<QnstGraphicsEntity*> QnstGraphicsEntity::getnstGraphicsEntities()
{
  return entities;
}

bool QnstGraphicsEntity::isDraggable()
{
  return draggable;
}

void QnstGraphicsEntity::setDraggable(bool isDraggable)
{
  this->draggable = isDraggable;
}

void QnstGraphicsEntity::addnstGraphicsEntity(QnstGraphicsEntity* entity)
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

      connect(entity, SIGNAL(entityAdded(QnstGraphicsEntity*)),
                      SIGNAL(entityAdded(QnstGraphicsEntity*)));
      connect(entity, SIGNAL(entityChanged(QnstGraphicsEntity*)),
                      SIGNAL(entityChanged(QnstGraphicsEntity*)));

      connect(entity, SIGNAL(entityAboutToChange(QnstGraphicsEntity*,
                                                 QMap<QString,QString>)),
                      SIGNAL(entityAboutToChange(QnstGraphicsEntity*,
                                                 QMap<QString,QString>)));
      connect(entity, SIGNAL(entityRemoved(QnstGraphicsEntity*)),
                      SIGNAL(entityRemoved(QnstGraphicsEntity*)));

      connect(entity, SIGNAL(entitySelected(QnstGraphicsEntity*)),
                      SIGNAL(entitySelected(QnstGraphicsEntity*)));

      addnstEntity(entity);
      addncgGraphicsEntity(entity);

      if(entity)
        entity->setnstGraphicsParent(this);
    }
    else
    {
#ifdef Q_WS_MAC
      qWarning() << "[QNST] Warning! You are adding the same entity twice as \
        child of " << this << __FILE__ << __LINE__;
#else
      qWarning() << "[QNST] Warning! You are adding the same entity twice as \
        child of " << (int) this << __FILE__ << __LINE__;
#endif
    }
  }
}

void QnstGraphicsEntity::removenstGraphicsEntity(QnstGraphicsEntity* entity)
{
  if (entity != NULL)
  {
    entities.remove(entity);

    removenstEntity(entity);
    removencgGraphicsEntity(entity);

    entity->setnstGraphicsParent(NULL);
  }
}

void QnstGraphicsEntity::requestEntityChange()
{
  emit entityChanged(this);
}

void QnstGraphicsEntity::requestEntityPreparation(QMap<QString, QString> props)
{
  emit entityAboutToChange(this, props);
}

void QnstGraphicsEntity::requestEntitySelection()
{
  emit entitySelected(this);
}

bool QnstGraphicsEntity::createEntity(Qnst::EntityType type)
{
  // \todo Check if type is an media type allowed to me!
  QnstGraphicsEntity *entity = QnstUtil::makeGraphicsEntity(type, this);

  if(entity == NULL) return false;

  QnstGraphicsMedia *content  = dynamic_cast<QnstGraphicsMedia*>(entity);

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
    QnstGraphicsComposition *composition =
        dynamic_cast<QnstGraphicsComposition*>(entity);

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

void QnstGraphicsEntity::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
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

bool QnstGraphicsEntity::hasMouseHover()
{
  return hover;
}

void QnstGraphicsEntity::setMouseHover(bool hover)
{
  this->hover = hover;
}

void QnstGraphicsEntity::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  QncgGraphicsEntity::hoverEnterEvent(event);

  hover = true;
}

void QnstGraphicsEntity::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  QncgGraphicsEntity::hoverLeaveEvent(event);

  hover = false;
}

void QnstGraphicsEntity::setError(bool hasError)
{
  this->hasError = hasError;
}

void QnstGraphicsEntity::setProperties(const QMap<QString, QString> &props)
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

void QnstGraphicsEntity::getProperties(QMap <QString, QString> &props)
{
  props["id"] = getnstId();

  props["top"] = QString::number(getTop());
  props["left"] = QString::number(getLeft());
  props["width"] = QString::number(getWidth());
  props["height"] = QString::number(getHeight());
  props["zIndex"] = QString::number(getzIndex());
}

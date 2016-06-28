#include "StructuralComposition.h"

#include "StructuralContent.h"
#include "StructuralUtil.h"

#include <QDebug>

StructuralComposition::StructuralComposition(StructuralEntity* parent)
  : StructuralNode(parent)
{
  //setnstSubtype(Qnst::Composition);

  setColor("#EEEEEE");

  setAcceptDrops(true);


  lastw = 0;
  lasth = 0;

  setCollapsed(false);

  tmp = NULL;

  //tmp = new QnstGraphicsComposition();
  //tmp->hide();

  /* Default size and position */
  if(parent)
  {
    setTop(parent->getHeight()/2 - DEFAULT_CONTEXT_HEIGHT/2);
    setLeft(parent->getWidth()/2 - DEFAULT_CONTEXT_WIDTH/2);
  }
  else
  {
    setTop(0);
    setLeft(0);
  }

  setWidth(DEFAULT_CONTEXT_WIDTH);
  setHeight(DEFAULT_CONTEXT_HEIGHT);


  createObjects();
  createConnections();

  // By default composition will not be able to show mouse hover highlight
  setHovering(false);
  setAcceptDrops(true);
}

StructuralComposition::~StructuralComposition()
{

}

void StructuralComposition::createObjects()
{
  menu->deleteAction->setEnabled(true);

  menu->snapshotAction->setEnabled(true);

  menu->insertMenu->setEnabled(true);

  menu->mediaAction->setEnabled(true);
  menu->contextAction->setEnabled(true);
  menu->switchAction->setEnabled(true);
  menu->portAction->setEnabled(true);
  menu->propertyAction->setEnabled(true);

}

void StructuralComposition::createConnections()
{
  // connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
  // connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));


//  connect(menu, SIGNAL(menuAddEntityTriggered(QnstSubtype)),SLOT(newChild(QnstSubtype)));

//  connect(menu, SIGNAL(autoRequested()), SLOT(adjustWithSpring()));
}

QString StructuralComposition::getColor() const
{
  return color;
}

void StructuralComposition::setColor(QString color)
{
  this->color = color;
}

void StructuralComposition::setStructuralProperty(const QString &name, const QString &value)
{
  if (name == PLG_ENTITY_COLLAPSED)
    setCollapsed((value == "1" ? true : false));
  else
    StructuralNode::setStructuralProperty(name,value);
}

void StructuralComposition::setStructuralType(const StructuralType subtype)
{
  if (subtype == Structural::Context){
    setColor("#F4E4CC");
    menu->switchPortAction->setEnabled(false);
    menu->portAction->setEnabled(true);
    if (getStructuralType() == Structural::NoType){
      setWidth(DEFAULT_CONTEXT_WIDTH);
      setHeight(DEFAULT_CONTEXT_HEIGHT);
    }
  }else if (subtype == Structural::Switch){
    setColor("#C6E2FF");
    menu->switchPortAction->setEnabled(true);
    menu->portAction->setEnabled(false);
    if (getStructuralType() == Structural::NoType){
      setWidth(DEFAULT_CONTEXT_WIDTH);
      setHeight(DEFAULT_CONTEXT_HEIGHT);
    }
  }else if (subtype == Structural::Body){
    setColor("#EEEEEE");
    menu->switchPortAction->setEnabled(false);
    menu->portAction->setEnabled(true);
    if (getStructuralType() == Structural::NoType){
      setWidth(DEFAULT_BODY_WIDTH);
      setHeight(DEFAULT_BODY_HEIGHT);
    }
  }

  StructuralNode::setStructuralType(subtype);

  refresh();
}

void StructuralComposition::refresh()
{
  QString tip = "";
  QString name = (getStructuralId() != "" ? getStructuralId() : "?");

  if (getStructuralType() == Structural::Context)
  {
    tip += "Context ("+name+")";
  }
  else if (getStructuralType() == Structural::Switch)
  {
    tip += "Switch ("+name+")";
  }
  else if (getStructuralType() == Structural::Body)
  {
    tip += "Body ("+name+")";
  }
  else
  {
    tip += "Composition ("+name+")";
  }

  if (hasError)
    tip += " - Error: " + erroMsg;

  setToolTip(tip);
}

void StructuralComposition::draw(QPainter* painter)
{
  if (!isCollapsed())
  {
    painter->setRenderHint(QPainter::Antialiasing,true);

    painter->setBrush(QColor(color));

    if (hasError)
      painter->setPen(QPen(QBrush(Qt::red), 2));
    else
      painter->setPen(QPen(QBrush(Qt::black), 0));

    painter->drawEllipse(4, 4, getWidth()-1, getHeight()-1);

    drawMouseHoverHighlight(painter); // This should not be HERE!!

    if (isMoving())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

      painter->drawEllipse(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);

    }
    else if (isResizing())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

      painter->drawEllipse(getResizeLeft()+4-getLeft(), getResizeTop()+4-getTop(), getResizeWidth()-1, getResizeHeight()-1);
    }
  }
  else
  {
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(StructuralUtil::iconFromEntityType(getStructuralType())));

    drawMouseHoverHighlight(painter); // This should not be HERE!!

    painter->setPen(QPen(QBrush(Qt::black),0));

    // draw a formated text with underline when there is error
    if(hasError)
    {
      painter->drawPixmap((getWidth()-8)/2 + 12, (getHeight()-8)/2 + 4, 12, 12, QPixmap(":/icon/delete"));

      int N_STEPS = 20;
      int begin_w = 8;
      int end_w = getWidth();
      double current_x = begin_w;
      double step = (double) ( end_w - begin_w ) / N_STEPS;

      QPolygonF polygon;
      painter->setPen(QPen(QBrush(Qt::red), 0)); // 0px = cosmetic border
      painter->setRenderHint(QPainter::Antialiasing, true);

      for (int i = 0; i < N_STEPS; i++)
      {
        current_x = begin_w + (double) i * step;

        if( i % 2)
          polygon << QPointF( current_x, getHeight() - 3 );
        else
          polygon << QPointF( current_x, getHeight() );
      }

      painter->drawPolyline(polygon);
    }

    QString localid = (getStructuralId() != "" ? getStructuralId() : "?");

    if (localid.length() > 5)
    {
      localid = localid.replace(3,localid.length()-3,"...");
    }

    painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignCenter, localid);

    if (isMoving())
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

      painter->setRenderHint(QPainter::Antialiasing,false);
      painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);
    }
  }
}

void StructuralComposition::delineate(QPainterPath* painter) const
{
  painter->addEllipse(4, 4, getWidth(), getHeight());
}

void StructuralComposition::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
  bool enter = false;

  foreach(QUrl url, event->mimeData()->urls())
  {
    event->acceptProposedAction();

    enter = true;
  }

    if (!enter)
  StructuralNode::dragEnterEvent(event);
}

void StructuralComposition::collapse(bool notify)
{

  QMap<QString,QString> prev = getStructuralProperties();

  if (isCollapsed())
  {
    setHovering(false);

    setTop(getTop() - (getUncollapedHeight()/2 - DEFAULT_MEDIA_HEIGHT/2));
    setLeft(getLeft() - (getUncollapedWidth()/2 - DEFAULT_MEDIA_WIDTH/2));
    setWidth(getUncollapedWidth());
    setHeight(getUncollapedHeight());

    foreach(StructuralEntity* e, getStructuralEntities())
    {
      if (e->getStructuralCategory() == Structural::Interface)
      {
        e->setTop(((e->getTop()*getUncollapedHeight())/DEFAULT_MEDIA_HEIGHT));
        e->setLeft(((e->getLeft()*getUncollapedWidth())/DEFAULT_MEDIA_WIDTH));
      }
      else
      {
//        e->setStructuralParent(this);
//        tmp->remove(e);
        e->setHidden(false);

        if (!e->isCollapsed())
        {
          e->setWidth(e->getUncollapedWidth());
          e->setHeight(e->getUncollapedHeight());
        }

        e->setTop(e->getUncollapedTop());
        e->setLeft(e->getUncollapedLeft());

      }

      e->adjust();

    }

//    adjust();

//    setUncollapedWidth(getWidth());
//    setUncollapedHeight(getHeight());

    setResizable(true);

  }
  else
  {
    setHovering(true);

//    if (getUncollapedHeight() == 0 && getUncollapedWidth() == 0){
      setUncollapedWidth(getWidth());
      setUncollapedHeight(getHeight());
//    }

    setTop(getTop() + getUncollapedHeight()/2 - DEFAULT_MEDIA_HEIGHT/2);
    setLeft(getLeft() + getUncollapedWidth()/2 - DEFAULT_MEDIA_WIDTH/2);
    setWidth(DEFAULT_MEDIA_WIDTH);
    setHeight(DEFAULT_MEDIA_HEIGHT);


    foreach(StructuralEntity* e, getStructuralEntities())
    {

      if (e->getStructuralCategory() == Structural::Interface)
      {

        e->setTop(((e->getTop()*DEFAULT_MEDIA_HEIGHT)/getUncollapedHeight()));
        e->setLeft(((e->getLeft()*DEFAULT_MEDIA_WIDTH)/getUncollapedWidth()));

      }else{
//        e->setStructuralParent(NULL);
        e->setHidden(true);

        if (!e->isCollapsed())
        {
          e->setUncollapedWidth(e->getWidth());
          e->setUncollapedHeight(e->getHeight());
        }

        e->setUncollapedTop(e->getTop());
        e->setUncollapedLeft(e->getLeft());


//        remove(e);
      }

       e->adjust();
    }


    adjust();


    setResizable(false);
  }

  if (getStructuralParent() != NULL)
  {
    getStructuralParent()->adjust();
  }

  setCollapsed(!isCollapsed());

  if (notify)
    emit changed(getStructuralUid(),getStructuralProperties(),prev,StructuralUtil::createSettings(true,false));
}

void StructuralComposition::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
  collapse(true);
}


void StructuralComposition::dropEvent(QGraphicsSceneDragDropEvent *event)
{
  if(event->mimeData()->urls().size() > 0)
  {
  foreach(QUrl url, event->mimeData()->urls())
  {
    QString filename = url.toLocalFile();
    QString suffix = QFileInfo(filename).suffix().toLower();

    event->acceptProposedAction();
    dropsrc = filename;

    QMap<QString,QString> properties;
    properties[PLG_ENTITY_TYPE] = StructuralUtil::convertSubtype(Structural::Media);
    properties[PLG_ENTITY_ID] = StructuralUtil::normalizeXMLID(QFileInfo(filename).baseName());
    properties[PLG_ENTITY_SRC] = filename;
    properties[PLG_ENTITY_MEDIA] = StructuralUtil::getnstTypeFromExtension(suffix);

    QMap<QString,QString> settings;
    settings[PLG_SETTING_UNDO] = "1";
    settings[PLG_SETTING_NOTIFY] = "1";
    settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();

    inserted(StructuralUtil::CreateUid(), getStructuralUid(), properties, settings);

  }
  }

  StructuralNode::dropEvent(event);
}


void StructuralComposition::adjustWithSpring()
{
}

qreal StructuralComposition::getLastW()
{
  return lastw;
}

qreal StructuralComposition::getLastH()
{
  return lasth;
}

void StructuralComposition::setLastW(qreal lastW)
{
  this->lastw = lastW;
}

void StructuralComposition::setLastH(qreal lastH)
{
  this->lasth = lastH;
}

//void QnstGraphicsComposition::setCollpsed(bool collapsed)
//{
//    this->collapsed = collapsed;
//}

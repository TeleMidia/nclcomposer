#include "StructuralView.h"

//
// ATTENTION: This code needs a refactoring.
//

#include <assert.h>
#include <QDebug>

#include "StructuralUtil.h"
#include "StructuralBind.h"

std::map <Structural::EntityName, QString> StructuralView::mediaTypeToXMLStr =
    create_map<Structural::EntityName, QString>
      (Structural::Media, "media");
//      (Qnst::Image, "image")
//      (Qnst::Audio, "audio")
//      (Qnst::Video, "video")
//      (Qnst::Html, "text/html")
//      (Qnst::NCL, "application/x-ginga-NCL")
//      (Qnst::Text, "text")
//      (Qnst::NCLua, "application/x-ginga-NCLua")
//      (Qnst::Settings, "application/x-ncl-settings");

StructuralView::StructuralView(QWidget* parent)
    : QGraphicsView(parent)
{
  createObjects();
  createConnection();

  zoomStep = 0;


  linking = false;

  modified = false;

  link = NULL;

  _selected = NULL;

  clipboard = NULL;

  e_clip = NULL;

  clip_cut = "";
  clip_copy = "";

  hasCutted = false;

  lastLinkMouseOver = NULL;

  // Initialize entity counters
  for(int i = Structural::Node; i < Structural::NoName; i += 1)
    entityCounter[ (Structural::EntityName) i ] = 0;

  setAttribute(Qt::WA_TranslucentBackground);

  condition = "onBegin";
  action = "start";

//  minimap = new MiniMap(this);
//  minimap->init(this);
//  minimap->setMinimumSize(MINIMAP_DEFAULT_W, MINIMAP_DEFAULT_H);
}

StructuralView::~StructuralView()
{

}

void StructuralView::createObjects()
{
  scene = new StructuralScene();
  scene->setParent(this);
  scene->setSceneRect(0, 0, 3200, 1800);

  setScene(scene);

  this->resize(scene->itemsBoundingRect().size().width(), scene->itemsBoundingRect().height());
}

void StructuralView::createConnection()
{

}

bool StructuralView::hasEntity(QString uid)
{
  if (entities.contains(uid)  ||
      binds.contains(uid) ||
      links.contains(uid))
  {
    return true;
  }

  return false;
}

StructuralEntity* StructuralView::getEntity(QString uid)
{
  if (links.contains(uid))
    return links.value(uid);

  else if (binds.contains(uid))
    return binds.value(uid);

  else if (entities.contains(uid))
    return entities.value(uid);

  return NULL;
}

/**********************************************************/

void StructuralView::serialize(QString &data)
{
  QDomDocument* document = new QDomDocument();
  QDomElement root = document->createElement("structural");

  foreach(StructuralEntity* entity, entities.values())
  {
    if (entity->getLocalName() == Structural::Body)
    {
      exportDataFromEntity(entity, document, root);
    }
  }

  document->appendChild(root);
  data = document->toString();

  qDebug() << data;

}

void StructuralView::exportDataFromEntity(StructuralEntity* entity, QDomDocument* doc, QDomElement parent)
{  
  QDomElement element = doc->createElement(StructuralUtil::getStrFromNstType(entity->getLocalName()));

  element.setAttribute("uid", entity->getLocalUid());

  foreach (QString name, entity->getLocalProperties().keys())
  {
    QDomElement property = doc->createElement("property");
    property.setAttribute("name",name);
    property.setAttribute("value", entity->getLocalProperty(name));

    element.appendChild(property);
  }

  foreach (StructuralEntity* child, entity->getLocalChildren()) {
    exportDataFromEntity(child, doc, element);
  }

  parent.appendChild(element);
}

void StructuralView::insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  if (!entities.contains(uid))
  {
    StructuralEntity* entity; LocalName type = StructuralUtil::getnstTypeFromStr(properties["LOCAL:NAME"]);

    switch (type)
    {
      case Structural::Context:
      {
        entity = new StructuralComposition();
        entity->setLocalName(Structural::Context);

        break;
      }

      case Structural::Switch:
      {
        entity = new StructuralComposition();
        entity->setLocalName(Structural::Switch);
        break;
      }

      case Structural::Media:
      {
        entity = new StructuralMedia();
        break;
      }

      case Structural::Port:
      {
        entity = new StructuralInterface();
        entity->setLocalName(Structural::Port);
        break;
      }

      case Structural::SwitchPort:
      {
        entity = new StructuralInterface();
        entity->setLocalName(Structural::SwitchPort);
        break;
      }

      case Structural::Area:
      {
        entity = new StructuralInterface();
        entity->setLocalName(Structural::Area);
        break;
      }

      case Structural::Property:
      {
        entity = new StructuralInterface();
        entity->setLocalName(Structural::Property);
        break;
      }

      case Structural::Link:
      {
        entity = new StructuralLink();
        break;
      }

      case Structural::Bind:
      {
        if (entities.contains(properties.value(":nst:a")) && entities.contains(properties.value(":nst:b")))
        {
            entity = new StructuralBind();

            StructuralBind* bind = (StructuralBind*) entity;
            bind->setEntityA(entities[properties.value(":nst:a")]);
            bind->setEntityB(entities[properties.value(":nst:b")]);

            qDebug() << "================" << properties.value(":nst:bindtype");
            bind->setType(StructuralUtil::getBindTypeFromStr(properties.value(":nst:bindtype")));
        }

        break;
      }

      case Structural::Reference:
      {
        if (entities.contains(properties.value(":nst:a")) && entities.contains(properties.value(":nst:b")))
        {
            StructuralEntity* a = entities[properties.value(":nst:a")];

            if (a->getLocalName() == Structural::Port)
            {
                foreach (StructuralEntity* e, entities.values()) {
                    if (e->getLocalName() == Structural::Reference)
                    {
                        StructuralReference* r = (StructuralReference*) e;

                        if (e->getLocalProperty(":nst:a") == a->getLocalUid())
                        {
                            QMap<QString, QString> localset;
                            localset["UNDO"] = "1";
                            localset["NOTIFY"] = "1";
                            localset["CODE"] = settings["CODE"];

                            remove(r->getLocalUid(),localset);
                        }
                    }
                }
            }

            entity = new StructuralReference(entities[parent]);

            StructuralReference* bind = (StructuralReference*) entity;
            bind->setEntityA(entities[properties.value(":nst:a")]);
            bind->setEntityB(entities[properties.value(":nst:b")]);
        }

        break;
      }

      case Structural::Body:
      {
        entity = new StructuralComposition();
        entity->setLocalName(Structural::Body);
        break;
      }

      default:
      {
        entity = NULL;
        break;
      }
    }

    if (entity != NULL)
    {
      if (entities.contains(parent))
      {
        entity->setLocalParent(entities[parent]);
      }
      else
      {
        scene->addItem(entity);

        entity->setWidth(DEFAULT_BODY_WIDTH);
        entity->setHeight(DEFAULT_BODY_HEIGHT);
        entity->setTop(scene->sceneRect().height()/2 - entity->getHeight()/2);
        entity->setLeft(scene->sceneRect().width()/2 - entity->getWidth()/2);
      }

      entity->setLocalUid(uid);
      entity->setLocalProperties(properties);


      entities[uid] = entity;
      if (entity->getLocalProperty("LOCAL:ID").isEmpty())
        updateEntityWithUniqueNstId(entity);

      entity->updateToolTip();

      connect(entity, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(removed(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(entity, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      entity->adjust(true);

      if (settings["UNDO"] == "1")
      {
        Insert* command = new Insert(uid, parent, entity->getLocalProperties(), settings);

        connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
        connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

        command->setText(settings["CODE"]);

        commnads.push(command);
      }

      if (settings["NOTIFY"] == "1")
      {
        emit inserted(uid, parent, entity->getLocalProperties(), settings);
      }
    }
  }
}

Command* StructuralView::rmcmd(StructuralEntity* parent, Command* cmdparent, QMap<QString, QString> settings)
{
  foreach (StructuralEntity* entity, parent->getLocalChildren())
  {
      //qDebug() << entity->getnstUid();
    Remove* command = new Remove(entity->getLocalUid(), entity->getLocalParent()->getLocalUid(), entity->getLocalProperties(), settings, cmdparent);

    connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
    connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
    connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
    connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

    rmcmd(entity, command, settings);
  }
}

void StructuralView::remove(QString uid, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
    if (settings["UNDO"] == "1")
    {
      Remove* command;

      if (entities[uid]->getLocalParent() != NULL)
        command = new Remove(entities[uid]->getLocalUid(), entities[uid]->getLocalParent()->getLocalUid(), entities[uid]->getLocalProperties(), settings);
      else
        command = new Remove(entities[uid]->getLocalUid(), "", entities[uid]->getLocalProperties(), settings);

      command->setText(settings["CODE"]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      foreach (StructuralEntity* entity, entities.values())
      {
          if (entity->getLocalType() == Structural::Edge)
          {
              StructuralEdge* bind = (StructuralEdge*) entity;

              if (bind->getLocalProperty(":nst:a") == uid || bind->getLocalProperty(":nst:b") == uid)
              {
                  remove(bind->getLocalUid(), settings);
              }
          }
      }

      foreach (StructuralEntity* entity, entities[uid]->getLocalChildren())
      {
          remove(entity->getLocalUid(), settings);
      }

      commnads.push(command); return;
    }

    StructuralEntity* entity = entities[uid];

    StructuralEntity* parent = entity->getLocalParent();

    if (parent != NULL)
    {
      parent->removeLocalChild(entity);
    }
    else
      scene->removeItem(entity);

    entities.remove(uid);

    delete entity; entity = NULL; _selected = NULL;

    if (settings["NOTIFY"] == "1")
    {
      emit removed(uid, settings);
    }
  }
}

void StructuralView::change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
    if (settings["UNDO"] == "1")
    {
      Change* command = new Change(uid, properties, previous, settings);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      command->setText(settings["CODE"]);

      commnads.push(command); return;
    }

     StructuralEntity* entity = entities[uid];
     entity->setLocalProperties(properties);
     entity->adjust();

     foreach (StructuralEntity* entity, entities.values())
     {
         if (entity->getLocalType() == Structural::Edge)
         {
             if (entity->getLocalProperty(":nst:a") == uid ||
                 entity->getLocalProperty(":nst:b") == uid)
             entity->adjust();
         }
     }

    if (settings["NOTIFY"] == "1")
    {
      emit changed(uid, properties, previous, settings);
    }
  }
}

void StructuralView::select(QString uid, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
      StructuralEntity* entity = entities[uid];
      entity->setSelected(true);

      if (_selected != NULL && _selected != 0)
      {
        _selected->setSelected(false);
        _selected->adjust();
      }

      _selected = entity;

      // TODO
//      if (settings["NOTIFY"] == "1")
//      {
        emit selected(uid, settings);
//      }
  }
}

void StructuralView::create(LocalName name, QMap<QString, QString> &properties, QMap<QString, QString> &settings)
{
  QString uid = StructuralUtil::createUid();
  QString parent = "";

  switch (name)
  {
//    case Structural::Body:
//    {
//      if (!properties.contains("LOCAL:TOP"))
//        properties["LOCAL:TOP"] = QString::number(scene->height()/2 - DEFAULT_BODY_HEIGHT/2);

//      if (!properties.contains("LOCAL:LEFT"))
//        properties["LOCAL:LEFT"] = QString::number(scene->width()/2 - DEFAULT_BODY_WIDTH/2);

//      if (!properties.contains("LOCAL:WIDTH"))
//        properties["LOCAL:WIDTH"] = QString::number(DEFAULT_BODY_WIDTH);

//      if (!properties.contains("LOCAL:HEIGHT"))
//        properties["LOCAL:HEIGHT"] = QString::number(DEFAULT_BODY_HEIGHT);

//      break;
//    }

    default:
    {
      if (_selected != NULL)
      {
        parent = _selected->getLocalUid();
      }

      break;
    }
  }

  if (!properties.contains("LOCAL:NAME"))
    properties["LOCAL:NAME"] = StructuralUtil::getStrFromNstType(name);

  if (!settings.contains("UNDO"))
    settings["UNDO"] = "1";

  if (!settings.contains("NOTIFY"))
    settings["NOTIFY"] = "1";

  if (!settings.contains("CODE"))
    settings["CODE"] = StructuralUtil::createUid();

  insert(uid, parent, properties, settings);
}


bool StructuralView::performHelp()
{
    return false;
}

bool StructuralView::performUndo()
{
  if (commnads.canUndo())
  {
      QString code = commnads.undoText();

      while(code == commnads.undoText())
        commnads.undo();

      return true;
  }

  return false;
}

bool StructuralView::performRedo()
{
  if (commnads.canRedo())
  {
    QString code = commnads.redoText();

    while(code == commnads.redoText())
      commnads.redo();

    return true;
  }

  return false;
}

bool StructuralView::performCut()
{
    if (_selected != NULL)
    {
      if (!clip_copy.isEmpty())
      {
        clip_copy =  "";
      }

      if (!clip_cut.isEmpty())
      {
        QMap<QString, QString> settings;
        settings["UNDO"] = "0";
        settings["NOTIFY"] = "0";
        settings["CODE"] = StructuralUtil::createUid();

        remove(clip_cut, settings);
      }



        clip_cut = _selected->getLocalUid();

//        _selected->getnstParent()->removeChild(_selected);
//        _selected->setnstParent(NULL);

        QMap<QString, QString> previous;
        previous = _selected->getLocalProperties();

        QMap<QString, QString> properties;
        properties = _selected->getLocalProperties();
        properties["hidden"] = "true";

        QMap<QString, QString> settings;
        settings["UNDO"] = "1";
        settings["NOTIFY"] = "1";
        settings["CODE"] = StructuralUtil::createUid();

        change(clip_cut, properties, previous, settings);

        return true;
    }

    return false;
}

//void StructuralView::rec_clip(StructuralEntity* e, StructuralEntity* parent)
//{
//  parent->setnstUid(e->getnstUid());
//  parent->setnstProperties(e->getnstProperties());

//  foreach (StructuralEntity* child, e->getnstChildren()) {
//    StructuralEntity* e_c = new StructuralEntity();

//    e_c->setnstParent(parent);

//    rec_clip(child, e_c);
//  }
//}

bool StructuralView::performCopy()
{
    if (_selected != NULL)
    {
      if (!clip_cut.isEmpty())
      {
        QMap<QString, QString> settings;
        settings["UNDO"] = "0";
        settings["NOTIFY"] = "0";
        settings["CODE"] = StructuralUtil::createUid();

        remove(clip_cut, settings);

        clip_cut = "";
      }

        clip_copy = _selected->getLocalUid();
        //qDebug() << "======================" << clip;

        return true;
    }

    return false;
}

bool StructuralView::isChild(StructuralEntity* e , StructuralEntity* p)
{
    bool r = false;

    foreach (StructuralEntity* ec, p->getLocalChildren()) {
        if (ec == e)
        {
            return true;
        }
        else
        {
            isChild(e, ec);
        }
    }

    return r;
}

bool StructuralView::performPaste()
{
    QString clip = "";

    if (!clip_copy.isEmpty())
      clip = clip_copy;
    else if (!clip_cut.isEmpty())
      clip = clip_cut;

    if (entities.contains(clip))
    {
        if (_selected != NULL && _selected != entities[clip] && !isChild(_selected, entities[clip]))
        {



            StructuralEntity* entity = entities[clip];

            if (entity->getLocalType() == Structural::Interface)
            {

            }
            else if (entity->getLocalType() == Structural::Node)
            {
                if (_selected->getLocalName() == Structural::Body ||
                    _selected->getLocalName() == Structural::Context ||
                    _selected->getLocalName() == Structural::Switch)
                {
                    performPaste(entity, _selected, QUuid::createUuid().toString());
                }
            }

        }

        return true;
    }
    else if (e_clip != NULL && _selected != NULL)
    {
      performPaste(e_clip, _selected, QUuid::createUuid().toString());
    }

    return false;
}

void StructuralView::performPaste(StructuralEntity* entity, StructuralEntity* parent, QString CODE)
{
    QString uid = QUuid::createUuid().toString();
    QString parentuid = parent->getLocalUid();
    QMap<QString, QString> properties = entity->getLocalProperties();
    properties["hidden"] = "false";

    QMap<QString, QString> settings;
    settings["UNDO"] = "1";
    settings["NOTIFY"] = "1";
    settings["CODE"] = CODE;

    insert(uid, parentuid, properties, settings);

    if (entities.contains(uid))
    {
        foreach (StructuralEntity* e, entity->getLocalChildren()) {
            performPaste(e, entities[uid], CODE);
        }
    }
}

bool StructuralView::updateEntityWithUniqueNstId(StructuralEntity *entity)
{
  if(entity != NULL)
  {
    int n = 0;
    Structural::EntityName type = entity->getLocalName();

    // check if type is in the map
    if(!entityCounter.count(type))
      entityCounter[type] = 0;

    // change this check in future
    QString prefix = StructuralUtil::getPrefixIdFromType(type);

    bool match = false;

//    foreach(StructuralEntity* e, entities.values())
//    {
//      if (e->getnstProperty("LOCAL:ID") == entity->getnstProperty("LOCAL:ID") &&
//          e != entity)
//      {
//        match = true;
//        break;
//      }
//    }

//    if (!match)
//      return true;

    while(1) //
    {
      match = false;

      foreach(StructuralEntity* e, entities.values())
      {
          if (e->getLocalProperty("LOCAL:ID") == QString(prefix + QString::number(n)))
          {
            match = true;
            break;
          }
      }

      if (match)
        n++;
      else
        break;
    }

    entity->setLocalProperty("LOCAL:ID", StructuralUtil::getPrefixIdFromType(type) + QString::number(n));

    return true;
  }

  return false;
}

bool StructuralView::performDelete()
{
  if (_selected != NULL)
  {

      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";
      settings["CODE"] = QUuid::createUuid().toString();

      remove(_selected->getLocalUid(), settings);

      return true;
  }

  return false;
}

bool StructuralView::performSnapshot()
{
  /*
  QString location = QFileDialog::getSaveFileName(NULL, "Export...", "", tr("Images (*.png)"));

  if (location != ""){
    qreal top = 0;
    qreal left = 0;
    qreal width = scene->width();
    qreal height = scene->height();

    foreach (StructuralEntity* entity, scene->getRoots())
    {
      if (entity->getTop() > top)
        top = entity->getTop();

      if (entity->getLeft() > left)
        left = entity->getLeft();

      if (entity->getWidth() < width)
          width = entity->getWidth();

      if (entity->getHeight() < height)
          height = entity->getHeight();
    }

    QImage image(width, height, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&image);

    scene->render(&painter, QRect(), QRect(left-25,top-25,width+50,height+50));

    painter.end();

    image.save(location, "PNG");
  }
  */

  return false;
}

bool StructuralView::performZoomIn()
{
  if (zoomStep > 0)
  {
    zoomStep--;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);

    return true;
  }

  return false;
}

bool StructuralView::performZoomOut()
{
  if (zoomStep*0.05 < 0.9)
  {
    zoomStep++;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);

    return true;
  }

  return false;
}

bool StructuralView::performZoomReset()
{
  zoomStep = 0;
  resetMatrix();

  return false;
}

bool StructuralView::performFullscreen()
{
    return false;
}

bool StructuralView::performBringfront()
{
    return false;
}

bool StructuralView::performBringforward()
{
    return false;
}

bool StructuralView::performSendback()
{
    return false;
}

bool StructuralView::performSendbackward()
{
    return false;
}

bool StructuralView::performHide()
{
    return false;
}

bool StructuralView::performProperties()
{
    return false;
}

void StructuralView::resizeEvent(QResizeEvent *event)
{
  QGraphicsView::resizeEvent(event);
}

void StructuralView::mouseMoveEvent(QMouseEvent* event)
{
  if (linking)
  {
    if (lastLinkMouseOver != NULL)
    {
      lastLinkMouseOver->setMouseHover(false);

      // \fixme This is not the best place to control this!!
      if(
         lastLinkMouseOver->getLocalName() == Structural::Context ||
         lastLinkMouseOver->getLocalName() == Structural::Switch)
      {
        lastLinkMouseOver->setMouseHoverHighlight(false);
      }

      lastLinkMouseOver = NULL;
    }

    QList<QGraphicsItem *> itemsa = scene->items(link->getLine().p1());

    if (itemsa.count() && itemsa.first() == link)
    {
      itemsa.removeFirst();
    }

    if (itemsa.count())
    {
      StructuralEntity* entitya = (StructuralEntity*) itemsa.first();
      entitya->setMouseHover(true);
    }

    QList<QGraphicsItem*> itemsb = scene->items(link->getLine().p2());

    if (itemsb.count() && itemsb.first() == link)
    {
      itemsb.removeFirst();
    }

    if (itemsb.count())
    {
      StructuralEntity* entityb = (StructuralEntity*) itemsb.first();
      entityb->setMouseHover(true);

      // \fixme This is not the best place to control this!!
      if(
         entityb->getLocalName() == Structural::Context ||
         entityb->getLocalName() == Structural::Switch)
      {
        entityb->setMouseHoverHighlight(true);
      }

      lastLinkMouseOver = entityb;
    }

    link->setLine(QLineF(link->getLine().p1(), mapToScene(event->pos())));
  }

  QGraphicsView::mouseMoveEvent(event);
}

void StructuralView::mousePressEvent(QMouseEvent* event)
{
  if (modified)
  {
    if (link != NULL)
    {
      delete (link);
      link = NULL;
    }

    link = new StructuralViewLink();
    scene->addItem(link);
    link->setLine(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
    link->adjust();

    linking = true;
  }
  else
  {
    QGraphicsView::mousePressEvent(event);

    if (!event->isAccepted())
    {
      if (event->button() == Qt::LeftButton)
      {
        if (_selected != NULL)
        {
          _selected->setSelected(false);
          _selected->adjust();
        }

        _selected = NULL;

        emit selected("", QMap<QString, QString>());
      }

      event->accept();
    }
  }
}

void StructuralView::mouseReleaseEvent(QMouseEvent* event)
{
  if (linking)
  {
    QList<QGraphicsItem *> itemsa = scene->items(link->getLine().p1());

    if (itemsa.count() && itemsa.first() == link)
      itemsa.removeFirst();

    QList<QGraphicsItem*> itemsb = scene->items(link->getLine().p2());

    if (itemsb.count() && itemsb.first() == link)
      itemsb.removeFirst();

    if (itemsa.count() && itemsb.count())
    {
      StructuralEntity* entitya = (StructuralEntity*) itemsa.first();
      StructuralEntity* entityb = (StructuralEntity*) itemsb.first();

      if (entitya != entityb)
      {
        // if linking NODE to NODE
        if (entitya->getLocalType() == Structural::Node &&
            entityb->getLocalType() == Structural::Node)
        {
            if (entitya->getLocalName() != Structural::Link && entityb->getLocalName() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else if (entitya->getLocalName() == Structural::Link || entityb->getLocalName() == Structural::Link)
            {
                createBind(entitya, entityb);
            }

//           addNodetoNodeEdge(entitya, entityb);
        }
        // if linking NODE to INTERFACE
        else if (entitya->getLocalType() == Structural::Node &&
                 entityb->getLocalType() == Structural::Interface)
        {
            if (entitya->getLocalName() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else
            {
                createBind(entitya, entityb);
            }

//          addNodetoInterfaceEdge(entitya, entityb);
        }
        // if linking INTERFACE to NODE
        else if (entitya->getLocalType() == Structural::Interface &&
                 entityb->getLocalType() == Structural::Node)
        {
            StructuralEntity* pa = entitya->getLocalParent();
            StructuralEntity* pb = entityb->getLocalParent();

            if (pa != NULL && pb != NULL && pa == pb)
            {
                createReference(entitya, entityb);
            }
            else if (entityb->getLocalName() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else
            {
                createBind(entitya, entityb);
            }

//          addInterfacetoNodeEdge(entitya, entityb);
        }
        // if linking INTERFACE to INTERFACE
        else if (entitya->getLocalType() == Structural::Interface &&
                 entityb->getLocalType() == Structural::Interface)
        {
            StructuralEntity* pa = entitya->getLocalParent();
            StructuralEntity* pb = entityb->getLocalParent();

            if (pa != NULL && pb != NULL && (pa == pb || pa == pb->getLocalParent()))
            {
                createReference(entitya, entityb);
            }
            else
            {
                createLink(entitya, entityb);
            }

//          addInterfacetoInterfaceEdge(entitya, entityb);
        }
      }

      entitya->setMouseHover(false);
      entityb->setMouseHover(false);

      // \fixme This is not the best place to control this!!
      if(
         entityb->getLocalName() == Structural::Context ||
         entityb->getLocalName() == Structural::Switch)
      {
        entityb->setMouseHoverHighlight(false);
      }
    }

    if (link != NULL)
    {
      scene->removeItem(link);
      scene->update();

      delete link;
      link = NULL;
    }

    linking = false;
  }

  StructuralEntity *entity;
  foreach(entity, entities.values())
  {
    assert(entity != NULL);
    entity->setDraggable(false);
  }

  QGraphicsView::mouseReleaseEvent(event);
}

void StructuralView::createLink(StructuralEntity* a, StructuralEntity* b)
{
    StructuralEntity* parenta = (StructuralEntity*) a->getLocalParent();
    StructuralEntity* parentb = (StructuralEntity*) b->getLocalParent();

    if (parenta == parentb && parenta != NULL && parentb != NULL)
    {
        QString uid = QUuid::createUuid().toString();
        QString parent = parenta->getLocalUid();
        QMap<QString, QString> properties;
        properties[":nst:subtype"] = "link";
//        properties[":nst:a"] = entitya->getnstUid();
//        properties[":nst:b"] = entityb->getnstUid();
//        properties[":nst:bindtype"] = "onBegin";


        if (a->getTop() > b->getTop())
            properties["LOCAL:TOP"] = QString::number(b->getTop() + (a->getTop() - b->getTop())/2);
        else
            properties["LOCAL:TOP"] = QString::number(a->getTop() + (b->getTop() - a->getTop())/2);


        if (a->getLeft() > b->getLeft())
            properties["LOCAL:LEFT"] = QString::number(b->getLeft() + (a->getLeft() - b->getLeft())/2);
        else
            properties["LOCAL:LEFT"] = QString::number(a->getLeft() + (b->getLeft() - a->getLeft())/2);


        QMap<QString, QString> settings;
        settings["UNDO"] = "1";
        settings["NOTIFY"] = "1";
        settings["CODE"] = QUuid::createUuid().toString();

        insert(uid, parent, properties, settings);

        if (entities.contains(uid))
        {
//            QnstLinkDialog* dialog = new QnstLinkDialog();
//            dialog->init();
//            dialog->exec();
//            createBind(a,entities[uid],dialog->form.cbCondition->currentText(),settings["CODE"]);
//            createBind(entities[uid],b,dialog->form.cbAction->currentText(),settings["CODE"]);

            createBind(a,entities[uid],condition,settings["CODE"]);
            createBind(entities[uid],b,action,settings["CODE"]);
        }
    }
    else if (parenta == NULL && parentb == NULL)
    {
      QString uid = QUuid::createUuid().toString();
      QMap<QString, QString> properties;
      properties[":nst:subtype"] = "link";
//        properties[":nst:a"] = entitya->getnstUid();
//        properties[":nst:b"] = entityb->getnstUid();
//        properties[":nst:bindtype"] = "onBegin";


      if (a->getTop() > b->getTop())
          properties["LOCAL:TOP"] = QString::number(b->getTop() + (a->getTop() - b->getTop())/2);
      else
          properties["LOCAL:TOP"] = QString::number(a->getTop() + (b->getTop() - a->getTop())/2);


      if (a->getLeft() > b->getLeft())
          properties["LOCAL:LEFT"] = QString::number(b->getLeft() + (a->getLeft() - b->getLeft())/2);
      else
          properties["LOCAL:LEFT"] = QString::number(a->getLeft() + (b->getLeft() - a->getLeft())/2);


      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";
      settings["CODE"] = QUuid::createUuid().toString();

      insert(uid, "", properties, settings);

      if (entities.contains(uid))
      {
//            QnstLinkDialog* dialog = new QnstLinkDialog();
//            dialog->init();
//            dialog->exec();
//            createBind(a,entities[uid],dialog->form.cbCondition->currentText(),settings["CODE"]);
//            createBind(entities[uid],b,dialog->form.cbAction->currentText(),settings["CODE"]);

          createBind(a,entities[uid],condition,settings["CODE"]);
          createBind(entities[uid],b,action,settings["CODE"]);
      }
    }

//    linking = false;
//    modified = false;
}

void StructuralView::createBind(StructuralEntity* a, StructuralEntity* b, QString type, QString code)
{
    StructuralEntity* pa = a->getLocalParent();
    StructuralEntity* pb = b->getLocalParent();

    if (pa != NULL && pb != NULL)
    {
      QString parent;

      if (pa == pb)
      {
          parent = pa->getLocalUid();
      }
      else if (pa->getLocalParent() == pb)
      {
          parent = pb->getLocalUid();
      }
      else if (pa == pb->getLocalParent())
      {
          parent = pa->getLocalUid();
      }
      else
      {
          return;
      }

      QString uid = QUuid::createUuid().toString();

      QMap<QString, QString> properties;
      properties[":nst:subtype"] = "bind";
      properties[":nst:a"] = a->getLocalUid();
      properties[":nst:b"] = b->getLocalUid();

      if (type == "")
      {
          if (a->getLocalName() == Structural::Link)
          {
  //            QnstActionDialog* dialog = new QnstActionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbAction->currentText();
              properties[":nst:bindtype"] = action;
          }
          else if (b->getLocalName() == Structural::Link)
          {
  //            QnstConditionDialog* dialog = new QnstConditionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbCondition->currentText();
              properties[":nst:bindtype"] = condition;
          }

      }
      else
      {
          properties[":nst:bindtype"] = type;
      }



      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";

      if (code == "")
          settings["CODE"] = QUuid::createUuid().toString();
      else
          settings["CODE"] = code;

      insert(uid, parent, properties, settings);

  //    linking = false;
  //    modified = false;
    }
    else
    {
      QString parent = "";

      QString uid = QUuid::createUuid().toString();

      QMap<QString, QString> properties;
      properties[":nst:subtype"] = "bind";
      properties[":nst:a"] = a->getLocalUid();
      properties[":nst:b"] = b->getLocalUid();

      if (type == "")
      {
          if (a->getLocalName() == Structural::Link)
          {
  //            QnstActionDialog* dialog = new QnstActionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbAction->currentText();
              properties[":nst:bindtype"] = action;
          }
          else if (b->getLocalName() == Structural::Link)
          {
  //            QnstConditionDialog* dialog = new QnstConditionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbCondition->currentText();
              properties[":nst:bindtype"] = condition;
          }

      }
      else
      {
          properties[":nst:bindtype"] = type;
      }



      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";

      if (code == "")
          settings["CODE"] = QUuid::createUuid().toString();
      else
          settings["CODE"] = code;

      insert(uid, parent, properties, settings);

  //    linking = false;
  //    modified = false;
    }

}

void StructuralView::createReference(StructuralEntity* a, StructuralEntity* b)
{
    if (a->getLocalName() == Structural::Port || a->getLocalName() == Structural::SwitchPort)
    {
        StructuralEntity* pa = a->getLocalParent();
        StructuralEntity* pb = b->getLocalParent();

        if (pa != NULL && pb != NULL && (pa == pb || pa == pb->getLocalParent()))
        {

            QString code = QUuid::createUuid().toString();

            QString uid = QUuid::createUuid().toString();
            QString parent = pa->getLocalUid();
            QMap<QString, QString> properties;
            properties[":nst:subtype"] = "reference";
            properties[":nst:a"] = a->getLocalUid();
            properties[":nst:b"] = b->getLocalUid();

            QMap<QString, QString> settings;
            settings["UNDO"] = "1";
            settings["NOTIFY"] = "1";
            settings["CODE"] = code;

            insert(uid, parent, properties, settings);
        }
    }
}

void StructuralView::keyPressEvent(QKeyEvent *event)
{
  // CTRL+X - Cut
  if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_X)
  {
    performCut();

    event->accept();
  }
  // CTRL+C - Copy
  else if (event->modifiers() == Qt::ControlModifier &&
           event->key() == Qt::Key_C)
  {
    performCopy();

    event->accept();
  }
  // CTRL+V - Paste
  else if (event->modifiers() == Qt::ControlModifier &&
           event->key() == Qt::Key_V)
  {
    performPaste();

    event->accept();
  }
  // DELETE - Delete
  else if (event->key() == Qt::Key_Delete)
  {
    performDelete();

    event->accept();
  }
  // BACKSPACE - Delete
  else if (event->key() == Qt::Key_Backspace)
  {
    performDelete();

    event->accept();
  }
  // SHIFT - Enabling liking
  else if (event->key() == Qt::Key_Shift)
  {
    if (_selected != NULL)
    {
      _selected->setSelected(false);
      _selected->adjust();
    }

    _selected = NULL;
    modified = true;

    event->accept();
  }
  else if(event->key() == Qt::Key_Control)
  {
//      modified = true;
    StructuralEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(true);
    }

    event->accept();
  }
  //Ctrl + 0 -> reset to default zoom
  else if( event->modifiers() == Qt::ControlModifier &&
           event->key() == Qt::Key_0)
  {
    performZoomReset();
  }
  //Ctrl + + -> perform zoom in
  else if((event->modifiers() == Qt::ControlModifier ||
           event->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier)) &&
           event->key() == Qt::Key_Plus)
  {
    performZoomIn();

    event->accept();
  }
  //Ctrl + - -> perform zoom out
  else if(event->modifiers() == Qt::ControlModifier &&
          event->key() == Qt::Key_Minus)
  {
    performZoomOut();

    event->accept();
  }
  //Ctrl + Z
  /* else if(event->modifiers() == Qt::ControlModifier &&
          event->key() == Qt::Key_Z)
  {
    performUndo();

    event->accept();
  }
  //Ctrl + Y
  else if(event->modifiers() == Qt::ControlModifier &&
          event->key() == Qt::Key_Y)
  {
    performRedo();

    event->accept();
  }*/

  if(!event->isAccepted())
    QGraphicsView::keyPressEvent(event);
}

void StructuralView::keyReleaseEvent(QKeyEvent *event)
{
  // SHIFT - Disabling linking
  if (event->key() == Qt::Key_Shift)
  {
    modified = false;
  }
  else if(event->key() == Qt::Key_Control)
  {
    StructuralEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(false);
    }
  }

  QGraphicsView::keyReleaseEvent(event);
}

void StructuralView::wheelEvent(QWheelEvent * event)
{
  if(event->modifiers() == Qt::ControlModifier)
  {

    if (event->delta() > 0)
        performZoomIn();
    else
        performZoomOut();


    event->accept();
  }
  else
  {
    // call the parent wheelEvent
    QGraphicsView::wheelEvent(event);
  }
}

void StructuralView::focusOutEvent(QFocusEvent *event)
{
  StructuralEntity *entity;
  foreach(entity, entities.values())
  {
    entity->setDraggable(false);
  }
}

void StructuralView::clearAllData()
{
  /*
  if(scene->getRoots().size())
    scene->removeRoot(scene->getRoots().at(0));
  */

  entities.clear();
  links.clear();

  refers.clear();
  importBases.clear();

  bindParamUIDToBindUID.clear();

  //connectors.clear();
  //connectors2.clear();

  deletePendingEntities();
}

void StructuralView::deletePendingEntities()
{
  foreach(StructuralEntity* e, toDelete)
  {
    e->deleteLater();
  }

  toDelete.clear();
}

void StructuralView::setEnableLink(bool status)
{
    if (status)
    {
        if (_selected != NULL)
        {
          _selected->setSelected(false);
          _selected->adjust();
        }

        _selected = NULL;
        modified = true;
    }
    else
    {
        modified = false;
    }
}

void StructuralView::setAction(QString action)
{
    this->action = action;
}

void StructuralView::setCondition(QString condition)
{
    this->condition = condition;
}

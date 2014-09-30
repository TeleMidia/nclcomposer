#include "StructuralView.h"

//
// ATTENTION: This code needs a refactoring.
//

#include <assert.h>
#include <QDebug>

#include "StructuralUtil.h"
#include "StructuralBind.h"

std::map <Structural::EntitySubtype, QString> StructuralView::mediaTypeToXMLStr =
    create_map<Structural::EntitySubtype, QString>
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

  hasCutted = false;

  lastLinkMouseOver = NULL;

  // Initialize entity counters
  for(int i = Structural::Node; i < Structural::NoSubtype; i += 1)
    entityCounter[ (Structural::EntitySubtype) i ] = 0;

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

void StructuralView::insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  if (!entities.contains(uid))
  {
    if (settings["UNDO"] == "1")
    {
      Insert* command = new Insert(uid, parent, properties, settings);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      command->setText(settings["CODE"]);

      commnads.push(command); return;
    }

    StructuralEntity* entity; QnstSubtype type = StructuralUtil::getnstTypeFromStr(properties[":nst:subtype"]);

    switch (type)
    {
      case Structural::Context:
      {
        entity = new StructuralComposition();
        entity->setnstSubtype(Structural::Context);

        break;
      }

      case Structural::Switch:
      {
        entity = new StructuralComposition();
        entity->setnstSubtype(Structural::Switch);
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
        entity->setnstSubtype(Structural::Port);
        break;
      }

      case Structural::SwitchPort:
      {
        entity = new StructuralInterface();
        entity->setnstSubtype(Structural::SwitchPort);
        break;
      }

      case Structural::Area:
      {
        entity = new StructuralInterface();
        entity->setnstSubtype(Structural::Area);
        break;
      }

      case Structural::Property:
      {
        entity = new StructuralInterface();
        entity->setnstSubtype(Structural::Property);
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

            if (a->getnstSubtype() == Structural::Port)
            {
                foreach (StructuralEntity* e, entities.values()) {
                    if (e->getnstSubtype() == Structural::Reference)
                    {
                        StructuralReference* r = (StructuralReference*) e;

                        if (e->getnstProperty(":nst:a") == a->getnstUid())
                        {
                            QMap<QString, QString> localset;
                            localset["UNDO"] = "1";
                            localset["NOTIFY"] = "1";
                            localset["CODE"] = settings["CODE"];

                            remove(r->getnstUid(),localset);
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
        entity->setnstSubtype(Structural::Body);
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
        entity->setnstParent(entities[parent]);
      }
      else
      {
        scene->addItem(entity);

        entity->setTop(scene->sceneRect().height()/2 - entity->getWidth()/2);
        entity->setLeft(scene->sceneRect().width()/2 - entity->getWidth()/2);
      }

      entity->setnstUid(uid);
      entity->setnstProperties(properties);


      if (entity->getnstProperty(":nst:id").isEmpty())
        updateEntityWithUniqueNstId(entity);

      entity->updateToolTip();

      connect(entity, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(removed(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(entity, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      entity->adjust(true);

      entities[uid] = entity;

      if (settings["NOTIFY"] == "1")
      {
        emit inserted(uid, parent, properties, settings);
      }
    }
  }
}

Command* StructuralView::rmcmd(StructuralEntity* parent, Command* cmdparent, QMap<QString, QString> settings)
{
  foreach (StructuralEntity* entity, parent->getnstChildren())
  {
      //qDebug() << entity->getnstUid();
    Remove* command = new Remove(entity->getnstUid(), entity->getnstParent()->getnstUid(), entity->getnstProperties(), settings, cmdparent);

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

      if (entities[uid]->getnstParent() != NULL)
        command = new Remove(entities[uid]->getnstUid(), entities[uid]->getnstParent()->getnstUid(), entities[uid]->getnstProperties(), settings);
      else
        command = new Remove(entities[uid]->getnstUid(), "", entities[uid]->getnstProperties(), settings);

      command->setText(settings["CODE"]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      foreach (StructuralEntity* entity, entities.values())
      {
          if (entity->getnstType() == Structural::Edge)
          {
              StructuralEdge* bind = (StructuralEdge*) entity;

              if (bind->getnstProperty(":nst:a") == uid || bind->getnstProperty(":nst:b") == uid)
              {
                  remove(bind->getnstUid(), settings);
              }
          }
      }

      foreach (StructuralEntity* entity, entities[uid]->getnstChildren())
      {
          remove(entity->getnstUid(), settings);
      }

      commnads.push(command); return;
    }

    StructuralEntity* entity = entities[uid];

    StructuralEntity* parent = entity->getnstParent();

    if (parent != NULL)
    {
      parent->removeChild(entity);
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
     entity->setnstProperties(properties);
     entity->adjust();

     foreach (StructuralEntity* entity, entities.values())
     {
         if (entity->getnstType() == Structural::Edge)
         {
             if (entity->getnstProperty(":nst:a") == uid ||
                 entity->getnstProperty(":nst:b") == uid)
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
  }
}

void StructuralView::create(QnstSubtype subtype, QMap<QString, QString> extraproperties, QMap<QString, QString> extrasettings)
{
  switch (subtype)
  {
    case Structural::Body:
    {
      QString uid = QUuid::createUuid().toString();
      QString parent = "";
      QMap<QString, QString> properties = extraproperties;
      properties[":nst:subtype"] = "body";

      if (!properties.contains(":nst:top"))
        properties[":nst:top"] = QString::number(scene->height()/2 - DEFAULT_BODY_HEIGHT/2);

      if (!properties.contains(":nst:left"))
        properties[":nst:left"] = QString::number(scene->width()/2 - DEFAULT_BODY_WIDTH/2);

      if (!properties.contains(":nst:width"))
        properties[":nst:width"] = QString::number(DEFAULT_BODY_WIDTH);

      if (!properties.contains(":nst:height"))
        properties[":nst:height"] = QString::number(DEFAULT_BODY_HEIGHT);

      QMap<QString, QString> settings = extrasettings;

      if (!settings.contains("UNDO"))
      settings["UNDO"] = "1";

      if (!settings.contains("NOTIFY"))
      settings["NOTIFY"] = "1";

      if (!settings.contains("CODE"))
      settings["CODE"] = QUuid::createUuid().toString();

      insert(uid, parent, properties, settings);
      break;
    }

    default:
    {
      if (_selected != NULL)
      {
        QString uid = QUuid::createUuid().toString();
        QString parent = _selected->getnstUid();
        QMap<QString, QString> properties;
        properties[":nst:subtype"] = StructuralUtil::getStrFromNstType(subtype);

        QMap<QString, QString> settings;
        settings["UNDO"] = "1";
        settings["NOTIFY"] = "1";
        settings["CODE"] = QUuid::createUuid().toString();

        insert(uid, parent, properties, settings);
      }
      break;
    }
  }
}


void StructuralView::performHelp()
{
    // TODO
}

void StructuralView::performUndo()
{
  if (commnads.canUndo())
  {
      QString code = commnads.undoText();

      while(code == commnads.undoText())
        commnads.undo();
  }
}

void StructuralView::performRedo()
{
  if (commnads.canRedo())
  {
    QString code = commnads.redoText();

    while(code == commnads.redoText())
      commnads.redo();
  }
}

void StructuralView::performCut()
{
    if (_selected != NULL)
    {
        clip = _selected->getnstUid();

        _selected->setnstParent(NULL);
        _selected->setVisible(false);
    }
}

void StructuralView::performCopy()
{
    if (_selected != NULL)
    {
        clip = _selected->getnstUid();
        //qDebug() << "======================" << clip;
    }
}

bool StructuralView::isChild(StructuralEntity* e , StructuralEntity* p)
{
    bool r = false;

    foreach (StructuralEntity* ec, p->getnstChildren()) {
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

void StructuralView::performPaste()
{
    if (entities.contains(clip))
    {
        if (_selected != NULL && _selected != entities[clip] && !isChild(_selected, entities[clip]))
        {



            StructuralEntity* entity = entities[clip];

            if (entity->getnstType() == Structural::Interface)
            {

            }
            else if (entity->getnstType() == Structural::Node)
            {
                if (_selected->getnstSubtype() == Structural::Body ||
                    _selected->getnstSubtype() == Structural::Context ||
                    _selected->getnstSubtype() == Structural::Switch)
                {
                    performPaste(entity, _selected, QUuid::createUuid().toString());
                }
            }

        }
    }
}

void StructuralView::performPaste(StructuralEntity* entity, StructuralEntity* parent, QString CODE)
{
    QString uid = QUuid::createUuid().toString();
    QString parentuid = parent->getnstUid();
    QMap<QString, QString> properties = entity->getnstProperties();

    QMap<QString, QString> settings;
    settings["UNDO"] = "1";
    settings["NOTIFY"] = "1";
    settings["CODE"] = CODE;

    insert(uid, parentuid, properties, settings);

    if (entities.contains(uid))
    {
        foreach (StructuralEntity* e, entity->getnstChildren()) {
            performPaste(e, entities[uid], CODE);
        }
    }
}

bool StructuralView::updateEntityWithUniqueNstId(StructuralEntity *entity)
{
  if(entity != NULL)
  {
    int n = 0;
    Structural::EntitySubtype type = entity->getnstSubtype();

    // check if type is in the map
    if(!entityCounter.count(type))
      entityCounter[type] = 0;

    // change this check in future
    QString prefix = StructuralUtil::getPrefixIdFromType(type);

    while(1) //
    {
      bool match = false;

      foreach(StructuralEntity* entity, entities.values())
      {

          if (entity->getnstProperty(":nst:id") == QString(prefix + QString::number(n)))
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

    entity->setnstProperty(":nst:id", StructuralUtil::getPrefixIdFromType(type) + QString::number(n));

    return true;
  }

  return false;
}

void StructuralView::performDelete()
{
  if (_selected != NULL)
  {

      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";
      settings["CODE"] = QUuid::createUuid().toString();

      remove(_selected->getnstUid(), settings);
  }
}

void StructuralView::performSnapshot()
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
}

void StructuralView::performZoomIn()
{
  if (zoomStep > 0)
  {
    zoomStep--;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
  }
}

void StructuralView::performZoomOut()
{
  if (zoomStep*0.05 < 0.9)
  {
    zoomStep++;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
  }
}

void StructuralView::performZoomReset()
{
  zoomStep = 0;
  resetMatrix();
}

void StructuralView::performFullscreen()
{
    // TODO
}

void StructuralView::performBringfront()
{
    // TODO
}

void StructuralView::performBringforward()
{
    // TODO
}

void StructuralView::performSendback()
{
    // TODO
}

void StructuralView::performSendbackward()
{
    // TODO
}

void StructuralView::performHide()
{
    // TODO
}

void StructuralView::performProperties()
{
    // TODO
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
         lastLinkMouseOver->getnstSubtype() == Structural::Context ||
         lastLinkMouseOver->getnstSubtype() == Structural::Switch)
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
         entityb->getnstSubtype() == Structural::Context ||
         entityb->getnstSubtype() == Structural::Switch)
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
        if (entitya->getnstType() == Structural::Node &&
            entityb->getnstType() == Structural::Node)
        {
            if (entitya->getnstSubtype() != Structural::Link && entityb->getnstSubtype() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else if (entitya->getnstSubtype() == Structural::Link || entityb->getnstSubtype() == Structural::Link)
            {
                createBind(entitya, entityb);
            }

//           addNodetoNodeEdge(entitya, entityb);
        }
        // if linking NODE to INTERFACE
        else if (entitya->getnstType() == Structural::Node &&
                 entityb->getnstType() == Structural::Interface)
        {
            if (entitya->getnstSubtype() != Structural::Link)
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
        else if (entitya->getnstType() == Structural::Interface &&
                 entityb->getnstType() == Structural::Node)
        {
            StructuralEntity* pa = entitya->getnstParent();
            StructuralEntity* pb = entityb->getnstParent();

            if (pa != NULL && pb != NULL && pa == pb)
            {
                createReference(entitya, entityb);
            }
            else if (entityb->getnstSubtype() != Structural::Link)
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
        else if (entitya->getnstType() == Structural::Interface &&
                 entityb->getnstType() == Structural::Interface)
        {
            StructuralEntity* pa = entitya->getnstParent();
            StructuralEntity* pb = entityb->getnstParent();

            if (pa != NULL && pb != NULL && (pa == pb || pa == pb->getnstParent()))
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
         entityb->getnstSubtype() == Structural::Context ||
         entityb->getnstSubtype() == Structural::Switch)
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
    StructuralEntity* parenta = (StructuralEntity*) a->getnstParent();
    StructuralEntity* parentb = (StructuralEntity*) b->getnstParent();

    if (parenta == parentb && parenta != NULL && parentb != NULL)
    {
        QString uid = QUuid::createUuid().toString();
        QString parent = parenta->getnstUid();
        QMap<QString, QString> properties;
        properties[":nst:subtype"] = "link";
//        properties[":nst:a"] = entitya->getnstUid();
//        properties[":nst:b"] = entityb->getnstUid();
//        properties[":nst:bindtype"] = "onBegin";


        if (a->getTop() > b->getTop())
            properties[":nst:top"] = QString::number(b->getTop() + (a->getTop() - b->getTop())/2);
        else
            properties[":nst:top"] = QString::number(a->getTop() + (b->getTop() - a->getTop())/2);


        if (a->getLeft() > b->getLeft())
            properties[":nst:left"] = QString::number(b->getLeft() + (a->getLeft() - b->getLeft())/2);
        else
            properties[":nst:left"] = QString::number(a->getLeft() + (b->getLeft() - a->getLeft())/2);


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
          properties[":nst:top"] = QString::number(b->getTop() + (a->getTop() - b->getTop())/2);
      else
          properties[":nst:top"] = QString::number(a->getTop() + (b->getTop() - a->getTop())/2);


      if (a->getLeft() > b->getLeft())
          properties[":nst:left"] = QString::number(b->getLeft() + (a->getLeft() - b->getLeft())/2);
      else
          properties[":nst:left"] = QString::number(a->getLeft() + (b->getLeft() - a->getLeft())/2);


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
    StructuralEntity* pa = a->getnstParent();
    StructuralEntity* pb = b->getnstParent();

    if (pa != NULL && pb != NULL)
    {
      QString parent;

      if (pa == pb)
      {
          parent = pa->getnstUid();
      }
      else if (pa->getnstParent() == pb)
      {
          parent = pb->getnstUid();
      }
      else if (pa == pb->getnstParent())
      {
          parent = pa->getnstUid();
      }
      else
      {
          return;
      }

      QString uid = QUuid::createUuid().toString();

      QMap<QString, QString> properties;
      properties[":nst:subtype"] = "bind";
      properties[":nst:a"] = a->getnstUid();
      properties[":nst:b"] = b->getnstUid();

      if (type == "")
      {
          if (a->getnstSubtype() == Structural::Link)
          {
  //            QnstActionDialog* dialog = new QnstActionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbAction->currentText();
              properties[":nst:bindtype"] = action;
          }
          else if (b->getnstSubtype() == Structural::Link)
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
      properties[":nst:a"] = a->getnstUid();
      properties[":nst:b"] = b->getnstUid();

      if (type == "")
      {
          if (a->getnstSubtype() == Structural::Link)
          {
  //            QnstActionDialog* dialog = new QnstActionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbAction->currentText();
              properties[":nst:bindtype"] = action;
          }
          else if (b->getnstSubtype() == Structural::Link)
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
    if (a->getnstSubtype() == Structural::Port || a->getnstSubtype() == Structural::SwitchPort)
    {
        StructuralEntity* pa = a->getnstParent();
        StructuralEntity* pb = b->getnstParent();

        if (pa != NULL && pb != NULL && (pa == pb || pa == pb->getnstParent()))
        {

            QString code = QUuid::createUuid().toString();

            QString uid = QUuid::createUuid().toString();
            QString parent = pa->getnstUid();
            QMap<QString, QString> properties;
            properties[":nst:subtype"] = "reference";
            properties[":nst:a"] = a->getnstUid();
            properties[":nst:b"] = b->getnstUid();

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

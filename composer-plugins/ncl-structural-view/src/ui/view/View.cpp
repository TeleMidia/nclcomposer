#include "View.h"

//
// ATTENTION: This code needs a refactoring.
//

#include <assert.h>
#include <QDebug>

#include "Util.h"
#include "Bind.h"

std::map <Qnst::EntitySubtype, QString> View::mediaTypeToXMLStr =
    create_map<Qnst::EntitySubtype, QString>
      (Qnst::Media, "media");
//      (Qnst::Image, "image")
//      (Qnst::Audio, "audio")
//      (Qnst::Video, "video")
//      (Qnst::Html, "text/html")
//      (Qnst::NCL, "application/x-ginga-NCL")
//      (Qnst::Text, "text")
//      (Qnst::NCLua, "application/x-ginga-NCLua")
//      (Qnst::Settings, "application/x-ncl-settings");

View::View(QWidget* parent)
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
  for(int i = Qnst::Node; i < Qnst::NoSubtype; i += 1)
    entityCounter[ (Qnst::EntitySubtype) i ] = 0;

  setAttribute(Qt::WA_TranslucentBackground);

  condition = "onBegin";
  action = "start";

//  minimap = new MiniMap(this);
//  minimap->init(this);
//  minimap->setMinimumSize(MINIMAP_DEFAULT_W, MINIMAP_DEFAULT_H);
}

View::~View()
{

}

void View::createObjects()
{
  scene = new QnstScene();
  scene->setParent(this);
  scene->setSceneRect(0, 0, 3200, 1800);

  setScene(scene);

  this->resize(scene->itemsBoundingRect().size().width(), scene->itemsBoundingRect().height());
}

void View::createConnection()
{
  // connect(scene, SIGNAL(undoRequested()), SLOT(performUndo()));
  // connect(scene, SIGNAL(redoRequested()), SLOT(performRedo()));

  connect(scene, SIGNAL(cutRequested()), SLOT(performCut()));
  connect(scene, SIGNAL(copyRequested()), SLOT(performCopy()));
  connect(scene, SIGNAL(pasteRequested()), SLOT(performPaste()));

  connect(scene, SIGNAL(deleteRequested()), SLOT(performDelete()));

  connect(scene, SIGNAL(exportRequested()), SLOT(performExport()));

  connect(scene, SIGNAL(zoominRequested()), SLOT(performZoomIn()));
  connect(scene, SIGNAL(zoomoutRequested()), SLOT(performZoomOut()));
  connect(scene, SIGNAL(zoomresetRequested()), SLOT(performZoomReset()));
  connect(scene, SIGNAL(fullscreenRequested()), SLOT(performFullscreen()));

//  connect(scene, SIGNAL(entityAdded(QnstEntity*)),
//                 SLOT(requestEntityAddition(QnstEntity*)));
//  connect(scene, SIGNAL(changed(QnstEntity*)),
//                 SLOT(requestEntityChange(QnstEntity*)));
//  connect(scene, SIGNAL(removed(QnstEntity*)),
//                 SLOT(requestEntityRemotion(QnstEntity*)));
//  connect(scene, SIGNAL(_selected(QnstEntity*)),
//                 SLOT(requestEntitySelection(QnstEntity*)));

//  connect(scene,
//          SIGNAL(entityAboutToChange(QnstEntity*,
//                                     QMap<QString,QString>)),
//          SLOT(requestEntityPreparation(QnstEntity*,
//                                        QMap<QString,QString>)));

  connect(scene, SIGNAL(insertEntityResquested(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
  connect(scene, SIGNAL(removeEntityResquested(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
  connect(scene, SIGNAL(changeEntityResquested(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
  connect(scene, SIGNAL(selectEntityResquested(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));
}

bool View::hasEntity(QString uid)
{
  if (entities.contains(uid)  ||
      binds.contains(uid) ||
      links.contains(uid))
  {
    return true;
  }

  return false;
}

QnstEntity* View::getEntity(QString uid)
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

void View::insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  if (!entities.contains(uid))
  {
    if (settings["UNDO"] == "1")
    {
      QnstInsert* command = new QnstInsert(uid, parent, properties, settings);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      command->setText(settings["CODE"]);

      commnads.push(command); return;
    }

    QnstEntity* entity; QnstSubtype type = QnstUtil::getnstTypeFromStr(properties[":nst:subtype"]);

    switch (type)
    {
      case Qnst::Context:
      {
        entity = new QnstComposition();
        entity->setnstSubtype(Qnst::Context);

        break;
      }

      case Qnst::Switch:
      {
        entity = new QnstComposition();
        entity->setnstSubtype(Qnst::Switch);
        break;
      }

      case Qnst::Media:
      {
        entity = new QnstMedia();
        break;
      }

      case Qnst::Port:
      {
        entity = new QnstInterface();
        entity->setnstSubtype(Qnst::Port);
        break;
      }

      case Qnst::SwitchPort:
      {
        entity = new QnstInterface();
        entity->setnstSubtype(Qnst::SwitchPort);
        break;
      }

      case Qnst::Area:
      {
        entity = new QnstInterface();
        entity->setnstSubtype(Qnst::Area);
        break;
      }

      case Qnst::Property:
      {
        entity = new QnstInterface();
        entity->setnstSubtype(Qnst::Property);
        break;
      }

      case Qnst::Link:
      {
        entity = new QnstLink();
        break;
      }

      case Qnst::Bind:
      {
        if (entities.contains(properties.value(":nst:a")) && entities.contains(properties.value(":nst:b")))
        {
            entity = new QnstBind();

            QnstBind* bind = (QnstBind*) entity;
            bind->setEntityA(entities[properties.value(":nst:a")]);
            bind->setEntityB(entities[properties.value(":nst:b")]);

            qDebug() << "================" << properties.value(":nst:bindtype");
            bind->setType(QnstUtil::getBindTypeFromStr(properties.value(":nst:bindtype")));
        }

        break;
      }

      case Qnst::Reference:
      {
        if (entities.contains(properties.value(":nst:a")) && entities.contains(properties.value(":nst:b")))
        {
            QnstEntity* a = entities[properties.value(":nst:a")];

            if (a->getnstSubtype() == Qnst::Port)
            {
                foreach (QnstEntity* e, entities.values()) {
                    if (e->getnstSubtype() == Qnst::Reference)
                    {
                        QnstReference* r = (QnstReference*) e;

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

            entity = new QnstReference(entities[parent]);

            QnstReference* bind = (QnstReference*) entity;
            bind->setEntityA(entities[properties.value(":nst:a")]);
            bind->setEntityB(entities[properties.value(":nst:b")]);
        }

        break;
      }

      case Qnst::Body:
      {
        entity = new QnstComposition();
        entity->setnstSubtype(Qnst::Body);
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

      updateEntityWithUniqueNstId(entity); entity->updateToolTip();

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

QnstCommand* View::rmcmd(QnstEntity* parent, QnstCommand* cmdparent, QMap<QString, QString> settings)
{
  foreach (QnstEntity* entity, parent->getnstChildren())
  {
      //qDebug() << entity->getnstUid();
    QnstRemove* command = new QnstRemove(entity->getnstUid(), entity->getnstParent()->getnstUid(), entity->getnstProperties(), settings, cmdparent);

    connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
    connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
    connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
    connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

    rmcmd(entity, command, settings);
  }
}

void View::remove(QString uid, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
    if (settings["UNDO"] == "1")
    {
      QnstRemove* command;

      if (entities[uid]->getnstParent() != NULL)
        command = new QnstRemove(entities[uid]->getnstUid(), entities[uid]->getnstParent()->getnstUid(), entities[uid]->getnstProperties(), settings);
      else
        command = new QnstRemove(entities[uid]->getnstUid(), "", entities[uid]->getnstProperties(), settings);

      command->setText(settings["CODE"]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      foreach (QnstEntity* entity, entities.values())
      {
          if (entity->getnstType() == Qnst::Edge)
          {
              QnstEdge* bind = (QnstEdge*) entity;

              if (bind->getnstProperty(":nst:a") == uid || bind->getnstProperty(":nst:b") == uid)
              {
                  remove(bind->getnstUid(), settings);
              }
          }
      }

      foreach (QnstEntity* entity, entities[uid]->getnstChildren())
      {
          remove(entity->getnstUid(), settings);
      }

      commnads.push(command); return;
    }

    QnstEntity* entity = entities[uid];

    QnstEntity* parent = entity->getnstParent();

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

void View::change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
    if (settings["UNDO"] == "1")
    {
      QnstChange* command = new QnstChange(uid, properties, previous, settings);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      command->setText(settings["CODE"]);

      commnads.push(command); return;
    }

     QnstEntity* entity = entities[uid];
     entity->setnstProperties(properties);
     entity->adjust();

     foreach (QnstEntity* entity, entities.values())
     {
         if (entity->getnstType() == Qnst::Edge)
         {
             entity->adjust();
         }
     }

    if (settings["NOTIFY"] == "1")
    {
      emit changed(uid, properties, previous, settings);
    }
  }
}

void View::select(QString uid, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
      QnstEntity* entity = entities[uid];
      entity->setSelected(true);

      if (_selected != NULL && _selected != 0)
      {
        _selected->setSelected(false);
        _selected->adjust();
      }

      _selected = entity;
  }
}

void View::create(QnstSubtype subtype)
{
  switch (subtype)
  {
    case Qnst::Body:
    {
      QString uid = QUuid::createUuid().toString();
      QString parent = "";
      QMap<QString, QString> properties;
      properties[":nst:subtype"] = "body";
      properties[":nst:top"] = QString::number(scene->height()/2 - DEFAULT_BODY_HEIGHT/2);
      properties[":nst:left"] = QString::number(scene->width()/2 - DEFAULT_BODY_WIDTH/2);
      properties[":nst:width"] = QString::number(DEFAULT_BODY_WIDTH);
      properties[":nst:height"] = QString::number(DEFAULT_BODY_HEIGHT);

      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";
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
        properties[":nst:subtype"] = QnstUtil::getStrFromNstType(subtype);

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


void View::performHelp()
{
    // TODO
}

void View::performUndo()
{
  if (commnads.canUndo())
  {
      QString code = commnads.undoText();

      while(code == commnads.undoText())
        commnads.undo();
  }
}

void View::performRedo()
{
  if (commnads.canRedo())
  {
    QString code = commnads.redoText();

    while(code == commnads.redoText())
      commnads.redo();
  }
}

void View::performCut()
{
    if (_selected != NULL)
    {
        clip = _selected->getnstUid();

        _selected->setnstParent(NULL);
        _selected->setVisible(false);
    }
}

void View::performCopy()
{
    if (_selected != NULL)
    {
        clip = _selected->getnstUid();
        //qDebug() << "======================" << clip;
    }
}

bool View::isChild(QnstEntity* e , QnstEntity* p)
{
    bool r = false;

    foreach (QnstEntity* ec, p->getnstChildren()) {
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

void View::performPaste()
{
    if (entities.contains(clip))
    {
        if (_selected != NULL && _selected != entities[clip] && !isChild(_selected, entities[clip]))
        {



            QnstEntity* entity = entities[clip];

            if (entity->getnstType() == Qnst::Interface)
            {

            }
            else if (entity->getnstType() == Qnst::Node)
            {
                if (_selected->getnstSubtype() == Qnst::Body ||
                    _selected->getnstSubtype() == Qnst::Context ||
                    _selected->getnstSubtype() == Qnst::Switch)
                {
                    performPaste(entity, _selected, QUuid::createUuid().toString());
                }
            }

        }
    }
}

void View::performPaste(QnstEntity* entity, QnstEntity* parent, QString CODE)
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
        foreach (QnstEntity* e, entity->getnstChildren()) {
            performPaste(e, entities[uid], CODE);
        }
    }
}

bool View::updateEntityWithUniqueNstId(QnstEntity *entity)
{
  if(entity != NULL)
  {
    int n = 0;
    Qnst::EntitySubtype type = entity->getnstSubtype();

    // check if type is in the map
    if(!entityCounter.count(type))
      entityCounter[type] = 0;

    // change this check in future
    QString prefix = QnstUtil::getPrefixIdFromType(type);

    while(1) //
    {
      bool match = false;

      foreach(QnstEntity* entity, entities.values())
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

    entity->setnstProperty(":nst:id", QnstUtil::getPrefixIdFromType(type) + QString::number(n));

    return true;
  }

  return false;
}

void View::performDelete()
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

void View::performExport()
{
  QString location = QFileDialog::getSaveFileName(NULL, "Export...", "", tr("Images (*.png)"));

  if (location != ""){
    qreal top = 0;
    qreal left = 0;
    qreal width = scene->width();
    qreal height = scene->height();

    foreach (QnstEntity* entity, scene->getRoots())
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
}

void View::performZoomIn()
{
  if (zoomStep > 0)
  {
    zoomStep--;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
  }
}

void View::performZoomOut()
{
  if (zoomStep*0.05 < 0.9)
  {
    zoomStep++;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
  }
}

void View::performZoomReset()
{
  zoomStep = 0;
  resetMatrix();
}

void View::performFullscreen()
{
    // TODO
}

void View::performBringfront()
{
    // TODO
}

void View::performBringforward()
{
    // TODO
}

void View::performSendback()
{
    // TODO
}

void View::performSendbackward()
{
    // TODO
}

void View::performHide()
{
    // TODO
}

void View::performProperties()
{
    // TODO
}

void View::resizeEvent(QResizeEvent *event)
{
  QGraphicsView::resizeEvent(event);
}

void View::mouseMoveEvent(QMouseEvent* event)
{
  if (linking)
  {
    if (lastLinkMouseOver != NULL)
    {
      lastLinkMouseOver->setMouseHover(false);

      // \fixme This is not the best place to control this!!
      if(
         lastLinkMouseOver->getnstSubtype() == Qnst::Context ||
         lastLinkMouseOver->getnstSubtype() == Qnst::Switch)
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
      QnstEntity* entitya = (QnstEntity*) itemsa.first();
      entitya->setMouseHover(true);
    }

    QList<QGraphicsItem*> itemsb = scene->items(link->getLine().p2());

    if (itemsb.count() && itemsb.first() == link)
    {
      itemsb.removeFirst();
    }

    if (itemsb.count())
    {
      QnstEntity* entityb = (QnstEntity*) itemsb.first();
      entityb->setMouseHover(true);

      // \fixme This is not the best place to control this!!
      if(
         entityb->getnstSubtype() == Qnst::Context ||
         entityb->getnstSubtype() == Qnst::Switch)
      {
        entityb->setMouseHoverHighlight(true);
      }

      lastLinkMouseOver = entityb;
    }

    link->setLine(QLineF(link->getLine().p1(), mapToScene(event->pos())));
  }

  QGraphicsView::mouseMoveEvent(event);
}

void View::mousePressEvent(QMouseEvent* event)
{
  if (modified)
  {
    if (link != NULL)
    {
      delete (link);
      link = NULL;
    }

    link = new ViewLink();
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

void View::mouseReleaseEvent(QMouseEvent* event)
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
      QnstEntity* entitya = (QnstEntity*) itemsa.first();
      QnstEntity* entityb = (QnstEntity*) itemsb.first();

      if (entitya != entityb)
      {
        // if linking NODE to NODE
        if (entitya->getnstType() == Qnst::Node &&
            entityb->getnstType() == Qnst::Node)
        {
            if (entitya->getnstSubtype() != Qnst::Link && entityb->getnstSubtype() != Qnst::Link)
            {
                createLink(entitya, entityb);
            }
            else if (entitya->getnstSubtype() == Qnst::Link || entityb->getnstSubtype() == Qnst::Link)
            {
                createBind(entitya, entityb);
            }

//           addNodetoNodeEdge(entitya, entityb);
        }
        // if linking NODE to INTERFACE
        else if (entitya->getnstType() == Qnst::Node &&
                 entityb->getnstType() == Qnst::Interface)
        {
            if (entitya->getnstSubtype() != Qnst::Link)
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
        else if (entitya->getnstType() == Qnst::Interface &&
                 entityb->getnstType() == Qnst::Node)
        {
            QnstEntity* pa = entitya->getnstParent();
            QnstEntity* pb = entityb->getnstParent();

            if (pa != NULL && pb != NULL && pa == pb)
            {
                createReference(entitya, entityb);
            }
            else if (entityb->getnstSubtype() != Qnst::Link)
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
        else if (entitya->getnstType() == Qnst::Interface &&
                 entityb->getnstType() == Qnst::Interface)
        {
            QnstEntity* pa = entitya->getnstParent();
            QnstEntity* pb = entityb->getnstParent();

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
         entityb->getnstSubtype() == Qnst::Context ||
         entityb->getnstSubtype() == Qnst::Switch)
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

  QnstEntity *entity;
  foreach(entity, entities.values())
  {
    assert(entity != NULL);
    entity->setDraggable(false);
  }

  QGraphicsView::mouseReleaseEvent(event);
}

void View::createLink(QnstEntity* a, QnstEntity* b)
{
    QnstEntity* parenta = (QnstEntity*) a->getnstParent();
    QnstEntity* parentb = (QnstEntity*) b->getnstParent();

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

void View::createBind(QnstEntity* a, QnstEntity* b, QString type, QString code)
{
    QnstEntity* pa = a->getnstParent();
    QnstEntity* pb = b->getnstParent();

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
          if (a->getnstSubtype() == Qnst::Link)
          {
  //            QnstActionDialog* dialog = new QnstActionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbAction->currentText();
              properties[":nst:bindtype"] = action;
          }
          else if (b->getnstSubtype() == Qnst::Link)
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
          if (a->getnstSubtype() == Qnst::Link)
          {
  //            QnstActionDialog* dialog = new QnstActionDialog();
  //            dialog->init();
  //            dialog->exec();

  //            properties[":nst:bindtype"] = dialog->form.cbAction->currentText();
              properties[":nst:bindtype"] = action;
          }
          else if (b->getnstSubtype() == Qnst::Link)
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

void View::createReference(QnstEntity* a, QnstEntity* b)
{
    if (a->getnstSubtype() == Qnst::Port || a->getnstSubtype() == Qnst::SwitchPort)
    {
        QnstEntity* pa = a->getnstParent();
        QnstEntity* pb = b->getnstParent();

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

void View::keyPressEvent(QKeyEvent *event)
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
    QnstEntity *entity;
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

void View::keyReleaseEvent(QKeyEvent *event)
{
  // SHIFT - Disabling linking
  if (event->key() == Qt::Key_Shift)
  {
    modified = false;
  }
  else if(event->key() == Qt::Key_Control)
  {
    QnstEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(false);
    }
  }

  QGraphicsView::keyReleaseEvent(event);
}

void View::wheelEvent(QWheelEvent * event)
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

void View::focusOutEvent(QFocusEvent *event)
{
  QnstEntity *entity;
  foreach(entity, entities.values())
  {
    entity->setDraggable(false);
  }
}

void View::clearAllData()
{
  if(scene->getRoots().size())
    scene->removeRoot(scene->getRoots().at(0));

  entities.clear();
  links.clear();

  refers.clear();
  importBases.clear();

  bindParamUIDToBindUID.clear();

  //connectors.clear();
  //connectors2.clear();

  deletePendingEntities();
}

void View::deletePendingEntities()
{
  foreach(QnstEntity* e, toDelete)
  {
    e->deleteLater();
  }

  toDelete.clear();
}

void View::setEnableLink(bool status)
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

void View::setAction(QString action)
{
    this->action = action;
}

void View::setCondition(QString condition)
{
    this->condition = condition;
}

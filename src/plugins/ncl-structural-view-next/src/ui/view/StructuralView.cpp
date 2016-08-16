#include "StructuralView.h"

#include <assert.h>
#include <QDebug>

#include "StructuralUtil.h"

std::map <Structural::StructuralType, QString> StructuralView::mediaTypeToXMLStr =
    create_map<Structural::StructuralType, QString>
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
  _selected_UID = "";

  clipboard = NULL;

  e_clip = NULL;

  clip_cut = "";
  clip_copy = "";

  link = NULL;


  hasCutted = false;

  lastLinkMouseOver = NULL;

  // Initialize entity counters
  for(int i = Structural::Node; i < Structural::NoType; i += 1)
    entityCounter[ (Structural::StructuralType) i ] = 0;

  setAttribute(Qt::WA_TranslucentBackground);

  minimap = new StructuralMiniMap(this);
  minimap->init(this);
  minimap->setMinimumSize(MINIMAP_DEFAULT_W, MINIMAP_DEFAULT_H);
  minimap->setMaximumSize(MINIMAP_DEFAULT_W*2, MINIMAP_DEFAULT_H*2);
}

StructuralView::~StructuralView()
{
  // TODO
}

void StructuralView::createObjects()
{
  scene = new StructuralScene();
  scene->setParent(this);
  scene->setSceneRect(0, 0, 3600, 1800);

  setScene(scene); centerOn(3600/2,1800/2);

  linkDialog = new StructuralLinkDialog(this);

  connect(scene->_menu->redoAction, SIGNAL(triggered(bool)), this, SLOT(performRedo()));
  connect(scene->_menu->undoAction, SIGNAL(triggered(bool)), this, SLOT(performUndo()));
  connect(scene->_menu->pasteAction, SIGNAL(triggered(bool)), this, SLOT(performPaste()));
  connect(scene->_menu->snapshotAction, SIGNAL(triggered(bool)), this, SLOT(performSnapshot()));
  connect(scene->_menu,SIGNAL(insert(StructuralType)), this,SLOT(create(StructuralType)));

  this->resize(scene->itemsBoundingRect().size().width(), scene->itemsBoundingRect().height());
}

void StructuralView::setMiniMapVisible(bool enable)
{
  if (enable)
    minimap->show();
  else
    minimap->hide();
}

void StructuralView::switchMinimapVis()
{
  setMiniMapVisible(!minimap->isVisible());
}

void StructuralView::resizeEvent(QResizeEvent *event)
{
  minimap->move(event->size().width() - minimap->width(),
                event->size().height() - minimap->height());

  QGraphicsView::resizeEvent(event);
}


void StructuralView::createConnection()
{
  // TODO
}

bool StructuralView::hasEntity(QString uid)
{
  return entities.contains(uid);
}

StructuralEntity* StructuralView::getEntity(QString uid)
{
  return entities.value(uid,NULL);
}

QMap<QString, StructuralEntity*> StructuralView::getEntities()
{
  return entities;
}

/**********************************************************/

void StructuralView::load(QString &data)
{
  QDomDocument* dom = new QDomDocument();
  dom->setContent(data);


  QDomElement root = dom->firstChildElement();
  QDomElement noParentElement;

  QDomNodeList childList = root.childNodes();

  for (unsigned int k = 0; k < childList.length(); k++)
  {
    if (!childList.item(k).isElement())
      continue;

    noParentElement = childList.item(k).toElement();

    QMap<QString,QString> properties;


    QDomNodeList list = noParentElement.childNodes();

    for (unsigned int i = 0; i < list.length(); i++)
    {
      if (list.item(i).isElement())
      {
        QDomElement e = list.item(i).toElement();

        if (e.nodeName() == "property")
          properties.insert(e.attributeNode("name").nodeValue(), e.attributeNode("value").nodeValue());
      }
    }

    QMap<QString,QString> settings;
    settings[PLG_SETTING_UNDO] = "0";
    settings[PLG_SETTING_NOTIFY] = "0";
    settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();

    insert(properties.value(PLG_ENTITY_UID), "", properties, settings);

    for (unsigned int i = 0; i < list.length(); i++)
    {
      if (list.item(i).isElement())
      {
        QDomElement e = list.item(i).toElement();

        if (e.nodeName() == "entity")
          read(e, noParentElement);
      }
    }

      settings.insert(PLG_SETTING_NOTIFY,"1");
      select(properties.value(PLG_ENTITY_UID), settings);

      centerOn(entities.value(properties.value(PLG_ENTITY_UID)));
  }
}

void StructuralView::performZoomIn()
{

  if (zoomStep > 0)
  {
    zoomStep--;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
  }
  else
  {
    emit zoominStateChange(false);
  }

}

void StructuralView::performZoomOut()
{
  if (zoomStep*0.05 < 0.9)
  {
    emit zoominStateChange(true);

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

StructuralScene* StructuralView::getScene()
{
  return scene;
}

void StructuralView::setMod(bool mod)
{
  modified = mod;
}

void StructuralView::read(QDomElement element, QDomElement parent)
{
  QMap<QString,QString> properties;

  QDomNodeList list = element.childNodes();

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      if (e.nodeName() == "property")
        properties.insert(e.attributeNode("name").nodeValue(), e.attributeNode("value").nodeValue());
    }
  }

  QMap<QString,QString> settings;
  settings[PLG_SETTING_UNDO] = "0";
  settings[PLG_SETTING_NOTIFY] = "0";
  settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();


  insert(properties.value(PLG_ENTITY_UID), parent.attributeNode("uid").nodeValue(), properties, settings);

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      if (e.nodeName() == "entity")
        read(e, element);
    }
  }
}

QString StructuralView::serialize(QString data)
{
  QDomDocument* document = new QDomDocument();
  QDomElement root = document->createElement("structural");

  foreach(StructuralEntity* entity, entities.values())
  {
    if (entity->getStructuralParent() == NULL)
    {
      exportDataFromEntity(entity, document, root);
    }
  }

  document->appendChild(root);
  data = document->toString(4);

  return data;
}

void StructuralView::exportDataFromEntity(StructuralEntity* entity, QDomDocument* doc, QDomElement parent)
{  
  QDomElement element = doc->createElement("entity");

  element.setAttribute("uid",entity->getStructuralUid());

  foreach (QString name, entity->getStructuralProperties().keys())
  {
    QDomElement property = doc->createElement("property");
    property.setAttribute("name",name);
    property.setAttribute("value", entity->getStructuralProperty(name));

    element.appendChild(property);
  }


  foreach (StructuralEntity* child, entity->getStructuralEntities()) {
    exportDataFromEntity(child, doc, element);
  }

  parent.appendChild(element);
}

void StructuralView::insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  if (!entities.contains(uid))
  {
    StructuralEntity* entity; StructuralType type = (StructuralType) properties.value(PLG_ENTITY_TYPE).toInt();

    switch (type)
    {
      case Structural::Context:
      {
        entity = new StructuralComposition();
        entity->setStructuralType(Structural::Context);
        break;
      }

      case Structural::Switch:
      {
        entity = new StructuralComposition();
        entity->setStructuralType(Structural::Switch);
        break;
      }

      case Structural::Media:
      {
        entity = new StructuralContent();
        break;
      }

      case Structural::Port:
      {
        entity = new StructuralInterface();
        entity->setStructuralType(Structural::Port);
        break;
      }

      case Structural::SwitchPort:
      {
        entity = new StructuralInterface();
        entity->setStructuralType(Structural::SwitchPort);
        break;
      }

      case Structural::Area:
      {
        entity = new StructuralInterface();
        entity->setStructuralType(Structural::Area);
        break;
      }

      case Structural::Property:
      {
        entity = new StructuralInterface();
        entity->setStructuralType(Structural::Property);
        break;
      }

      case Structural::Body:
      {
        // check if it is allowed to insert body and
        // if the view already contains one
        if (DEFAULT_BODY_ENABLE  && getBody() == NULL) {

          entity = new StructuralComposition();
          entity->setStructuralType(Structural::Body);

          emit bodyStateChange(false);
        }

        break;
      }

      case Structural::Link:
      {
        entity = new StructuralLink();
        break;
      }

      case Structural::Bind:
      {
        entity = new StructuralBind();

        StructuralBind* bind = (StructuralBind*) entity;
        if (entities.contains(properties.value(PLG_ENTITY_START_UID)))
          bind->setEntityA(entities.value(properties.value(PLG_ENTITY_START_UID)));
        if (entities.contains(properties.value(PLG_ENTITY_END_UID)))
          bind->setEntityB(entities.value(properties.value(PLG_ENTITY_END_UID)));

        bind->setType((StructuralRole) properties.value(PLG_ENTITY_ROLE).toInt());

        if (properties.value(PLG_ENTITY_ANGLE).isEmpty())
        {
          adjustAngles(bind);
          bind->setStructuralProperty(PLG_ENTITY_ANGLE, QString::number(bind->getAngle()));
        }
        else
        {
          bind->setAngle(properties.value(PLG_ENTITY_ANGLE).toDouble());
        }

        break;
      }

      case Structural::Reference:
      {
        if (entities.contains(properties.value(PLG_ENTITY_START_UID)) &&
            entities.contains(properties.value(PLG_ENTITY_END_UID)))
        {
            entity = new StructuralReference();
            ((StructuralReference*) entity)->setEntityA(entities.value(properties.value(PLG_ENTITY_START_UID)));
            ((StructuralReference*) entity)->setEntityB(entities.value(properties.value(PLG_ENTITY_END_UID)));
        }

        break;
      }

      case Structural::Mapping:
      {
        entity = new StructuralReference();
        entity->setStructuralType(Structural::Mapping);
        if (entities.contains(properties.value(PLG_ENTITY_START_UID)))
          ((StructuralReference*) entity)->setEntityA(entities.value(properties.value(PLG_ENTITY_START_UID)));

        if (entities.contains(properties.value(PLG_ENTITY_END_UID)))
          ((StructuralReference*) entity)->setEntityB(entities.value(properties.value(PLG_ENTITY_END_UID)));

        break;
      }

      default:
      {
        entity = NULL;
        break;
      }
    }

    bool avoidCollision = true;

    if (entity != NULL)
    {
      if (entities.contains(parent))
      {

        StructuralEntity* p = entities.value(parent);

        bool uncollapseAndThenCollapse = false;

        if (p->isCollapsed()){
          if (p->getStructuralType() == Structural::Body ||
              p->getStructuralType() == Structural::Switch ||
              p->getStructuralType() == Structural::Context)
          {
            uncollapseAndThenCollapse = true;

            ((StructuralComposition*) p)->collapse();
          }
        }

        entity->setStructuralParent(entities.value(parent));

        if (uncollapseAndThenCollapse)
          ((StructuralComposition*) p)->collapse();


        if (entity->getStructuralCategory() != Structural::Edge){
          if (!properties.value(PLG_ENTITY_TOP).isEmpty() && !properties.value(PLG_ENTITY_LEFT).isEmpty())
          {
            avoidCollision = false;
          }
          else
          {
            entity->setTop(entities[parent]->getHeight()/2 - entity->getHeight()/2);
            entity->setLeft(entities[parent]->getWidth()/2 - entity->getWidth()/2);

            avoidCollision = true;
          }
        }
      }
      else
      {
        scene->addItem(entity);

        if (entity->getStructuralCategory() != Structural::Edge){
          entity->setTop(scene->sceneRect().height()/2 - entity->getHeight()/2);
          entity->setLeft(scene->sceneRect().width()/2 - entity->getWidth()/2);
        }
      }

      entity->setStructuralUid(uid);
      entity->setStructuralProperties(properties);

      entity->setSelected(false);

      entities[uid] = entity;
      if (entity->getStructuralId().isEmpty())
        updateEntityWithUniqueNstId(entity);

      entity->refresh();

      entity->adjust(avoidCollision);

      connect(entity, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(removed(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(entity, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));
      connect(entity, SIGNAL(move(QString,QString)), SLOT(move(QString,QString)));

      connect(entity->menu->deleteAction, SIGNAL(triggered(bool)), this, SLOT(performDelete()));
      connect(entity->menu->redoAction, SIGNAL(triggered(bool)), this, SLOT(performRedo()));
      connect(entity->menu->undoAction, SIGNAL(triggered(bool)), this, SLOT(performUndo()));
      connect(entity->menu->cutAction, SIGNAL(triggered(bool)), this, SLOT(performCut()));
      connect(entity->menu->pasteAction, SIGNAL(triggered(bool)), this, SLOT(performPaste()));
      connect(entity->menu->copyAction, SIGNAL(triggered(bool)), this, SLOT(performCopy()));
      connect(entity->menu->snapshotAction, SIGNAL(triggered(bool)), this, SLOT(performSnapshot()));
      connect(entity->menu,SIGNAL(insert(StructuralType)), this,SLOT(create(StructuralType)));

      if (entities.size() <= 1){
        centerOn(entity);
      }

      if (settings[PLG_SETTING_UNDO] == "1")
      {
        Insert* command = new Insert(uid, parent, entity->getStructuralProperties(), settings);

        connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
        connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));


        command->setText(settings[PLG_SETTING_CODE]);

        commnads.push(command);

        emit undoStateChange(true);
      }

      if (entity->getStructuralType() ==  Structural::Port)
        drawPortReference(entity);

      if (settings[PLG_SETTING_NOTIFY] == "1")
      {
        emit inserted(uid, parent, entity->getStructuralProperties(), settings);
      }
    }
  }
}


void StructuralView::adjustAngles(StructuralBind* edge)
{
  int MIN =  100000;
  int MAX = -100000;

  bool HAS = false;

  foreach (StructuralEntity *e, entities)
  {
    if (e->getStructuralType() == Structural::Bind)
    {
      StructuralBind *ea = (StructuralBind*) e;

      if ((edge->getEntityA() == ea->getEntityA() || edge->getEntityA() == ea->getEntityB()) &&
          (edge->getEntityB() == ea->getEntityB() || edge->getEntityB() == ea->getEntityA()) &&
          edge != ea)
      {
        HAS = true;

        int A = ea->getAngle();

        if (!ea->isCondition())
          A = -A;

        if (MAX < A )
          MAX = A;

        if (MIN > A )
          MIN = A;
      }
    }
  }

  int ANGLE = -1;

  if (!HAS)
    ANGLE = 0;
  else if (MAX <= abs(MIN))
    ANGLE = MAX+60;
  else if (MAX > abs(MIN))
    ANGLE = MIN-60;

  if (!edge->isCondition())
    edge->setAngle(-ANGLE);
  else
    edge->setAngle(ANGLE);

}

QVector<StructuralEntity*> StructuralView::getRoots()
{
  QVector<StructuralEntity*> r;

  foreach (StructuralEntity* e, entities) {
    if (e->getStructuralParent() == NULL)
      r.append(e);
  }

  return r;
}

void StructuralView::remove(QString uid, QMap<QString, QString> settings)
{

  if (entities.contains(uid))
  {
    if (settings[PLG_SETTING_UNDO] == "1")
    {
      Remove* command;

      if (entities[uid]->getStructuralParent() != NULL)
        command = new Remove(entities[uid]->getStructuralUid(), entities[uid]->getStructuralParent()->getStructuralUid(), entities[uid]->getStructuralProperties(), settings);
      else
        command = new Remove(entities[uid]->getStructuralUid(), "", entities[uid]->getStructuralProperties(), settings);

      command->setText(settings[PLG_SETTING_CODE]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      emit undoStateChange(true);

      commnads.push(command); return;
    }

    StructuralEntity* entity = entities.value(uid);
    StructuralEntity* parent = entity->getStructuralParent();

    while(!entity->getStructuralEntities().isEmpty())
    {
      StructuralEntity* e = entity->getStructuralEntities().first();

      if (settings.value(PLG_SETTING_UNDO_CHILDREN) != "0")
        settings[PLG_SETTING_UNDO] = "1";

        remove(e->getStructuralUid(), settings);
    }

    if (entity->getStructuralCategory() != Structural::Edge)
    {
      QVector<StructuralEntity*> roots;

      if (parent != NULL)
        roots = parent->getStructuralEntities();
      else
        roots = getRoots();

      if (entity->getStructuralCategory() == Structural::Interface)
      {
        if (parent != NULL)
          if (parent->getStructuralParent() != NULL)
            roots += parent->getStructuralParent()->getStructuralEntities();
          else
            roots += getRoots();
      }


      foreach (StructuralEntity* c, roots) {
        if (c->getStructuralCategory() == Structural::Edge){
          StructuralEdge *e = (StructuralEdge*) c;

          if (e->getEntityA() == entity || e->getEntityB() == entity){

            if (e->getStructuralType() != Structural::Reference)
              remove(e->getStructuralUid(),
                   StructuralUtil::createSettings("1",
                                                  settings.value(PLG_SETTING_NOTIFY),
                                                  settings.value(PLG_SETTING_CODE)));
            else{
              StructuralEntity* cport = entities.value(c->getStructuralProperty(PLG_ENTITY_START_UID));

              QMap<QString, QString> prev = cport->getStructuralProperties();
              QMap<QString, QString> properties = cport->getStructuralProperties();;

              properties[PLG_ENTITY_COMPONENT_ID] = "";
              properties[PLG_ENTITY_COMPONENT_UID] = "";
              properties[PLG_ENTITY_INTERFACE_ID] = "";
              properties[PLG_ENTITY_INTERFACE_UID] = "";

              change(cport->getStructuralUid(),properties, prev,
                     StructuralUtil::createSettings("1",
                                                    settings.value(PLG_SETTING_NOTIFY),
                                                    settings.value(PLG_SETTING_CODE)));
            }
          }
        }
      }
    }

    if (parent != NULL)
    {
      parent->removeStructuralEntity(entity);
    }
    else
      scene->removeItem(entity);

    entities.remove(uid);

    if (uid == _selected_UID){
      _selected_UID = "";
      emit selectChange("");
    }

    if (entity->getStructuralType() == Structural::Body)
      emit bodyStateChange(true);

    delete entity; entity = NULL;


    if (settings[PLG_SETTING_NOTIFY] == "1")
    {
      emit removed(uid, settings);
    }
  }
}

void StructuralView:: change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
    if (settings[PLG_SETTING_UNDO] == "1")
    {
      QMap<QString,QString> tmp = previous;

      if (entities.value(uid)->getStructuralType() == Structural::Port ||
          entities.value(uid)->getStructuralType() == Structural::Mapping ||
          entities.value(uid)->getStructuralType() == Structural::Bind){
      if (!properties.contains(PLG_ENTITY_COMPONENT_ID))
        tmp.remove(PLG_ENTITY_COMPONENT_ID);

      if (!properties.contains(PLG_ENTITY_COMPONENT_UID))
        tmp.remove(PLG_ENTITY_COMPONENT_UID);

      if (!properties.contains(PLG_ENTITY_INTERFACE_ID))
        tmp.remove(PLG_ENTITY_INTERFACE_UID);

      if (!properties.contains(PLG_ENTITY_COMPONENT_ID))
        tmp.remove(PLG_ENTITY_COMPONENT_ID);
      }

      foreach (QString key, properties.keys()) {
        tmp[key] = properties.value(key);
      };

      Change* command = new Change(uid, tmp, previous, settings);

      command->setText(settings[PLG_SETTING_CODE]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      emit undoStateChange(true);

      commnads.push(command); return;
    }


     StructuralEntity* entity = entities[uid];

     if ((!entity->isCollapsed() && properties.value(PLG_ENTITY_COLLAPSED) == "1") ||
         (entity->isCollapsed() && properties.value(PLG_ENTITY_COLLAPSED) == "0"))
       ((StructuralComposition*) entity)->collapse(false);

     entity->setStructuralProperties(properties);
     entity->adjust(false);

     if (entity->getStructuralType() ==  Structural::Bind) {
       StructuralBind* b = (StructuralBind*) entity;

       b->setType((StructuralRole) properties.value(PLG_ENTITY_ROLE).toInt());

       if (entities.contains(properties.value(PLG_ENTITY_START_UID)))
         b->setEntityA(entities.value(properties.value(PLG_ENTITY_START_UID)));

       if (entities.contains(properties.value(PLG_ENTITY_END_UID)))
         b->setEntityB(entities.value(properties.value(PLG_ENTITY_END_UID)));
     }

     if (entity->getStructuralType() ==  Structural::Mapping) {
       StructuralReference* map = (StructuralReference*) entity;

       if (entities.contains(properties.value(PLG_ENTITY_START_UID)))
         map->setEntityA(entities.value(properties.value(PLG_ENTITY_START_UID)));

       if (entities.contains(properties.value(PLG_ENTITY_END_UID)))
         map->setEntityB(entities.value(properties.value(PLG_ENTITY_END_UID)));
     }

     if (entity != NULL && entity->getStructuralType() == Structural::Port)
       drawPortReference(entity);

    if (settings[PLG_SETTING_NOTIFY] == "1")
    {
      emit changed(uid, properties, previous, settings);
    }

  }
}

void StructuralView::drawPortReference(StructuralEntity* entity)
{
  if (entity != NULL && entity->getStructuralType() == Structural::Port){
    QString uid = "";
    if (!entity->getStructuralProperty(PLG_ENTITY_INTERFACE_UID).isEmpty())
      uid =  entity->getStructuralProperty(PLG_ENTITY_INTERFACE_UID);

    else if (!entity->getStructuralProperty(PLG_ENTITY_COMPONENT_UID).isEmpty())
      uid =  entity->getStructuralProperty(PLG_ENTITY_COMPONENT_UID);

    foreach (StructuralEntity* e, entities.values()) {
      if (e->getStructuralType() == Structural::Reference)
      {
        if (e->getStructuralProperty(PLG_ENTITY_START_UID) == entity->getStructuralUid())
          remove(e->getStructuralUid(),StructuralUtil::createSettings(false,false));
      }
    }

    if (entities.contains(uid)){
      QMap<QString, QString> properties;
      properties[PLG_ENTITY_TYPE] = QString::number(Structural::Reference);
      properties[PLG_ENTITY_START_UID] = entity->getStructuralUid();
      properties[PLG_ENTITY_END_UID] = uid;

      insert(StructuralUtil::CreateUid(),
             entity->getStructuralParent()->getStructuralUid(),
             properties,
             StructuralUtil::createSettings(false,false));
    }
  }
}

void StructuralView::unSelect()
{

  if (entities.contains(_selected_UID))
  {
    entities.value(_selected_UID)->setSelected(false);

}

  _selected_UID = "";

  scene->update();

  emit cutStateChange(false);
  emit copyStateChange(false);
  emit pasteStateChange(false);

  emit selectChange("");
  emit selected("", QMap<QString, QString>());
}

StructuralEntity* StructuralView::getBody()
 {
   foreach (StructuralEntity* e, entities) {
    if (e->getStructuralType() == Structural::Body)
      return e;
   }

   return NULL;
 }

void StructuralView::clearErrors()
{
  foreach (StructuralEntity* e, entities.values()) {
    e->setError(false);
    e->setErrorMsg("");
  }
}

void StructuralView::markError(QString uid, QString msg)
{
  if(entities.contains(uid))
  {
    StructuralEntity* entity = entities.value(uid);
    entity->setError(true);
    entity->setErrorMsg(msg);
    entity->refresh();
  }
}

void StructuralView::select(QString uid, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
      if (uid == _selected_UID)
        return;

      StructuralEntity* _selected = NULL;

      if (!_selected_UID.isEmpty())
        _selected = entities.value(_selected_UID,NULL);

      if (_selected != NULL)
      {
        _selected->setSelected(false);
        _selected->adjust(false);
      }


      StructuralEntity* entity = entities[uid];
      entity->setSelected(true);

      _selected_UID = uid;

      if (entities.value(_selected_UID)->getStructuralType() == Structural::Body)
        centerOn(entities.value(_selected_UID));

      emit selectChange(uid);

//      if (settings[PLG_SETTING_NOTIFY] == "1")
//      {
        emit selected(uid, settings);
//      }

      emit cutStateChange(true);
      emit copyStateChange(true);

      if (!clip_cut.isEmpty() || !clip_copy.isEmpty()){
        StructuralEntity* e = NULL;

        if (!clip_cut.isEmpty())
          e = e_clip;

        if (!clip_copy.isEmpty())
          e = entities.value(clip_copy);

        if (StructuralUtil::hasValidKinshp(e->getStructuralType(), entities.value(_selected_UID)->getStructuralType()))
          emit pasteStateChange(true);
        else
          emit pasteStateChange(false);
      }

          scene->update();
  }
}

void StructuralView::move(QString uid, QString parent)
{
  if (entities.contains(uid) &&
      entities.contains(parent)){

      StructuralEntity* e = entities.value(uid);
      StructuralEntity* p = entities.value(parent);

    if (!StructuralUtil::hasValidKinshp(
          e->getStructuralType(),
          p->getStructuralType()))
      return;

    if (e->getStructuralParent()->getStructuralUid() == p->getStructuralUid())
      return;

    unSelect();

    StructuralEntity* moveEntity = clone(entities.value(uid), NULL);

    QString CODE = StructuralUtil::CreateUid();

    QMap<QString, QString> settings;
    settings[PLG_SETTING_UNDO] = "1";
    settings[PLG_SETTING_NOTIFY] = "1";
    settings[PLG_SETTING_CODE] = CODE;

    remove(uid, settings);

    performPaste(moveEntity, entities.value(parent), CODE, true);

    StructuralEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(false);
    }
  }

}

void StructuralView::create(StructuralType type)
{
  QMap<QString,QString> properties;
  properties[PLG_ENTITY_TYPE] = QString::number(type);

  QMap<QString,QString> settings;
  settings[PLG_SETTING_UNDO] = "1";
  settings[PLG_SETTING_NOTIFY] = "1";
  settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();

  create(type, properties, settings);
}

void StructuralView::create(StructuralType type, QMap<QString, QString> &properties, QMap<QString, QString> &settings)
{
  QString uid = StructuralUtil::CreateUid();
  QString parent = "";

  switch (type)
  {
    default:
    {
      StructuralEntity* _selected = NULL;

      if (!_selected_UID.isEmpty())
        _selected = entities[_selected_UID];

      if (_selected != NULL)
      {
        parent = _selected->getStructuralUid();
      }

      break;
    }
  }

  if (!properties.contains(PLG_ENTITY_TYPE))
    properties[PLG_ENTITY_TYPE] = QString::number(type);

  if (!settings.contains(PLG_SETTING_UNDO))
    settings[PLG_SETTING_UNDO] = "1";

  if (!settings.contains(PLG_SETTING_NOTIFY))
    settings[PLG_SETTING_NOTIFY] = "1";

  if (!settings.contains(PLG_SETTING_CODE))
    settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();

  insert(uid, parent, properties, settings);
}


void StructuralView::performHelp()
{

}

void StructuralView::performUndo()
{
  if (commnads.canUndo())
  {
      QString code = commnads.undoText();

      while(code == commnads.undoText())
        commnads.undo();

      emit redoStateChange(true);
  }

  if (!commnads.canUndo())
    emit undoStateChange(false);
}

void StructuralView::performRedo()
{
  if (commnads.canRedo())
  {
    QString code = commnads.redoText();

    while(code == commnads.redoText())
      commnads.redo();

    emit undoStateChange(true);
  }

  if (!commnads.canRedo())
    emit redoStateChange(false);

}

void StructuralView::performCut()
{
    StructuralEntity* _selected = NULL;

    if (!_selected_UID.isEmpty())
      _selected = entities[_selected_UID];

    if (_selected != NULL)
    {
      if (!clip_copy.isEmpty())
      {
        clip_copy =  "";
      }


      if (!clip_cut.isEmpty())
      {
        clip_cut = "";

        delete(e_clip); e_clip = NULL;
      }

        clip_cut = _selected->getStructuralUid();


        unSelect();

        e_clip = clone(entities.value(clip_cut), NULL);

        QMap<QString, QString> settings;
        settings[PLG_SETTING_UNDO] = "1";
        settings[PLG_SETTING_NOTIFY] = "1";
        settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();

        remove(clip_cut, settings);

        emit pasteStateChange(true);
    }

}

StructuralEntity* StructuralView::clone(StructuralEntity* e, StructuralEntity* new_p)
{
  StructuralEntity* eclone = NULL;

  switch (e->getStructuralType()) {
    case Structural::Media:
      eclone = new StructuralContent();
      break;
    case Structural::Area:
    case Structural::Property:
    case Structural::SwitchPort:
    case Structural::Port:
      eclone = new StructuralInterface();
      break;
    default:
      eclone = new StructuralComposition();
      break;
  }

  if (new_p != NULL){
    eclone->setStructuralParent(new_p);
  }

  eclone->setStructuralProperties(e->getStructuralProperties());

  foreach (StructuralEntity* c, e->getStructuralEntities()) {
   eclone->addStructuralEntity(clone(c,eclone));
  }

  return eclone;
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

void StructuralView::performCopy()
{
    StructuralEntity* _selected = NULL;

    if (!_selected_UID.isEmpty())
      _selected = entities[_selected_UID];

    if (_selected != NULL)
    {
      if (!clip_cut.isEmpty())
      {
        clip_cut = "";

        delete(e_clip); e_clip = NULL;
      }

      emit pasteStateChange(true);
        clip_copy = _selected->getStructuralUid();
    }
}

bool StructuralView::isChild(StructuralEntity* e , StructuralEntity* p)
{
    bool r = false;

    foreach (StructuralEntity* ec, p->getStructuralEntities()) {
        if (ec == e)
          return true;
        else
          isChild(e, ec);
    }

    return r;
}

void StructuralView::createLink(StructuralEntity* a, StructuralEntity* b)
{

    StructuralEntity* pa = (StructuralEntity*) a->getStructuralParent();
    StructuralEntity* pb = (StructuralEntity*) b->getStructuralParent();

    if (pa != NULL && pb != NULL)
    {
      QString uid = QUuid::createUuid().toString();
      StructuralEntity* parent = NULL;
      QMap<QString, QString> properties;
      properties[PLG_ENTITY_TYPE] = QString::number(Structural::Link);

      QPointF pt_a;
      QPointF pt_b;

      if (pa == pb)
      {
          parent = pa;

          pt_a.setX(a->getLeft()), pt_a.setY(a->getTop());
          pt_b.setX(b->getLeft()), pt_b.setY(b->getTop());
      }
      else if (pa->getStructuralParent() == pb)
      {
          parent = pb;

          QPointF cv = pa->mapToParent(a->getLeft(), a->getTop());

          pt_a.setX(cv.x()), pt_a.setY(cv.y());
          pt_b.setX(b->getLeft()), pt_b.setY(b->getTop());
      }
      else if (pa == pb->getStructuralParent())
      {
          parent = pa;

          QPointF cv = pb->mapToParent(b->getLeft(), b->getTop());

          pt_a.setX(a->getLeft()), pt_a.setY(a->getTop());
          pt_b.setX(cv.x()), pt_b.setY(cv.y());
      }
      else if (pa->getStructuralParent() == pb->getStructuralParent() &&
               pa->getStructuralParent() != NULL)
      {
        parent = pa->getStructuralParent();

        QPointF ca = pa->mapToParent(a->getLeft(), a->getTop());
        QPointF cb = pb->mapToParent(b->getLeft(), b->getTop());

        pt_a.setX(ca.x()), pt_a.setY(ca.y());
        pt_b.setX(cb.x()), pt_b.setY(cb.y());
      }
      else
        return;

      pt_a.setX(pt_a.x()+a->getWidth()/2);
      pt_a.setY(pt_a.y()+a->getHeight()/2);

      pt_b.setX(pt_b.x()+b->getWidth()/2);
      pt_b.setY(pt_b.y()+b->getHeight()/2);


        if (pt_a.y() > pt_b.y())
            properties[PLG_ENTITY_TOP] = QString::number(pt_b.y() + (pt_a.y() - pt_b.y())/2);
        else
            properties[PLG_ENTITY_TOP] = QString::number(pt_a.y() + (pt_b.y() - pt_a.y())/2);


        if (pt_a.x() > pt_b.x())
            properties[PLG_ENTITY_LEFT] = QString::number(pt_b.x() + (pt_a.x() - pt_b.x())/2);
        else
            properties[PLG_ENTITY_LEFT] = QString::number(pt_a.x() + (pt_b.x() - pt_a.x())/2);

        emit requestLinkDialogUpdate();

        modified = false;            // turn off link mode
        emit linkStateChange(false); // turn off link mode on UI

        linkDialog->init();

        if (linkDialog->exec()){

          properties.insert(PLG_ENTITY_XCONNECTOR_ID, linkDialog->getCurrentConnector());

          QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

          insert(uid, parent->getStructuralUid(), properties, settings);

          if (entities.contains(uid))
          {
            StructuralRole con = StructuralUtil::getStructuralRoleFromStr(linkDialog->form.cbCondition->currentText());
            StructuralRole act = StructuralUtil::getStructuralRoleFromStr(linkDialog->form.cbAction->currentText());

            createBind(a,entities.value(uid),con,settings.value(PLG_SETTING_CODE));
            createBind(entities.value(uid),b,act,settings.value(PLG_SETTING_CODE));
          }
      }
    }
}

void StructuralView::createBind(StructuralEntity* a, StructuralEntity* b, StructuralRole type, QString code)
{
    StructuralEntity* pa = a->getStructuralParent();
    StructuralEntity* pb = b->getStructuralParent();

    if (pa != NULL && pb != NULL)
    {
      QString parent;

      if (pa == pb)
      {
          parent = pa->getStructuralUid();
      }
      else if (pa->getStructuralParent() == pb)
      {
          parent = pb->getStructuralUid();
      }
      else if (pa == pb->getStructuralParent())
      {
          parent = pa->getStructuralUid();
      }
      else
      {
          return;
      }

      QString uid = QUuid::createUuid().toString();

      QMap<QString, QString> properties;
      properties[PLG_ENTITY_TYPE] = QString::number(Structural::Bind);
      properties[PLG_ENTITY_START_UID] = a->getStructuralUid();
      properties[PLG_ENTITY_END_UID] = b->getStructuralUid();

      properties[PLG_ENTITY_ROLE] = QString::number(type);
      properties[PLG_ENTITY_ID] = StructuralUtil::getStrFromStructuralRole(type);

      StructuralEntity* e_link = NULL;
      StructuralEntity* e_nolink = NULL;
      StructuralEntity* pe_nolink = NULL;

      if (a->getStructuralType() == Structural::Link)
      {
        e_link = a;
        e_nolink = b;
        pe_nolink = pb;

        if (type == Structural::NoRole){
          emit requestLinkDialogUpdate();

          modified = false;            // turn off link mode
          emit linkStateChange(false); // turn off link mode on UI

          linkDialog->init(e_link->getStructuralProperty(PLG_ENTITY_XCONNECTOR_ID),"","",
                           StructuralLinkDialog::ActionMode);

          if (linkDialog->exec()){
            QString role = linkDialog->form.cbAction->currentText();

            properties[PLG_ENTITY_ROLE] = QString::number(StructuralUtil::getStructuralRoleFromStr(role));
            properties[PLG_ENTITY_ID] = role;
          }else{
            return;
          }
        }
      }
      else if (b->getStructuralType() == Structural::Link)
      {
        e_link = b;
        e_nolink = a;
        pe_nolink = pa;

        if (type == Structural::NoRole){
          emit requestLinkDialogUpdate();

          modified = false;            // turn off link mode
          emit linkStateChange(false); // turn off link mode on UI

          linkDialog->init(e_link->getStructuralProperty(PLG_ENTITY_XCONNECTOR_ID),"","",
                           StructuralLinkDialog::ConditionMode);

          if (linkDialog->exec()){
            QString role = linkDialog->form.cbCondition->currentText();

            properties[PLG_ENTITY_ROLE] = QString::number(StructuralUtil::getStructuralRoleFromStr(role));
            properties[PLG_ENTITY_ID] = role;
          }else{
            return;
          }
        }
      }

      if (e_link != NULL && e_nolink != NULL && pe_nolink != NULL){
        properties[PLG_ENTITY_LINK_UID] = e_link->getStructuralUid();

        if (e_nolink->getStructuralCategory() == Structural::Interface)
        {
          properties[PLG_ENTITY_INTERFACE_ID] = e_nolink->getStructuralId();
          properties[PLG_ENTITY_INTERFACE_UID] = e_nolink->getStructuralUid();
          properties[PLG_ENTITY_COMPONENT_ID] = pe_nolink->getStructuralId();
          properties[PLG_ENTITY_COMPONENT_UID] = pe_nolink->getStructuralUid();
        }
        else
        {
          properties[PLG_ENTITY_COMPONENT_ID] = e_nolink->getStructuralId();
          properties[PLG_ENTITY_COMPONENT_UID] = e_nolink->getStructuralUid();
          properties[PLG_ENTITY_INTERFACE_ID] = "";
          properties[PLG_ENTITY_INTERFACE_UID] = "";
        }
      }

      QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

      if (!code.isEmpty())
          settings[PLG_SETTING_CODE] = code;

      insert(uid, parent, properties, settings);
    }
}

void StructuralView::createReference(StructuralEntity* a, StructuralEntity* b)
{
  if (a->getStructuralType() == Structural::Port || a->getStructuralType() == Structural::SwitchPort)
  {
      StructuralEntity* pa = a->getStructuralParent();
      StructuralEntity* pb = b->getStructuralParent();

      if (pa != NULL && pb != NULL && (pa == pb || pa == pb->getStructuralParent()))
      {
          QMap<QString, QString> prev = a->getStructuralProperties();
          QMap<QString, QString> properties;

          if (a->getStructuralType() == Structural::Port)
            properties = a->getStructuralProperties();;

          if (b->getStructuralCategory() == Structural::Interface)
          {
            properties[PLG_ENTITY_INTERFACE_ID] = b->getStructuralId();
            properties[PLG_ENTITY_INTERFACE_UID] = b->getStructuralUid();
            properties[PLG_ENTITY_COMPONENT_ID] = pb->getStructuralId();
            properties[PLG_ENTITY_COMPONENT_UID] = pb->getStructuralUid();
          }
          else
          {
            properties[PLG_ENTITY_COMPONENT_ID] = b->getStructuralId();
            properties[PLG_ENTITY_COMPONENT_UID] = b->getStructuralUid();
            properties[PLG_ENTITY_INTERFACE_ID] = "";
            properties[PLG_ENTITY_INTERFACE_UID] = "";
          }

          QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

          if (a->getStructuralType() == Structural::Port)
            change(a->getStructuralUid(),properties, prev, settings);

          else  if (a->getStructuralType() == Structural::SwitchPort){
            properties[PLG_ENTITY_TYPE] = QString::number(Structural::Mapping);
            properties[PLG_ENTITY_START_UID] = a->getStructuralUid();
            properties[PLG_ENTITY_END_UID] = b->getStructuralUid();

            insert(StructuralUtil::CreateUid(),
                   pa->getStructuralUid(),properties,settings);
          }
      }
  }

}

void StructuralView::performPaste()
{
    QString clip = "";

    if (!clip_copy.isEmpty())
      clip = clip_copy;
    else if (!clip_cut.isEmpty())
      clip = clip_cut;

    StructuralEntity* _selected = NULL;

    if (!_selected_UID.isEmpty())
      _selected = entities.value(_selected_UID, NULL);


    if (entities.contains(clip))
    {
      StructuralEntity* entity = entities.value(clip);

        if (_selected != NULL && _selected != entities.value(clip))
        {
          if (!isChild(_selected, entities.value(clip)))
          {


              if (entity->getStructuralCategory() == Structural::Interface)
              {

              }
              else if (entity->getStructuralCategory() == Structural::Node)
              {
                  if (_selected->getStructuralType() == Structural::Body ||
                      _selected->getStructuralType() == Structural::Context ||
                      _selected->getStructuralType() == Structural::Switch)
                  {
                      performPaste(entity, _selected, QUuid::createUuid().toString(), true);
                  }
              }
          }
        }else if (!DEFAULT_BODY_ENABLE){
          performPaste(entity, NULL, QUuid::createUuid().toString(), true);
        }

    }
    else if (e_clip != NULL)
    {
      if (_selected == NULL && DEFAULT_BODY_ENABLE)
        return;

      performPaste(e_clip, _selected, QUuid::createUuid().toString(), true);
    }

}

void StructuralView::performPaste(StructuralEntity* entity, StructuralEntity* parent, QString CODE, bool newPos)
{
    QString parentuid;

    if (parent == NULL)
        parentuid = "";
    else
      parentuid = parent->getStructuralUid();

    QString uid = QUuid::createUuid().toString();

    QMap<QString, QString> properties = entity->getStructuralProperties();
    properties[PLG_ENTITY_UID] = uid;

    if (newPos){
      properties.remove(PLG_ENTITY_TOP);
     properties.remove(PLG_ENTITY_LEFT);
    }


    QMap<QString, QString> settings;
    settings[PLG_SETTING_UNDO] = "1";
    settings[PLG_SETTING_NOTIFY] = "1";
    settings[PLG_SETTING_CODE] = CODE;

    insert(uid, parentuid, properties, settings);

    if (entities.contains(uid))
    {
//        StructuralEntity* ee = entities.value(uid);

//        if (ee->isCollapsed())
//        {
//          qreal lastw = ee->getUncollapedWidth();
//          qreal lasth = ee->getUncollapedHeight();

//          ee->setTop(ee->getTop() - (lasth/2 - DEFAULT_MEDIA_HEIGHT/2));
//          ee->setLeft(ee->getLeft() - (lastw/2 - DEFAULT_MEDIA_WIDTH/2));
//          ee->setWidth(lastw);
//          ee->setHeight(lasth);
//        }

        foreach (StructuralEntity* e, entity->getStructuralEntities()) {
            performPaste(e, entities[uid], CODE, false);
        }

//        if(ee->isCollapsed())
//        {
//          ee->setCollapsed(false);
//          ((StructuralComposition*) ee)->collapse();
//        }
    }


}

bool StructuralView::updateEntityWithUniqueNstId(StructuralEntity *entity)
{
  if(entity != NULL)
  {
    int n = 0;
    Structural::StructuralType type = entity->getStructuralType();

    // check if type is in the map
    if(!entityCounter.count(type))
      entityCounter[type] = 0;

    // change this check in future
    QString prefix = StructuralUtil::getPrefixIdFromType(type);


    bool match = false;

    while(1)
    {
      match = false;

      foreach(StructuralEntity* e, entities.values())
      {
          if (e->getStructuralId() == QString(prefix + QString::number(n)))
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

    entity->setStructuralId(QString(prefix + QString::number(n)));

    return true;
  }

  return false;
}

void StructuralView::performDelete()
{
  if (entities.contains(_selected_UID))
  {
      QMap<QString, QString> settings;
      settings[PLG_SETTING_UNDO] = "1";
      settings[PLG_SETTING_NOTIFY] = "1";
      settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();

      remove(_selected_UID, settings);

  }
}

void StructuralView::performSnapshot()
{
  QString location = QFileDialog::getSaveFileName(NULL, "Snapshot...", "", tr("Images (*.png)"));

    if (location != ""){
      qreal top = 0;
      qreal left = 0;
      qreal width = scene->width();
      qreal height = scene->height();

      StructuralEntity* entity = getBody();

      if (entity != NULL)
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



      if (!_selected_UID.isEmpty())
        entities.value(_selected_UID)->setSelected(false);

      scene->render(&painter, QRect(), QRect(left-25,top-25,width+50,height+50));

      if (!_selected_UID.isEmpty())
        entities.value(_selected_UID)->setSelected(true);


      painter.end();

      image.save(location, "PNG");
    }
}

void StructuralView::performProperties()
{

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
         lastLinkMouseOver->getStructuralType() == Structural::Context ||
         lastLinkMouseOver->getStructuralType() == Structural::Switch)
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
         entityb->getStructuralType() == Structural::Context ||
         entityb->getStructuralType() == Structural::Switch)
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

    if (!event->isAccepted()){

      if (event->button() == Qt::LeftButton)
      {
        StructuralEntity* _selected = NULL;

        if (!_selected_UID.isEmpty())
          _selected = entities[_selected_UID];

        if (_selected != NULL)
        {
          _selected->setSelected(false);
          _selected->adjust(false);
        }

        _selected_UID = "";

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
        if (entitya->getStructuralCategory() == Structural::Node &&
            entityb->getStructuralCategory() == Structural::Node)
        {
          StructuralUtil::dbg(this, "Linking NODE to NODE");

          if (entitya->getStructuralType() != Structural::Link && entityb->getStructuralType() != Structural::Link)
          {
            createLink(entitya, entityb);
          }

          else if (entitya->getStructuralType() == Structural::Link)
          {
            createBind(entitya, entityb);

          }else if (entityb->getStructuralType() == Structural::Link)
          {
            createBind(entitya, entityb);
          }

        }
        // if linking NODE to INTERFACE
        else if (entitya->getStructuralCategory() == Structural::Node &&
                 entityb->getStructuralCategory() == Structural::Interface)
        {
            StructuralUtil::dbg(this, "Linking NODE to INTERFACE");

            if (entitya->getStructuralType() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else
            {
              createBind(entitya, entityb);
            }
        }
        // if linking INTERFACE to NODE
        else if (entitya->getStructuralCategory() == Structural::Interface &&
                 entityb->getStructuralCategory() == Structural::Node)
        {
            StructuralEntity* pa = entitya->getStructuralParent();
            StructuralEntity* pb = entityb->getStructuralParent();

            StructuralUtil::dbg(this, "Linking INTERFACE to NODE");

            if (pa != NULL && pb != NULL && pa == pb)
            {
                createReference(entitya, entityb);
            }
            else if (entityb->getStructuralType() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else
            {
              createBind(entitya, entityb);
            }
        }
        // if linking INTERFACE to INTERFACE
        else if (entitya->getStructuralCategory() == Structural::Interface &&
                 entityb->getStructuralCategory() == Structural::Interface)
        {
            StructuralEntity* pa = entitya->getStructuralParent();
            StructuralEntity* pb = entityb->getStructuralParent();

            StructuralUtil::dbg(this, "Linking INTERFACE to INTERFACE");

            if (pa != NULL && pb != NULL &&  pa == pb->getStructuralParent())
            {
                createReference(entitya, entityb);
            }
            else
            {
                createLink(entitya, entityb);
            }
        }
      }

      entitya->setMouseHover(false);
      entityb->setMouseHover(false);

      // \fixme This is not the best place to control this!!
      if(
         entityb->getStructuralType() == Structural::Context ||
         entityb->getStructuralType() == Structural::Switch)
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


void StructuralView::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_Delete)
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
    StructuralEntity* _selected = NULL;

    if (!_selected_UID.isEmpty())
      _selected = entities[_selected_UID];

    if (_selected != NULL)
    {
      _selected->setSelected(false);
      _selected->adjust(false);
    }

    _selected_UID =  "";
    modified = true;

    emit linkStateChange(true);

    event->accept();
  }
  else if(event->key() == Qt::Key_Control)
  {

    StructuralEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(true);
    }

    event->accept();
  }

  if(!event->isAccepted())
    QGraphicsView::keyPressEvent(event);
}

void StructuralView::keyReleaseEvent(QKeyEvent *event)
{
  // SHIFT - Disabling linking
  if (event->key() == Qt::Key_Shift)
  {
    modified = false;

    emit linkStateChange(false);
  }
  else if(event->key() == Qt::Key_Control)
  {
    StructuralEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(false);
    }
  }
  else if(event->key() == Qt::Key_Control)
  {

    StructuralEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(false);
    }

    event->accept();
  }

  QGraphicsView::keyReleaseEvent(event);
}

void StructuralView::wheelEvent(QWheelEvent * event)
{
  if(event->modifiers() == Qt::ControlModifier)
  {

//    if (event->delta() > 0)
//        performZoomIn();
//    else
//        performZoomOut();


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

  unSelect();

  foreach (QString uid, entities.keys()) {
    QMap<QString,QString> settings;
    settings[PLG_SETTING_UNDO] = "1";
    settings[PLG_SETTING_UNDO_CHILDREN] = "0";
    settings[PLG_SETTING_NOTIFY] = "0";
    settings[PLG_SETTING_CODE] = StructuralUtil::CreateUid();

    remove(uid,settings);
  }



  entities.clear();
  importBases.clear();

  commnads.clear();

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

void StructuralView::cleanUndoRedo()
{
  emit redoStateChange(false);
 emit undoStateChange(false);
}

void StructuralView::updateLinkDialog(QMap<QString, QVector<QString> > conditions,
                                      QMap<QString, QVector<QString> > actions,
                                      QMap<QString, QVector<QString> > params)
{
  linkDialog->setData(conditions, actions, params);
}

//void StructuralView::setAction(QString action)
//{
//    this->action = action;
//}

//void StructuralView::setCondition(QString condition)
//{
//    this->condition = condition;
//}

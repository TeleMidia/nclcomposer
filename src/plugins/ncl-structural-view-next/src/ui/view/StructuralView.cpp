#include "StructuralView.h"

#include <assert.h>
#include <QDebug>

#include <QUuid>
#include <QFileDialog>
#include "StructuralUtil.h"

StructuralView::StructuralView(QWidget* parent)
    : QGraphicsView(parent)
{
  createObjects();
  createConnection();

  _zoomStep = 0;

  linking = false;

  _mode = Structural::Pointing;

  _selected = "";

  _clipboard = NULL;

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
}

StructuralView::~StructuralView()
{
  // TODO
}

StructuralMode StructuralView::getMode() const
{
  return _mode;
}

void StructuralView::setMode(StructuralMode mode)
{
  _mode = mode;
}

void StructuralView::createObjects()
{
  // Creating menu...
  _menu = new StructuralMenu(this);

  // Creating scene...
  _scene = new StructuralScene(this);
  _scene->setMenu(_menu);

  // Creating minimap...
  _minimap = new StructuralMinimap(this);
  _minimap->init(this);
  _minimap->setMinimumSize(STR_DEFAULT_MINIMAP_W, STR_DEFAULT_MINIMAP_H);
  _minimap->setMaximumSize(STR_DEFAULT_MINIMAP_W * 2, STR_DEFAULT_MINIMAP_H * 2);

  // Creating dialogs
  _linkDialog = new StructuralLinkDialog(this);

  // Setting...
  setScene(_scene);
  centerOn(STR_DEFAULT_SCENE_W/2, STR_DEFAULT_SCENE_H/2);

  resize(STR_DEFAULT_SCENE_W, STR_DEFAULT_SCENE_H);
}

void StructuralView::setMiniMapVisible(bool enable)
{
  if (enable)
    _minimap->show();
  else
    _minimap->hide();
}

void StructuralView::switchMinimapVis()
{
  setMiniMapVisible(!_minimap->isVisible());
}

void StructuralView::resizeEvent(QResizeEvent *event)
{
  _minimap->move(event->size().width() - _minimap->width(),
                event->size().height() - _minimap->height());

  QGraphicsView::resizeEvent(event);
}


void StructuralView::createConnection()
{
  connect(this, SIGNAL(switchedUndo(bool)), _menu, SLOT(switchedUndo(bool)));
  connect(this, SIGNAL(switchedRedo(bool)), _menu, SLOT(switchedRedo(bool)));
  connect(this, SIGNAL(switchedCut(bool)), _menu, SLOT(switchedCut(bool)));
  connect(this, SIGNAL(switchedCopy(bool)), _menu, SLOT(switchedCopy(bool)));
  connect(this, SIGNAL(switchedPaste(bool)), _menu, SLOT(switchedPaste(bool)));
  connect(this, SIGNAL(switchedBody(bool)), _menu, SLOT(switchedBody(bool)));

  connect(_menu, SIGNAL(performedHelp()), SLOT(performHelp()));
  connect(_menu, SIGNAL(performedUndo()), SLOT(performUndo()));
  connect(_menu, SIGNAL(performedRedo()), SLOT(performRedo()));
  connect(_menu, SIGNAL(performedCut()), SLOT(performCut()));
  connect(_menu, SIGNAL(performedCopy()), SLOT(performCopy()));
  connect(_menu, SIGNAL(performedPaste()), SLOT(paste()));
  connect(_menu, SIGNAL(performedDelete()), SLOT(performDelete()));
  connect(_menu, SIGNAL(performedSnapshot()), SLOT(performSnapshot()));
  connect(_menu, SIGNAL(performedMedia()), SLOT(performMedia()));
  connect(_menu, SIGNAL(performedContext()), SLOT(performContext()));
  connect(_menu, SIGNAL(performedSwitch()), SLOT(performSwitch()));
  connect(_menu, SIGNAL(performedBody()), SLOT(performBody()));
  connect(_menu, SIGNAL(performedArea()), SLOT(performArea()));
  connect(_menu, SIGNAL(performedProperty()), SLOT(performProperty()));
  connect(_menu, SIGNAL(performedPort()), SLOT(performPort()));
  connect(_menu, SIGNAL(performedSwitchPort()), SLOT(performSwitchPort()));

#ifdef WITH_GRAPHVIZ
  connect(_menu, SIGNAL(performedAutoAdjust()), SLOT(performAutoAdjust()));
#endif

  connect(_menu, SIGNAL(performedProperties()), SLOT(performProperties()));

  connect(_menu, SIGNAL(performedInsert(StructuralType,QMap<QString,QString>)), SLOT(performInsert(StructuralType,QMap<QString,QString>)));
}

bool StructuralView::hasEntity(QString uid)
{
  return _entities.contains(uid);
}

StructuralEntity* StructuralView::getEntity(const QString &uid)
{
  return _entities.value(uid, NULL);
}

QMap<QString, StructuralEntity*> StructuralView::getEntities()
{
  return _entities;
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
    settings[STR_SETTING_UNDO] = "0";
    settings[STR_SETTING_NOTIFY] = "0";
    settings[STR_SETTING_CODE] = StructuralUtil::createUid();

    insert(properties.value(STR_PROPERTY_ENTITY_UID), "", properties, settings);

    for (unsigned int i = 0; i < list.length(); i++)
    {
      if (list.item(i).isElement())
      {
        QDomElement e = list.item(i).toElement();

        if (e.nodeName() == "entity")
          read(e, noParentElement);
      }
    }

      settings.insert(STR_SETTING_NOTIFY,"1");
      select(properties.value(STR_PROPERTY_ENTITY_UID), settings);

      centerOn(_entities.value(properties.value(STR_PROPERTY_ENTITY_UID)));
  }
}

void StructuralView::performZoomIn()
{
  if (_zoomStep > 0) {
    _zoomStep--;

    resetMatrix();
    scale(1 - _zoomStep*0.05, 1 - _zoomStep*0.05);

    emit switchedZoomOut(true);
  }

  if (_zoomStep <= 0)
    emit switchedZoomIn(false);
}

void StructuralView::performZoomOut()
{
  if (_zoomStep*0.05 < 0.9) {
    _zoomStep++;

    resetMatrix();
    scale(1 - _zoomStep*0.05, 1 - _zoomStep*0.05);

    emit switchedZoomIn(true);
  }

  if (_zoomStep*0.05 >= 0.9)
    emit switchedZoomOut(false);
}

void StructuralView::performPointer()
{
  setMode(Structural::Pointing);
}

void StructuralView::performLink()
{
  setMode(Structural::Linking);
}

StructuralScene* StructuralView::getScene()
{
  return _scene;
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
  settings[STR_SETTING_UNDO] = "0";
  settings[STR_SETTING_NOTIFY] = "0";
  settings[STR_SETTING_CODE] = StructuralUtil::createUid();


  insert(properties.value(STR_PROPERTY_ENTITY_UID), parent.attributeNode("uid").nodeValue(), properties, settings);

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

QString StructuralView::serialize()
{
  QDomDocument* document = new QDomDocument();
  QDomElement root = document->createElement("structural");

  foreach(StructuralEntity* entity, _entities.values())
  {
    if (entity->getStructuralParent() == NULL)
    {
      exportDataFromEntity(entity, document, root);
    }
  }

  document->appendChild(root);

  return document->toString(4);
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
  if (!_entities.contains(uid))
  {
    StructuralEntity* entity; StructuralType type = StructuralUtil::translateStringToType(properties[STR_PROPERTY_ENTITY_TYPE]);

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
        if (STR_DEFAULT_WITH_BODY  && getBody() == NULL) {

          entity = new StructuralComposition();
          entity->setStructuralType(Structural::Body);
          entity->setWidth(STR_DEFAULT_BODY_W);
          entity->setHeight(STR_DEFAULT_BODY_H);

          emit switchedBody(false);
        }

        break;
      }

      case Structural::Link:
      {
        entity = new StructuralLink();
        connect((StructuralLink*) entity, SIGNAL(performedEdit(StructuralLink*)),SLOT(showEditLinkDialog(StructuralLink*)));
        break;
      }

      case Structural::Bind:
      {
        entity = new StructuralBind();

        StructuralBind* bind = (StructuralBind*) entity;
        if (_entities.contains(properties.value(STR_PROPERTY_EDGE_TAIL)))
          bind->setTail(_entities.value(properties.value(STR_PROPERTY_EDGE_TAIL)));
        if (_entities.contains(properties.value(STR_PROPERTY_EDGE_HEAD)))
          bind->setHead(_entities.value(properties.value(STR_PROPERTY_EDGE_HEAD)));

        bind->setRole(StructuralUtil::translateStringToRole(properties.value(STR_PROPERTY_BIND_ROLE)));

        if (properties.value(STR_PROPERTY_EDGE_ANGLE).isEmpty())
        {
          adjustAngles(bind);
          bind->setStructuralProperty(STR_PROPERTY_EDGE_ANGLE, QString::number(bind->getAngle()));
        }
        else
        {
          bind->setAngle(properties.value(STR_PROPERTY_EDGE_ANGLE).toDouble());
        }

        connect(bind, SIGNAL(performedEdit(StructuralBind*)),SLOT(showEditBindDialog(StructuralBind*)));

        break;
      }

      case Structural::Reference:
      {
        if (_entities.contains(properties.value(STR_PROPERTY_EDGE_TAIL)) &&
            _entities.contains(properties.value(STR_PROPERTY_EDGE_HEAD)))
        {
            entity = new StructuralEdge();
            entity->setStructuralType(Structural::Reference);
            ((StructuralEdge*) entity)->setTail(_entities.value(properties.value(STR_PROPERTY_EDGE_TAIL)));
            ((StructuralEdge*) entity)->setHead(_entities.value(properties.value(STR_PROPERTY_EDGE_HEAD)));
        }

        break;
      }

      case Structural::Mapping:
      {
        entity = new StructuralEdge();
        entity->setStructuralType(Structural::Mapping);
        if (_entities.contains(properties.value(STR_PROPERTY_EDGE_TAIL)))
          ((StructuralEdge*) entity)->setTail(_entities.value(properties.value(STR_PROPERTY_EDGE_TAIL)));

        if (_entities.contains(properties.value(STR_PROPERTY_EDGE_HEAD)))
          ((StructuralEdge*) entity)->setHead(_entities.value(properties.value(STR_PROPERTY_EDGE_HEAD)));

        break;
      }

      default:
      {
        entity = NULL;
        break;
      }
    }

    bool collision = false;

    if (entity != NULL)
    {
      if (_entities.contains(parent))
      {

        StructuralEntity* p = _entities.value(parent);

        bool uncollapseAndThenCollapse = false;

        if (!p->isUncollapsed()){
          if (p->getStructuralType() == Structural::Body ||
              p->getStructuralType() == Structural::Switch ||
              p->getStructuralType() == Structural::Context)
          {
            uncollapseAndThenCollapse = true;

            ((StructuralComposition*) p)->collapse();
          }
        }

        entity->setStructuralParent(_entities.value(parent));

        if (uncollapseAndThenCollapse)
          ((StructuralComposition*) p)->collapse();


        if (entity->getStructuralCategory() != Structural::Edge){
          if (!properties.value(STR_PROPERTY_ENTITY_TOP).isEmpty() && !properties.value(STR_PROPERTY_ENTITY_LEFT).isEmpty())
          {
            collision = true;
          }
          else
          {
            entity->setTop(_entities[parent]->getHeight()/2 - entity->getHeight()/2);
            entity->setLeft(_entities[parent]->getWidth()/2 - entity->getWidth()/2);

            collision = false;
          }
        }
      }
      else
      {
        _scene->addItem(entity);

        if (entity->getStructuralCategory() != Structural::Edge){
          entity->setTop(_scene->sceneRect().height()/2 - entity->getHeight()/2);
          entity->setLeft(_scene->sceneRect().width()/2 - entity->getWidth()/2);
        }
      }

      entity->setStructuralUid(uid);
      entity->setStructuralProperties(properties);

      entity->setMenu(_menu);

      entity->setSelected(false);

      _entities[uid] = entity;
      if (entity->getStructuralId().isEmpty())
        updateEntityWithUniqueNstId(entity);

      entity->adjust(collision);

      connect(entity, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(removed(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(entity, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(entity, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));
      connect(entity, SIGNAL(move(QString,QString)), SLOT(move(QString,QString)));

      if (_entities.size() <= 1){
        centerOn(entity);
      }

      if (settings[STR_SETTING_UNDO] == "1")
      {
        Insert* command = new Insert(uid, parent, entity->getStructuralProperties(), settings);

        connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
        connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));


        command->setText(settings[STR_SETTING_CODE]);

        _commnads.push(command);

        emit switchedUndo(true);
      }

      if (entity->getStructuralType() ==  Structural::Port)
        drawPortReference(entity);

      if (settings[STR_SETTING_NOTIFY] == "1")
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

  foreach (StructuralEntity *e, _entities)
  {
    if (e->getStructuralType() == Structural::Bind)
    {
      StructuralBind *ea = (StructuralBind*) e;

      if ((edge->getTail() == ea->getTail() || edge->getTail() == ea->getHead()) &&
          (edge->getHead() == ea->getHead() || edge->getHead() == ea->getTail()) &&
          edge != ea)
      {
        HAS = true;

        int A = ea->getAngle();

        if (!StructuralUtil::isConditionRole(ea->getRole()))
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

  if (!StructuralUtil::isConditionRole(edge->getRole()))
    edge->setAngle(-ANGLE);
  else
    edge->setAngle(ANGLE);

}

QVector<StructuralEntity*> StructuralView::getRoots()
{
  QVector<StructuralEntity*> r;

  foreach (StructuralEntity* e, _entities) {
    if (e->getStructuralParent() == NULL)
      r.append(e);
  }

  return r;
}

void StructuralView::remove(QString uid, QMap<QString, QString> settings)
{

  if (_entities.contains(uid))
  {
    if (settings[STR_SETTING_UNDO] == "1")
    {
      Remove* command;

      if (_entities[uid]->getStructuralParent() != NULL)
        command = new Remove(_entities[uid]->getStructuralUid(), _entities[uid]->getStructuralParent()->getStructuralUid(), _entities[uid]->getStructuralProperties(), settings);
      else
        command = new Remove(_entities[uid]->getStructuralUid(), "", _entities[uid]->getStructuralProperties(), settings);

      command->setText(settings[STR_SETTING_CODE]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      emit switchedUndo(true);

      _commnads.push(command); return;
    }

    StructuralEntity* entity = _entities.value(uid);
    StructuralEntity* parent = entity->getStructuralParent();

    while(!entity->getStructuralEntities().isEmpty())
    {
      StructuralEntity* e = entity->getStructuralEntities().first();

      if (settings.value(STR_SETTING_UNDO_TRACE) != "0")
        settings[STR_SETTING_UNDO] = "1";

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

          if (e->getTail() == entity || e->getHead() == entity){

            if (e->getStructuralType() != Structural::Reference)
              remove(e->getStructuralUid(),
                   StructuralUtil::createSettings("1",
                                                  settings.value(STR_SETTING_NOTIFY),
                                                  settings.value(STR_SETTING_CODE)));
            else{
              StructuralEntity* cport = _entities.value(c->getStructuralProperty(STR_PROPERTY_EDGE_TAIL));

              QMap<QString, QString> prev = cport->getStructuralProperties();
              QMap<QString, QString> properties = cport->getStructuralProperties();;

              properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = "";
              properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = "";
              properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
              properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";

              change(cport->getStructuralUid(),properties, prev,
                     StructuralUtil::createSettings("1",
                                                    settings.value(STR_SETTING_NOTIFY),
                                                    settings.value(STR_SETTING_CODE)));
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
      _scene->removeItem(entity);

    if (entity->getStructuralType() == Structural::Link){
      foreach (QString name, entity->getStructuralProperties().keys()) {
        if (name.contains(STR_PROPERTY_LINKPARAM_NAME)){
          QString lpUid = name.right(name.length() - name.lastIndexOf(':') - 1);

          if (settings[STR_SETTING_NOTIFY] == "1")
          {
            emit removed(lpUid, settings);
          }
        }
      }
    }

    if (entity->getStructuralType() == Structural::Bind){
      foreach (QString name, entity->getStructuralProperties().keys()) {
        if (name.contains(STR_PROPERTY_BINDPARAM_NAME)){
          QString bpUid = name.right(name.length() - name.lastIndexOf(':') - 1);

          if (settings[STR_SETTING_NOTIFY] == "1")
          {
            emit removed(bpUid, settings);
          }
        }
      }
    }

    _entities.remove(uid);

    if (uid == _selected){
      _selected = "";
      emit selectChange("");
    }

    if (entity->getStructuralType() == Structural::Body)
      emit switchedBody(true);

    delete entity; entity = NULL;


    if (settings[STR_SETTING_NOTIFY] == "1")
    {
      emit removed(uid, settings);
    }
  }
}

void StructuralView:: change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  if (_entities.contains(uid))
  {
    if (settings[STR_SETTING_UNDO] == "1")
    {
      /*
      QMap<QString,QString> tmp = previous;

      if (entities.value(uid)->getStructuralType() == Structural::Port ||
          entities.value(uid)->getStructuralType() == Structural::Mapping ||
          entities.value(uid)->getStructuralType() == Structural::Bind){
      if (!properties.contains(STR_PROPERTY_REFERENCE_COMPONENT_ID))
        tmp.remove(STR_PROPERTY_REFERENCE_COMPONENT_ID);

      if (!properties.contains(STR_PROPERTY_REFERENCE_COMPONENT_UID))
        tmp.remove(STR_PROPERTY_REFERENCE_COMPONENT_UID);

      if (!properties.contains(STR_PROPERTY_REFERENCE_INTERFACE_ID))
        tmp.remove(STR_PROPERTY_REFERENCE_INTERFACE_UID);

      if (!properties.contains(STR_PROPERTY_REFERENCE_COMPONENT_ID))
        tmp.remove(STR_PROPERTY_REFERENCE_COMPONENT_ID);
      }

      if (entities.value(uid)->getStructuralType() == Structural::Link ||
          entities.value(uid)->getStructuralType() == Structural::Bind){
        foreach (QString key, previous.keys()) {
          if (key.contains(STR_PROPERTY_LINKPARAM_NAME) ||
              key.contains(STR_PROPERTY_LINKPARAM_VALUE) ||
              key.contains(STR_PROPERTY_BINDPARAM_NAME) ||
              key.contains(STR_PROPERTY_BINDPARAM_VALUE)) {
            if (!properties.contains(key))
              tmp.remove(key);
          }
        }
      }


      foreach (QString key, properties.keys()) {
        tmp[key] = properties.value(key);
      };

      */

      Change* command = new Change(uid, properties, previous, settings);

      command->setText(settings[STR_SETTING_CODE]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      emit switchedUndo(true);

      _commnads.push(command); return;
    }


    qDebug() << "====" << previous;

    qDebug() << "====" << properties;

     StructuralEntity* entity = _entities[uid];

     if ((!entity->isUncollapsed() && properties.value(STR_PROPERTY_ENTITY_UNCOLLAPSED) == "1") ||
         (entity->isUncollapsed() && properties.value(STR_PROPERTY_ENTITY_UNCOLLAPSED) == "0"))
       ((StructuralComposition*) entity)->collapse(false);

     entity->setStructuralProperties(properties);
     entity->adjust(true);

     if (entity->getStructuralType() ==  Structural::Bind) {
       StructuralBind* b = (StructuralBind*) entity;

       b->setRole(StructuralUtil::translateStringToRole(properties.value(STR_PROPERTY_BIND_ROLE)));

       bool toHide = true;

       if (_entities.contains(properties.value(STR_PROPERTY_EDGE_TAIL)))
         b->setTail(_entities.value(properties.value(STR_PROPERTY_EDGE_TAIL)));
       else
         b->setTail(NULL);

       if (_entities.contains(properties.value(STR_PROPERTY_EDGE_HEAD)))
         b->setHead(_entities.value(properties.value(STR_PROPERTY_EDGE_HEAD)));
       else
         b->setHead(NULL);

       if (b->getTail() != NULL && b->getHead() != NULL)
         toHide = false;
       else
         toHide = true;

       StructuralEntity* co = _entities.value(properties.value(STR_PROPERTY_REFERENCE_COMPONENT_UID), NULL);
       StructuralEntity* in = _entities.value(properties.value(STR_PROPERTY_REFERENCE_INTERFACE_UID), NULL);

       if (in != NULL) {
         if (co != NULL){
           toHide = true;

           foreach (StructuralEntity* e, co->getStructuralEntities()) {
             if (e == in){
               toHide = false;
               break;
             }
           }
         }else{
           toHide = true;
         }
       }

       b->setHidden(toHide);
     }

     if (entity->getStructuralType() ==  Structural::Mapping) {
       StructuralEdge* map = (StructuralEdge*) entity;

       bool toHide = true;

       if (_entities.contains(properties.value(STR_PROPERTY_EDGE_TAIL))){
         map->setTail(_entities.value(properties.value(STR_PROPERTY_EDGE_TAIL)));
       }else
         map->setTail(NULL);

       if (_entities.contains(properties.value(STR_PROPERTY_EDGE_HEAD))){
         map->setHead(_entities.value(properties.value(STR_PROPERTY_EDGE_HEAD)));
       }else
         map->setHead(NULL);

       if (map->getTail() != NULL && map->getHead() != NULL)
         toHide = false;
       else
         toHide = true;

       StructuralEntity* co = _entities.value(properties.value(STR_PROPERTY_REFERENCE_COMPONENT_UID), NULL);
       StructuralEntity* in = _entities.value(properties.value(STR_PROPERTY_REFERENCE_INTERFACE_UID), NULL);

       if (in != NULL) {
         if (co != NULL){
           toHide = true;

           foreach (StructuralEntity* e, co->getStructuralEntities()) {
             if (e == in){
               toHide = false;
               break;
             }
           }
         }else{
           toHide = true;
         }
       }

       map->setHidden(toHide);
     }

     if (entity != NULL && entity->getStructuralType() == Structural::Port)
       drawPortReference(entity);

    if (settings[STR_SETTING_NOTIFY] == "1")
    {
      emit changed(uid, properties, previous, settings);
    }

  }
}

void StructuralView::drawPortReference(StructuralEntity* entity)
{
  if (entity != NULL && entity->getStructuralType() == Structural::Port){
    QString uid = "";
    if (!entity->getStructuralProperty(STR_PROPERTY_REFERENCE_INTERFACE_UID).isEmpty())
      uid =  entity->getStructuralProperty(STR_PROPERTY_REFERENCE_INTERFACE_UID);

    else if (!entity->getStructuralProperty(STR_PROPERTY_REFERENCE_COMPONENT_UID).isEmpty())
      uid =  entity->getStructuralProperty(STR_PROPERTY_REFERENCE_COMPONENT_UID);

    foreach (StructuralEntity* e, _entities.values()) {
      if (e->getStructuralType() == Structural::Reference)
      {
        if (e->getStructuralProperty(STR_PROPERTY_EDGE_TAIL) == entity->getStructuralUid())
          remove(e->getStructuralUid(),StructuralUtil::createSettings(false,false));
      }
    }

    if (_entities.contains(uid)){
      QMap<QString, QString> properties;
      properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Reference);
      properties[STR_PROPERTY_EDGE_TAIL] = entity->getStructuralUid();
      properties[STR_PROPERTY_EDGE_HEAD] = uid;

      insert(StructuralUtil::createUid(),
             entity->getStructuralParent()->getStructuralUid(),
             properties,
             StructuralUtil::createSettings(false,false));
    }
  }
}

void StructuralView::unSelect()
{

  if (_entities.contains(_selected))
  {
    _entities.value(_selected)->setSelected(false);

}

  _selected = "";

  _scene->update();

  emit switchedCut(false);
  emit switchedCopy(false);
  emit switchedPaste(false);

  emit selectChange("");
  emit selected("", QMap<QString, QString>());
}

StructuralEntity* StructuralView::getBody()
 {
   foreach (StructuralEntity* e, _entities) {
    if (e->getStructuralType() == Structural::Body)
      return e;
   }

   return NULL;
 }

void StructuralView::clearErrors()
{
  foreach (StructuralEntity* e, _entities.values()) {
    e->setError("");
    e->setWarning("");
    e->setInfo("");
  }
}

void StructuralView::markError(const QString &uid, const QString &msg)
{
  if(_entities.contains(uid))
  {
    StructuralEntity* entity = _entities.value(uid);
    entity->setError(msg);
    entity->adjust(true);
  }
}

void StructuralView::select(QString uid, QMap<QString, QString> settings)
{
  if (_entities.contains(uid))
  {
      if (uid == _selected)
        return;

      StructuralEntity* selected_e = NULL;

      if (!_selected.isEmpty())
        selected_e = _entities.value(_selected,NULL);

      if (selected_e != NULL)
      {
        selected_e->setSelected(false);
        selected_e->adjust(true);
      }


      StructuralEntity* entity = _entities[uid];
      entity->setSelected(true);

      _selected = uid;

      if (_entities.value(_selected)->getStructuralType() == Structural::Body)
        centerOn(_entities.value(_selected));

      emit selectChange(uid);

//      if (settings[STR_SETTING_NOTIFY] == "1")
//      {
        emit selected(uid, settings);
//      }

      emit switchedCut(true);
      emit switchedCopy(true);

      if (!clip_cut.isEmpty() || !clip_copy.isEmpty()){
        StructuralEntity* e = NULL;

        if (!clip_cut.isEmpty())
          e = e_clip;

        if (!clip_copy.isEmpty())
          e = _entities.value(clip_copy);

        if (StructuralUtil::validateKinship(e->getStructuralType(), _entities.value(_selected)->getStructuralType()))
          emit switchedPaste(true);
        else
          emit switchedPaste(false);
      }

          _scene->update();
  }
}

void StructuralView::move(QString uid, QString parent)
{
  if (_entities.contains(uid) &&
      _entities.contains(parent)){

      StructuralEntity* e = _entities.value(uid);
      StructuralEntity* p = _entities.value(parent);

    if (!StructuralUtil::validateKinship(
          e->getStructuralType(),
          p->getStructuralType()))
      return;

    if (e->getStructuralParent()->getStructuralUid() == p->getStructuralUid())
      return;

    unSelect();

    StructuralEntity* moveEntity = clone(_entities.value(uid), NULL);

    QString CODE = StructuralUtil::createUid();

    QMap<QString, QString> settings;
    settings[STR_SETTING_UNDO] = "1";
    settings[STR_SETTING_NOTIFY] = "1";
    settings[STR_SETTING_CODE] = CODE;

    remove(uid, settings);

    paste(moveEntity, _entities.value(parent), CODE, true);

    StructuralEntity *entity;
    foreach(entity, _entities.values())
    {
      entity->setDraggable(false);
    }
  }

}

void StructuralView::create(StructuralType type)
{
  QMap<QString,QString> properties;
  properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(type);

  create(type, properties, StructuralUtil::createSettings());
}

void StructuralView::create(StructuralType type, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  QString uid = StructuralUtil::createUid();
  QString parent = "";

  if (_entities.contains(_selected))
    parent = _selected;

  if (!properties.contains(STR_PROPERTY_ENTITY_TYPE))
    properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(type);

  if (!settings.contains(STR_SETTING_UNDO))
    settings[STR_SETTING_UNDO] = STR_VALUE_TRUE;

  if (!settings.contains(STR_SETTING_NOTIFY))
    settings[STR_SETTING_NOTIFY] = STR_VALUE_TRUE;

  if (!settings.contains(STR_SETTING_CODE))
    settings[STR_SETTING_CODE] = StructuralUtil::createUid();

  insert(uid, parent, properties, settings);
}


void StructuralView::performHelp()
{
  // TODO
}

void StructuralView::performUndo()
{
  if (_commnads.canUndo()) {
    QString code = _commnads.undoText();

    while(code == _commnads.undoText())
      _commnads.undo();

    emit switchedRedo(true);
  }

  if (!_commnads.canUndo())
    emit switchedUndo(false);
}

void StructuralView::performRedo()
{
  if (_commnads.canRedo()) {
    QString code = _commnads.redoText();

    while(code == _commnads.redoText())
      _commnads.redo();

    emit switchedUndo(true);
  }

  if (!_commnads.canRedo())
    emit switchedRedo(false);
}

void StructuralView::performCut()
{
  performCopy();

  StructuralEntity* entity = _entities.value(_selected);

  if (entity != NULL)
    if (_clipboard->getStructuralUid() == entity->getStructuralUid())
      remove(_selected, StructuralUtil::createSettings());
}

void StructuralView::performCopy()
{
  if (_entities.contains(_selected)) {
    if (_clipboard != NULL) {
      delete (_clipboard);

      _clipboardReferences.clear();
    }

    StructuralEntity* entity = _entities.value(_selected);

    if (entity != NULL)
      if (entity->getStructuralCategory() != Structural::Edge &&
          entity->getStructuralType() != Structural::Body) {
        _clipboard = clone(entity);

        emit switchedPaste(true);
      }
  }
}

void StructuralView::performPaste()
{
  if (_clipboard != NULL) {
    StructuralEntity* parent = NULL;

    if (_entities.contains(_selected))
      parent = _entities.value(_selected, NULL);

    if (parent != NULL)
      if (_clipboard->getStructuralUid() != parent->getStructuralUid())
        if (StructuralUtil::validateKinship(_clipboard->getStructuralType(), parent->getStructuralType()) &&
            !isChild(_clipboard, parent)) {
          paste(_clipboard, parent);
        }
  }
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
      properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Link);

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
            properties[STR_PROPERTY_ENTITY_TOP] = QString::number(pt_b.y() + (pt_a.y() - pt_b.y())/2);
        else
            properties[STR_PROPERTY_ENTITY_TOP] = QString::number(pt_a.y() + (pt_b.y() - pt_a.y())/2);


        if (pt_a.x() > pt_b.x())
            properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(pt_b.x() + (pt_a.x() - pt_b.x())/2);
        else
            properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(pt_a.x() + (pt_b.x() - pt_a.x())/2);

        emit requestLinkDialogUpdate();

        setMode(Structural::Pointing);
        emit switchedPointer(true);

        _linkDialog->init();

        if (_linkDialog->exec()){

          properties.insert(STR_PROPERTY_REFERENCE_XCONNECTOR_ID, _linkDialog->getCurrentConnector());


          QMap<QString, QString> p = _linkDialog->getLinkParams();

          foreach (QString name, p.keys()) {
            if (!p.value(name).isEmpty()){
              QString uid = StructuralUtil::createUid();

              properties.insert(QString(STR_PROPERTY_LINKPARAM_NAME)+":"+uid, name);
              properties.insert(QString(STR_PROPERTY_LINKPARAM_VALUE)+":"+uid, p.value(name));
            }
          }

          QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

          insert(uid, parent->getStructuralUid(), properties, settings);

          if (_entities.contains(uid))
          {
            StructuralRole con = StructuralUtil::translateStringToRole(_linkDialog->form.cbCondition->currentText());
            StructuralRole act = StructuralUtil::translateStringToRole(_linkDialog->form.cbAction->currentText());

            createBind(a,_entities.value(uid),con,settings.value(STR_SETTING_CODE));
            createBind(_entities.value(uid),b,act,settings.value(STR_SETTING_CODE));
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
      properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Bind);
      properties[STR_PROPERTY_EDGE_TAIL] = a->getStructuralUid();
      properties[STR_PROPERTY_EDGE_HEAD] = b->getStructuralUid();

      properties[STR_PROPERTY_BIND_ROLE] = StructuralUtil::translateRoleToString(type);
      properties[STR_PROPERTY_ENTITY_ID] = StructuralUtil::translateRoleToString(type);

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

          setMode(Structural::Pointing);
          emit switchedPointer(true);

          _linkDialog->init(e_link->getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID),"","",
                           StructuralLinkDialog::CreateAction);

          if (_linkDialog->exec()){
            QString role = _linkDialog->form.cbAction->currentText();

            properties[STR_PROPERTY_BIND_ROLE] = role;
            properties[STR_PROPERTY_ENTITY_ID] = role;
          }else{
            return;
          }
        }

        QMap<QString, QString> p = _linkDialog->getActionParams();

        foreach (QString name, p.keys()) {
          if (!p.value(name).isEmpty()){
            QString uid = StructuralUtil::createUid();

            properties.insert(QString(STR_PROPERTY_BINDPARAM_NAME)+":"+uid, name);
            properties.insert(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+uid, p.value(name));
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

          setMode(Structural::Pointing);
          emit switchedPointer(true);

          _linkDialog->init(e_link->getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID),"","",
                           StructuralLinkDialog::CreateCondition);

          if (_linkDialog->exec()){
            QString role = _linkDialog->form.cbCondition->currentText();

            properties[STR_PROPERTY_BIND_ROLE] = role;
            properties[STR_PROPERTY_ENTITY_ID] = role;
          }else{
            return;
          }
        }

        QMap<QString, QString> p = _linkDialog->getConditionParams();

        foreach (QString name, p.keys()) {
          if (!p.value(name).isEmpty()){
            QString uid = StructuralUtil::createUid();

            properties.insert(QString(STR_PROPERTY_BINDPARAM_NAME)+":"+uid, name);
            properties.insert(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+uid, p.value(name));
          }
        }
      }

      if (e_link != NULL && e_nolink != NULL && pe_nolink != NULL){
        properties[STR_PROPERTY_REFERENCE_LINK_UID] = e_link->getStructuralUid();

        if (e_nolink->getStructuralCategory() == Structural::Interface)
        {
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = e_nolink->getStructuralId();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = e_nolink->getStructuralUid();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = pe_nolink->getStructuralId();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = pe_nolink->getStructuralUid();
        }
        else
        {
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = e_nolink->getStructuralId();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = e_nolink->getStructuralUid();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
        }
      }

      QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

      if (!code.isEmpty())
          settings[STR_SETTING_CODE] = code;

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
            properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = b->getStructuralId();
            properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = b->getStructuralUid();
            properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = pb->getStructuralId();
            properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = pb->getStructuralUid();
          }
          else
          {
            properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = b->getStructuralId();
            properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = b->getStructuralUid();
            properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
            properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
          }

          QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

          setMode(Structural::Pointing);
          emit switchedPointer(true);

          if (a->getStructuralType() == Structural::Port)
            change(a->getStructuralUid(),properties, prev, settings);

          else  if (a->getStructuralType() == Structural::SwitchPort){
            properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Mapping);
            properties[STR_PROPERTY_EDGE_TAIL] = a->getStructuralUid();
            properties[STR_PROPERTY_EDGE_HEAD] = b->getStructuralUid();

            insert(StructuralUtil::createUid(),
                   pa->getStructuralUid(),properties,settings);
          }
      }
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
    QString prefix = StructuralUtil::getPrefix(type);


    bool match = false;

    while(1)
    {
      match = false;

      foreach(StructuralEntity* e, _entities.values())
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
  if (_entities.contains(_selected))
    remove(_selected, StructuralUtil::createSettings());
}

void StructuralView::performSnapshot()
{
  QString location =
      QFileDialog::getSaveFileName(NULL,
                                   tr("Save snapshot..."),
                                   QDir::homePath(),
                                   tr("Images (*.png)"));

  if (!location.isEmpty()) {
    qreal top = 0;
    qreal left = 0;
    qreal width = _scene->width();
    qreal height = _scene->height();

    StructuralEntity* entity = getBody();

    if (entity != NULL) {
      if (entity->getTop() > top)
        top = entity->getTop();

      if (entity->getLeft() > left)
        left = entity->getLeft();

      if (entity->getWidth() < width)
        width = entity->getWidth();

      if (entity->getHeight() < height)
        height = entity->getHeight();
    }

    StructuralEntity* selected = NULL;

    if (_entities.contains(_selected))
      selected = _entities.value(_selected);

    if (selected != NULL)
      selected->setSelected(false);

    QImage image = QImage(width, height, QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&image);
    _scene->render(&painter, QRect(), QRect(left-25, top-25, width+50, height+50));
    painter.end();

    if (!location.endsWith(".png"))
      location += ".png";

    image.save(location, "png");

    if (selected != NULL)
      selected->setSelected(true);
  }
}

void StructuralView::performMinimap()
{
  if (!_minimap->isVisible())
    _minimap->show();
  else
    _minimap->hide();
}

void StructuralView::performBody()
{
  create(Structural::Body);
}

void StructuralView::performContext()
{
  create(Structural::Context);
}

void StructuralView::performSwitch()
{
  create(Structural::Switch);
}

void StructuralView::performMedia()
{
  create(Structural::Media);
}

void StructuralView::performPort()
{
  create(Structural::Port);
}

void StructuralView::performArea()
{
  create(Structural::Area);
}

void StructuralView::performProperty()
{
  create(Structural::Property);
}

void StructuralView::performSwitchPort()
{
  create(Structural::SwitchPort);
}

#ifdef WITH_GRAPHVIZ
void StructuralView::performAutoAdjust()
{
  autoadjust();
}
#endif

void StructuralView::performProperties()
{
  // TODO
}

void StructuralView::performInsert(StructuralType type, QMap<QString, QString> properties)
{
  create(type, properties, StructuralUtil::createSettings());
}

void StructuralView::mouseMoveEvent(QMouseEvent* event)
{
  if (linking)
  {
    if (lastLinkMouseOver != NULL)
    {
      lastLinkMouseOver->setHoverable(false);

      // \fixme This is not the best place to control this!!
      if(
         lastLinkMouseOver->getStructuralType() == Structural::Context ||
         lastLinkMouseOver->getStructuralType() == Structural::Switch)
      {
        lastLinkMouseOver->setHoverable(false);
      }

      lastLinkMouseOver = NULL;
    }

    QList<QGraphicsItem *> itemsa = _scene->items(link->getLine().p1());

    if (itemsa.count() && itemsa.first() == link)
    {
      itemsa.removeFirst();
    }

    if (itemsa.count())
    {
      StructuralEntity* entitya = (StructuralEntity*) itemsa.first();
      entitya->setHoverable(true);
    }

    QList<QGraphicsItem*> itemsb = _scene->items(link->getLine().p2());

    if (itemsb.count() && itemsb.first() == link)
    {
      itemsb.removeFirst();
    }

    if (itemsb.count())
    {
      StructuralEntity* entityb = (StructuralEntity*) itemsb.first();
      entityb->setHoverable(true);

      // \fixme This is not the best place to control this!!
      if(
         entityb->getStructuralType() == Structural::Context ||
         entityb->getStructuralType() == Structural::Switch)
      {
        entityb->setHoverable(true);
      }

      lastLinkMouseOver = entityb;
    }

    link->setLine(QLineF(link->getLine().p1(), mapToScene(event->pos())));
    _scene->update();
  }

  QGraphicsView::mouseMoveEvent(event);
}

void StructuralView::mousePressEvent(QMouseEvent* event)
{
  if (_mode == Structural::Linking)
  {
    if (link != NULL)
    {
      delete (link);
      link = NULL;
    }

    link = new StructuralLinkTool();
    _scene->addItem(link);
    link->setLine(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
    link->adjust(true);

    linking = true;
  }
  else
  {
    QGraphicsView::mousePressEvent(event);

    if (!event->isAccepted()){

      if (event->button() == Qt::LeftButton)
      {
        StructuralEntity* selected_e = NULL;

        if (!_selected.isEmpty())
          selected_e = _entities[_selected];

        if (selected_e != NULL)
        {
          selected_e->setSelected(false);
          selected_e->adjust(true);
        }

        _selected = "";

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
    QList<QGraphicsItem *> itemsa = _scene->items(link->getLine().p1());

    if (itemsa.count() && itemsa.first() == link)
      itemsa.removeFirst();

    QList<QGraphicsItem*> itemsb = _scene->items(link->getLine().p2());

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

      entitya->setHoverable(false);
      entityb->setHoverable(false);

      // \fixme This is not the best place to control this!!
      if(
         entityb->getStructuralType() == Structural::Context ||
         entityb->getStructuralType() == Structural::Switch)
      {
        entityb->setHoverable(false);
      }
    }

    if (link != NULL)
    {
      _scene->removeItem(link);
      _scene->update();

      delete link;
      link = NULL;
    }

    linking = false;
  }

  StructuralEntity *entity;
  foreach(entity, _entities.values())
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
//  else if (event->key() == Qt::Key_I){
//    adjustAllWithGraphiviz();

//    event->accept();
//  }
  // SHIFT - Enabling liking
  else if (event->key() == Qt::Key_Shift)
  {
    StructuralEntity* selected_e = NULL;

    if (!_selected.isEmpty())
      selected_e = _entities[_selected];

    if (selected_e != NULL)
    {
      selected_e->setSelected(false);
      selected_e->adjust(true);
    }

    _selected =  "";

    setMode(Structural::Linking);

    emit switchedLink(true);

    event->accept();
  }
  else if(event->key() == Qt::Key_Control)
  {

    StructuralEntity *entity;
    foreach(entity, _entities.values())
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
    setMode(Structural::Pointing);

    emit switchedPointer(true);
  }
  else if(event->key() == Qt::Key_Control)
  {
    StructuralEntity *entity;
    foreach(entity, _entities.values())
    {
      entity->setDraggable(false);
    }
  }
  else if(event->key() == Qt::Key_Control)
  {

    StructuralEntity *entity;
    foreach(entity, _entities.values())
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
  foreach(entity, _entities.values())
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

  foreach (QString uid, _entities.keys()) {
    QMap<QString,QString> settings;
    settings[STR_SETTING_UNDO] = "1";
    settings[STR_SETTING_UNDO_TRACE] = "0";
    settings[STR_SETTING_NOTIFY] = "0";
    settings[STR_SETTING_CODE] = StructuralUtil::createUid();

    remove(uid,settings);
  }



  _entities.clear();
  importBases.clear();

  _commnads.clear();

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
  emit switchedRedo(false);
 emit switchedUndo(false);
}

void StructuralView::updateLinkDialog(QMap<QString, QVector<QString> > conditions,
                                      QMap<QString, QVector<QString> > actions,
                                      QMap<QString, QVector<QString> > params)
{
  _linkDialog->setData(conditions, actions, params);
}

void StructuralView::showEditLinkDialog(StructuralLink* entity)
{
  emit requestLinkDialogUpdate();

  _linkDialog->init(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID),"","",
                   StructuralLinkDialog::EditLink);

  QMap<QString, QString> pLink;
  foreach (QString name, entity->getStructuralProperties().keys()) {
    if (name.contains(STR_PROPERTY_LINKPARAM_NAME)){
      QString lpUid = name.right(name.length() - name.lastIndexOf(':') - 1);
      QString lpName = entity->getStructuralProperties().value(name);
      QString lpValue = entity->getStructuralProperties().value(QString(STR_PROPERTY_LINKPARAM_VALUE)+":"+lpUid);

      pLink.insert(lpName, lpValue);
    }
  }

  _linkDialog->updateCurrentLinkParam(pLink);

  if (_linkDialog->exec()){

    QMap<QString, QString> prev = entity->getStructuralProperties();
    QMap<QString, QString> properties = entity->getStructuralProperties();

    properties.insert(STR_PROPERTY_REFERENCE_XCONNECTOR_ID, _linkDialog->getCurrentConnector());

    QMap<QString, QString> p = _linkDialog->getLinkParams();

    foreach (QString name, p.keys()) {
      if (properties.key(name).contains(STR_PROPERTY_LINKPARAM_NAME)) {
        QString key = properties.key(name);
        QString uid = key.right(key.length() - key.lastIndexOf(':') - 1);

        if (!p.value(name).isEmpty()){
          properties.insert(QString(STR_PROPERTY_LINKPARAM_NAME)+":"+uid, name);
          properties.insert(QString(STR_PROPERTY_LINKPARAM_VALUE)+":"+uid, p.value(name));
        }else{
          properties.remove(QString(STR_PROPERTY_LINKPARAM_NAME)+":"+uid);
          properties.remove(QString(STR_PROPERTY_LINKPARAM_VALUE)+":"+uid);
        }

      } else if (!p.value(name).isEmpty()){
        QString uid = StructuralUtil::createUid();

        properties.insert(QString(STR_PROPERTY_LINKPARAM_NAME)+":"+uid, name);
        properties.insert(QString(STR_PROPERTY_LINKPARAM_VALUE)+":"+uid, p.value(name));
      }
    }

    QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

    change(entity->getStructuralUid(), properties, prev, settings);
  }
}

void StructuralView::showEditBindDialog(StructuralBind* entity)
{
  QMap<QString, QString> pBind;
  foreach (QString name, entity->getStructuralProperties().keys()) {
    if (name.contains(STR_PROPERTY_BINDPARAM_NAME)){
      QString bpUid = name.right(name.length() - name.lastIndexOf(':') - 1);
      QString bpName = entity->getStructuralProperties().value(name);
      QString bpValue = entity->getStructuralProperties().value(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+bpUid);

      pBind.insert(bpName, bpValue);
    }
  }

  emit requestLinkDialogUpdate();

  if (StructuralUtil::isConditionRole(entity->getRole())){
    _linkDialog->init(entity->getHead()->getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID),"","",
                     StructuralLinkDialog::EditCondition);

    int index = _linkDialog->form.cbCondition->findText(entity->getStructuralId());
    _linkDialog->form.cbCondition->setCurrentIndex(index);
    _linkDialog->updateCurrentConditionParam(pBind);
  }else if (StructuralUtil::isActionRole(entity->getRole())){

    _linkDialog->init(entity->getTail()->getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID),"","",
                     StructuralLinkDialog::EditAction);

    int index = _linkDialog->form.cbAction->findText(entity->getStructuralId());
    _linkDialog->form.cbAction->setCurrentIndex(index);
    _linkDialog->updateCurrentActionParam(pBind);
  }


  if (_linkDialog->exec()){

    QMap<QString, QString> prev = entity->getStructuralProperties();
    QMap<QString, QString> properties = entity->getStructuralProperties();

    QString role;
    QMap<QString, QString> p;

    if (StructuralUtil::isConditionRole(entity->getRole())){
      role = _linkDialog->form.cbCondition->currentText();
      p = _linkDialog->getConditionParams();

    }else if (StructuralUtil::isActionRole(entity->getRole())){
      role = _linkDialog->form.cbAction->currentText();
      p = _linkDialog->getActionParams();
    }

    properties[STR_PROPERTY_BIND_ROLE] = role;
    properties[STR_PROPERTY_ENTITY_ID] = role;

    foreach (QString name, p.keys()) {
      if (properties.key(name).contains(STR_PROPERTY_BINDPARAM_NAME)) {
        QString key = properties.key(name);
        QString uid = key.right(key.length() - key.lastIndexOf(':') - 1);

        if (!p.value(name).isEmpty()){
          properties.insert(QString(STR_PROPERTY_BINDPARAM_NAME)+":"+uid, name);
          properties.insert(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+uid, p.value(name));
        }else{
          properties.remove(QString(STR_PROPERTY_BINDPARAM_NAME)+":"+uid);
          properties.remove(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+uid);
        }

      } else if (!p.value(name).isEmpty()){
        QString uid = StructuralUtil::createUid();

        properties.insert(QString(STR_PROPERTY_BINDPARAM_NAME)+":"+uid, name);
        properties.insert(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+uid, p.value(name));
      }
    }

    QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

    change(entity->getStructuralUid(), properties, prev, settings);
  }
}

StructuralEntity* StructuralView::clone(StructuralEntity* entity, StructuralEntity* newparent)
{
  StructuralEntity* newentity = NULL;

  switch (entity->getStructuralType()) {
    case Structural::Media:
    {
      newentity = new StructuralContent();
      break;
    }

    case Structural::Link:
    {
      newentity = new StructuralLink();
      break;
    }

    case Structural::Body:
    case Structural::Context:
    case Structural::Switch:
    {
      newentity = new StructuralComposition();
      break;
    }

    case Structural::Area:
    case Structural::Property:
    case Structural::SwitchPort:
    case Structural::Port:
    {
      newentity = new StructuralInterface();
      break;
    }

    case Structural::Mapping:
    {
      newentity = new StructuralEdge();
      break;
    }

    case Structural::Bind:
    {
      newentity = new StructuralBind();
      break;
    }

    default:
    {
      break;
    }
  }

  if (newentity != NULL) {

    newentity->setStructuralProperties(entity->getStructuralProperties());

    if (newparent != NULL)
      newentity->setStructuralParent(newparent);

    foreach (StructuralEntity* child, entity->getStructuralEntities())
     newentity->addStructuralEntity(clone(child, newentity));
  }

  return newentity;
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

void StructuralView::paste(StructuralEntity* entity, StructuralEntity* parent)
{
  paste(entity, parent, StructuralUtil::createUid(), true);
}

void StructuralView::paste(StructuralEntity* entity, StructuralEntity* parent, const QString &code, bool adjust)
{
  if (entity != NULL && parent != NULL) {
    QString uid = StructuralUtil::createUid();

    QMap<QString, QString> properties = entity->getStructuralProperties();
    properties[STR_PROPERTY_ENTITY_UID] = uid;

    if (adjust) {
      properties.remove(STR_PROPERTY_ENTITY_TOP);
      properties.remove(STR_PROPERTY_ENTITY_LEFT);
    }

    QMap<QString, QString> setting = StructuralUtil::createSettings(STR_VALUE_TRUE, STR_VALUE_TRUE, code);

    insert(uid, parent->getStructuralUid(), properties, setting);

    if (_entities.contains(uid)) {
      _clipboardReferences[entity->getStructuralUid()] = uid;

      foreach (StructuralEntity* e, entity->getStructuralEntities())
        if (e->getStructuralCategory() != Structural::Edge &&
            e->getStructuralType() != Structural::Port &&
            e->getStructuralType() != Structural::Link) {

          paste(entity, _entities.value(uid), code, false);
        }

      foreach (StructuralEntity* e, entity->getStructuralEntities())
        if (e->getStructuralCategory() == Structural::Edge ||
            e->getStructuralType() == Structural::Port ||
            e->getStructuralType() == Structural::Link) {

          QMap<QString, QString> p = e->getStructuralProperties();

          QString property;

          property = STR_PROPERTY_EDGE_TAIL;

          if (p.contains(property))
            if (_clipboardReferences.contains(p.value(property)))
              e->setStructuralProperty(property, _clipboardReferences.value(p.value(property)));

          property = STR_PROPERTY_EDGE_HEAD;

          if (p.contains(property))
            if (_clipboardReferences.contains(p.value(property)))
              e->setStructuralProperty(property, _clipboardReferences.value(p.value(property)));

          property = STR_PROPERTY_REFERENCE_COMPONENT_UID;

          if (p.contains(property))
            if (_clipboardReferences.contains(p.value(property)))
              e->setStructuralProperty(property, _clipboardReferences.value(p.value(property)));

          property = STR_PROPERTY_REFERENCE_INTERFACE_UID;

          if (p.contains(property))
            if (_clipboardReferences.contains(p.value(property)))
              e->setStructuralProperty(property, _clipboardReferences.value(p.value(property)));

          property = STR_PROPERTY_REFERENCE_LINK_UID;

          if (p.contains(property))
            if (_clipboardReferences.contains(p.value(property)))
              e->setStructuralProperty(property, _clipboardReferences.value(p.value(property)));

          property = STR_PROPERTY_REFERENCE_XCONNECTOR_UID;

          if (p.contains(property))
            if (_clipboardReferences.contains(p.value(property)))
              e->setStructuralProperty(property, _clipboardReferences.value(p.value(property)));

          foreach (QString k, p.keys())
            if (k.contains(STR_PROPERTY_LINKPARAM_NAME) ||
                k.contains(STR_PROPERTY_LINKPARAM_VALUE) ||
                k.contains(STR_PROPERTY_BINDPARAM_NAME) ||
                k.contains(STR_PROPERTY_BINDPARAM_VALUE)) {

              property = k.left(k.length() - k.lastIndexOf(':') - 1);

              e->setStructuralProperty(property+":"+StructuralUtil::createUid(), p.value(k));
              e->setStructuralProperty(k, "");
            }

          paste(e, _entities.value(uid), code, false);
        }
    }
  }
}

#ifdef WITH_GRAPHVIZ
#include "gvc.h"
#include "cgraph.h"

#define WITH_INTERFACE 0

void StructuralView::autoadjust()
{
  StructuralEntity* entity = NULL;

  if (_entities.contains(_selected))
    entity = _entities.value(_selected);
  else
    entity = getBody();

  autoadjust(entity, StructuralUtil::createUid());
}

void StructuralView::autoadjust(StructuralEntity* entity, const QString &code)
{
  if (entity != NULL) {
    // Autoadjust children
    foreach (StructuralEntity* e, entity->getStructuralEntities()) {
      if (e->getStructuralType() == Structural::Context ||
          e->getStructuralType() == Structural::Switch ||
          e->getStructuralType() == Structural::Body) {

        autoadjust(e, code);
      }
    }

    // Create graphviz context
    GVC_t *c;
    c = gvContext();

    // Create graphviz graph
    Agraph_t *g;
    g = agopen("g", Agdirected, NULL);

    agattr(g, AGRAPH, "splines", "ortho");
    agattr(g, AGRAPH, "nodesep", "0.4");
    agattr(g, AGNODE, "shape", "box");
    agattr(g, AGNODE, "nodesep", "0.4");
    agattr(g, AGEDGE, "minlen", "3");

    qreal GRAPHVIZ_DPI = 72;

    QMap<QString, Agnode_t*> nodes;

    // Add nodes in graphviz graph based on children entities.
    // Skip link entities.
    foreach (StructuralEntity* e, entity->getStructuralEntities()) {
      if (e->getStructuralCategory() == Structural::Node &&
          e->getStructuralType() != Structural::Link) {

        QString w = QString::number(e->getWidth()/GRAPHVIZ_DPI);
        QString h = QString::number(e->getHeight()/GRAPHVIZ_DPI);

        Agnode_t* node = agnode(g,(char*) e->getStructuralUid().toStdString().c_str(),1);
        agsafeset(node, "width", (char*) w.toStdString().c_str(), (char*) w.toStdString().c_str() );
        agsafeset(node, "height", (char*) h.toStdString().c_str(), (char*) h.toStdString().c_str() );

        nodes.insert(e->getStructuralUid(), node);
      }
    }

//    QString r = nodes.firstKey();
//    agsafeset(g, "root", (char*) r.toStdString().c_str(), (char*) r.toStdString().c_str());

    // Load condition and action entities for each link entity.
    QMap<QString, StructuralEntity*> links;
    QMap<QString, QVector<StructuralEntity*> > tails;
    QMap<QString, QVector<StructuralEntity*> > heads;

    foreach (StructuralEntity* e, entity->getStructuralEntities()) {
      if (e->getStructuralType() == Structural::Bind ) {
        StructuralBind* b = (StructuralBind*) e;

        StructuralEntity* tail = b->getTail();
        StructuralEntity* head = b->getHead();

        if (tail != NULL && head != NULL) {
          if (tail->getStructuralType() == Structural::Link) {
            QString uid = tail->getStructuralUid();

            links.insert(uid, tail);

            if (!heads.contains(uid))
              heads.insert(uid, QVector<StructuralEntity*>());

            // In case the bind connect the link entity with a interface entity
            // (Node -> Interface) consider the parent of the interface entity
            // instead (Node -> Node).
            if (head->getStructuralCategory() == Structural::Interface &&
                head->getStructuralParent() != entity)
              head = head->getStructuralParent();

            QVector<StructuralEntity*> entities = heads.value(uid);
            entities.append(head);

            heads.insert(uid, entities);

          } else if (head->getStructuralType() == Structural::Link) {
            QString uid = head->getStructuralUid();

            links.insert(uid, head);

            if (!tails.contains(uid))
              tails.insert(uid, QVector<StructuralEntity*>());

            // In case the bind connect the link entity with a interface entity
            // (Interface -> Node) consider the parent of the interface entity
            // instead (Node -> Node).
            if (tail->getStructuralCategory() == Structural::Interface &&
                tail->getStructuralParent() != entity)
              tail = tail->getStructuralParent();

            QVector<StructuralEntity*> entities = tails.value(uid);
            entities.append(tail);

            tails.insert(uid, entities);
          }
        }
      }
    }

    // Add edges in graphviz graph based on the tails and heads loaded
    // from each link entity.
    foreach (StructuralEntity* link, links) {
      QString uid = link->getStructuralUid();

      foreach (StructuralEntity* tail, tails.value(uid)) {
        foreach (StructuralEntity* head, heads.value(uid)) {
          if (nodes.contains(tail->getStructuralUid()) &&
              nodes.contains(head->getStructuralUid())) {

            agedge(g,nodes.value(tail->getStructuralUid()),
                   nodes.value(head->getStructuralUid()),
                   NULL,1);
          }
        }
      }
    }

    // Adjust graph, nodes and edges size and pos using 'dot' algorithm.
    gvLayout(c, g, "dot");

    // Update entity size in view.
    qreal PADDING = 64*2;

    qreal nextTop = entity->getTop();
    qreal nextLeft = entity->getLeft();
    qreal nextWidth = GD_bb(g).UR.x + PADDING;
    qreal nextHeight = GD_bb(g).UR.y + PADDING;

    change(entity->getStructuralUid(),
           StructuralUtil::createProperties(nextTop, nextLeft, nextWidth, nextHeight),
           entity->getStructuralProperties(),
           StructuralUtil::createSettings(STR_VALUE_TRUE, STR_VALUE_FALSE, code));

    // Update children entities 'pos' in view. Skip link entities for now.
    foreach (QString key, nodes.keys()) {
      Agnode_t* n = nodes.value(key);
      StructuralEntity* e = _entities.value(key);

      nextTop = (GD_bb(g).UR.y - ND_coord(n).y) + entity->getHeight()/2 - GD_bb(g).UR.y/2 - e->getHeight()/2;
      nextLeft = ND_coord(n).x + entity->getWidth()/2 - GD_bb(g).UR.x/2 - e->getWidth()/2;

      change(e->getStructuralUid(),
             StructuralUtil::createProperties(nextTop, nextLeft),
             e->getStructuralProperties(),
             StructuralUtil::createSettings(STR_VALUE_TRUE, STR_VALUE_FALSE, code));
    }

    // Update link entities 'pos'.
    //
    // Note:
    // This is NOT the best solution.
    //
    // Known Issues:
    // - link entity can be moved into a composition
    // - links entity can be moved to a same position
    //
    // Todo:
    // Use the middle 'pos' of the edges path generated by graphviz instead
    // of bounding box center.
    qreal top;
    qreal left;
    qreal width;
    qreal height;

    foreach (StructuralEntity* link, links.values()) {
      top = 1024*1024;;
      left = 1024*1024;
      width = 0;
      height = 0;

      foreach (StructuralEntity* tail, tails.value(link->getStructuralUid())) {
        if (tail->getTop() < top)
          top = tail->getTop();

        if (tail->getLeft() < left)
          left = tail->getLeft();

        if (tail->getLeft() + tail->getWidth() > width)
          width = tail->getLeft() + tail->getWidth();

        if (tail->getTop() + tail->getHeight() > height)
          height = tail->getTop() + tail->getHeight();
      }

      foreach (StructuralEntity* head, heads.value(link->getStructuralUid())) {
        if (head->getTop() < top)
          top = head->getTop();

        if (head->getLeft() < left)
          left = head->getLeft();

        if (head->getLeft() + head->getWidth() > width)
          width = head->getLeft() + head->getWidth();

        if (head->getTop() + head->getHeight() > height)
          height = head->getTop() + head->getHeight();
      }

      width = width - left;
      height = height - top;

      nextTop = top + height/2 - link->getHeight()/2;
      nextLeft = left + width/2 - link->getWidth()/2;

      change(link->getStructuralUid(),
             StructuralUtil::createProperties(nextTop, nextLeft),
             link->getStructuralProperties(),
             StructuralUtil::createSettings(STR_VALUE_TRUE, STR_VALUE_FALSE, code));
    }

    // Update interface entities 'pos'.
    // TODO
  }

}
#endif

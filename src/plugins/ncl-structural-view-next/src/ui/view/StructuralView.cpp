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

  _linking = false;

  _mode = Structural::Pointing;

  _selected = "";

  _clipboard = NULL;

  _linkTool = NULL;

  lastLinkMouseOver = NULL;

  _tailHovered = NULL;
  _headHovered = NULL;


  hasCutted = false;

  foreach (StructuralType key, entityCount.keys()) {
    entityCount[key] = 0;
  }

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

void StructuralView::load(const QString &data)
{
  QDomDocument* dom = new QDomDocument();
  dom->setContent(data);

  QDomElement root = dom->firstChildElement();
  QDomNodeList rootChildren = root.childNodes();

  for (unsigned int i = 0; i < rootChildren.length(); i++) {
    if (rootChildren.item(i).isElement()) {
      QDomElement body = rootChildren.item(i).toElement();
      QDomNodeList bodyChildren = body.childNodes();

      QString bodyUid = body.attributeNode("uid").nodeValue();
      QString parentUid = "";

      QMap<QString,QString> properties;

      for (unsigned int j = 0; j < bodyChildren.length(); j++) {
        if (bodyChildren.item(j).isElement()) {
          QDomElement property = bodyChildren.item(j).toElement();

          if (property.nodeName() == "property")
            properties.insert(property.attributeNode("name").nodeValue(),
                              property.attributeNode("value").nodeValue());
        }
      }

      insert(bodyUid, parentUid, properties, StructuralUtil::createSettings(false, false));

      for (unsigned int j = 0; j < bodyChildren.length(); j++) {
        if (bodyChildren.item(j).isElement()) {
          QDomElement entity = bodyChildren.item(j).toElement();

          if (entity.nodeName() == "entity")
            load(entity, body);
        }
      }

      foreach (StructuralEntity* e, _entities.values()) {
        if (e->getStructuralCategory() == Structural::Edge ||
            e->getStructuralType() == Structural::Port)
          adjustReferences(e);
      }

      select(bodyUid, StructuralUtil::createSettings());
    }
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

void StructuralView::load(QDomElement entity, QDomElement parent)
{
  if (!entity.attributeNode("uid").nodeValue().isEmpty() &&
      !parent.attributeNode("uid").nodeValue().isEmpty()) {

    QDomNodeList entityChildren = entity.childNodes();

    QString entityUid = entity.attributeNode("uid").nodeValue();
    QString parentUid = parent.attributeNode("uid").nodeValue();

    QMap<QString,QString> properties;

    for (unsigned int i = 0; i < entityChildren.length(); i++) {
      if (entityChildren.item(i).isElement()) {
        QDomElement child = entityChildren.item(i).toElement();

        if (child.nodeName() == "property")
          properties.insert(child.attributeNode("name").nodeValue(),
                            child.attributeNode("value").nodeValue());
      }
    }

    insert(entityUid, parentUid, properties, StructuralUtil::createSettings(false, false));

    for (unsigned int i = 0; i < entityChildren.length(); i++) {
      if (entityChildren.item(i).isElement()) {
        QDomElement child = entityChildren.item(i).toElement();

        if (child.nodeName() == "entity")
          load(child, entity);
      }
    }
  }
}

QString StructuralView::save()
{
  QDomDocument* document = new QDomDocument();

  QDomElement root = document->createElement("structural");

  foreach(StructuralEntity* e, _entities.values())
    if (e->getStructuralType() == Structural::Body)
      createDocument(e, document, root);

  document->appendChild(root);

  return document->toString(4);
}

void StructuralView::createDocument(StructuralEntity* entity, QDomDocument* document, QDomElement parent)
{  
  if (entity->getStructuralType() != Structural::Reference) {
    QDomElement element = document->createElement("entity");
    element.setAttribute("uid",entity->getStructuralUid());

    foreach (QString key, entity->getStructuralProperties().keys()) {
      QDomElement property = document->createElement("property");
      property.setAttribute("name", key);
      property.setAttribute("value", entity->getStructuralProperty(key));

      element.appendChild(property);
    }

    foreach (StructuralEntity* e, entity->getStructuralEntities())
      createDocument(e, document, element);

    parent.appendChild(element);
  }
}

void StructuralView::insert(QString uid, QString parent, QMap<QString, QString> properties, QMap<QString, QString> settings)
{
  if (!_entities.contains(uid)) {
    //
    // Initializing....
    //

    StructuralEntity* e = NULL;
    StructuralEntity* p = _entities.value(parent, NULL);

    //
    // Creating...
    //

    StructuralType type = StructuralUtil::translateStringToType(properties[STR_PROPERTY_ENTITY_TYPE]);

    if (p != NULL) {
      switch (type) {
        case Structural::Media:
          // Creating...
          e = new StructuralContent();

          // Connecting...
          // nothing

          break;

        case Structural::Context:
        case Structural::Switch:
        case Structural::Body:
          // Creating...
          e = new StructuralComposition();
          e->setStructuralType(type);

          // Connecting...
          // nothing

          break;

        case Structural::Link:
        {
          // Creating...
          e = new StructuralLink();
          e->setStructuralType(type);

          // Connecting...
          connect((StructuralLink*) e, SIGNAL(performedEdit(StructuralLink*)),SLOT(showEditLinkDialog(StructuralLink*)));

          break;
        }

        case Structural::Area:
        case Structural::Property:
        case Structural::Port:
        case Structural::SwitchPort:
        {
          // Creating...
          e = new StructuralInterface();
          e->setStructuralType(type);


          // Connecting...
          // nothing

          break;
        }

        case Structural::Reference:
        case Structural::Mapping:
        {
          // Creating...
          e = new StructuralEdge();
          e->setStructuralType(type);

          // Setting...
          // nothing

          // Connecting...
          // Nothing

          break;
        }

        case Structural::Bind:
        {
          // Creating...
          e = new StructuralBind();
          e->setStructuralType(type);

          // Setting...
          // nothing

          // Connecting...
          connect(((StructuralBind*) e), SIGNAL(performedEdit(StructuralBind*)),SLOT(showEditBindDialog(StructuralBind*)));

          break;
        }

        default:
        {
          break;
        }
      }
    } else {

      switch (type) {
        case Structural::Body:
        {
          if (getBody() == NULL) {
            // Creating...
            e = new StructuralComposition();
            e->setStructuralType(type);

            // Setting...
            e->setWidth(STR_DEFAULT_BODY_W);
            e->setHeight(STR_DEFAULT_BODY_H);

            // Connecting...
            // nothing

            // Notifing...
            switchedBody(false);
          }

          break;
        }

        default:
        {
          break;
        }
      }
    }


    if (e != NULL) {

      //
      // Setting...
      //

      //


      // Setting 'uid'...
      e->setStructuralUid(uid);

      // Setting 'id'...
      if (!properties.contains(STR_PROPERTY_ENTITY_ID))
        e->setStructuralId(getNewId(e));

      // Setting 'parent'...
      if (p != NULL)
        e->setStructuralParent(p);
      else
        _scene->addItem(e);

      // Setting 'top'...
      if (!properties.contains(STR_PROPERTY_ENTITY_TOP)) {
        if (p != NULL)
          e->setTop(p->getHeight()/2 - e->getHeight()/2);
        else
          e->setTop(_scene->sceneRect().height()/2 - e->getHeight()/2);
      }

      // Setting 'left'...
      if (!properties.contains(STR_PROPERTY_ENTITY_LEFT)) {
        if (p != NULL)
          e->setLeft(p->getWidth()/2 - e->getWidth()/2);
        else
          e->setLeft(_scene->sceneRect().width()/2 - e->getWidth()/2);
      }

      // Setting 'menu'...
      e->setMenu(_menu);

      // Setting 'others'...
      e->setStructuralProperties(properties);

      //
      // Adjusting...
      //

      // Adjust 'compositions'
      if (p != NULL) {
        if (!p->isUncollapsed()) {
          if (p->getStructuralType() == Structural::Body ||
              p->getStructuralType() == Structural::Switch ||
              p->getStructuralType() == Structural::Context) {

            ((StructuralComposition*) p)->collapse();
            ((StructuralComposition*) p)->collapse();
          }
        }
      }

      // Adjust 'references'
      adjustReferences(e);

      // Adjust 'others'
      if (!properties.contains(STR_PROPERTY_ENTITY_TOP) || !properties.contains(STR_PROPERTY_ENTITY_LEFT))
        e->adjust(false);
      else
        e->adjust(true);

      //
      // Connecting...
      //

      connect(e, SIGNAL(inserted(QString,QString,QMap<QString,QString>,QMap<QString,QString>)),SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(e, SIGNAL(removed(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(e, SIGNAL(changed(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(e, SIGNAL(selected(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));
      connect(e, SIGNAL(move(QString,QString)), SLOT(move(QString,QString)));

      //
      // Saving...
      //

      _entities[uid] = e;

      if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE) {
        Insert* command = new Insert(uid, parent, e->getStructuralProperties(), settings);
        command->setText(settings[STR_SETTING_CODE]);

        connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
        connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
        connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

        _commnads.push(command); emit switchedUndo(true);
      }

      //
      // Notifing...
      //

      if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE) {
        emit inserted(uid, parent, e->getStructuralProperties(), settings);
      }
    }
  }
}

qreal StructuralView::getNewAngle(StructuralBind* bind)
{
  int min =  100000;
  int max = -100000;

  foreach (StructuralEntity *e, _entities.values()) {
    if (e->getStructuralType() == Structural::Bind) {
      if (e != bind) {
        StructuralBind *b = (StructuralBind*) e;

        if ((bind->getTail() == b->getTail() || bind->getTail() == b->getHead()) &&
            (bind->getHead() == b->getHead() || bind->getHead() == b->getTail())) {

          int current = b->getAngle();

          if (b->getHead()->getStructuralType() != Structural::Link)
            current = -current;

          if (max < current )
            max = current;

          if (min > current )
            min = current;
        }
      }
    }
  }

  qreal angle = 0;

  if (min != 100000 || max != -100000) {
    if (max <= abs(min))
      angle = max+60;
    else if (max > abs(min))
      angle = min-60;
  }

  if (bind->getHead()->getStructuralType() != Structural::Link)
    return -angle;

  return angle;
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
  if (_entities.contains(uid)) {

    //
    // Saving...
    //

    if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE) {
      QString parent = "";

      if (_entities[uid]->getStructuralParent() != NULL)
        parent = _entities[uid]->getStructuralParent()->getStructuralUid();

      Remove* command = new Remove(_entities[uid]->getStructuralUid(), parent, _entities[uid]->getStructuralProperties(), settings);
      command->setText(settings[STR_SETTING_CODE]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      _commnads.push(command); emit switchedUndo(true); return;
    }

    //
    // Initializing...
    //

    StructuralEntity* e = _entities.value(uid);
    StructuralEntity* p = e->getStructuralParent();

    StructuralType type = e->getStructuralType();

    //
    // Removing...
    //

    // Removing 'children'...
    if (settings.value(STR_SETTING_UNDO_TRACE) != STR_VALUE_FALSE)
      settings[STR_SETTING_UNDO] = STR_VALUE_TRUE;

    while (!e->getStructuralEntities().isEmpty())
      remove(e->getStructuralEntities().first()->getStructuralUid(), settings);

    // Removing 'edges'...
    if (e->getStructuralCategory() != Structural::Edge) {
      QVector<StructuralEntity*> relatives;

      if (p != NULL) {
        relatives = p->getStructuralEntities();

        if (e->getStructuralCategory() == Structural::Interface)
          if (p->getStructuralParent() != NULL)
            relatives += p->getStructuralParent()->getStructuralEntities();
      }

      foreach (StructuralEntity* entity, relatives) {
        if (entity->getStructuralCategory() == Structural::Edge) {
          StructuralEdge *edge = (StructuralEdge*) entity;

          if (edge->getTail() == e || edge->getHead() == e) {
            if (edge->getStructuralType() != Structural::Reference) {
              remove(edge->getStructuralUid(), StructuralUtil::createSettings(STR_VALUE_TRUE, settings.value(STR_SETTING_NOTIFY), settings.value(STR_SETTING_CODE)));

            } else {
              // In case of 'Structural::Reference' edge, just change
              // 'Structural::Port' properties.
              //
              // Note:
              // The 'tail' of a 'Structural::Reference' edge
              // is always a 'Structural::Port' entity.
              QMap<QString, QString> previous = edge->getTail()->getStructuralProperties();
              QMap<QString, QString> properties = edge->getTail()->getStructuralProperties();;

              properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = "";
              properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = "";
              properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
              properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";

              change(edge->getTail()->getStructuralUid(),properties, previous, StructuralUtil::createSettings(STR_VALUE_TRUE, settings.value(STR_SETTING_NOTIFY), settings.value(STR_SETTING_CODE)));
            }
          }
        }
      }
    }

    // Removing 'params'...
    if (type == Structural::Link || type == Structural::Bind) {

      foreach (QString key, e->getStructuralProperties().keys()) {
        if (key.contains(STR_PROPERTY_LINKPARAM_NAME) ||
            key.contains(STR_PROPERTY_BINDPARAM_NAME)) {

          if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE) {
            emit removed(key.right(key.length() - key.lastIndexOf(':') - 1), settings);
          }
        }
      }
    }

    // Removing 'others'...
    if (p != NULL)
      p->removeStructuralEntity(e);
    else
      _scene->removeItem(e);

    _entities.remove(uid); delete e; e = NULL;

    //
    // Notifing...
    //

    if (uid == _selected) {
      emit select("", settings);
    }

    if (type == Structural::Body) {
      emit switchedBody(true);
    }

    if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE) {
      emit removed(uid, settings);
    }
  }
}

void StructuralView:: change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  if (_entities.contains(uid)) {

    //
    // Saving...
    //

    if (settings[STR_SETTING_UNDO] == STR_VALUE_TRUE) {

      Change* command = new Change(uid, properties, previous, settings);
      command->setText(settings[STR_SETTING_CODE]);

      connect(command, SIGNAL(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)), SLOT(insert(QString,QString,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(remove(QString,QMap<QString,QString>)),SLOT(remove(QString,QMap<QString,QString>)));
      connect(command, SIGNAL(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)), SLOT(change(QString,QMap<QString,QString>,QMap<QString,QString>,QMap<QString,QString>)));
      connect(command, SIGNAL(select(QString,QMap<QString,QString>)),SLOT(select(QString,QMap<QString,QString>)));

      _commnads.push(command); emit switchedUndo(true); return;
    }

    //
    // Initializing...
    //

    StructuralEntity* entity = _entities[uid];

    //
    // Setting...
    //

    //
    if (entity->isUncollapsed())
      if (properties.value(STR_PROPERTY_ENTITY_UNCOLLAPSED) == STR_VALUE_FALSE)
        ((StructuralComposition*) entity)->collapse();

    if (!entity->isUncollapsed())
      if (properties.value(STR_PROPERTY_ENTITY_UNCOLLAPSED) == STR_VALUE_TRUE)
        ((StructuralComposition*) entity)->collapse();

    // Setting 'others'...
    entity->setStructuralProperties(properties);

    //
    // Adjusting...
    //

    // Adjust 'references'
    adjustReferences(entity);

    // Adjust 'others'
    entity->adjust(true);

    //
    // Notifing...
    //

    if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE) {
      emit changed(uid, properties, previous, settings);
    }
  }
}

void StructuralView::adjustReferences(StructuralEntity* entity)
{
  if (entity != NULL){
    StructuralType type = entity->getStructuralType();

    switch (type) {
      case Structural::Reference:
      case Structural::Mapping:
      case Structural::Bind:
      {
        if (_entities.contains(entity->getStructuralProperty(STR_PROPERTY_EDGE_TAIL)))
          ((StructuralEdge*) entity)->setTail(_entities.value(entity->getStructuralProperty(STR_PROPERTY_EDGE_TAIL)));

        if (_entities.contains(entity->getStructuralProperty(STR_PROPERTY_EDGE_HEAD)))
          ((StructuralEdge*) entity)->setHead(_entities.value(entity->getStructuralProperty(STR_PROPERTY_EDGE_HEAD)));

        bool isVisible = false;

        if (((StructuralEdge*) entity)->getTail() != NULL &&
            ((StructuralEdge*) entity)->getHead() != NULL)
          isVisible = true;

        StructuralEntity* component = NULL;
        StructuralEntity* interface = NULL;

        if (_entities.contains(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_INTERFACE_UID)))
          interface = _entities.value(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_INTERFACE_UID));

        if (_entities.contains(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_COMPONENT_UID)))
          component = _entities.value(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_COMPONENT_UID));

        if (type == Structural::Bind)
          if (!_entities.contains(entity->getStructuralProperty(STR_PROPERTY_EDGE_ANGLE)))
            ((StructuralBind*) entity)->setAngle(getNewAngle((StructuralBind*) entity));

        if (component != NULL && interface != NULL)
          if (interface->getStructuralParent() == component)
            isVisible = true;

        setHidden(!isVisible);

        break;
      }

      case Structural::Port:
      {
        foreach (StructuralEntity* e, _entities.values())
          if (e->getStructuralType() == Structural::Reference)
            if (e->getStructuralProperty(STR_PROPERTY_EDGE_TAIL) == entity->getStructuralUid())
              remove(e->getStructuralUid(), StructuralUtil::createSettings(false,false));

        StructuralEntity* component = NULL;
        StructuralEntity* interface = NULL;

        if (_entities.contains(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_INTERFACE_UID)))
          interface = _entities.value(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_INTERFACE_UID));

        if (_entities.contains(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_COMPONENT_UID)))
          component = _entities.value(entity->getStructuralProperty(STR_PROPERTY_REFERENCE_COMPONENT_UID));

        if (component != NULL) {
          QString uid = "";

          if (interface != NULL) {
            if (interface->getStructuralParent() == component) {
              uid = interface->getStructuralUid();
            }
          } else {
            uid = component->getStructuralUid();
          }

          if (!uid.isEmpty()) {
            QMap<QString, QString> properties;
            properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Reference);
            properties[STR_PROPERTY_EDGE_TAIL] = entity->getStructuralUid();
            properties[STR_PROPERTY_EDGE_HEAD] = uid;

            insert(StructuralUtil::createUid(), entity->getStructuralParent()->getStructuralUid(), properties, StructuralUtil::createSettings(false,false));
          }
        }

        break;
      }

      default:
      {
        break;
      }
    }
  }
}

void StructuralView::unselect()
{
  select("", StructuralUtil::createSettings());
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
  if (_entities.contains(uid)) {

    if (uid != _selected) {
      //
      // Initializing...
      //

      StructuralEntity* entity = _entities.value(_selected);

      if (entity != NULL)
        entity->setSelected(false);

      //
      // Selecting...
      //

      entity = _entities.value(uid);
      entity->setSelected(true);

      _selected = entity->getStructuralUid();


      //
      // Notifing...
      //

      emit switchedCut(true);
      emit switchedCopy(true);

      if (_clipboard != NULL) {
        if (StructuralUtil::validateKinship(entity->getStructuralType(), _clipboard->getStructuralType()) &&
            isChild(entity, _clipboard))
          emit switchedPaste(true);
      } else {
        emit switchedPaste(false);
      }

      emit selectChange(uid); // Remove this after StructuralWindow refactory

      if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE) {
        emit selected(uid, settings);
      }
    }

  } else {
    //
    // Initializing...
    //

    StructuralEntity* entity = _entities.value(_selected);

    if (entity != NULL)
      entity->setSelected(false);

    //
    // Unselecting...
    //

    _selected = "";

    //
    // Notifing...
    //

    emit switchedCut(false);
    emit switchedCopy(false);
    emit switchedPaste(false);

    emit selectChange(""); // Remove this after StructuralWindow refactory

    if (settings[STR_SETTING_NOTIFY] == STR_VALUE_TRUE) {
      emit selected(uid, settings);
    }
  }

  _scene->update();
}

void StructuralView::move(QString uid, QString parent)
{
  if (_entities.contains(uid) && _entities.contains(parent)) {

    //
    // Initializing...
    //

    StructuralEntity* e = _entities.value(uid);
    StructuralEntity* p = _entities.value(parent);

    if (StructuralUtil::validateKinship(e->getStructuralType(), p->getStructuralType()) &&
        e->getStructuralParent() != p) {

      //
      // Setting...
      //

      foreach(StructuralEntity *entity, _entities.values())
        entity->setDraggable(false);

      unselect();

      //
      // Moving...
      //

      QString code = StructuralUtil::createUid();

      QMap<QString, QString> settings = StructuralUtil::createSettings();
      settings[STR_SETTING_CODE] = code;

      paste(clone(e, NULL), p, code, true); remove(uid, settings);
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

void StructuralView::createLink(StructuralEntity* tail, StructuralEntity* head)
{
  StructuralEntity* parentTail = (StructuralEntity*) tail->getStructuralParent();
  StructuralEntity* parentHead = (StructuralEntity*) head->getStructuralParent();

  if (parentTail != NULL && parentHead != NULL) {

    setMode(Structural::Pointing); emit switchedPointer(true);

    emit requestLinkDialogUpdate();
    _linkDialog->init();

    if (_linkDialog->exec()) {

      StructuralEntity* parent = NULL;

      qreal x;
      qreal y;
      qreal z;
      qreal w;

      if (parentTail == parentHead) {
        parent = parentTail;

        x = tail->getLeft();
        y = tail->getTop();
        z = head->getLeft();
        w = head->getTop();

      } else if (parentTail->getStructuralParent() == parentHead) {
        QPointF pointTail = parentTail->mapToParent(tail->getLeft(),
                                                    tail->getTop());

        parent = parentHead;

        x = pointTail.x();
        y = pointTail.y();
        z = head->getLeft();
        w = head->getTop();

      } else if (parentTail == parentHead->getStructuralParent()) {
        QPointF pointHead = parentHead->mapToParent(head->getLeft(),
                                                    head->getTop());

        parent = parentTail;

        x = tail->getLeft();
        y = tail->getTop();
        z = pointHead.x();
        w = pointHead.y();

      } else if (parentTail->getStructuralParent() == parentHead->getStructuralParent() &&
                 parentTail->getStructuralParent() != NULL) {
        QPointF pointTail = parentTail->mapToParent(tail->getLeft(),
                                                    tail->getTop());

        QPointF pointHead = parentHead->mapToParent(head->getLeft(),
                                                    head->getTop());

        parent = parentTail->getStructuralParent();

        x = pointTail.x();
        y = pointTail.y();
        z = pointHead.x();
        w = pointHead.y();
      }

      if (parent != NULL) {
        x += tail->getWidth()/2;
        y += tail->getHeight()/2;
        z += head->getWidth()/2;
        w += head->getHeight()/2;

        QString uid = StructuralUtil::createUid();

        QMap<QString, QString> properties;
        properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Link);
        properties[STR_PROPERTY_REFERENCE_XCONNECTOR_ID] =_linkDialog->getCurrentConnector();

        if (y > w)
          properties[STR_PROPERTY_ENTITY_TOP] = QString::number(w + (y - w)/2);
        else
          properties[STR_PROPERTY_ENTITY_TOP] = QString::number(y + (w - y)/2);

        if (x > z)
          properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(z + (z - z)/2);
        else
          properties[STR_PROPERTY_ENTITY_LEFT] = QString::number(x + (z - x)/2);


        QMap<QString, QString> params = _linkDialog->getLinkParams();

        foreach (QString key, params.keys()) {
          if (!params.value(key).isEmpty()){
            QString uid = StructuralUtil::createUid();

            properties[QString(STR_PROPERTY_LINKPARAM_NAME)+":"+uid]  = key;
            properties[QString(STR_PROPERTY_LINKPARAM_VALUE)+":"+uid] = params.value(key);
          }
        }

        QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

        insert(uid, parent->getStructuralUid(), properties, settings);

        if (_entities.contains(uid)) {
          StructuralRole condition = StructuralUtil::translateStringToRole(_linkDialog->form.cbCondition->currentText());
          StructuralRole action = StructuralUtil::translateStringToRole(_linkDialog->form.cbAction->currentText());

          createBind(tail,_entities.value(uid),condition,settings.value(STR_SETTING_CODE));
          createBind(_entities.value(uid),head,action,settings.value(STR_SETTING_CODE));
        }
      }
    }
  }
}

void StructuralView::createBind(StructuralEntity* tail, StructuralEntity* head, StructuralRole type, QString code)
{
  StructuralEntity* parentTail = tail->getStructuralParent();
  StructuralEntity* parentHead = head->getStructuralParent();

  if (parentTail != NULL && parentHead != NULL) {
    StructuralEntity* parent = NULL;

    if (parentTail == parentHead)
        parent = parentTail;

    else if (parentTail->getStructuralParent() == parentHead)
        parent = parentHead;

    else if (parentTail == parentHead->getStructuralParent())
        parent = parentTail;

    if (parent != NULL) {
      QString uid = StructuralUtil::createUid();

      QMap<QString, QString> properties;
      properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Bind);
      properties[STR_PROPERTY_EDGE_TAIL] = tail->getStructuralUid();
      properties[STR_PROPERTY_EDGE_HEAD] = head->getStructuralUid();

      properties[STR_PROPERTY_BIND_ROLE] = StructuralUtil::translateRoleToString(type);
      properties[STR_PROPERTY_ENTITY_ID] = StructuralUtil::translateRoleToString(type);

      StructuralEntity* entityLink = NULL;
      StructuralEntity* entityNonLink = NULL;
      StructuralEntity* parentNonLink = NULL;

      if (tail->getStructuralType() == Structural::Link) {
        entityLink = tail;
        entityNonLink = head;
        parentNonLink = parentHead;

        if (type == Structural::NoRole) {
          emit requestLinkDialogUpdate();

          setMode(Structural::Pointing);
          emit switchedPointer(true);

          _linkDialog->init(entityLink->getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID),"","", StructuralLinkDialog::CreateAction);

          if (_linkDialog->exec()){
            QString role = _linkDialog->form.cbAction->currentText();

            properties[STR_PROPERTY_BIND_ROLE] = role;
            properties[STR_PROPERTY_ENTITY_ID] = role;
          }

          QMap<QString, QString> params = _linkDialog->getActionParams();

          foreach (QString key, params.keys()) {
            if (!params.value(key).isEmpty()) {
              QString uid = StructuralUtil::createUid();

              properties.insert(QString(STR_PROPERTY_BINDPARAM_NAME)+":"+uid, key);
              properties.insert(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+uid, params.value(key));
            }
          }
        }
      } else if (head->getStructuralType() == Structural::Link) {
        entityLink = head;
        entityNonLink = tail;
        parentNonLink = parentTail;

        if (type == Structural::NoRole) {
          emit requestLinkDialogUpdate();

          setMode(Structural::Pointing);
          emit switchedPointer(true);

          _linkDialog->init(entityLink->getStructuralProperty(STR_PROPERTY_REFERENCE_XCONNECTOR_ID),"","", StructuralLinkDialog::CreateCondition);

          if (_linkDialog->exec()){
            QString role = _linkDialog->form.cbCondition->currentText();

            properties[STR_PROPERTY_BIND_ROLE] = role;
            properties[STR_PROPERTY_ENTITY_ID] = role;
          }

          QMap<QString, QString> params = _linkDialog->getConditionParams();

          foreach (QString key, params.keys()) {
            if (!params.value(key).isEmpty()){
              QString uid = StructuralUtil::createUid();

              properties.insert(QString(STR_PROPERTY_BINDPARAM_NAME)+":"+uid, key);
              properties.insert(QString(STR_PROPERTY_BINDPARAM_VALUE)+":"+uid, params.value(key));
            }
          }
        }
      }

      if (entityLink != NULL && entityNonLink != NULL && parentNonLink != NULL){
        properties[STR_PROPERTY_REFERENCE_LINK_UID] = entityLink->getStructuralUid();

        if (entityNonLink->getStructuralCategory() == Structural::Interface) {
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = entityNonLink->getStructuralId();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = entityNonLink->getStructuralUid();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = parentNonLink->getStructuralId();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = parentNonLink->getStructuralUid();

        } else {
          properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = entityNonLink->getStructuralId();
          properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = entityNonLink->getStructuralUid();
          properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
          properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
        }
      }

      QMap<QString, QString> settings = StructuralUtil::createSettings(true, true);

      if (!code.isEmpty())
        settings[STR_SETTING_CODE] = code;

      insert(uid, parent->getStructuralUid(), properties, settings);
    }
  }
}

void StructuralView::createReference(StructuralEntity* tail, StructuralEntity* head)
{
  if (tail->getStructuralType() == Structural::Port || tail->getStructuralType() == Structural::SwitchPort) {
    StructuralEntity* parentTail = tail->getStructuralParent();
    StructuralEntity* parentHead = head->getStructuralParent();

    if (parentTail != NULL && parentHead != NULL && (parentTail == parentHead || parentTail == parentHead->getStructuralParent())) {
      QMap<QString, QString> previous = tail->getStructuralProperties();
      QMap<QString, QString> properties;

      if (tail->getStructuralType() == Structural::Port)
        properties = tail->getStructuralProperties();

      if (head->getStructuralCategory() == Structural::Interface) {
        properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = head->getStructuralId();
        properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = head->getStructuralUid();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = parentHead->getStructuralId();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = parentHead->getStructuralUid();

      } else {
        properties[STR_PROPERTY_REFERENCE_COMPONENT_ID] = head->getStructuralId();
        properties[STR_PROPERTY_REFERENCE_COMPONENT_UID] = head->getStructuralUid();
        properties[STR_PROPERTY_REFERENCE_INTERFACE_ID] = "";
        properties[STR_PROPERTY_REFERENCE_INTERFACE_UID] = "";
      }

      setMode(Structural::Pointing);
      emit switchedPointer(true);

      if (tail->getStructuralType() == Structural::Port) {
        change(tail->getStructuralUid(), properties, previous, StructuralUtil::createSettings(true, true));

      } else  if (tail->getStructuralType() == Structural::SwitchPort){
        properties[STR_PROPERTY_ENTITY_TYPE] = StructuralUtil::translateTypeToString(Structural::Mapping);
        properties[STR_PROPERTY_EDGE_TAIL] = tail->getStructuralUid();
        properties[STR_PROPERTY_EDGE_HEAD] = head->getStructuralUid();

        insert(StructuralUtil::createUid(), parentTail->getStructuralUid(),properties,StructuralUtil::createSettings(true, true));
      }
    }
  }
}

QString StructuralView::getNewId(StructuralEntity *entity)
{
  QString name = "";

  if (entity != NULL) {
    StructuralType type = entity->getStructuralType();

    if (entityCount.contains(type))
      entityCount[type] = 0;

    int n;
    QString prefix;

    n = entityCount.value(type);
    prefix = StructuralUtil::getPrefix(type);

    if (!prefix.isEmpty()) {
      bool contains;

      do {
        contains = false;

        foreach(StructuralEntity* e, _entities.values()) {
          if (e->getStructuralId() == QString(prefix + QString::number(n))) {
            contains = true; ++n;
            break;
          }
        }

      } while(contains);

      name = QString(prefix + QString::number(n));
    }
  }

  return name;
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
  if (_linking) {
    if (_tailHovered != NULL)
      _tailHovered->setHovering(false);

    if (_headHovered != NULL)
      _headHovered->setHovering(false);

    QList<QGraphicsItem*> tailItems = _scene->items(_linkTool->getLine().p1());

    if (tailItems.count() && tailItems.first() == _linkTool) {
      tailItems.removeFirst();
    }

    QList<QGraphicsItem*> headItems = _scene->items(_linkTool->getLine().p2());

    if (headItems.count() && headItems.first() == _linkTool) {
      headItems.removeFirst();
    }

    if (tailItems.count()) {
      _tailHovered = (StructuralEntity*) tailItems.first();

      if (_tailHovered != NULL)
        _tailHovered->setHovering(true);
    }

    if (headItems.count()) {
      _headHovered = (StructuralEntity*) headItems.first();

      if (_headHovered != NULL)
        _headHovered->setHovering(true);
    }

    _linkTool->setLine(QLineF(_linkTool->getLine().p1(),mapToScene(event->pos())));
    _scene->update();
  }

  QGraphicsView::mouseMoveEvent(event);
}

void StructuralView::mousePressEvent(QMouseEvent* event)
{
  if (_mode == Structural::Linking) {
    if (_linkTool != NULL) {
      delete (_linkTool); _linkTool = NULL;
    }

    _linkTool = new StructuralLinkTool(); _scene->addItem(_linkTool);
    _linkTool->setLine(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
    _linkTool->adjust(true);

    _linking = true;

  } else {
    QGraphicsView::mousePressEvent(event);

    if (!event->isAccepted()) {
      event->accept();

      unselect();
    }
  }
}

void StructuralView::mouseReleaseEvent(QMouseEvent* event)
{
  if (_linking) {
    if (_tailHovered != NULL)
      _tailHovered->setHovering(false);

    if (_headHovered != NULL)
      _headHovered->setHovering(false);

    QList<QGraphicsItem *> tailItems = _scene->items(_linkTool->getLine().p1());

    if (tailItems.count() && tailItems.first() == _linkTool)
      tailItems.removeFirst();

    QList<QGraphicsItem*> headtems = _scene->items(_linkTool->getLine().p2());

    if (headtems.count() && headtems.first() == _linkTool)
      headtems.removeFirst();

    if (tailItems.count() && headtems.count()) {
      StructuralEntity* tail = (StructuralEntity*) tailItems.first();
      StructuralEntity* head = (StructuralEntity*) headtems.first();

      if (tail != head) {
        // if linking NODE to NODE
        if (tail->getStructuralCategory() == Structural::Node &&
            head->getStructuralCategory() == Structural::Node) {

          if (tail->getStructuralType() != Structural::Link && head->getStructuralType() != Structural::Link)
            createLink(tail, head);
          else if (tail->getStructuralType() == Structural::Link)
            createBind(tail, head);
          else if (head->getStructuralType() == Structural::Link)
            createBind(tail, head);

        // if linking NODE to INTERFACE
        } else if (tail->getStructuralCategory() == Structural::Node &&
                 head->getStructuralCategory() == Structural::Interface) {

          if (tail->getStructuralType() != Structural::Link)
            createLink(tail, head);
          else
            createBind(tail, head);

        // if linking INTERFACE to NODE
        } else if (tail->getStructuralCategory() == Structural::Interface &&
                 head->getStructuralCategory() == Structural::Node) {

            StructuralEntity* parentTail = tail->getStructuralParent();
            StructuralEntity* parentHead = head->getStructuralParent();

            if (parentTail != NULL && parentHead != NULL && parentTail == parentHead)
              createReference(tail, head);
            else if (head->getStructuralType() != Structural::Link)
              createLink(tail, head);
            else
              createBind(tail, head);

        // if linking INTERFACE to INTERFACE
        } else if (tail->getStructuralCategory() == Structural::Interface &&
                 head->getStructuralCategory() == Structural::Interface) {

            StructuralEntity* parentTail = tail->getStructuralParent();
            StructuralEntity* parentHead = head->getStructuralParent();

            if (parentTail != NULL && parentHead != NULL &&  parentTail == parentHead->getStructuralParent())
              createReference(tail, head);
            else
              createLink(tail, head);
        }
      }
    }

    if (_linkTool != NULL) {
      _scene->removeItem(_linkTool);
      _scene->update();

      delete _linkTool; _linkTool = NULL;
    }

    _linking = false;
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
    unselect();

    setMode(Structural::Linking); emit switchedLink(true);

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

//void StructuralView::focusOutEvent(QFocusEvent *event)
//{
//  StructuralEntity *entity;
//  foreach(entity, _entities.values())
//  {
//    entity->setDraggable(false);
//  }
//}

void StructuralView::clearAllData()
{
  /*
  if(scene->getRoots().size())
    scene->removeRoot(scene->getRoots().at(0));
  */

  unselect();

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

          paste(e, _entities.value(uid), code, false);
        }

      foreach (StructuralEntity* e, entity->getStructuralEntities()) {
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

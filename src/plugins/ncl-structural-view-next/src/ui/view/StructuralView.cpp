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

  setScene(scene); centerOn(3600/2, 1800/2);

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


  QDomElement parent = dom->firstChildElement();

  if (parent.hasChildNodes())
  {


  parent = parent.firstChildElement();

  QMap<QString,QString> properties;

  QDomNodeList list = parent.childNodes();

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

  StructuralType subt = (StructuralType) properties.value(PLG_ENTITY_TYPE).toInt();

  StructuralComposition* ee = NULL;

  if (subt == Structural::Body ||
      subt == Structural::Context ||
      subt == Structural::Switch){
    ee = (StructuralComposition*)entities.value(properties.value(PLG_ENTITY_UID));
  }

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      if (e.nodeName() == "entity")
        read(e, parent);
    }
  }

  centerOn(entities.value(properties.value(PLG_ENTITY_UID)));


  if (ee != NULL){
    settings.insert(PLG_SETTING_NOTIFY,"1");

    // select body to receive mensagens from validator plugin
    select(ee->getStructuralUid(), settings);
  }
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
    if (entity->getStructuralType() == Structural::Body)
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
        entity = new StructuralComposition();
        entity->setStructuralType(Structural::Body);

        emit bodyStateChange(false);
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

        entity->setStructuralParent(entities[parent]);

        if (uncollapseAndThenCollapse)
          ((StructuralComposition*) p)->collapse();


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
      else
      {
        scene->addItem(entity);

        entity->setTop(scene->sceneRect().height()/2 - entity->getHeight()/2);
        entity->setLeft(scene->sceneRect().width()/2 - entity->getWidth()/2);

        centerOn(entity);
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

      if (settings[PLG_SETTING_NOTIFY] == "1")
      {
        emit inserted(uid, parent, entity->getStructuralProperties(), settings);
      }
    }
  }
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

    if (entities[uid]->getStructuralCategory() != Structural::Edge)
    {
      foreach (StructuralEntity* entity, entities[uid]->getStructuralEntities())
      {
        if (settings.value(PLG_SETTING_UNDO_CHILDREN) != "0")
          settings[PLG_SETTING_UNDO] = "1";

          remove(entity->getStructuralUid(), settings);
      }
    }

    StructuralEntity* entity = entities[uid];

    StructuralEntity* parent = entity->getStructuralParent();

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

void StructuralView::change(QString uid, QMap<QString, QString> properties, QMap<QString, QString> previous, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
    if (settings[PLG_SETTING_UNDO] == "1")
    {


      QMap<QString,QString> tmp = previous;

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


    if (settings[PLG_SETTING_NOTIFY] == "1")
    {
      emit changed(uid, properties, previous, settings);
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

        if (_selected != NULL && _selected != entities.value(clip) && !isChild(_selected, entities.value(clip)))
        {
            StructuralEntity* entity = entities.value(clip);

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

    }
    else if (e_clip != NULL)
    {

      performPaste(e_clip, _selected, QUuid::createUuid().toString(), true);
    }
}

void StructuralView::performPaste(StructuralEntity* entity, StructuralEntity* parent, QString CODE, bool newPos)
{
    QString parentuid;

    if (parent == NULL)
    {

      if (entity->getStructuralType() != Structural::Body){

        return;

      }
      else
        parentuid = "";
    }
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

  QGraphicsView::mouseMoveEvent(event);
}

void StructuralView::mousePressEvent(QMouseEvent* event)
{
  QGraphicsView::mousePressEvent(event);

  if (!event->isAccepted())
  {
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

void StructuralView::mouseReleaseEvent(QMouseEvent* event)
{
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

//void StructuralView::setAction(QString action)
//{
//    this->action = action;
//}

//void StructuralView::setCondition(QString condition)
//{
//    this->condition = condition;
//}

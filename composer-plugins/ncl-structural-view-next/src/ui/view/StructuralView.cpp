#include "StructuralView.h"

//
// ATTENTION: This code needs a refactoring.
//

#include <assert.h>
#include <QDebug>

#include "StructuralUtil.h"
#include "StructuralBind.h"

#include "StructuralEdge.h"

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

  _selected_UID = "";

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

  condition = Structural::onBegin;
  action = Structural::Start;

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

  centerOn(3200/2, 1000/2);

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

void StructuralView::load(QString &data)
{
  QDomDocument* dom = new QDomDocument();
  dom->setContent(data);


  QDomElement parent = dom->firstChildElement();

  if (parent.hasChildNodes())
  {


  parent = parent.firstChildElement();

  qDebug() << "==================" << parent.nodeName();

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
  settings["UNDO"] = "0";
  settings["NOTIFY"] = "0";
  settings["CODE"] = StructuralUtil::createUid();

  insert(properties.value("LOCAL:UID"), "", properties, settings);

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      if (e.nodeName() != "property")
        read(e, parent);
    }
  }

  centerOn(entities.value(properties.value("LOCAL:UID")));
  }
}

void StructuralView::read(QDomElement element, QDomElement parent)
{
  qDebug() << "==================" << element.nodeName();

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
  settings["UNDO"] = "0";
  settings["NOTIFY"] = "0";
  settings["CODE"] = StructuralUtil::createUid();

  insert(properties.value("LOCAL:UID"), parent.attributeNode("uid").nodeValue(), properties, settings);

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      if (e.nodeName() != "property")
        read(e, parent);
    }
  }
}

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


    StructuralEntity* entity; LocalName type = (LocalName) properties["LOCAL:NAME"].toInt();

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
        if (entities.contains(properties.value("LOCAL:BEGIN")) && entities.contains(properties.value("LOCAL:END")))
        {
            entity = new StructuralBind();

            StructuralBind* bind = (StructuralBind*) entity;
            bind->setEntityA(entities[properties.value("LOCAL:BEGIN")]);
            bind->setEntityB(entities[properties.value("LOCAL:END")]);

            bind->setType((Structural::BindType) properties.value("LOCAL:BIND").toInt());

            if (properties.value("LOCAL:ANGLE").isEmpty())
            {
              adjustAngles(bind);
              bind->setLocalProperty("LOCAL:ANGLE", QString::number(bind->getAngle()));
            }
            else
            {
              bind->setAngle(properties.value("LOCAL:ANGLE").toDouble());
            }
        }

        break;
      }

      case Structural::Reference:
      {
        if (entities.contains(properties.value("LOCAL:BEGIN")) && entities.contains(properties.value("LOCAL:END")))
        {
            StructuralEntity* a = entities[properties.value("LOCAL:BEGIN")];

            if (a->getLocalName() == Structural::Port)
            {
                foreach (StructuralEntity* e, entities.values()) {
                    if (e->getLocalName() == Structural::Reference)
                    {
                        StructuralReference* r = (StructuralReference*) e;

                        if (e->getLocalProperty("LOCAL:BEGIN") == a->getLocalUid())
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
            bind->setEntityA(entities[properties.value("LOCAL:BEGIN")]);
            bind->setEntityB(entities[properties.value("LOCAL:END")]);

            qDebug() << "MISTICO";
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

        entity->setTop(entities[parent]->getHeight()/2 - entity->getHeight()/2);
        entity->setLeft(entities[parent]->getWidth()/2 - entity->getWidth()/2);
      }
      else
      {
        scene->addItem(entity);

        entity->setTop(scene->sceneRect().height()/2 - entity->getHeight()/2);
        entity->setLeft(scene->sceneRect().width()/2 - entity->getWidth()/2);

        centerOn(entity);
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

//      qDebug() << entity->getLocalProperties();
//      qDebug() << "ANGLE" << entity->getLocalProperty("LOCAL:ANGLE");

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

      qDebug() << "INSERT ============================" << entities.size();
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

      commnads.push(command); return;
    }

    if (entities[uid]->getLocalType() != Structural::Edge)
    {
      foreach (StructuralEntity* entity, entities.values())
      {
          if (entity->getLocalType() == Structural::Edge)
          {
              StructuralEdge* bind = (StructuralEdge*) entity;

              if (bind->getLocalProperty("LOCAL:BEGIN") == uid || bind->getLocalProperty("LOCAL:END") == uid)
              {
                  settings["UNDO"] = "1";
                  settings["NOTIFY"] = "1";
                  remove(bind->getLocalUid(), settings);
              }
          }
      }

      foreach (StructuralEntity* entity, entities[uid]->getLocalChildren())
      {
          settings["UNDO"] = "1";
          settings["NOTIFY"] = "1";

          remove(entity->getLocalUid(), settings);
      }
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

    delete entity; entity = NULL;
     _selected_UID = "";

    if (settings["NOTIFY"] == "1")
    {
      emit removed(uid, settings);
    }

    qDebug() << "REMOVE ============================" << entities.size();
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

     foreach (StructuralEntity* e, entities.values())
     {
         if (e->getLocalType() == Structural::Edge)
         {
             if (e->getLocalProperty("LOCAL:BEGIN") == uid ||
                 e->getLocalProperty("LOCAL:END") == uid)
             {
                e->adjust();
             }
             else
             {
               foreach (StructuralEntity* child, entity->getLocalChildren())
               {
                 if (child->getLocalType() == Structural::Interface)
                 {
                   if (e->getLocalProperty("LOCAL:BEGIN") == child->getLocalUid() ||
                       e->getLocalProperty("LOCAL:END") == child->getLocalUid())
                   {
                     e->adjust();
                   }
                 }
               }
             }
         }
     }

    if (settings["NOTIFY"] == "1")
    {
      emit changed(uid, properties, previous, settings);
    }

    qDebug() << "CHANGE ============================" << entities.size();
  }
}

void StructuralView::select(QString uid, QMap<QString, QString> settings)
{
  if (entities.contains(uid))
  {
      StructuralEntity* entity = entities[uid];
      entity->setSelected(true);

      StructuralEntity* _selected = NULL;

      if (!_selected_UID.isEmpty())
        _selected = entities[_selected_UID];

      if (_selected != NULL && _selected != 0)
      {
        _selected->setSelected(false);
        _selected->adjust();
      }

      _selected_UID = uid;

      // TODO
//      if (settings["NOTIFY"] == "1")
//      {
        emit selected(uid, settings);
//      }
  }
}

bool StructuralView::canUndo()
{
  return commnads.canUndo();
}

bool StructuralView::canRedo()
{
  return commnads.canRedo();
}

void StructuralView::create(LocalName name, QMap<QString, QString> &properties, QMap<QString, QString> &settings)
{
  QString uid = StructuralUtil::createUid();
  QString parent = "";

  switch (name)
  {
    default:
    {
      StructuralEntity* _selected = NULL;

      if (!_selected_UID.isEmpty())
        _selected = entities[_selected_UID];

      if (_selected != NULL)
      {
        parent = _selected->getLocalUid();
      }

      break;
    }
  }

  if (!properties.contains("LOCAL:NAME"))
    properties["LOCAL:NAME"] = QString::number(name);

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
        properties["LOCAL:HIDDEN"] = "TRUE";

        QMap<QString, QString> settings;
        settings["UNDO"] = "1";
        settings["NOTIFY"] = "1";
        settings["CODE"] = StructuralUtil::createUid();

        change(clip_cut, properties, previous, settings);

        _selected->setSelected(false);

        _selected_UID = "";

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
    StructuralEntity* _selected = NULL;

    if (!_selected_UID.isEmpty())
      _selected = entities[_selected_UID];

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

    StructuralEntity* _selected = NULL;

    if (!_selected_UID.isEmpty())
      _selected = entities[_selected_UID];

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
    properties["LOCAL:UID"] = uid;
    properties["LOCAL:HIDDEN"] = "FALSE";


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
  StructuralEntity* _selected = NULL;

  if (!_selected_UID.isEmpty())
    _selected = entities[_selected_UID];

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
        StructuralEntity* _selected = NULL;

        if (!_selected_UID.isEmpty())
          _selected = entities[_selected_UID];

        if (_selected != NULL)
        {
          _selected->setSelected(false);
          _selected->adjust();
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
        if (entitya->getLocalType() == Structural::Node &&
            entityb->getLocalType() == Structural::Node)
        {
            qDebug()  << "if linking NODE to NODE";
            if (entitya->getLocalName() != Structural::Link && entityb->getLocalName() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else if (entitya->getLocalName() == Structural::Link || entityb->getLocalName() == Structural::Link)
            {
                createBind(entitya, entityb);
            }
        }
        // if linking NODE to INTERFACE
        else if (entitya->getLocalType() == Structural::Node &&
                 entityb->getLocalType() == Structural::Interface)
        {
            qDebug()  << "if linking NODE to INTERFACE";
            if (entitya->getLocalName() != Structural::Link)
            {
                createLink(entitya, entityb);
            }
            else
            {
                createBind(entitya, entityb);
            }
        }
        // if linking INTERFACE to NODE
        else if (entitya->getLocalType() == Structural::Interface &&
                 entityb->getLocalType() == Structural::Node)
        {
            qDebug()  << "if linking INTERFACE to NODE";
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
        }
        // if linking INTERFACE to INTERFACE
        else if (entitya->getLocalType() == Structural::Interface &&
                 entityb->getLocalType() == Structural::Interface)
        {
            qDebug()  << "if linking INTERFACE to INTERFACE";
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

    StructuralEntity* pa = (StructuralEntity*) a->getLocalParent();
    StructuralEntity* pb = (StructuralEntity*) b->getLocalParent();

    if (pa != NULL && pb != NULL)
    {
      QString uid = QUuid::createUuid().toString();
      StructuralEntity* parent = NULL;
      QMap<QString, QString> properties;
      properties["LOCAL:NAME"] = QString::number(Structural::Link);

      QPointF pt_a;
      QPointF pt_b;

      if (pa == pb)
      {
          parent = pa;

          pt_a.setX(a->getLeft()), pt_a.setY(a->getTop());
          pt_b.setX(b->getLeft()), pt_b.setY(b->getTop());
      }
      else if (pa->getLocalParent() == pb)
      {
          parent = pb;

          QPointF cv = pa->mapToParent(a->getLeft(), a->getTop());

          pt_a.setX(cv.x()), pt_a.setY(cv.y());
          pt_b.setX(b->getLeft()), pt_b.setY(b->getTop());
      }
      else if (pa == pb->getLocalParent())
      {
          parent = pa;

          QPointF cv = pb->mapToParent(b->getLeft(), b->getTop());

          pt_a.setX(a->getLeft()), pt_a.setY(a->getTop());
          pt_b.setX(cv.x()), pt_b.setY(cv.y());
      }
      else
      {
          return;
      }

      qDebug() << "AX:" << a->getLeft() << "AY:" << a->getTop();
      qDebug() << "BX:" << b->getLeft() << "BY:" << b->getTop();

      qDebug() << "RAX:" << pt_a.x() << "RAY:" << pt_a.y();
      qDebug() << "RBX:" << pt_b.x() << "RBY:" << pt_b.y();

      if (pt_a.y() > pt_b.y())
          properties["LOCAL:TOP"] = QString::number(pt_b.y() + (pt_a.y() - pt_b.y())/2);
      else
          properties["LOCAL:TOP"] = QString::number(pt_a.y() + (pt_b.y() - pt_a.y())/2);


      if (pt_a.x() > pt_b.x())
          properties["LOCAL:LEFT"] = QString::number(pt_b.x() + (pt_a.x() - pt_b.x())/2);
      else
          properties["LOCAL:LEFT"] = QString::number(pt_a.x() + (pt_b.x() - pt_a.x())/2);


      QMap<QString, QString> settings;
      settings["UNDO"] = "1";
      settings["NOTIFY"] = "1";
      settings["CODE"] = QUuid::createUuid().toString();

      insert(uid, parent->getLocalUid(), properties, settings);

      if (entities.contains(uid))
      {
          createBind(a,entities[uid],condition,settings["CODE"]);
          createBind(entities[uid],b,action,settings["CODE"]);
      }
    }

/*
    if (parenta == parentb && parenta != NULL && parentb != NULL)
    {
        qDebug() << "#1 void StructuralView::createLink(StructuralEntity* a, StructuralEntity* b)";

        QString uid = QUuid::createUuid().toString();
        QString parent = parenta->getLocalUid();
        QMap<QString, QString> properties;
        properties["LOCAL:NAME"] = QString::number(Structural::Link);
//        properties["LOCAL:BEGIN"] = a->getLocalUid();
//        properties["LOCAL:END"] = b->getLocalUid();
//        properties["LOCAL:BIND"] = "onBegin";


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
      qDebug() << "#2 void StructuralView::createLink(StructuralEntity* a, StructuralEntity* b)";
      QString uid = QUuid::createUuid().toString();
      QMap<QString, QString> properties;
      properties["LOCAL:NAME"] = QString::number(Structural::Link);
//      properties["LOCAL:BEGIN"] = a->getLocalUid();
//      properties["LOCAL:END"] = b->getLocalUid();
//        properties["LOCAL:BIND"] = "onBegin";


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
*/
//    linking = false;
//    modified = false;
}

void StructuralView::createBind(StructuralEntity* a, StructuralEntity* b, Structural::BindType type, QString code)
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
      properties["LOCAL:NAME"] = QString::number(Structural::Bind);
      properties["LOCAL:BEGIN"] = a->getLocalUid();
      properties["LOCAL:END"] = b->getLocalUid();

      properties["LOCAL:BIND"] = QString::number(type);;

      if (a->getLocalName() == Structural::Link)
      {
//            QnstActionDialog* dialog = new QnstActionDialog();
//            dialog->init();
//            dialog->exec();

//            properties["LOCAL:BIND"] = dialog->form.cbAction->currentText();

          if (type == Structural::NoBindType)
            properties["LOCAL:BIND"] = QString::number(action);


          properties["LOCAL:LINK:ID"] = a->getLocalId();
          properties["LOCAL:LINK:UID"] = a->getLocalUid();

          if (b->getLocalType() == Structural::Interface)
          {
            properties["LOCAL:INTERFACE:ID"] = b->getLocalId();
            properties["LOCAL:COMPONENT:ID"] = pb->getLocalId();
            properties["LOCAL:INTERFACE:UID"] = b->getLocalUid();
            properties["LOCAL:COMPONENT:UID"] = pb->getLocalUid();
          }
          else
          {
            properties["LOCAL:COMPONENT:ID"] = b->getLocalId();
            properties["LOCAL:COMPONENT:UID"] = b->getLocalUid();
          }
      }
      else if (b->getLocalName() == Structural::Link)
      {
//            QnstConditionDialog* dialog = new QnstConditionDialog();
//            dialog->init();
//            dialog->exec();

//            properties["LOCAL:BIND"] = dialog->form.cbCondition->currentText();

        if (type == Structural::NoBindType)
          properties["LOCAL:BIND"] = QString::number(condition);

          properties["LOCAL:LINK:ID"] = b->getLocalId();
          properties["LOCAL:LINK:UID"] = b->getLocalUid();

          if (a->getLocalType() == Structural::Interface)
          {
            properties["LOCAL:INTERFACE:ID"] = a->getLocalId();
            properties["LOCAL:COMPONENT:ID"] = pa->getLocalId();
            properties["LOCAL:INTERFACE:UID"] = a->getLocalUid();
            properties["LOCAL:COMPONENT:UID"] = pa->getLocalUid();
          }
          else
          {
            properties["LOCAL:COMPONENT:ID"] = a->getLocalId();
            properties["LOCAL:COMPONENT:UID"] = a->getLocalUid();
          }
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
  //    modified = false
    }
//    else
//    {
//      QString parent = "";

//      QString uid = QUuid::createUuid().toString();

//      QMap<QString, QString> properties;
//      properties["LOCAL:NAME"] = QString::number(Structural::Bind);
//      properties["LOCAL:BEGIN"] = a->getLocalUid();
//      properties["LOCAL:END"] = b->getLocalUid();

//     properties["LOCAL:BIND"] = type;

//     if (a->getLocalName() == Structural::Link)
//      {
//  //            QnstActionDialog* dialog = new QnstActionDialog();
//  //            dialog->init();
//  //            dialog->exec();

//  //            properties["LOCAL:BIND"] = dialog->form.cbAction->currentText();


//          properties["LOCAL:LINK:ID"] = a->getLocalId();
//          properties["LOCAL:LINK:UID"] = a->getLocalUid();

//          if (b->getLocalType() == Structural::Interface)
//          {
//            properties["LOCAL:INTERFACE:UID"] = b->getLocalUid();
//            properties["LOCAL:COMPONENT:UID"] = pb->getLocalUid();
//          }
//          else
//          {
//            properties["LOCAL:COMPONENT:UID"] = b->getLocalUid();
//          }
//      }
//      else if (b->getLocalName() == Structural::Link)
//      {
//  //            QnstConditionDialog* dialog = new QnstConditionDialog();
//  //            dialog->init();
//  //            dialog->exec();

//  //            properties["LOCAL:BIND"] = dialog->form.cbCondition->currentText();

//          properties["LOCAL:LINK:ID"] = b->getLocalId();
//          properties["LOCAL:LINK:UID"] = b->getLocalUid();

//          if (a->getLocalType() == Structural::Interface)
//          {
//            properties["LOCAL:INTERFACE:UID"] = a->getLocalUid();
//            properties["LOCAL:COMPONENT:UID"] = pa->getLocalUid();
//          }
//          else
//          {
//            properties["LOCAL:COMPONENT:UID"] = a->getLocalUid();
//          }
//      }






  //    linking = false;
  //    modified = false;
//    }

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
            properties["LOCAL:NAME"] = QString::number(Structural::Reference);
            properties["LOCAL:BEGIN"] = a->getLocalUid();
            properties["LOCAL:END"] = b->getLocalUid();

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
    StructuralEntity* _selected = NULL;

    if (!_selected_UID.isEmpty())
      _selected = entities[_selected_UID];

    if (_selected != NULL)
    {
      _selected->setSelected(false);
      _selected->adjust();
    }

    _selected_UID =  "";
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
      StructuralEntity* _selected = NULL;

      if (!_selected_UID.isEmpty())
        _selected = entities[_selected_UID];

      if (_selected != NULL)
      {
        _selected->setSelected(false);
        _selected->adjust();
      }

      _selected_UID =  "";
      modified = true;
    }
    else
    {
      modified = false;
    }
}

void StructuralView::adjustAngles(StructuralBind* edge)
{
  int MIN =  100000;
  int MAX = -100000;

  bool HAS = false;

  bool isCondition = false;

  foreach (StructuralEntity *e, entities)
  {
    if (e->getLocalName() == Structural::Bind)
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

//  if (isCondition == edge->isCondition())
//    edge->setAngle(ANGLE);
//  else
//    edge->setAngle(-ANGLE);

  if (!edge->isCondition())
    edge->setAngle(-ANGLE);
  else
    edge->setAngle(ANGLE);

}

//void StructuralView::setAction(QString action)
//{
//    this->action = action;
//}

//void StructuralView::setCondition(QString condition)
//{
//    this->condition = condition;
//}

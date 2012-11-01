#include "qnstview.h"

//
// ATTENTION: This code needs a refactoring.
//

#include <assert.h>
#include "qnstutil.h"

#include "qnstgraphicsbind.h"

std::map <Qnst::EntityType, QString> QnstView::mediaTypeToXMLStr =
    create_map<Qnst::EntityType, QString>
      (Qnst::Media, "media")
      (Qnst::Image, "image")
      (Qnst::Audio, "audio")
      (Qnst::Video, "video")
      (Qnst::Html, "text/html")
      (Qnst::NCL, "application/x-ginga-NCL")
      (Qnst::Text, "text")
      (Qnst::NCLua, "application/x-ginga-NCLua")
      (Qnst::Settings, "application/x-ncl-settings");

QnstView::QnstView(QWidget* parent)
    : QGraphicsView(parent)
{
  createObjects();
  createConnection();

  zoomStep = 0;

  linking = false;

  modified = false;

  link = NULL;

  selected = NULL;

  clipboard = NULL;

  hasCutted = false;

  lastLinkMouseOver = NULL;

  // Initialize entity counters
  for(int i = Qnst::Node; i < Qnst::NoType; i += 1)
    entityCounter[ (Qnst::EntityType) i ] = 0;
}

QnstView::~QnstView()
{

}

void QnstView::load(QString data)
{
  qDebug() << "###########################################";
  qDebug() << data;
  qDebug() << "###########################################";

  QDomDocument* dom = new QDomDocument();
  dom->setContent(data);

  QDomElement parent = dom->firstChildElement();

  QDomNodeList list = parent.childNodes();

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      read(e, parent);
    }
  }

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      readLink(e, parent);
    }
  }

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      if (e.nodeName() == "context" ||
          e.nodeName() == "switch" ||
          e.nodeName() == "body")
      {
        collapseCompositions(e, parent);
      }
    }
  }

  foreach (QnstGraphicsEntity* entity, entities.values())
  {
    if (entity->isMedia())
    {
      adjustMedia((QnstGraphicsMedia*) entity);
    }

    entity->adjust();
  }

  deletePendingEntities();
}

void QnstView::collapseCompositions(QDomElement element, QDomElement parent)
{
  QDomNodeList list = element.childNodes();

  for (unsigned int i = 0; i < list.length(); i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      if (e.nodeName() == "context" ||
          e.nodeName() == "switch" ||
          e.nodeName() == "body")
      {
        collapseCompositions(e, parent);
      }
    }
  }

  if (element.attribute("collapsed") == "1")
  {
    if(entities.contains(element.attribute("uid")))
    {
      QnstGraphicsComposition* c =
          ((QnstGraphicsComposition*) entities[element.attribute("uid")]);

      c->collapse();

      c->setTop(element.attribute("top").toDouble());
      c->setLeft(element.attribute("left").toDouble());
    }
  }
}

void QnstView::read(QDomElement element, QDomElement parent)
{
  QMap<QString, QString> properties;

  QString uid = element.attribute("uid");

  // \todo before fill the properties, check if we are a valid element.

  // Fill common properties
  properties["TYPE"] = element.nodeName();
  QDomNamedNodeMap attributeMap = element.attributes();

  // copy every attribute from XML to proerties variable
  for (int i = 0; i < attributeMap.count(); ++i)
  {
    QDomNode attribute = attributeMap.item(i);
    properties[attribute.nodeName()] = attribute.nodeValue();
  }
  // end fill

  if (element.nodeName() == "body" ||
      element.nodeName() == "context" ||
      element.nodeName() == "switch" ||
      element.nodeName() == "media" ||
      element.nodeName() == "port" ||
      element.nodeName() == "switchPort" ||
      element.nodeName() == "property" ||
      element.nodeName() == "area")
  {
    if(element.nodeName() == "body")
      addEntity(uid, "", properties, true);
    else
      addEntity(uid, parent.attribute("uid"), properties, true);
  }
  else if (element.nodeName() == "interfaceReference")
  {
// deprecated since version 0.1.2
//    interfaceRefers[element.attribute("interfaceReferUID")] =
//        element.attribute("interfaceOriginUID");
  }
  else if (element.nodeName() == "nodeReference")
  {
    refers[element.attribute("nodeReferUID")] =
        element.attribute("nodeOriginUID");
  }
  else if (element.nodeName() == "importBase")
  {
    importBases[element.attribute("uid")] = element.attribute("connUID");
  }
  else if (element.nodeName() == "connector")
  {
    QnstConnector* conn = new QnstConnector();
    conn->setnstUid(element.attribute("uid"));
    conn->setName(element.attribute("id"));

    QDomNodeList list = element.childNodes();

    for (unsigned int i=0;i<list.length();i++)
    {
      if (list.item(i).isElement())
      {
        QDomElement e = list.item(i).toElement();

        if (e.nodeName() == "condition")
        {
          conn->addCondition(e.attribute("type"), e.attribute("type"));
        }
        else if (e.nodeName() == "action")
        {
          conn->addAction(e.attribute("type"), e.attribute("type"));
        }
        else if (e.nodeName() == "param")
        {
          conn->addParam(e.attribute("uid"), e.attribute("name"));
        }
      }
    }

    connectors[conn->getName()] = conn;
    connectors2[conn->getnstUid()] = conn;

    return;
  }
  // \fixme This is deprecated since version 0.1.3 and is here only for
  // compatibility! We will remove it soon!!
  else if(element.nodeName() == "aggregator")
  {
    // \todo Look for link that point to this aggregatorUID and set its position
    // to the position saved by aggregator element.
  }

  QDomNodeList list = element.childNodes();

  for (unsigned int i=0;i<list.length();i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      read(e, element);
    }
  }
}

void QnstView::readLink(QDomElement element, QDomElement parent)
{
  // \fixme Why this is here, and not inside the read method ?
  QMap<QString, QString> properties;

  if (element.nodeName() == "linkdata" || //linkdata is deprecated since version
                                          // 0.1.3. And is here only for
                                          // compatibility issues.
      element.nodeName() == "link")
  {
    // Fill common properties
    properties["TYPE"] = "link";
    // This block of code is here only for compatibility with older versions,
    // since in older version we were saving with the name "xconnetor"
    if(element.hasAttribute("xconnetor"))
      properties["xconnector"] = element.attribute("xconnetor");
    if(element.hasAttribute("xconnetorUID"))
      properties["xconnectorUID"] = element.attribute("xconnetorUID");
    // end compatibility block of code

    // copy every attribute from XML to proerties variable
    QDomNamedNodeMap attributeMap = element.attributes();
    for (int i = 0; i < attributeMap.count(); ++i)
    {
      QDomNode attribute = attributeMap.item(i);
      properties[attribute.nodeName()] = attribute.nodeValue();
    }
    // end fill

    addEntity(element.attribute("uid"),
              element.attribute("parent"),
              properties,
              true);
  }
  else if (element.nodeName() == "mapping")
  {

    properties["TYPE"] = element.nodeName();
    // copy every attribute from XML to proerties variable
    QDomNamedNodeMap attributeMap = element.attributes();
    for (int i = 0; i < attributeMap.count(); ++i)
    {
      QDomNode attribute = attributeMap.item(i);
      properties[attribute.nodeName()] = attribute.nodeValue();
    }
    // end fill

    addEntity(properties["uid"],
              element.attribute("switchportUid"),
              properties,
              true);
  }
  else if (element.nodeName() == "port")
  {
    if (entities.contains(element.attribute("uid")))
    {
      if (entities.contains(element.attribute("interfaceUID")))
      {
        QnstGraphicsInterface* entitya = (QnstGraphicsInterface*) entities[element.attribute("uid")];
        QnstGraphicsInterface* entityb = (QnstGraphicsInterface*) entities[element.attribute("interfaceUID")];

        if (entitya != entityb)
        {
          QnstGraphicsReference* entity = new QnstGraphicsReference();
          entity->setEntityA(entitya);
          entity->setEntityB(entityb);
          entity->adjust();

          entitya->getnstGraphicsParent()->addnstGraphicsEntity(entity);

          entitya->addnstGraphicsEdge(entity);
          entityb->addnstGraphicsEdge(entity);
        }
      }
      else if (entities.contains(element.attribute("componentUID")))
      {
        QnstGraphicsInterface* entitya =
            (QnstGraphicsInterface*)entities[element.attribute("uid")];

        QnstGraphicsNode* entityb =
            (QnstGraphicsNode*)entities[element.attribute("componentUID")];

        QnstGraphicsReference* entity = new QnstGraphicsReference();
        entity->setEntityA(entitya);
        entity->setEntityB(entityb);
        entity->adjust();

        entitya->getnstGraphicsParent()->addnstGraphicsEntity(entity);

        entitya->addnstGraphicsEdge(entity);
        entityb->addnstGraphicsEdge(entity);
      }
    }
  }
  else if (element.nodeName() == "bind")
  {
    if (!entities.contains(element.attribute("uid")) &&
        links.contains(element.attribute("linkUID")))
    {
      // Fill common properties
      properties["TYPE"] = "bind";
      QDomNamedNodeMap attributeMap = element.attributes();

      // copy every attribute from XML to proerties variable
      for (int i = 0; i < attributeMap.count(); ++i)
      {
        QDomNode attribute = attributeMap.item(i);
        properties[attribute.nodeName()] = attribute.nodeValue();
      }

      addEntity(properties["uid"],
                entities[element.attribute("linkUID")]->getnstParent()->getnstUid(),
                properties,
                true);
      // end fill

      // SET THE BIND PARAMS
      // Add bindParams to the entity that we just added to model
      if(entities.contains(properties["uid"]))
      {
        QnstGraphicsBind *bind =
            dynamic_cast<QnstGraphicsBind *> (entities[properties["uid"]]);

        if(bind)
        {
          QMap<QString, QString> params;
          QMap<QString, QString> name_uid;

          // Load from XML
          QDomNodeList list = element.childNodes();
          for (unsigned int i=0;i<list.length();i++)
          {
            if (list.item(i).isElement())
            {
              QDomElement e = list.item(i).toElement();

              if (e.nodeName() == "param")
              {
                params[e.attribute("name")] = e.attribute("value");
                name_uid[e.attribute("name")] = e.attribute("uid");

                bindParamUIDToBindUID[e.attribute("uid")] = bind->getnstUid();
              }
            }
          }

          // Set bindParams
          bind->setParams(params);
          bind->setNamesUIDs(name_uid);

          connect(bind,
                  SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                  SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

          connect(bind,
                  SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                  SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

        }
      }
    }
    else
    {
      qWarning() << "[QNST] Warning!! Trying to add a bind to a link that \
                    does not exists!! It will be ignored!!";
    }
    //FINISH BIND PARAMS
  }

  QDomNodeList list = element.childNodes();
  for (unsigned int i=0;i<list.length();i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      readLink(e, element);
    }
  }
}

QString QnstView::serialize()
{
  QDomDocument* dom = new QDomDocument();
  QDomElement root = dom->createElement("qnst");

  foreach(QString key, importBases.keys())
  {
    QDomElement e = dom->createElement("importBase");

    e.setAttribute("uid", key);
    e.setAttribute("connUID", importBases[key]);

    root.appendChild(e);
  }

  foreach(QnstConnector* conn, connectors.values())
  {
    assert(conn != NULL);

    QDomElement e = dom->createElement("connector");

    e.setAttribute("id", conn->getName());
    e.setAttribute("uid", conn->getnstUid());

    foreach(QString condition, conn->getConditions())
    {
      QDomElement c = dom->createElement("condition");
      c.setAttribute("type", condition);

      e.appendChild(c);
    }

    foreach(QString action, conn->getActions())
    {
      QDomElement a = dom->createElement("action");
      a.setAttribute("type", action);

      e.appendChild(a);
    }

    foreach(QString key, conn->getParams().keys())
    {
      QDomElement p = dom->createElement("param");

      p.setAttribute("uid", key);
      p.setAttribute("name", conn->getParams()[key]);

      e.appendChild(p);
    }

    root.appendChild(e);
  }

//  // deprecated since version 0.1.2
//  foreach(QString key, interfaceRefers.keys())
//  {
//    QDomElement e = dom->createElement("interfaceReference");

//    e.setAttribute("interfaceReferUID", key);
//    e.setAttribute("interfaceOriginUID", interfaceRefers[key]);

//    root.appendChild(e);
//  }

  foreach(QString key, refers.keys())
  {
    QDomElement e = dom->createElement("nodeReference");

    e.setAttribute("nodeReferUID", key);
    e.setAttribute("nodeOriginUID", refers[key]);

    root.appendChild(e);
  }

  foreach(QnstGraphicsLink* link, links.values())
  {
    QDomElement e = dom->createElement("link"); // Previously from 0.1.3 thi was linkdata

    e.setAttribute("id", link->getnstId());
    e.setAttribute("uid", link->getnstUid());

    e.setAttribute("top", link->getTop());
    e.setAttribute("left", link->getLeft());
    e.setAttribute("width", link->getWidth());
    e.setAttribute("height", link->getHeight());

    // \fixme Be careful, there are cases where even with no body element the
    // links map is not empty!
//    if(link->getnstParent() != NULL)
//    {
      e.setAttribute("parent",
                     link->getnstParent()->getnstUid());
//    }

    e.setAttribute("xconnector", link->getxConnector());
    e.setAttribute("xconnectorUID", link->getxConnectorUID());

    // \fixme This is deprecated since version 0.1.3
    // e.setAttribute("aggregatorUID", link->getAggregatorUID());

    root.appendChild(e);
  }

  foreach(QnstGraphicsEntity* entity, scene->getRoots())
  {
    QDomElement e = dom->createElement("body");

    e.setAttribute("id", entity->getnstId());
    e.setAttribute("uid", entity->getnstUid());

    e.setAttribute("top", entity->getTop());
    e.setAttribute("left", entity->getLeft());
    e.setAttribute("width", entity->getWidth());
    e.setAttribute("height", entity->getHeight());

    e.setAttribute("collapsed", ((QnstGraphicsComposition*) entity)->isCollapsed());

    e.setAttribute("expandWidth", ((QnstGraphicsComposition*) entity)->getLastW());
    e.setAttribute("expandHeight", ((QnstGraphicsComposition*) entity)->getLastH());

    foreach(QnstGraphicsEntity* c, entity->getnstGraphicsEntities())
    {
      write(e, dom, c);
    }

    writeLink(e, dom, entity);

    root.appendChild(e);
  }

  dom->appendChild(root);

  linkWriterAux.clear();

  return dom->toString(4);
}

void QnstView::write(QDomElement element, QDomDocument* dom,
                     QnstGraphicsEntity* entity)
{
  QDomElement e;

  switch(entity->getnstType())
  {
    case Qnst::Body:
      e = dom->createElement("body");

      e.setAttribute("collapsed", ((QnstGraphicsComposition*) entity)->isCollapsed());

      e.setAttribute("expandWidth", ((QnstGraphicsComposition*) entity)->getLastW());
      e.setAttribute("expandHeight", ((QnstGraphicsComposition*) entity)->getLastH());
      break;

    case Qnst::Context:
      e = dom->createElement("context");

      e.setAttribute("collapsed", ((QnstGraphicsComposition*) entity)->isCollapsed());

      e.setAttribute("expandWidth", ((QnstGraphicsComposition*) entity)->getLastW());
      e.setAttribute("expandHeight", ((QnstGraphicsComposition*) entity)->getLastH());
      break;

    case Qnst::Switch:
      e = dom->createElement("switch");

      e.setAttribute("collapsed", ((QnstGraphicsComposition*) entity)->isCollapsed());

      e.setAttribute("expandWidth", ((QnstGraphicsComposition*) entity)->getLastW());
      e.setAttribute("expandHeight", ((QnstGraphicsComposition*) entity)->getLastH());
      break;

    case Qnst::Media:
    case Qnst::Image:
    case Qnst::Audio:
    case Qnst::Video:
    case Qnst::Html:
    case Qnst::NCL:
    case Qnst::Text:
    case Qnst::Settings:
    case Qnst::NCLua:
      e = dom->createElement("media");

      e.setAttribute("type", mediaTypeToXMLStr[entity->getnstType()]);

      if (((QnstGraphicsMedia*) entity)->getSource() != "" )
      {
        e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
      }

      e.setAttribute("refer", ((QnstGraphicsMedia*) entity)->getRefer());
      e.setAttribute("referUID", ((QnstGraphicsMedia*) entity)->getReferUID());
      e.setAttribute("instance", ((QnstGraphicsMedia*) entity)->getInstance());
      break;

    case Qnst::Property:
      e = dom->createElement("property");

      if (((QnstGraphicsInterface*) entity)->isRefer())
          e.setAttribute("isRefer","true");

      break;

    case Qnst::SwitchPort:
      e = dom->createElement("switchPort");
      break;

    case Qnst::Area:
      e = dom->createElement("area");

      if (((QnstGraphicsInterface*) entity)->isRefer())
          e.setAttribute("isRefer","true");

      break;

    case Qnst::Aggregator:
      // \fixme This is deprecated!! Now, aggregator is the same of link!!
      // e = dom->createElement("aggregator");
      break;

    case Qnst::Port:
    {
      e = dom->createElement("port");

      QnstGraphicsPort* pentity = ((QnstGraphicsPort*) entity);

      foreach(QnstGraphicsEdge* edge, pentity->getnstGraphicsEdges())
      {
        if (edge->getnstType() == Qnst::Reference)
        {
          if (edge->getEntityB()->getncgType() == Qncg::Node)
          {
            e.setAttribute("componentUID", edge->getEntityB()->getnstUid());
          }
          else if (edge->getEntityB()->getncgType() == Qncg::Interface)
          {
            e.setAttribute("componentUID", edge->getEntityB()->getnstParent()->getnstUid());
            e.setAttribute("interfaceUID", edge->getEntityB()->getnstUid());
          }
          break;
        }
      }
    }
    break;

    default:
      // do nothing
      break;
  }

  e.setAttribute("id", entity->getnstId());
  e.setAttribute("uid", entity->getnstUid());

  e.setAttribute("top", entity->getTop());
  e.setAttribute("left", entity->getLeft());
  e.setAttribute("width", entity->getWidth());
  e.setAttribute("height", entity->getHeight());

  writeLink(element, dom, entity);

  foreach(QnstGraphicsEntity* c, entity->getnstGraphicsEntities())
  {
    write(e, dom, c);
  }

  element.appendChild(e);
}

void QnstView::writeLink(QDomElement element, QDomDocument* dom,
                         QnstGraphicsEntity* entity)
{
  if (entity->getncgType() == Qncg::Node)
  {
    QnstGraphicsNode* node = (QnstGraphicsNode*) entity;

    qWarning() << node->getnstType();

    foreach(QnstGraphicsEdge* edge, node->getnstGraphicsEdges())
    {
      //We already have this link, so skipping
      if(linkWriterAux.contains(edge->getnstUid())) continue;
      QDomElement e; // the element that will be inserted

      if (edge->getnstType() == Qnst::Condition ||
          edge->getnstType() == Qnst::Action)
      {
        QnstGraphicsBind* bind = dynamic_cast <QnstGraphicsBind*> (edge);

        if(bind)
        {
          e = dom->createElement("bind");
          e.setAttribute("id", bind->getnstId());
          e.setAttribute("uid", bind->getnstUid());

          e.setAttribute("linkUID", bind->getLink()->getnstUid());

          // We are writing a condition
          if (edge->getnstType() ==  Qnst::Condition)
          {
            e.setAttribute("type", "condition");

            e.setAttribute("condition",
                           QnstUtil::getStrFromBindType(
                             bind->getType()) );
          }
          // We are writing an action
          else if(edge->getnstType() == Qnst::Action)
          {
            e.setAttribute("type", "action");
            e.setAttribute("action",
                           QnstUtil::getStrFromBindType(bind->getType()));
          }

          e.setAttribute("componentaUID", bind->getEntityA()->getnstUid());
          e.setAttribute("componentbUID", bind->getEntityB()->getnstUid());

          e.setAttribute("component", bind->getComponent());
          e.setAttribute("componentUid", bind->getComponentUid());

          e.setAttribute("interface", bind->getInterface());
          e.setAttribute("interfaceUid", bind->getInterfaceUid());

          element.appendChild(e);

          linkWriterAux.insert(bind->getnstUid());

          // Write the bind parameters
          if(bind != NULL)
          {
            foreach(QString param, bind->getParams().keys())
            {
              QDomElement ebp = dom->createElement("param");

              ebp.setAttribute("name", param);
              ebp.setAttribute("value", bind->getParams()[param]);
              ebp.setAttribute("uid", bind->getNameUIDs()[param]);

              e.appendChild(ebp);
            }
          }
        }
      }
      else if (edge->getnstType() == Qnst::Mapping)
      {
        QnstGraphicsMapping* link = (QnstGraphicsMapping*) edge;

        e = dom->createElement("mapping");

        e.setAttribute("uid", edge->getnstUid());

        e.setAttribute("switchportUid", link->getSwitchPortUid());
        e.setAttribute("component", link->getComponent());
        e.setAttribute("componentUid", link->getComponentUid());
        e.setAttribute("interface", link->getInterfaceUid());
        e.setAttribute("interfaceUid", link->getInterfaceUid());

        element.appendChild(e);

        linkWriterAux.insert(link->getnstUid());
      }
    }
  }
}

void QnstView::createObjects()
{
  scene = new QnstScene();
  scene->setParent(this);
  scene->setSceneRect(0, 0, 4000, 4000);

  setScene(scene);

  linkDialog = new QnstGraphicsLinkDialog(this);

  conditionDialog = new QnstGraphicsConditionDialog(this);

  actionDialog = new QnstGraphicsActionDialog(this);
}

void QnstView::createConnection()
{
  connect(scene, SIGNAL(undoRequested()), SLOT(performUndo()));
  connect(scene, SIGNAL(redoRequested()), SLOT(performRedo()));

  connect(scene, SIGNAL(cutRequested()), SLOT(performCut()));
  connect(scene, SIGNAL(copyRequested()), SLOT(performCopy()));
  connect(scene, SIGNAL(pasteRequested()), SLOT(performPaste()));

  connect(scene, SIGNAL(deleteRequested()), SLOT(performDelete()));

  connect(scene, SIGNAL(exportRequested()), SLOT(performExport()));

  connect(scene, SIGNAL(zoominRequested()), SLOT(performZoomIn()));
  connect(scene, SIGNAL(zoomoutRequested()), SLOT(performZoomOut()));
  connect(scene, SIGNAL(zoomresetRequested()), SLOT(performZoomReset()));
  connect(scene, SIGNAL(fullscreenRequested()), SLOT(performFullscreen()));

  connect(scene, SIGNAL(entityAdded(QnstGraphicsEntity*)),
                 SLOT(requestEntityAddition(QnstGraphicsEntity*)));
  connect(scene, SIGNAL(entityChanged(QnstGraphicsEntity*)),
                 SLOT(requestEntityChange(QnstGraphicsEntity*)));
  connect(scene, SIGNAL(entityRemoved(QnstGraphicsEntity*)),
                 SLOT(requestEntityRemotion(QnstGraphicsEntity*)));
  connect(scene, SIGNAL(entitySelected(QnstGraphicsEntity*)),
                 SLOT(requestEntitySelection(QnstGraphicsEntity*)));

  connect(scene,
          SIGNAL(entityAboutToChange(QnstGraphicsEntity*,
                                     QMap<QString,QString>)),
          SLOT(requestEntityPreparation(QnstGraphicsEntity*,
                                        QMap<QString,QString>)));
}

bool QnstView::hasEntity(QString uid)
{
  if (entities.contains(uid)  ||
      binds.contains(uid) ||
      links.contains(uid))
  {
    return true;
  }

  return false;
}

QnstEntity* QnstView::getEntity(QString uid)
{
  if (links.contains(uid))
    return links.value(uid);

  else if (binds.contains(uid))
    return binds.value(uid);

  else if (entities.contains(uid))
    return entities.value(uid);

  return NULL;
}

void QnstView::addEntity(const QString uid, const QString parent,
                         const QMap<QString, QString> &properties, bool undo,
                         bool adjust)
{
  // \fixme Move this logic to AddCommand

  bool ok = false;

  Qnst::EntityType type = QnstUtil::getnstTypeFromStr(properties["TYPE"]);
  QnstGraphicsEntity *entityParent = 0;

  if(entities.count(parent))
    entityParent = dynamic_cast<QnstGraphicsNode*> (entities[parent]);

  qDebug() << "[QNST]" << ":" << "Adding entity '" + uid + "'" << properties;

  if(!entityParent &&
     type != Qnst::Body &&
     type != Qnst::Bind) // for bind we will discover the parent after that
  {
    qWarning() << "[QNST] I cannot add an entity without a parent!!!";
    return;
  }

  QnstGraphicsEntity *entity = NULL;

  // \fixme There are a lot of duplicated code among the cases bellow!
  switch (type)
  {
    case Qnst::Body:
      entity = QnstUtil::makeGraphicsEntity(type);

      entity->setnstUid(uid);
      entity->setnstId(properties["id"]);

      entity->setTop(scene->height()/2 - DEFAULT_BODY_HEIGHT/2);
      entity->setLeft(scene->width()/2 - DEFAULT_BODY_WIDTH/2);
      entity->setWidth(DEFAULT_BODY_WIDTH);
      entity->setHeight(DEFAULT_BODY_HEIGHT);

      entity->setProperties(properties);

      scene->addRoot(entity);

      entities[uid] = entity;

      ok = true;

      break;

    case Qnst::Context:
    case Qnst::Switch:
    {
      entity = QnstUtil::makeGraphicsEntity(type, entityParent);
      if(entity != NULL)
      {
        entity->setnstUid(uid);

        if (properties["id"] != "") entity->setnstId(properties["id"]);

        entity->setProperties(properties);
        entities[uid] = entity;

        entity->adjust();

        ok = true;
      }
      break;
    }

    case Qnst::Media:
    {
      entity = QnstUtil::makeGraphicsEntity(type, entityParent);

      if(entity != NULL)
      {
        entity->setnstType(QnstUtil::getnstTypeFromMime(properties["type"]));

        entity->setnstUid(uid);
        entities[uid] = entity;

        // Update the entity properties
        changeEntity(uid, properties, false);

        entity->adjust();

        ok = true;
      }
      break;
    }

    case Qnst::Port:
    {
      entity = QnstUtil::makeGraphicsEntity(type, entityParent);

      if(entity != NULL)
      {
        entity->setnstUid(uid);

        entity->setnstId(properties["id"]);
        entities[uid] = entity;

        // Update the entity properties
        changeEntity(uid, properties);

        ok = true;
      }
      break;
    }

    case Qnst::Link:
    {
      if (!links.contains(uid))
      {
        entity = QnstUtil::makeGraphicsEntity(type, entityParent);
        QnstGraphicsLink *link = dynamic_cast <QnstGraphicsLink*>(entity);

        if(link)
        {
          link->setnstUid(uid);
          link->setProperties(properties);

          if (properties["id"] != "")
            link->setnstId(properties["id"]);

          entities[uid] = entity;
          links[uid] = link;

          entity->adjust();
          adjustLink(link);

          ok = true;
        }
      }
      else
      {
        qWarning() << "[QNST] Warning! Link already added!!";
      }
      break;
    }

    case Qnst::Bind:
    case Qnst::Action:
    case Qnst::Condition:
    {
      if (!binds.contains(uid))
      {
        QnstGraphicsEntity *entitya = NULL, *entityb = NULL;
        QString action = "", condition = "";

        if(entities.contains(properties["linkUID"]))
          entitya = entities[properties["linkUID"]];
        else
          qWarning() << "[QNST] Trying to make a bind that componentaUID \
                        does not exist!";

        if(entities.contains(properties["componentbUID"]))
          entityb = entities[properties["componentbUID"]];
        else
          qWarning() << "[QNST] Trying to make a bind that componentbUID \
                        does not exist!";

        if(properties.contains("action"))
           action = properties["action"];
        else
          condition = properties["condition"];

        if(entitya != NULL)
        {
          if(links.contains(entitya->getnstUid()))
          {
            QnstGraphicsLink *link = links[ entitya->getnstUid() ];

            QnstGraphicsBind* bind = createBind( link,
                                                 entityb,
                                                 action,
                                                 condition,
                                                 false ); // \todo Use makeGraphicsEntity!

            // p.s. The parent set is made inside createBind!
            bind->setnstUid(uid);
            bind->setProperties(properties);
            entities[uid] = bind;
            binds[uid] = bind;

            ok = false;
            changeEntity(bind->getnstUid(), properties, true);
          }
        }
      }
      break;
    }

    case Qnst::BindParam: /* Warning: this is not a QnstEntity subclass */
      if (entities.contains(parent))
      {
        QnstGraphicsBind *bind = dynamic_cast<QnstGraphicsBind *>(entityParent);

        if(bind)
        {
          bind->addParam(uid, properties["name"], properties["value"]);
          bindParamUIDToBindUID[uid] = parent;
        }
        else
        {
          qWarning() << "[QNST] Warning! Trying to add a bind param inside a \
                        not bind parent. This command will be ignored!";
        }
      }
      break;

    case Qnst::SwitchPort:
    {
      entity = QnstUtil::makeGraphicsEntity(type, entityParent);

      entity->setnstUid(uid);

      entity->setnstId(properties["id"]);

      entity->setProperties(properties);
      entities[uid] = entity;

      entity->adjust();

      ok = true;
      break;
    }


    case Qnst::Mapping:
    {
      QnstGraphicsNode* oparent =
          (QnstGraphicsNode*) entityParent->getnstGraphicsParent();

      QnstGraphicsMapping* entity = new QnstGraphicsMapping(oparent);

      entity->setnstUid(uid);
      entity->setSwitchPortUid(parent);

      entity->setProperties(properties);
      oparent->addnstGraphicsEntity(entity);

      entities[entity->getnstUid()] = entity;

      adjustMapping(entity);
      break;
    }

    case Qnst::Aggregator:
    {
      qWarning() << "ERROR!!! Trying to add an Aggregator!! This is not \
                    supported anymore!";
      break;
    }


    case Qnst::Area:
    case Qnst::Property:
    {
      entity = QnstUtil::makeGraphicsEntity(type, entityParent);
      entity->setnstUid(uid);
      entity->setnstId(properties["id"]);

      if (properties.contains("isRefer"))
      {
          ((QnstGraphicsInterface*) entity)->setRefer(true);
      }

      entity->setProperties(properties);
      entities[uid] = entity;

      entity->adjust();

      if (adjust) // update media
      {
        if(entityParent->isMedia())
          adjustMedia((QnstGraphicsMedia*) entityParent);

        // Update the medias that refer to this one.
        foreach (QString key, refers.keys(entityParent->getnstUid()))
        {
          if (entities.contains(key) && entities[key]->isMedia())
              adjustMedia((QnstGraphicsMedia*) entities[key]);
        }
      }

      ok = true;
      break;
    }

    default:
      //do nothing
      break;
  }

  // if the entity type is CONNECTOR
  if (properties["TYPE"] == "causalConnector")
  {
    addConnector(uid, parent, properties);
  }
  // if the entity type is CONDITION
  else if (properties["TYPE"] == "simpleCondition")
  {
    addCondition(uid, parent, properties);
  }
  // if the entity type is ACTION
  else if (properties["TYPE"] == "simpleAction")
  {
    addAction(uid, parent, properties);
  }
  // if the entity type is IMPORTBASE
  else if (properties["TYPE"] == "importBase")
  {
    addImportBase(uid, properties);
  }
  else if (properties["TYPE"] == "connectorParam")
  {
    addConnectorParam(uid, parent, properties);
  }

  if(ok && !undo && entity != NULL)
  {
    QnstAddCommand* cmd = new QnstAddCommand(this, entity);
    history.push(cmd);
  }

//  if (entityCounter.find(type) != entityCounter.end())
//  {
//    entityCounter[type]++;
//  }
}

void QnstView::removeEntity(const QString uid, bool undo, bool rmRef)
{
  // traceEntities();

  // \fixme Move this logic to RemoveCommand
  if (entities.contains(uid))
  {
    QnstGraphicsEntity* entity = entities[uid];
    qWarning() << "[QNST]" << ":" << "Removing entity '"+uid+"'"
             << (int)entity << QnstUtil::getStrFromNstType(entity->getnstType());

    if (entity != NULL)
    {
      Qncg::EntityType qncgType = entity->getncgType();
      bool mustRemoveFromParent = false;

      //handle UNDO: By now, we aren't handling Links!!
      if(entity->getncgType() == Qncg::Node ||
         entity->getncgType() == Qncg::Interface)
      {
        if (!undo)
        {
          QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
          history.push(cmd);
        }
      }

      // We are removing a Node
      switch(qncgType)
      {
        case Qncg::Node:
        {
          QnstGraphicsNode *node = dynamic_cast <QnstGraphicsNode*>(entity);
          if(node)
            removeEdgesOfEntity(node, false);

          // Remove recursively
          foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities())
          {
            qWarning() << "parent = " << (int)entity << entity->getnstType()
                       << "remove child = " << (int) e << e->getncgType();

            if (e->getnstType() != Qnst::Edge &&
                e->getnstType() != Qnst::Condition &&
                e->getnstType() != Qnst::Mapping &&
                e->getnstType() != Qnst::Action &&
                e->getnstType() != Qnst::Reference)
            {
              removeEntity(e->getnstUid(), true, false);
            }
          }

          // If we are removing a link, also remove it from the links map
          if (entity->getnstType() == Qnst::Link ||
              entity->getnstType() == Qnst::Aggregator)
          {
            if (links.contains(entity->getnstUid()))
            {
              links.remove(entity->getnstUid());
            }
          }

          mustRemoveFromParent = true;
          break;
        }

        case Qncg::Interface:
        {
          QnstGraphicsInterface *interface =
              dynamic_cast <QnstGraphicsInterface*>(entity);

          if(interface)
            removeEdgesOfEntity(interface, false); // Remove the edges of this entity
          else
            qWarning() << "Could not make a cast to QnstGraphicsInterface*";

          mustRemoveFromParent = true;
          break;
        }

        case Qncg::Edge:
        {
          QnstGraphicsEdge *edge = dynamic_cast<QnstGraphicsEdge *>(entity);

          if(edge)
            removeEdge(edge, false);
          break;
        }

        default:
          //do nothing ?
          mustRemoveFromParent = true;
          break;
      }

      // Everything is ok, now only remove the entity from its parent
      if(mustRemoveFromParent)
      {
        //Remove the entity of its parent
        QnstGraphicsEntity* parent = entity->getnstGraphicsParent();
        if (parent != NULL)
        {
          qDebug() << "parent = " << (int)parent << parent->getnstUid();
          parent->removenstGraphicsEntity(entity);
        }
        else
          scene->removeRoot(entity);

        if (selected == entity)
          selected = NULL;

        if (clipboard == entity)
          clipboard = NULL;

        if (entity->getnstType() == Qnst::Link &&
            links.contains(entity->getnstUid()))
        {
          links.remove(entity->getnstUid());
        }

        entities.remove(entity->getnstUid());

        // emit entityRemoved(entity->getnstUid());
        entity->deleteLater();


        // Update refer entities
        if (qncgType == Qncg::Interface)
        {
          qDebug() << "I am Qncg::Interface" << uid;
          if (refers.contains(uid))
          {
            qDebug() << "Only remove from refer";
            refers.remove(uid);
          }
          else
          {
            qDebug() << "Calling adjust... :P" << uid;
            qDebug() << "Before adjustMedia";
            QnstGraphicsMedia* parentMedia
                = dynamic_cast<QnstGraphicsMedia*>(parent);
            if (parentMedia &&
                entities.contains(parentMedia->getnstUid()))
            {
              // adjustMedia(parentMedia);

              foreach(QString rUid, refers.keys(parentMedia->getnstUid()))
              {
                if (entities.contains(rUid))
                {
                  QnstGraphicsMedia* referMedia =
                      dynamic_cast <QnstGraphicsMedia*>(entities.value(rUid));

                  if (referMedia)
                  {
                    qDebug() << "referMedia is ok, so calling adjustMedia";
                    adjustMedia(referMedia);
                  }
                }
              }
            }

            qDebug() << "After adjust... :P";
          }
        }
        else
        {
          qDebug() << "Well, I am not inside refer :P...";
          foreach(QString ruid, refers.keys(uid))
          {
            refers.remove(ruid);

            if (entities.contains(ruid))
            {
              QnstGraphicsEntity* e = entities.value(ruid);

              if (e->isMedia())
                adjustMedia((QnstGraphicsMedia*) e);
            }
          }

          if (refers.contains(uid))
          {
            qDebug() << "Removing" + uid + "from refers.";
            refers.remove(uid);
          }
        }
        // End update
      }
    }
  }
  else if (importBases.contains(uid))
  {
    foreach(QnstConnector* cc, connectors.values())
    {
      if (cc->getnstUid() == importBases[uid])
        connectors.remove(cc->getName());
    }

    foreach(QnstConnector* cc, connectors2.values())
    {
      if (cc->getnstUid() == importBases[uid])
        connectors2.remove(cc->getnstId());
    }

    importBases.remove(uid);

    // foreach(QnstGraphicsBind* b, binds.values())
    // {
    //  adjustBind(b);
    // }
  }
  else if (connectors2.contains(uid))
  {
    connectors.remove(connectors2[uid]->getName());
    connectors2.remove(uid);
  }
  else if (bindParamUIDToBindUID.contains(uid))
  {
    if(bindParamUIDToBindUID.contains(uid))
    {
      if(entities.contains(bindParamUIDToBindUID[uid]))
      {
        QnstGraphicsEntity* e = entities[bindParamUIDToBindUID[uid]];

        QnstGraphicsBind *bind = dynamic_cast<QnstGraphicsBind*>(e);
        if (e)
        {
          bind->removeUId(uid);
        }
        else
        {
          qWarning() << "[QNST] Warning! The parent of bindParam is not a \
                        bind!";
        }
      }
      else
      {
        qWarning() << "[QNST] Warning! Trying to remove a bindParam that is \
                      mapped inside an entity!";
      }

      // We will remove it from map does not matter what!
      bindParamUIDToBindUID.remove(uid);
    }
    else
      qWarning() << "[QNST] Warning! Trying to remove a bindParam that is \
                    not inside bindParamUIDToBindUID";
  }
  else
  {
    qDebug() << "Warning! Calling removeEntity to a entity that does not exist in view!";
  }

  deletePendingEntities();
}

void QnstView::changeEntity(const QString uid,
                            const QMap<QString, QString> &properties,
                            bool adjust)
{
  qDebug() << "[QNST]" << ":" << "Changing entity '"+uid+"'"
           << properties;

  if (selected != NULL)
  {
    selected->setSelected(false);
    selected->adjust();
  }
  selected = NULL;

  if (links.contains(uid))
  {
    changeLink(links[uid], properties);
  }
  else if (entities.contains(uid))
  {
    QnstGraphicsEntity* entity = entities[uid];

    switch(entity->getnstType())
    {
      case Qnst::Body:
      {
        entity->setnstId(properties["id"]);
        entity->setProperties(properties);

        entity->adjust();
        break;
      }

      case Qnst::Context:
      case Qnst::Switch:
      {
        if (properties["id"] != "")
          entity->setnstId(properties["id"]);

        entity->setProperties(properties);
        // \todo refer
        break;
      }

      // if the entity type is MEDIA
      case Qnst::Audio:
      case Qnst::Text:
      case Qnst::Video:
      case Qnst::Image:
      case Qnst::NCLua:
      case Qnst::Settings:
      case Qnst::Media:
      case Qnst::Html:
      case Qnst::NCL:
      {
        QnstGraphicsMedia *media = dynamic_cast <QnstGraphicsMedia *>(entity);
        if(media)
        {
          media->setnstId(properties["id"]);
          media->setProperties(properties);

          if (properties["referUID"] != "")
          {
            if (entities.contains(properties["referUID"]))
              media->setSource(((QnstGraphicsMedia*)
                                entities[properties["referUID"]])->getSource());
          }

          QString src = media->getSource();
          media->setnstType(QnstUtil::getnstTypeFromMime(properties["type"]));

          if(src == "")
          {
            // do nothing
          }
          else {
            int start = src.lastIndexOf(".");
            if(start >= 0)
            {
              QString ext = QFileInfo(src).suffix().toLower();
              media->setnstType(QnstUtil::getnstTypeFromExtension(ext));
            }
          }

          media->updateToolTip();

          if(adjust)
          {
            adjustMedia(media);

            // \fixme It's not necessary to adjust all medias
//            foreach (QnstGraphicsEntity* e, entities.values())
//            {

//              QnstGraphicsMedia* m = dynamic_cast<QnstGraphicsMedia*>(e);
//              if (m)
//                adjustMedia(m);

//            }
          }
        }
        break;
      }

      case Qnst::Port:
      {
        entity->setnstId(properties["id"]);

        entity->setProperties(properties);

        foreach (QString key, refers.keys(entity->getnstUid()))
        {
          if (entities.contains(key))
            entities[key]->setnstId(entity->getnstId());
        }

        QnstGraphicsPort *port = dynamic_cast<QnstGraphicsPort*> (entity);
        // \todo updateToolTip!!
        if(port)
          adjustPort(port);
      }
        break;

      case Qnst::Area:
      case Qnst::Property:
      {
        entity->setnstId(properties["id"]);

        entity->setProperties(properties);

        foreach (QString key, refers.keys(entity->getnstUid()))
        {
          if (entities.contains(key))
            entities[key]->setnstId(entity->getnstId());
        }
        break;
      }

      case Qnst::SwitchPort:
      {
        if(properties.contains("id"))
          entity->setnstId(properties["id"]);

        entity->setProperties(properties);

        entity->adjust();
        break;
      }

      case Qnst::Mapping:
      {
        entity->setProperties(properties);

        QnstGraphicsMapping *mapping = dynamic_cast<QnstGraphicsMapping*> (entity);
        adjustMapping(mapping);
        break;
      }

      case Qnst::Link:
      {
        entity->setProperties(properties);
        // \todo Update Link!!
        break;
      }

      case Qnst::Bind:
      case Qnst::Action:
      case Qnst::Condition:
      {
        QnstGraphicsBind *bind = dynamic_cast<QnstGraphicsBind *> (entity);
        qWarning() << "[QNST] Changing BIND!!!";

        if(bind)
        {
          qWarning() << "[QNST] cast??";
          qWarning() << properties;
          bind->setProperties(properties);

          adjustBind(bind);
        }
        break;
      }

      default:
        // do nothing
        break;
    }
  }
  else
  {
    qWarning() << "Trying to change an Entity that does not exist!";

    if (properties["TYPE"] == "importBase")
    {
      changeImportBase(uid, properties);
    }
    else if (connectors2.contains(uid))
    {
      changeConnector(connectors2[uid], properties);
    }
    else if (properties["TYPE"] == "simpleCondition")
    {
      changeCondition(uid, properties);
    }
    else if (properties["TYPE"] == "simpleAction")
    {
      changeAction(uid, properties);
    }
    else if (properties["TYPE"] == "bindParam")
    {
      changeBindParam(uid, properties);
    }
    else if (properties["TYPE"] == "connectorParam")
    {
      changeConnectorParam(uid, properties);
    }

  }

  deletePendingEntities();
}

void QnstView::selectEntity(const QString uid)
{
  qDebug() << "[QNST]" << ":" << "Selecting entity '"+uid+"'";

  if (entities.contains(uid))
  {
    QnstGraphicsEntity* entity = entities[uid];

    if (entity != selected)
    {
      if (selected != NULL)
      {
        selected->setSelected(false);
        selected->adjust();
      }

      selected = entity;

      selected->setSelected(true);
      selected->adjust();
    }
  }
}

void QnstView::addImportBase(QString uid,
                             const QMap<QString, QString> &properties)
{
  QString connUID = QUuid::createUuid().toString();

  importBases[uid] = connUID;

  if (properties["documentURI"] != "" &&
      properties["projectURI"] != "" &&
      properties["alias"] != "")
  {
    int n = properties["projectURI"].lastIndexOf("/");

    QFile* file = new QFile(properties["projectURI"].left(n)+QDir::separator()+properties["documentURI"]);

    if (file->exists())
    {
      if (file->open(QIODevice::ReadOnly))
      {
        QDomDocument* domdoc = new QDomDocument();
        if (domdoc->setContent(file))
          readImportBase(connUID, domdoc->firstChildElement(), properties["alias"]);
      }
    }

    delete file;

    // foreach(QnstGraphicsBind* b, binds.values())
    // {
    //   adjustBind(b);
    // }
  }
}


void QnstView::changeImportBase(QString uid,
                                const QMap<QString, QString> &properties)
{
  foreach(QnstConnector* cc, connectors.values())
  {
    if (cc->getnstUid() == importBases[uid])
      connectors.remove(cc->getName());
  }

  foreach(QnstConnector* cc, connectors2.values())
  {
    if (cc->getnstUid() == importBases[uid])
      connectors2.remove(cc->getnstId());
  }

  if (properties["documentURI"] != "" &&
      properties["projectURI"] != "" &&
      properties["alias"] != "")
  {
    int n = properties["projectURI"].lastIndexOf("/");
    QFile* file = new QFile(properties["projectURI"].left(n)+QDir::separator()+properties["documentURI"]);

    if (file->exists())
    {
      if (file->open(QIODevice::ReadOnly))
      {

        QDomDocument* domdoc = new QDomDocument();

        if (domdoc->setContent(file))
        {
          readImportBase(importBases[uid], domdoc->firstChildElement(), properties["alias"]);
        }
      }
    }

    delete file;
  }

  // foreach(QnstGraphicsBind* b, binds.values())
  // {
  //  adjustBind(b);
  // }
}

void QnstView::readImportBase(QString uid, QDomElement element, QString alias)
{
  if (element.tagName() == "causalConnector")
  {
    QDomNodeList list = element.childNodes();

    QnstConnector* conn = new QnstConnector();
    conn->setnstId(alias+"#"+element.attribute("id"));
    conn->setName(alias+"#"+element.attribute("id"));
    conn->setnstUid(uid);

    for (unsigned int i=0;i<list.length();i++)
    {
      if (list.item(i).isElement())
      {
        QDomElement e = list.item(i).toElement();

        readConnector(e, conn);
      }
    }

    connectors[conn->getName()] = conn;
    connectors2[conn->getnstId()] = conn;

  }
  else
  {
    QDomNodeList list = element.childNodes();

    for (unsigned int i=0;i<list.length();i++)
    {
      if (list.item(i).isElement())
      {
        QDomElement e = list.item(i).toElement();

        readImportBase(uid, e, alias);
      }
    }
  }
}

void QnstView::readConnector(QDomElement element, QnstConnector* conn)
{
  if (element.tagName() == "connectorParam")
  {
    if (element.attribute("name") != "")
      conn->addParam(QUuid::createUuid().toString(), element.attribute("name"));
  }

  if (element.tagName() == "simpleCondition" ||
      element.tagName() == "attributeAssessment")
  {
    if (element.attribute("role") != "")
      conn->addCondition(element.attribute("role"), element.attribute("role"));
  }

  if (element.tagName() == "simpleAction")
  {
    if (element.attribute("role") != "")
    {
      conn->addAction(element.attribute("role"), element.attribute("role"));
    }
  }

  QDomNodeList list = element.childNodes();

  for (unsigned int i=0;i<list.length();i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();
      readConnector(e, conn);
    }
  }
}

void QnstView::adjustContext(QnstGraphicsContext* entity)
{

}

// \todo refactory
void QnstView::adjustMedia(QnstGraphicsMedia* entity)
{
  QString referUID = entity->getReferUID();
  QString refer = entity->getRefer();

  if (!refer.isEmpty() && !referUID.isEmpty())
  {
    if (entities.contains(referUID)) // media already refer to other media
    {
      qWarning() << "Media already refer to other media";
      QnstGraphicsMedia* origin =
          dynamic_cast<QnstGraphicsMedia*>(entities[referUID]);

      if (origin)
      {
        entity->setnstType(origin->getnstType());
        entity->setSource(origin->getSource());

        // cleaning interface without origin
        foreach(QnstGraphicsEntity* rinterface,
                entity->getnstGraphicsEntities())
        {
          if (rinterface->getncgType() == Qncg::Interface)
          {
            if (refers.contains(rinterface->getnstUid()))
            {
              bool validReference = false;

              foreach(QnstGraphicsEntity* interface,
                      origin->getnstGraphicsEntities())
              {
                if (interface->getnstUid() == refers[rinterface->getnstUid()])
                {
                  validReference = true;
                  break;
                }
              }

              if (!validReference)
              {
                entity->removenstGraphicsEntity(rinterface);

                refers.remove(rinterface->getnstUid());
                entities.remove(rinterface->getnstUid());
                toDelete.push_back(rinterface);
              }
            }
          }
        }

        // copying interfaces from origin to entity
        foreach (QnstGraphicsEntity* child, origin->getnstGraphicsEntities())
        {
          bool hasRefer = false;
          QnstGraphicsInterface *interface = dynamic_cast <QnstGraphicsInterface*> (child);

          if(interface && !interface->isRefer())
          {
            foreach(QnstGraphicsEntity* rinterface,
                    entity->getnstGraphicsEntities())
            {
              if (refers.value(rinterface->getnstUid()) == interface->getnstUid() ||
                  refers.value(interface->getnstUid()) == rinterface->getnstUid())
              {
                hasRefer = true;
                break;
              }
            }

            if (!hasRefer)
            {
              QnstGraphicsInterface* copy = (QnstGraphicsInterface*)
                  QnstUtil::makeGraphicsEntity(interface->getnstType(), entity);

              copy->setnstId(interface->getnstId());

              copy->setTop(interface->getTop());
              copy->setLeft(interface->getLeft());
              copy->setWidth(interface->getWidth());
              copy->setHeight(interface->getHeight());
              copy->setRefer(true);
              copy->adjust();

              refers[copy->getnstUid()] = interface->getnstUid();
              entities[copy->getnstUid()] = copy;
            }
          }
        }

        if (entity->getInstance() == "instSame")
        {
          //cleaning
          foreach(QnstGraphicsEntity* interface,
                    origin->getnstGraphicsEntities())
          {
            if (interface->getncgType() == Qncg::Interface)
            {
              if (refers.contains(interface->getnstUid()))
              {
                bool validReference = false;

                foreach(QnstGraphicsEntity* rinterface,
                          entity->getnstGraphicsEntities())
                {
                  if (rinterface->getnstUid() == refers[interface->getnstUid()])
                  {
                    validReference = true;
                    break;
                  }
                }

                if (!validReference)
                {
                  origin->removenstGraphicsEntity(interface);

                  refers.remove(interface->getnstUid());
                  entities.remove(interface->getnstUid());

//                  delete interface;
                  toDelete.push_back(interface);
                }
              }
            }
          }

          foreach(QnstGraphicsEntity* rinterface,
                  entity->getnstGraphicsEntities())
          {
            if (!refers.contains(rinterface->getnstUid()))
            {
              bool containsRef = false;

              foreach(QnstGraphicsEntity* interface,
                        origin->getnstGraphicsEntities())
              {
                if (refers.value(interface->getnstUid(),"NULL") == rinterface->getnstUid())
                {
                  containsRef = true;
                  break;
                }
              }

              if (!containsRef)
              {
                QnstGraphicsInterface* copy = (QnstGraphicsInterface*)
                    QnstUtil::makeGraphicsEntity(rinterface->getnstType(), origin);

                copy->setnstId(rinterface->getnstId());

                copy->setTop(rinterface->getTop());
                copy->setLeft(rinterface->getLeft());
                copy->setWidth(rinterface->getWidth());
                copy->setHeight(rinterface->getHeight());
                copy->setRefer(true);
                copy->adjust();

                refers[copy->getnstUid()] = rinterface->getnstUid();
                entities[copy->getnstUid()] = copy;
              }
            }
          }
        }
        else
        {
          foreach(QnstGraphicsEntity* interface, origin->getnstGraphicsEntities())
          {
            if (refers.contains(interface->getnstUid()))
            {
              bool containsRef = false;

              foreach(QnstGraphicsEntity* rinterface,
                      entity->getnstGraphicsEntities())
              {
                if (refers.value(interface->getnstUid()) == rinterface->getnstUid())
                {
                  containsRef = true;
                  break;
                }
              }

              if (containsRef)
              {
                origin->removenstGraphicsEntity(interface);

                refers.remove(interface->getnstUid());
                entities.remove(interface->getnstUid());
                toDelete.push_back(interface);
              }
            }
          }
        }

        refers[entity->getnstUid()] = origin->getnstUid();
      }
    }
    else
    {
      foreach(QnstGraphicsEntity* interface, entity->getnstGraphicsEntities())
      {
        QnstGraphicsInterface* i =
            dynamic_cast<QnstGraphicsInterface*>(interface);

        if (i != 0)
        {
          if (i->isRefer())
          {
            entity->removenstGraphicsEntity(interface);

            refers.remove(interface->getnstUid());
            entities.remove(interface->getnstUid());
            //              delete interface;
            toDelete.push_back(interface);
          }
        }
      }

      entity->setnstType(Qnst::Media);
      entity->setSource("");
      entity->setReferUID("");
    }
  }
  else // I do not refer to any media, so remove all references
  {
    qWarning() << "I do not refer to any media, so remove all references";
    if (refers.contains(entity->getnstUid()))
    {
      qWarning() << "Refers.contains" << entity->getnstUid();
      foreach(QnstGraphicsEntity* interface, entity->getnstGraphicsEntities())
      {
        if (refers.contains(interface->getnstUid()))
        {
          entity->removenstGraphicsEntity(interface);

          refers.remove(interface->getnstUid());
          entities.remove(interface->getnstUid());

//          delete interface;
          toDelete.push_back(interface);
        }
        else
        {
          if (refers.contains(entity->getnstUid()))
          {
            foreach(QString rUid, refers.keys(interface->getnstUid()))
            {
              if (entities.contains(rUid))
              {
                QnstGraphicsEntity* e = entities.value(rUid);
                QnstGraphicsEntity* p = e->getnstGraphicsParent();

                p->removenstGraphicsEntity(e);

                refers.remove(e->getnstUid());
                entities.remove(e->getnstUid());

//                delete e;
                toDelete.push_back(e);
              }
            }
          }
        }
      }
    }
    else
    {
      qWarning() << "Refers does not contains" << entity->getnstUid();
      if (refers.keys(entity->getnstUid()).size() == 0)
      {
        foreach(QnstGraphicsEntity* interface, entity->getnstGraphicsEntities())
        {
          QnstGraphicsInterface* i =
              dynamic_cast<QnstGraphicsInterface*>(interface);

          if (i)
          {
            if (i->isRefer())
            {
              entity->removenstGraphicsEntity(interface);

              entities.remove(interface->getnstUid());
              refers.remove(interface->getnstUid());

//              delete interface;
              toDelete.push_back(interface);
            }
          }
        }
      }
    }

    if (refers.contains(entity->getnstUid()))
    {
      entity->setnstType(Qnst::Media);
      entity->setSource("");

      refers.remove(entity->getnstUid());
    }
  }

  entity->adjust();
}

// \todo refactory
void QnstView::adjustPort(QnstGraphicsPort* entity)
{
  QnstGraphicsEntity* parent = entity->getnstGraphicsParent();

  if (parent != NULL)
  {
    // removing previous edge
    foreach(QnstGraphicsEntity* e, parent->getnstGraphicsEntities())
    {
      if (e->getnstType() == Qnst::Reference)
      {
        QnstGraphicsEdge* edge = (QnstGraphicsEdge*) e;

        if (entity == edge->getEntityA())
        {
          if (edge->getEntityA()->getncgType() == Qncg::Node ||
              edge->getEntityA()->getncgType() == Qncg::Interface )
          {
            ((QnstGraphicsEntityWithEdges*) edge->getEntityA())->removenstGraphicsEdge(edge);
          }

          if (edge->getEntityB()->getncgType() == Qncg::Node ||
              edge->getEntityB()->getncgType() == Qncg::Interface)
          {
            ((QnstGraphicsEntityWithEdges*) edge->getEntityB())->removenstGraphicsEdge(edge);
          }

          parent->removenstGraphicsEntity(edge);
          delete (edge);
          break;
        }
      }
    } // end foreach

    // adding new edge
    if (entity->getInterfaceUid() != "")
    {
      // changing
      if (entities.contains(entity->getInterfaceUid()))
      {
        QnstGraphicsEntity* entitya = (QnstGraphicsEntity*) entity;
        QnstGraphicsEntity* entityb = (QnstGraphicsEntity*) entities[entity->getInterfaceUid()];

        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL)
        {
          if (entitya != entityb && parenta == parentb->getnstGraphicsParent())
          {
              QnstGraphicsReference* edge = new QnstGraphicsReference();
              edge->setEntityA(entitya);
              edge->setEntityB(entityb);
              edge->adjust();

              parenta->addnstGraphicsEntity(edge);

              ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(edge);
              ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(edge);
          }
        }
      }
    }
    else if (entity->getComponentUid() != "")
    {
      // changing
      if (entities.contains(entity->getComponentUid()))
      {
        QnstGraphicsEntity* entitya = (QnstGraphicsEntity*) entity;
        QnstGraphicsEntity* entityb = (QnstGraphicsEntity*) entities[entity->getComponentUid()];

        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL)
        {
          if (entitya != entityb && parenta == parentb)
          {
            QnstGraphicsReference* edge = new QnstGraphicsReference();
            edge->setEntityA(entitya);
            edge->setEntityB(entityb);
            edge->adjust();

            parenta->addnstGraphicsEntity(edge);

            ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(edge);
            ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(edge);
          }
        }
      }
    }
  }

  entity->adjust();
}

// \todo refactory
void QnstView::adjustMapping(QnstGraphicsMapping* entity)
{
  if (entities.contains(entity->getComponentUid()) &&
      entities.contains(entity->getSwitchPortUid()))
  {
    QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

    if (edge->getEntityA() != NULL)
    {
      if (edge->getEntityA()->getncgType() == Qncg::Node ||
          edge->getEntityA()->getncgType() == Qncg::Interface )
      {
        ((QnstGraphicsEntityWithEdges*) edge->getEntityA())->removenstGraphicsEdge(edge);
      }
    }

    if (edge->getEntityB() != NULL)
    {
      if (edge->getEntityB()->getncgType() == Qncg::Node ||
          edge->getEntityB()->getncgType() == Qncg::Interface )
      {
        ((QnstGraphicsEntityWithEdges*) edge->getEntityB())->removenstGraphicsEdge(edge);

      }
    }

    if(entities.contains(entity->getSwitchPortUid()))
    {
      QnstGraphicsEntity* parent =
          entities[entity->getSwitchPortUid()]->getnstGraphicsParent();
      parent->addnstGraphicsEntity(edge);

      if (!parent->getnstGraphicsEntities().contains(edge))
        parent->addnstGraphicsEntity(edge);

      edge->setEntityA(entities[entity->getSwitchPortUid()]);

      if (entity->getInterfaceUid() != "" &&
          entities.contains(entity->getInterfaceUid()))
      {
        edge->setEntityB(entities[entity->getInterfaceUid()]);
      }
      else
      {
        edge->setEntityB(entities[entity->getComponentUid()]);
      }

      ((QnstGraphicsInterface*) edge->getEntityA())->addnstGraphicsEdge(entity);
      ((QnstGraphicsNode*) edge->getEntityB())->addnstGraphicsEdge(entity);

      edge->adjust();
    }
  }
  // entities does not contains componentUID or switchPortUID
  else
  {
    QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

    if (edge->getEntityA() != NULL)
    {
      if (edge->getEntityA()->getncgType() == Qncg::Node ||
          edge->getEntityA()->getncgType() == Qncg::Interface )
      {
        ((QnstGraphicsEntityWithEdges*) edge->getEntityA())->removenstGraphicsEdge(edge);
      }
    }

    if (edge->getEntityB() != NULL)
    {
      if (edge->getEntityB()->getncgType() == Qncg::Node ||
          edge->getEntityB()->getncgType() == Qncg::Interface )
      {
        ((QnstGraphicsEntityWithEdges*) edge->getEntityB())->removenstGraphicsEdge(edge);
      }
    }

    QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
    parent->removenstGraphicsEntity(edge);
  }
}

void QnstView::changeLink(QnstGraphicsLink* entity,
                          const QMap<QString, QString> &properties)
{
  if (entity != NULL)
  {
    if (properties["id"] != "")
    {
      entity->setnstId(properties["id"]);
    }

    if (properties["xconnector"] != "")
    {
      entity->setxConnector(properties["xconnector"]);
      entity->setxConnectorUID(properties["xconnectorUID"]);
    }

    adjustLink(entity);
  }
}

void QnstView::adjustLink(QnstGraphicsLink* entity)
{
  // TODO
}

void QnstView::adjustBind(QnstGraphicsBind* bind)
{
  QnstGraphicsLink *link = NULL;
  QnstGraphicsEntity *target = NULL;

  link = bind->getLink();

  // set the connector of bind
  if(connectors.contains(link->getxConnector()))
    bind->setConn(connectors[link->getxConnector()]);

  if(bind->getInterfaceUid() != "" && entities.contains(bind->getInterfaceUid()))
    target =  entities[bind->getInterfaceUid()];
  else if(entities.contains(bind->getComponentUid()))
    target = entities[bind->getComponentUid()];

  qWarning() << "adjustBind "<< (int) link << (int) target;

  //Remove old
  if(bind->getTarget())
  {
    ((QnstGraphicsEntityWithEdges*)bind->getTarget())->removenstGraphicsEdge(bind);
  }

  //Set new
  if(target)
    ((QnstGraphicsEntityWithEdges*)target)->addnstGraphicsEdge(bind);

  if(link)
  {
    link->removenstGraphicsEdge(bind);
    link->addnstGraphicsEdge(bind);
  }

  bind->setLink(link);
  bind->setTarget(target);

  // adjusting bind
  adjustAngles(bind);

  //check if the bind is valid
  if(link &&
     connectors.contains(link->getxConnector()))
  {
    QnstConnector *connector = connectors[link->getxConnector()];
    if(!connector->getConditions().contains(bind->getRole()) &&
       !connector->getActions().contains(bind->getRole()))
      bind->setInvalid(true);
    else
      bind->setInvalid(false);
  }
  else
    bind->setInvalid(true);

  bind->adjust();
  // \todo interface
}

void QnstView::addConnector(const QString uid, const QString parent,
                            const QMap<QString, QString> &properties)
{
  if (!connectors2.contains(uid))
  {
    QnstConnector* entity = new QnstConnector();
    entity->setnstUid(uid);

    if (properties["id"] != "")
    {
      entity->setName(properties["id"]);
      entity->setnstId(properties["id"]);
    }

    connectors2[uid] = entity;

    foreach(QnstConnector* c, connectors.values())
    {
      if (c->getnstUid() ==  entity->getnstUid())
      {
        connectors.remove(connectors.key(c));
        break;
      }
    }

    if (entity->getnstId() != "")
      connectors[entity->getnstId()] = entity;
  }
}

void QnstView::adjustAll()
{
  foreach(QnstGraphicsEntity *entity, entities.values())
  {
    switch(entity->getnstType())
    {
      case Qnst::Action:
      case Qnst::Condition:
      case Qnst::Bind:
      {
        QnstGraphicsBind *bind = dynamic_cast <QnstGraphicsBind *>(entity);
        if(bind)
          adjustBind(bind);
        break;
      }
      // if the entity type is MEDIA
      case Qnst::Audio:
      case Qnst::Text:
      case Qnst::Video:
      case Qnst::Image:
      case Qnst::NCLua:
      case Qnst::Settings:
      case Qnst::Media:
      case Qnst::Html:
      case Qnst::NCL:
      {
        QnstGraphicsMedia *media = dynamic_cast <QnstGraphicsMedia *>(entity);
        if(media)
          adjustMedia(media);
        break;
      }
      default:
        // nothing todo
        break;
    }
  }
}

void QnstView::changeConnector(QnstConnector* entity,
                               const QMap<QString, QString> &properties)
{
  if (properties["id"] != "")
  {
    entity->setName(properties["id"]);
    entity->setnstId(properties["id"]);
  }

  foreach(QnstConnector* c, connectors.values())
  {
    if (c->getnstUid() ==  entity->getnstUid())
    {
      connectors.remove(connectors.key(c));
      break;
    }
  }

  if (entity->getnstId() != "")
    connectors[entity->getnstId()] = entity;
}

void QnstView::addCondition(const QString uid, const QString parent,
                            const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    QnstConnector* connector = connectors2[properties["connector"]];

    if (properties["role"] != "")
      connector->addCondition(uid, properties["role"]);
  }
}

void QnstView::changeCondition(QString uid,
                               const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    QnstConnector* connector = connectors2[properties["connector"]];

    if (connector->getConditions().contains(uid))
      connector->getConditions().remove(uid);

    if (properties["role"] != "")
      connector->addCondition(uid, properties["role"]);
  }
}

void QnstView::addAction(const QString uid, const QString parent,
                         const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    QnstConnector* connector = connectors2[properties["connector"]];

    if (properties["role"] != "")
      connector->addAction(uid, properties["role"]);
  }
}

void QnstView::changeAction(QString uid,
                            const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    QnstConnector* connector = connectors2[properties["connector"]];

    if (connector->getActions().contains(uid))
      connector->getActions().remove(uid);

    if (properties["role"] != "")
      connector->addAction(uid, properties["role"]);
  }
}

void QnstView::changeBindParam(const QString uid,
                               const QMap<QString, QString> &properties)
{
  qWarning() << "changeBindParam";
  if (entities.contains(properties.value("parent")))
  {
    QnstGraphicsEntity* e = entities[properties.value("parent")];

    if (e->getnstType() == Qnst::Action)
    {
      QnstGraphicsBind* action = (QnstGraphicsBind*) e;
      action->setParam(properties.value("name",""),
                       properties.value("value",""));
    }
    else if (e->getnstType() == Qnst::Condition)
    {
      QnstGraphicsBind* condition = (QnstGraphicsBind*) e;
      condition->setParam(properties.value("name",""),
                          properties.value("value",""));
    }
  }
}

void QnstView::addConnectorParam(const QString uid, const QString parent,
                                 const QMap<QString, QString> &properties)
{
  if (connectors2.contains(parent))
  {
    QnstConnector* conn = connectors2[parent];
    conn->addParam(uid, properties["name"]);
  }
}

void QnstView::changeConnectorParam(const QString uid,
                                    const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["parent"]))
  {
    QnstConnector* conn = connectors2[properties["parent"]];
    conn->addParam(uid, properties["name"]);
  }
}

void QnstView::requestEntityAddition(QnstGraphicsEntity* entity, bool undo)
{
  qDebug() << "[QNST]" << ":" << "Requesting entity addition '"+entity->getnstUid()+"'";
  // qDebug() << links.keys();

  QMap<QString, QString> properties;
  QMap <QString, QString> additionalData;
  bool ok = false;

  if (entity != NULL)
  {
    entities[entity->getnstUid()] = entity;

    // Loading additional data
    additionalData = entity->getUsrData();
    foreach(QString key, additionalData.keys())
    {
      if(!properties.contains(key))
        properties.insert(key, additionalData.value(key));
    }
    //end loading additional data

    switch(entity->getnstType())
    {
      case Qnst::Body:
      {
        if (undo){  scene->addRoot(entity); }

        properties["TYPE"] = "body";

        properties["id"] = entity->getnstId();

        properties["top"] = QString::number(entity->getTop());
        properties["left"] = QString::number(entity->getLeft());
        properties["width"] = QString::number(entity->getWidth());
        properties["heigh"] = QString::number(entity->getHeight());
        properties["zindex"] = QString::number(entity->getzIndex());

        ok = true;
        break;
      }

      case Qnst::Context:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        properties["TYPE"] = "context";

        properties["id"] = entity->getnstId();

        properties["refer"] = "";

        properties["top"] = QString::number(entity->getTop());
        properties["left"] = QString::number(entity->getLeft());
        properties["width"] = QString::number(entity->getWidth());
        properties["heigh"] = QString::number(entity->getHeight());
        properties["zindex"] = QString::number(entity->getzIndex());

        ok = true;
        break;
      }

      case Qnst::Switch:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        properties["TYPE"] = "switch";

        properties["id"] = entity->getnstId();

        properties["refer"] = "";

        properties["top"] = QString::number(entity->getTop());
        properties["left"] = QString::number(entity->getLeft());
        properties["width"] = QString::number(entity->getWidth());
        properties["heigh"] = QString::number(entity->getHeight());
        properties["zindex"] = QString::number(entity->getzIndex());

        ok = true;
        break;
      }

      // if the entity type is MEDIA
      case Qnst::Audio:
      case Qnst::Text:
      case Qnst::Video:
      case Qnst::Image:
      case Qnst::NCLua:
      case Qnst::Settings:
      case Qnst::Html:
      case Qnst::NCL:
      case Qnst::Media:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);
        QnstGraphicsMedia *media = dynamic_cast<QnstGraphicsMedia *>(entity);

        if(media)
        {
          properties["TYPE"] = "media";

          properties["id"] = media->getnstId();
          properties["src"] = media->getSource();
          properties["refer"] = media->getRefer();
          properties["instance"] = media->getInstance();

          media->updateToolTip();

          ok = true;
        }
        break;
      }

      case Qnst::Port:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        QnstGraphicsPort *port = dynamic_cast <QnstGraphicsPort *> (entity);

        properties["TYPE"] = "port";
        properties["id"] = entity->getnstId();

        if(port)
        {
          foreach(QnstGraphicsEdge* edge, port->getnstGraphicsEdges())
          {
            if (edge->getnstType() == Qnst::Reference)
            {
              properties["COMPONENT"] = edge->getEntityA()->getnstUid();
              properties["component"] = edge->getEntityA()->getnstId();

              if (edge->getEntityB()->getnstType() == Qnst::Port ||
                  edge->getEntityB()->getnstType() == Qnst::Property ||
                  edge->getEntityB()->getnstType() == Qnst::Area)
              {
                properties["INTERFACE"] = edge->getEntityB()->getnstUid();;
                properties["interface"] = edge->getEntityB()->getnstId();;
              }
            }
          }

          ok = true;
        }
        break;
      }

      case Qnst::Area:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        properties["TYPE"] = "area";
        properties["id"] = entity->getnstId();

        if(entity->getnstGraphicsParent()->isMedia())
        {
          QnstGraphicsMedia *media =
            dynamic_cast <QnstGraphicsMedia *>(entity->getnstGraphicsParent());

          if(media != NULL)
            adjustMedia(media);
        }

        foreach (QString key,
                 refers.keys(entity->getnstGraphicsParent()->getnstUid()))
        {
          if (entities.contains(key))
          {
            if (entities[key]->isMedia())
              adjustMedia((QnstGraphicsMedia*) entities[key]);
          }
        }

        ok = true;
        break;
      }

      case Qnst::Property:
      {
        properties["TYPE"] = "property";

        if(entity->getnstGraphicsParent()->isMedia())
        {
          QnstGraphicsMedia *media =
            dynamic_cast <QnstGraphicsMedia *>(entity->getnstGraphicsParent());

          if(media != NULL)
            adjustMedia(media);
        }

        foreach (QString key,
                 refers.keys(entity->getnstGraphicsParent()->getnstUid()))
        {
          if (entities.contains(key))
          {
            if (entities[key]->isMedia())
              adjustMedia((QnstGraphicsMedia*) entities[key]);
          }
        }

        ok = true;
        break;
      }

      case Qnst::Aggregator:
      {
        qWarning() << "Trying to add an Aggregator!!! This is not supported \
                      anymore!!";

        // \fixme This should be deprecated!!
        // entities[entity->getnstUid()] = entity;
        break;
      }

      case Qnst::Link:
      {
        QnstGraphicsLink *link = dynamic_cast<QnstGraphicsLink*>(entity);

        if(link)
        {
          if (entity->getnstId() == "" && !undo)
            updateEntityWithUniqueNstId(entity);

          properties["TYPE"] = "link";
          properties["id"] = entity->getnstId();
          properties["xconnector"] = link->getxConnector();

          entities[entity->getnstUid()] = entity;
          links[entity->getnstUid()] = link;

          ok = true;
        }
        break;
      }

      case Qnst::Bind:
      case Qnst::Condition:
      case Qnst::Action:
      {
        if (!binds.contains(entity->getnstUid()))
        {
          QnstGraphicsBind* bind = dynamic_cast <QnstGraphicsBind*>(entity);
          if(bind)
          {
            properties["TYPE"] = "bind";

            if(bind->getLink() != NULL)
            {
              properties["connector"] = bind->getLink()->getxConnector();
              properties["linkUID"] = bind->getLink()->getnstUid();
              properties["link"] = bind->getLink()->getnstId();
            }
            else
            {
              qWarning() << "[QNST] Warning!!! Bind is not associated to link!";
              return;
            }

            if (bind->isCondition())
            {
              properties["role"] =
                  QnstUtil::getStrFromBindType(bind->getType());
              properties["condition"] = bind->getRole();
            }
            else
            {
              properties["action"] = bind->getRole();
              properties["role"] =
                  QnstUtil::getStrFromBindType(bind->getType());
            }

            properties["component"] = bind->getComponent();
            properties["interface"] = bind->getInterface();

            binds[bind->getnstUid()] = bind;

            ok = true;
          }
        }
        break;
      }

      case Qnst::SwitchPort:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        properties["TYPE"] = "switchPort";
        properties["id"] = entity->getnstId();

        ok = true;
        break;
      }

      default:
        // do nothing
        break;
    }

    if(ok)
    {
      if(entity->getnstType() == Qnst::Body)
      {
        emit entityAdded(entity->getnstUid(), "", properties);
      }
      else
      {
        emit entityAdded(entity->getnstUid(),
                         entity->getnstGraphicsParent()->getnstUid(),
                         properties);
      }

      if (!undo)
      {
        QnstAddCommand* cmd = new QnstAddCommand(this, entity);
        history.push(cmd);
      }
    }
  }
}

// \todo refactory
void QnstView::requestEntityRemotion(QnstGraphicsEntity* entity, bool undo,
                                     bool rmRefs, bool notify)
{
  qDebug() << "[QNST]" << ":" << "Requesting entity remotion '" +
              entity->getnstUid() + "'" +
              entity->getnstType() + "=" +
              QnstUtil::getStrFromNstType(entity->getnstType());

  // traceEntities();

  if (entity != NULL)
  {
    Qncg::EntityType qncgType = entity->getncgType();
    bool mustRemoveFromParent = false;

    // By now, we are handling undo only for node and interface!!
    if( entity->getncgType() == Qncg::Node ||
        entity->getncgType() == Qncg::Interface )
    {
      if (!undo)
      {
        QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
        history.push(cmd);
      }
    }

    QString UID = entity->getnstUid();

    switch (qncgType)
    {
      case Qncg::Node:
      {
        QnstGraphicsNode *node = dynamic_cast <QnstGraphicsNode*>(entity);

        if(node)
          removeEdgesOfEntity(node, false); // Remove the edges of this entity
        else
          qWarning() << "Could not make a cast to QnstGraphicsNode*";

        // Remove recursively
        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities())
        {
          if (e->getnstType() != Qnst::Edge &&
              e->getnstType() != Qnst::Condition &&
              e->getnstType() != Qnst::Mapping &&
              e->getnstType() != Qnst::Action &&
              e->getnstType() != Qnst::Reference)
          {
            requestEntityRemotion(e, true, false);
          }
        }

        mustRemoveFromParent = true;
        break;
      }

      case Qncg::Interface:
      {
        QnstGraphicsInterface *interface =
            dynamic_cast <QnstGraphicsInterface*>(entity);

        if(interface)
          removeEdgesOfEntity(interface, false); // Remove the edges of this entity
        else
          qWarning() << "Could not make a cast to QnstGraphicsInterface*";

        mustRemoveFromParent = true;
        break;
      }

      case Qncg::Edge:
      {
        QnstGraphicsEdge *edge = dynamic_cast<QnstGraphicsEdge *>(entity);

        if(edge)
          removeEdge(edge, notify);
        break;
      }

      default:
        // do nothing?
        mustRemoveFromParent = true;
        break;
    }

    // Everything is ok, now only remove the entity from its parent
    if(mustRemoveFromParent)
    {
      //Remove the entity of its parent
      QnstGraphicsEntity* parent = entity->getnstGraphicsParent();
      if (parent != NULL)
        parent->removenstGraphicsEntity(entity);
      else
        scene->removeRoot(entity);

      if (selected == entity)
        selected = NULL;

      if (clipboard == entity)
        clipboard = NULL;

      if (entity->getnstType() == Qnst::Link)
      {
        if (links.contains(entity->getnstUid()))
        {
          links.remove(entity->getnstUid());
        }
      }

      entities.remove(entity->getnstUid());

      if (entity->getnstType() == Qnst::Link)
      {
        if (links.contains(entity->getnstUid()))
        {
          links.remove(entity->getnstUid());
        }
      }

      if(notify) //\fixme We need a better way to handle that!!!
        emit entityRemoved(entity->getnstUid());

      delete entity;

      if (qncgType == Qncg::Interface)
      {
        if (refers.contains(UID))
        {
          refers.remove(UID);
        }
        else
        {
          if (parent != NULL && parent->isMedia())
          {
            adjustMedia((QnstGraphicsMedia*)parent);

            foreach(QString rUid, refers.keys(parent->getnstUid()))
            {
              if (entities.contains(rUid))
              {
                QnstGraphicsEntity* e = entities.value(rUid);

                if (e->isMedia())
                  adjustMedia((QnstGraphicsMedia*)e);
              }
            }
          }
        }
      }
      else
      {
        foreach(QString ruid, refers.keys(UID))
        {
          refers.remove(ruid);

          if (entities.contains(ruid))
          {
            QnstGraphicsEntity* e = entities.value(ruid);

            if (e->isMedia())
            {
              qDebug() << "=================== un, hello!";
              adjustMedia((QnstGraphicsMedia*) e);
            }
          }
        }

        if (refers.contains(UID))
        {
          qWarning() << "Removing" + UID + "from refers.";
          refers.remove(UID);
        }
      }
    }
  }

  deletePendingEntities();
}

void QnstView::removeEdgesOfEntity(QnstGraphicsEntityWithEdges *entity,
                                   bool notify)
{
  foreach(QnstGraphicsEdge* edge, entity->getnstGraphicsEdges())
  {
    requestEntityRemotion(edge, false, false, notify);
  }
}

void QnstView::removeEdge(QnstGraphicsEdge *edge, bool notify)
{
  if(edge->getEntityA())
  {
    if ( edge->getEntityA()->getncgType() == Qncg::Node ||
         edge->getEntityA()->getncgType() == Qncg::Interface )
    {
      ((QnstGraphicsEntityWithEdges*) edge->getEntityA())->removenstGraphicsEdge(edge);
    }
  }

  if( edge->getEntityB() )
  {
    if ( edge->getEntityB()->getncgType() == Qncg::Node ||
         edge->getEntityB()->getncgType() == Qncg::Interface )
    {
      ((QnstGraphicsEntityWithEdges*) edge->getEntityB())->removenstGraphicsEdge(edge);
    }
  }

  if (edge->getnstType() == Qnst::Condition ||
      edge->getnstType() == Qnst::Action ||
      edge->getnstType() == Qnst::Bind)
  {
    QnstGraphicsBind* bb = binds[edge->getnstUid()];

    if (bb != NULL)
    {
      QnstGraphicsLink* ll = (QnstGraphicsLink*) bb->getLink();
      if (ll != NULL)
      {
        if (ll->getConditions().contains(bb->getnstUid()))
          ll->removeCondition(bb);

        if (ll->getActions().contains(bb->getnstUid()))
          ll->removeAction(bb);
      }

      binds.remove(bb->getnstUid());
    }
  }
  else if (edge->getnstType() == Qnst::Reference)
  {
    QnstGraphicsPort* p = (QnstGraphicsPort*) edge->getEntityA();
    p->setComponent("");
    p->setComponentUid("");
    p->setInterface("");
    p->setInterfaceUid("");
    // requestEntityChange(p);
  }
  else if (edge->getnstType() == Qnst::Mapping)
  {
    // Do nothing ??
    // QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
    // parent->removenstGraphicsEntity(edge);
    // }

    // entities.remove(edge->getnstUid());
    // emit entityRemoved(edge->getnstUid());
  }

  QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
  if (parent)
  {
    parent->removenstGraphicsEntity(edge);
    entities.remove(edge->getnstUid());
  }

  if (edge->getnstType() != Qnst::Reference)
  {
    if(notify)
      emit entityRemoved(edge->getnstUid());
  }

  if (selected == edge)
    selected = NULL;

  if (clipboard == edge)
    clipboard = NULL;

  // delete edge;
}

void QnstView::requestEntityPreparation(QnstGraphicsEntity* entity,
                                        QMap<QString, QString> properties)
{
  QnstChangeCommand* cmd = new QnstChangeCommand(this, entity->getnstUid(),
                                                 properties);
  history.push(cmd);

  emit viewChanged();
}

void QnstView::requestEntityChange(QnstGraphicsEntity* entity)
{
  qDebug() << "[QNST]" << ":" << "Requesting entity change '" + entity->getnstUid() + "'";

  QMap<QString, QString> properties;
  QMap <QString, QString> additionalData;
  bool ok = false;

  if (entity != NULL)
  {
    // Load Additional data
    additionalData = entity->getUsrData();
    foreach(QString key, additionalData.keys())
    {
      if(!properties.contains(key))
        properties.insert(key, additionalData.value(key));
    }
    // end load of additional data

    switch(entity->getnstType())
    {
      case Qnst::Body:
      {
        properties["TYPE"] = "body";

        properties["id"] = entity->getnstId();

        properties["top"] = QString::number(entity->getTop());
        properties["left"] = QString::number(entity->getLeft());
        properties["width"] = QString::number(entity->getWidth());
        properties["heigh"] = QString::number(entity->getHeight());
        properties["zindex"] = QString::number(entity->getzIndex());

        ok = true;
        break;
      }

      case Qnst::Context:
      {
        properties["TYPE"] = "context";

        properties["id"] = entity->getnstId();

        properties["refer"] = ""; // We do not support refer on context yet!

        properties["top"] = QString::number(entity->getTop());
        properties["left"] = QString::number(entity->getLeft());
        properties["width"] = QString::number(entity->getWidth());
        properties["heigh"] = QString::number(entity->getHeight());
        properties["zindex"] = QString::number(entity->getzIndex());

        ok = true;
        break;
      }

      case Qnst::Switch:
      {
        properties["TYPE"] = "switch";

        properties["id"] = entity->getnstId();

        properties["refer"] = ""; // We do not support refer on switch yet!

        properties["top"] = QString::number(entity->getTop());
        properties["left"] = QString::number(entity->getLeft());
        properties["width"] = QString::number(entity->getWidth());
        properties["heigh"] = QString::number(entity->getHeight());
        properties["zindex"] = QString::number(entity->getzIndex());

        ok = true;
        break;
      }

      // if the entity type is MEDIA
      case Qnst::Audio:
      case Qnst::Text:
      case Qnst::Video:
      case Qnst::Image:
      case Qnst::NCLua:
      case Qnst::Settings:
      case Qnst::Html:
      case Qnst::NCL:
      case Qnst::Media:
      {
        properties["TYPE"] = "media";

        properties["id"] = entity->getnstId();
        properties["src"] = ((QnstGraphicsMedia*)entity)->getSource();

        // What else ??

        ok = true;
        break;
      }

      // if the entity type is PORT
      case Qnst::Port:
      {
        properties["TYPE"] = "port";

        properties["id"] = entity->getnstId();

        QnstGraphicsPort *port = dynamic_cast <QnstGraphicsPort *>  (entity);
        foreach(QnstGraphicsEdge* edge, port->getnstGraphicsEdges())
        {
          if (edge->getnstType() == Qnst::Reference)
          {
            if (edge->getEntityB()->getnstType() == Qnst::Port ||
                edge->getEntityB()->getnstType() == Qnst::Property ||
                edge->getEntityB()->getnstType() == Qnst::Area)
            {
              properties["INTERFACE"] = edge->getEntityB()->getnstUid();
              properties["interface"] = edge->getEntityB()->getnstId();

              properties["COMPONENT"] = edge->getEntityB()->getnstGraphicsParent()->getnstUid();
              properties["component"] = edge->getEntityB()->getnstGraphicsParent()->getnstId();

            }
            else
            {
              properties["COMPONENT"] = edge->getEntityB()->getnstUid();
              properties["component"] = edge->getEntityB()->getnstId();

              properties["INTERFACE"] = "";
              properties["interface"] = "";
            }

            break; // there is only one match, so 'break'.
          }
        }

        ok = true;
        break;
      }

      case Qnst::Link:
      {
        properties["TYPE"] = "link";

        properties["id"] = entity->getnstId();

        ok = true;
        break;
      }

      case Qnst::Area:
      case Qnst::Property:
      case Qnst::Aggregator:
      {
        // do nothing ??
        break;
      }

      case Qnst::SwitchPort:
      {
        properties["TYPE"] = "switchPort";

        properties["id"] = entity->getnstId();

        ok = true;
        break;
      }

      case Qnst::BindParam:
      {
        // \todo
      }

      default:
        // do nothing
        break;
    }

    if(ok)
    {
      emit entityChanged(entity->getnstUid(), properties);
    }

    // Undo ??
  }
}

void QnstView::requestEntitySelection(QnstGraphicsEntity* entity)
{
  qDebug() << "[QNST]" << ":"
           << "Requesting entity selection '" + entity->getnstUid() + "'";

  if (entity != NULL)
  {
    if (entity->isSelected())
    {
      if (selected != NULL)
      {
        selected->setSelected(false);
        selected->adjust();
      }

      selected = entity;

      if (!entity->isMedia() && refers.contains(entity->getnstUid()))
      {
          qDebug() << "========================" << "REF" << entity->getnstUid() << "->" << refers[entity->getnstUid()];
        emit entitySelected(refers[entity->getnstUid()]);
      }
      else
      {
           qDebug() << "========================" << entity->getnstUid();
        emit entitySelected(entity->getnstUid());
      }
    }
  }
}

void QnstView::performHelp()
{
    // TODO
}

void QnstView::performUndo()
{
  if (history.canUndo()){
    history.undo();
  }
}

void QnstView::performRedo()
{
  if (history.canRedo()){
    history.redo();
  }
}

void QnstView::performCut()
{
  if (selected != NULL)
  {
    if (selected->getnstType() != Qnst::Body)
    {
      if (clipboard != NULL)
      {
        delete clipboard; clipboard = NULL;
      }

      QnstGraphicsEntity* entity = selected;

      clipboard = QnstUtil::makeGraphicsEntity(entity->getnstType());

      if (clipboard != NULL)
      {
        clipboard->setnstId(entity->getnstId());

        clipboard->setTop(entity->getTop());
        clipboard->setLeft(entity->getLeft());
        clipboard->setWidth(entity->getWidth());
        clipboard->setHeight(entity->getHeight());
        clipboard->setnstGraphicsParent(NULL);

        clipboard->setUsrData(entity->getUsrData());

        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities())
        {
          performCopy(e, clipboard);
        }

        requestEntityRemotion(selected);

        foreach(QnstGraphicsEntity* e, entities.values())
        {
          if (e->getnstType() == Qnst::Body ||
              e->getnstType() == Qnst::Context ||
              e->getnstType() == Qnst::Switch)
          {
            e->menu->actionPaste->setEnabled(true);
          }
        }

        hasCutted = true;
      }
    }
    else
    {
      QMessageBox::warning(this, "Alert",
                           "You cannot cut a &lt;body&gt; element!");
    }
  }
}

void QnstView::performCopy()
{
  if (selected != NULL)
  {
    if (selected->getnstType() != Qnst::Body)
    {
      if (clipboard != NULL)
      {
        delete clipboard; clipboard = NULL;
      }

      QnstGraphicsEntity* entity = selected;
      clipboard = QnstUtil::makeGraphicsEntity(entity->getnstType());

      if (clipboard != NULL)
      {
        clipboard->setnstId(entity->getnstId());
        clipboard->setnstUid(entity->getnstUid());

        clipboard->setTop(entity->getTop());
        clipboard->setLeft(entity->getLeft());
        clipboard->setWidth(entity->getWidth());
        clipboard->setHeight(entity->getHeight());

        clipboard->setnstGraphicsParent(NULL);
        clipboard->setUsrData(entity->getUsrData());

        qDebug() << "[QNST]" << clipboard->getUsrData();
        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities())
        {
          performCopy(e, clipboard);
        }

        foreach(QnstGraphicsEntity* e, entities.values())
        {
          if (e->getnstType() == Qnst::Body ||
              e->getnstType() == Qnst::Context ||
              e->getnstType() == Qnst::Switch)
          {
            e->menu->actionPaste->setEnabled(true);
          }
        }

        hasCutted = false;
      }

    }
    else
    {
      QMessageBox::warning(this, "Alert",
                           "You cannot copy a &lt;body&gt; element!");
    }
  }
}

void QnstView::performCopy(QnstGraphicsEntity* entity,
                           QnstGraphicsEntity* parent)
{
  QnstGraphicsEntity* copy = NULL;

  copy = QnstUtil::makeGraphicsEntity(entity->getnstType());

  if (copy != NULL)
  {
    copy->setnstId(entity->getnstId());
    copy->setnstUid(entity->getnstUid());

    copy->setTop(entity->getTop());
    copy->setLeft(entity->getLeft());
    copy->setWidth(entity->getWidth());
    copy->setHeight(entity->getHeight());

    parent->addnstGraphicsEntity(copy);

    qDebug() << "[QNST]" << entity->getUsrData();
    copy->setUsrData(entity->getUsrData());

    foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities())
    {
      performCopy(e, copy);
    }
  }
}

void QnstView::performPaste()
{
  if (clipboard != NULL)
  {
    QnstGraphicsEntity* copy = clipboard;
    qDebug() << "[QNST] performPaste() cliboard userdata = "
             << clipboard->getUsrData();

    QnstGraphicsEntity* parent = selected;

    if (copy != NULL && parent != NULL)
    {
      if (parent->getnstType() == Qnst::Context ||
          parent->getnstType() == Qnst::Switch ||
          parent->getnstType() == Qnst::Body)
      {

        int result = 1;
        if(!hasCutted) // if the user has cutted obviously he/she does not
                       // want to make a reference
        {
          result = QMessageBox::question(
                this,
                tr("Copy"),
                tr("Would you like to make a reference instead?"),
                tr("Cancel"),
                tr("No"),
                tr("Yes"),
                1);
        }

        if (result == 0) // Cancel operation
        {
            return;
        }
        else  // Make the copy
        {
          // result == 2 when the user asked to make a refer
          performPaste(copy, parent, (result == 2), false);
        }
      }
    }
  }
}

bool QnstView::updateEntityWithUniqueNstId(QnstGraphicsEntity *entity)
{
  if(entity != NULL)
  {
    int n = 0;
    Qnst::EntityType type;

    // for any media, we will consider the same identifier prefix
    if(entity->isMedia())
      type = Qnst::Media;
    else
      type = entity->getnstType();

    // check if type is in the map
    if(!entityCounter.count(type))
      entityCounter[type] = 0;

    n = entityCounter[type];

    entity->setnstId(QnstUtil::getPrefixIdFromType(type) + QString::number(n));

    entityCounter[type] ++ ;

    return true;
  }

  return false;
}

void QnstView::performPaste(QnstGraphicsEntity* copy,
                            QnstGraphicsEntity* parent,
                            bool makeRefer, bool keepOriginalPos)
{
  QnstGraphicsEntity* entity = QnstUtil::makeGraphicsEntity(copy->getnstType(),
                                                            parent);

  if(entity != NULL)
  {
    // If I haven't cutted set a unique identifier to my entity
    if(!hasCutted)
      updateEntityWithUniqueNstId(entity);
    else
    {
      // \fixme Should we copy all properties?
      entity->setnstId(copy->getnstId());
    }

    // copy the position
    if(keepOriginalPos)
    {
      entity->setTop(copy->getTop());
      entity->setLeft(copy->getLeft());
    }
    else
    {
      entity->setTop(parent->getHeight()/2 - copy->getWidth()/2);
      entity->setLeft(parent->getWidth()/2 - copy->getHeight()/2);
    }

    // set the size
    entity->setWidth(copy->getWidth());
    entity->setHeight(copy->getHeight());

    // Should we make a Reference ??
    if (makeRefer)
    {
      performReference(entity, copy);
    }
    else if(entity->isMedia())
    {
      // \fixme Should we copy all properties?
      ((QnstGraphicsMedia*) entity)->setSource(copy->getUsrData()["src"]);
    }

    parent->addnstGraphicsEntity(entity);

    entity->adjust();
    entity->setUsrData(copy->getUsrData());

    requestEntityAddition(entity);

    if(entity->isMedia())
    {
      adjustMedia((QnstGraphicsMedia*) entity);
    }

    if (!makeRefer)
    {
      foreach(QnstGraphicsEntity* e, copy->getnstGraphicsEntities())
      {
        performPaste(e, entity, makeRefer, true);
      }
    }
  }
}

void QnstView::performReference(QnstGraphicsEntity* entity,
                                QnstGraphicsEntity* referred)
{
  if(entity != NULL)
  {
    // \fixme We should copy all properties
    if(entity->isMedia())
    {
      //Reference
      ((QnstGraphicsMedia*) entity)->setInstance("new");
      ((QnstGraphicsMedia*) entity)->setRefer(((QnstGraphicsMedia*) referred)->getnstId());
      ((QnstGraphicsMedia*) entity)->setReferUID(((QnstGraphicsMedia*) referred)->getnstUid());

      adjustMedia((QnstGraphicsMedia*) entity);
    }

    // \todo Refer for context and switch
  }
}

void QnstView::performDelete()
{
  if (selected != NULL)
  {
    if(selected->getncgType() == Qncg::Interface &&
       ((QnstGraphicsInterface*)selected)->isRefer())
    {
      // ignore
    }
    else
    {
      requestEntityRemotion(selected);
    }

    scene->update();
  }
}

void QnstView::performExport()
{
  QString location = QFileDialog::getSaveFileName(NULL, "Export...", "", tr("Images (*.png)"));

  if (location != ""){
    qreal top = 0;
    qreal left = 0;
    qreal width = scene->width();
    qreal height = scene->height();

    foreach (QnstGraphicsEntity* entity, scene->getRoots())
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

void QnstView::performZoomIn()
{
  if (zoomStep > 0)
  {
    zoomStep--;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
  }
}

void QnstView::performZoomOut()
{
  if (zoomStep*0.05 < 0.9)
  {
    zoomStep++;

    resetMatrix();

    scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
  }
}

void QnstView::performZoomReset()
{
  zoomStep = 0;
  resetMatrix();
}

void QnstView::performFullscreen()
{
    // TODO
}

void QnstView::performBringfront()
{
    // TODO
}

void QnstView::performBringforward()
{
    // TODO
}

void QnstView::performSendback()
{
    // TODO
}

void QnstView::performSendbackward()
{
    // TODO
}

void QnstView::performHide()
{
    // TODO
}

void QnstView::performProperties()
{
    // TODO
}

void QnstView::mouseMoveEvent(QMouseEvent* event)
{
  if (linking)
  {
    if (lastLinkMouseOver != NULL)
    {
      lastLinkMouseOver->setMouseHover(false);
      lastLinkMouseOver = NULL;
    }

    QList<QGraphicsItem *> itemsa = scene->items(link->getLine().p1());

    if (itemsa.count() && itemsa.first() == link)
    {
      itemsa.removeFirst();
    }

    if (itemsa.count())
    {
      QnstGraphicsEntity* entitya = (QnstGraphicsEntity*) itemsa.first();
      entitya->setMouseHover(true);
    }

    QList<QGraphicsItem*> itemsb = scene->items(link->getLine().p2());

    if (itemsb.count() && itemsb.first() == link)
    {
      itemsb.removeFirst();
    }

    if (itemsb.count())
    {
      QnstGraphicsEntity* entityb = (QnstGraphicsEntity*) itemsb.first();
      entityb->setMouseHover(true);

      lastLinkMouseOver = entityb;
    }

    link->setLine(QLineF(link->getLine().p1(), mapToScene(event->pos())));
  }

  QGraphicsView::mouseMoveEvent(event);
}

void QnstView::mousePressEvent(QMouseEvent* event)
{
  if (modified)
  {
    if (link != NULL)
    {
      delete (link);
      link = NULL;
    }

    link = new QnstViewLink();
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
        if (selected != NULL)
        {
          selected->setSelected(false);
          selected->adjust();
        }

        selected = NULL;
      }

      event->accept();
    }
  }
}

void QnstView::mouseReleaseEvent(QMouseEvent* event)
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
      QnstGraphicsEntity* entitya = (QnstGraphicsEntity*) itemsa.first();
      QnstGraphicsEntity* entityb = (QnstGraphicsEntity*) itemsb.first();

      if (entitya != entityb)
      {
        // if linking NODE to NODE
        if (entitya->getncgType() == Qncg::Node &&
            entityb->getncgType() == Qncg::Node)
        {
           addNodetoNodeEdge(entitya, entityb);
        }
        // if linking NODE to INTERFACE
        else if (entitya->getncgType() == Qncg::Node &&
                 entityb->getncgType() == Qncg::Interface)
        {
          addNodetoInterfaceEdge(entitya, entityb);
        }
        // if linking INTERFACE to NODE
        else if (entitya->getncgType() == Qncg::Interface &&
                 entityb->getncgType() == Qncg::Node)
        {
          addInterfacetoNodeEdge(entitya, entityb);
        }
        // if linking INTERFACE to INTERFACE
        else if (entitya->getncgType() == Qncg::Interface &&
                 entityb->getncgType() == Qncg::Interface)
        {
          addInterfacetoInterfaceEdge(entitya, entityb);
        }
      }

      entitya->setMouseHover(false);
      entityb->setMouseHover(false);
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

  QnstGraphicsEntity *entity;
  foreach(entity, entities.values())
  {
    assert(entity != NULL);
    entity->setDraggable(false);
  }

  QGraphicsView::mouseReleaseEvent(event);
}

void QnstView::addNodetoNodeEdge(QnstGraphicsEntity* entitya,
                                 QnstGraphicsEntity* entityb)
{
  QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {
    if (parenta == parentb)
    {
      // aggregator -> node
      if (entitya->getnstType() == Qnst::Link)
      {
        createActionWithDialog(entitya, entityb);
      }
      // node -> aggregator
      else if (entityb->getnstType() == Qnst::Link)
      {
        createConditionWithDialog(entitya, entityb);
      }
      // node -> node
      else
      {
        createLinkWithDialog(entitya, entityb);
      }

      modified = false;
    }
  }
}

void QnstView::addNodetoInterfaceEdge(QnstGraphicsEntity* entitya,
                                      QnstGraphicsEntity* entityb)
{
  QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {
    if (parenta == parentb->getnstGraphicsParent())
    {
      // aggregator -> interface
      if (entitya->getnstType() == Qnst::Link)
      {
        createActionWithDialog(entitya, entityb);
      }
      // node -> interface
      else
      {
        createLinkWithDialog(entitya, entityb);
      }

      modified = false;
    }
  }
}

void QnstView::addInterfacetoNodeLink(QnstGraphicsEntity* entitya,
                                      QnstGraphicsEntity* entityb)
{
  // interface -> aggreagator
  if (entityb->getnstType() == Qnst::Link)
  {
    createConditionWithDialog(entitya, entityb);
  }
  // interface -> node
  else
  {
    createLinkWithDialog(entitya, entityb);
  }
}

void QnstView::addInterfacetoNodeEdge(QnstGraphicsEntity* entitya,
                                      QnstGraphicsEntity* entityb)
{
  QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {
    if (parenta == parentb)
    {
      if (entitya->getnstType() == Qnst::Port)
      {
        createReference(entitya, entityb);
      }
      else if (entitya->getnstType() == Qnst::SwitchPort)
      {
        createMapping(entitya, entityb);
      }
    }
    else if (parenta->getnstGraphicsParent() == parentb && parenta != entityb)
    {
      addInterfacetoNodeLink(entitya, entityb);

      modified = false;
    }
  }
}

void QnstView::addInterfacetoInterfaceEdge(QnstGraphicsEntity* entitya,
                                           QnstGraphicsEntity* entityb)
{
  QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {
    if (parenta == parentb->getnstGraphicsParent())
    {
      if (entitya->getnstType() == Qnst::Port)
      {
        createReference(entitya, entityb);
      }
      else if (entitya->getnstType() == Qnst::SwitchPort)
      {
        createMapping(entitya, entityb);
      }
    }
    else if (parenta->getnstGraphicsParent() == parentb->getnstGraphicsParent())
    {
      createLinkWithDialog(entitya, entityb);

      modified = false;
    }
  }
}

QString QnstView::createNewConnector(QString condition, QString action)
{
  QnstConnector* conn = new QnstConnector();
  conn->setName("newConnector"+QString::number(connectors.size()));
  conn->addCondition("onBegin", "onBegin");
  conn->addCondition("onEnd", "onEnd");
  conn->addCondition("onSelection", "onSelection");
  conn->addCondition("onResume", "onResume");
  conn->addCondition("onPause", "onPause");

  conn->addAction("start", "start");
  conn->addAction("stop", "stop");
  conn->addAction("resume", "resume");
  conn->addAction("pause", "pause");
  conn->addAction("set", "set");

  connectors[conn->getName()] = conn;
  connectors2[conn->getnstUid()] = conn;

  QMap<QString, QString> properties;

  properties["TYPE"] = "complex-connector";

  properties["id"] = conn->getName();

  if (!action.isEmpty())
    properties["action"] = action;

  if (!condition.isEmpty())
    properties["condition"] = condition;

  emit entityAdded(conn->getnstUid(), "", properties);

  return conn->getName();
}

QnstGraphicsLink* QnstView::createLink(QnstGraphicsEntity* entitya,
                                       QnstGraphicsEntity* entityb,
                                       QString connName)
{
  QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {
      QnstGraphicsLink* linkDot = NULL;

      qreal xcenter = -1;
      qreal ycenter = -1;

      QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                     entitya->getTop() + entitya->getHeight()/2);

      QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                     entityb->getTop() + entityb->getHeight()/2);

      if(entitya->getncgType() == Qncg::Interface &&
         entityb->getncgType() == Qncg::Interface)
      {
        QnstGraphicsEntity* super = parenta->getnstGraphicsParent();

        linkDot = new QnstGraphicsLink((QnstGraphicsNode*) super);
        super->addnstGraphicsEntity(linkDot);

        pointa = super->mapFromItem(parenta, pointa);
        pointb = super->mapFromItem(parentb, pointb);
      }
      else if(entitya->getncgType()  == Qncg::Node &&
              entityb->getncgType() == Qncg::Interface)
      {
        linkDot = new QnstGraphicsLink((QnstGraphicsNode*) parenta);
        parenta->addnstGraphicsEntity(linkDot);

        pointb = parenta->mapFromItem(parentb, pointb);
      }
      else if(entitya->getncgType()  == Qncg::Interface &&
              entityb->getncgType() == Qncg::Node)
      {
        linkDot = new QnstGraphicsLink((QnstGraphicsNode*) parentb);
        parentb->addnstGraphicsEntity(linkDot);

        pointa = parentb->mapFromItem(parenta, pointa);

      }
      else
      {
        linkDot = new QnstGraphicsLink((QnstGraphicsNode*) parenta);
        parenta ->addnstGraphicsEntity(linkDot);
      }

      if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
      {
        xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
        ycenter = pointa.y() + (pointb.y() - pointa.y())/2;
      }
      else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
      {
        xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
        ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

      }
      else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
      {
        xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
        ycenter = pointb.y() + (pointa.y() - pointb.y())/2;

      }
      else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
      {
        xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
        ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
      }

      linkDot->setTop(ycenter - DEFAULT_AGGREGATOR_HEIGHT/2);
      linkDot->setLeft(xcenter - DEFAULT_AGGREGATOR_WIDTH/2);
      linkDot->setWidth(DEFAULT_AGGREGATOR_WIDTH);
      linkDot->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
      linkDot->adjust();

      updateEntityWithUniqueNstId(linkDot); // set a unique Id

      linkDot->setxConnector(connName);
      if(connectors.contains(connName))
        linkDot->setxConnectorUID(connectors[connName]->getnstUid());

      requestEntityAddition(linkDot);

      return linkDot;
  }

  return NULL;
}

void QnstView::createReference(QnstGraphicsEntity* entitya,
                               QnstGraphicsEntity* entityb)
{
  QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {
    // removing previous reference
    foreach(QnstGraphicsEntity* entity, parenta->getnstGraphicsEntities())
    {
      if (entity->getnstType() == Qnst::Reference)
      {
        QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

        if (entitya == edge->getEntityA()){
          if (edge->getEntityA()->getncgType() == Qncg::Node)
          {
            ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);
          }
          else if (edge->getEntityA()->getncgType() == Qncg::Interface)
          {
            ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
          }

          if (edge->getEntityB()->getncgType() == Qncg::Node)
          {
            ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

          }
          else if (edge->getEntityB()->getncgType() == Qncg::Interface)
          {
            ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
          }

          parenta->removenstGraphicsEntity(edge); delete (edge);

          break;
        }
      }
    }

    QnstGraphicsReference* entity = new QnstGraphicsReference();
    entity->setEntityA(entitya);
    entity->setEntityB(entityb);
    entity->adjust();

    parenta->addnstGraphicsEntity(entity);

    ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);

    if (entityb->getncgType() == Qncg::Node)
      ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(entity);
    else if (entityb->getncgType() == Qncg::Interface)
      ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

    requestEntityChange(entitya);
  }
}

void QnstView::createMapping(QnstGraphicsEntity* entitya,
                             QnstGraphicsEntity* entityb)
{
  QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {

    QMap<QString, QString> properties;

    properties["TYPE"] = "mapping";

    QnstGraphicsMapping* entity = new QnstGraphicsMapping();
    entity->setEntityA(entitya);
    entity->setEntityB(entityb);

    ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);

    if (entityb->getncgType() == Qncg::Node)
    {
      properties["component"] = entityb->getnstId();
      properties["componentUID"] = entityb->getnstUid();

      entity->setComponent(properties["component"]);
      entity->setComponentUid(properties["componentUID"]);

      ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(entity);
    }
    else if (entityb->getncgType() == Qncg::Interface)
    {
      properties["component"] = parentb->getnstId();
      properties["componentUID"] = parentb->getnstUid();
      properties["interface"] = entityb->getnstId();
      properties["interfaceUID"] = entityb->getnstUid();


      entity->setComponent(properties["component"]);
      entity->setComponentUid(properties["componentUID"]);
      entity->setInterface(properties["interface"]);
      entity->setInterfaceUid(properties["interfaceUID"]);

      ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);
    }

    parenta->addnstGraphicsEntity(entity);

    entity->adjust();

    entity->setSwitchPortUid(entitya->getnstUid());

    entities[entity->getnstUid()] = entity;

    // parent is the strucUID from the switchPort entity in this case
    emit entityAdded(entity->getnstUid(), entitya->getnstUid(), properties);
  }
}

void QnstView::createActionWithDialog(QnstGraphicsEntity* entitya,
                                      QnstGraphicsEntity* entityb)
{
  actionDialog->init(connectors, links[entitya->getnstUid()]);

  if (actionDialog->exec())
  {
    QString link = actionDialog->form.cbLink->currentText();
    QString act = actionDialog->form.cbAction->currentText();

    // creating connector
    QString connName;

    if (actionDialog->form.cbConnector->currentText() == "New...")
      connName = createNewConnector("", act);
    else
      connName = actionDialog->form.cbConnector->currentText();

    // creating link
    if (link == "New...")
    {
      createLink(entitya, entityb, connName);
    }
    else
    {
      QnstGraphicsLink* link = links[entitya->getnstUid()];

      // creating action bind
      QnstGraphicsBind* bind = createBind(link,
                                          entityb,
                                          "",
                                          act);

      // associating connector to bind
      if(connectors.contains(connName))
        bind->setConn(connectors[connName]);

      link->addAction(bind);
    }
  }
}

void QnstView::createConditionWithDialog(QnstGraphicsEntity* entitya,
                                         QnstGraphicsEntity* entityb)
{
  conditionDialog->init(connectors, links[entityb->getnstUid()]);

  if (conditionDialog->exec())
  {
    QString link = conditionDialog->form.cbLink->currentText();
    QString cond = conditionDialog->form.cbCondition->currentText();

    // creating connector
    QString connName;

    if (conditionDialog->form.cbConnector->currentText() == "New...")
      connName = createNewConnector(cond, "");
    else
      connName = conditionDialog->form.cbConnector->currentText();

    // creating link
    if (link == "New...")
    {
      createLink(entitya, entityb, connName);
    }
    else
    {
      QnstGraphicsLink* link = links[entityb->getnstUid()];
      // creating condition bind
      QnstGraphicsBind* bind = createBind(link,
                                          entitya,
                                          cond,
                                          "");

      // associating connector to bind
      if(connectors.contains(connName))
        bind->setConn(connectors[connName]);

      link->addCondition(bind);
    }
  }
}

void QnstView::createLinkWithDialog(QnstGraphicsEntity* entitya,
                                    QnstGraphicsEntity* entityb)
{
  linkDialog->init(connectors);

  if (linkDialog->exec())
  {
    QString con = linkDialog->form.cbCondition->currentText();
    QString act = linkDialog->form.cbAction->currentText();

    if (con != "" && act != "")
    {
      // creating connector
      QString connName;

      if (linkDialog->getCurrentConnector() == "New...")
        connName = createNewConnector(con, act);
      else
        connName = linkDialog->getCurrentConnector();

      // creating aggregator
      QnstGraphicsLink* link = createLink(entitya, entityb, connName);

      // creating condition bind
      QnstGraphicsBind* condition = createBind(link,
                                               entitya,
                                               con,
                                               "");
      // creating action bind
      QnstGraphicsBind* action = createBind(link,
                                            entityb,
                                            "",
                                            act);

      // associating connector to bind
      if(connectors.contains(connName))
      {
        action->setConn(connectors[connName]);
        condition->setConn(connectors[connName]);
      }

      link->addAction(action);
      link->addCondition(condition);
    }
  }
}

QnstGraphicsBind* QnstView::createBind(QnstGraphicsLink* link,
                                       QnstGraphicsEntity* target,
                                       QString condition,
                                       QString action,
                                       bool notifyCreation)
{
  QnstGraphicsEntity* parent = link->getnstGraphicsParent();

  QnstGraphicsBind* bind = new QnstGraphicsBind(); // \fixme Use makeGraphics
  bind->setLink(link);
  bind->setTarget(target);

  parent->addnstGraphicsEntity(bind);

  // setting type and component/interface
  if (!condition.isEmpty())
  {
    bind->setRole(condition);
    bind->setType(QnstUtil::getBindTypeFromStr(condition));
  }
  else
  {
    bind->setRole(action);
    bind->setType(QnstUtil::getBindTypeFromStr(action));
  }

  link->addnstGraphicsEdge(bind);

  if(target != NULL)
  {
    if(target->getncgType()  == Qncg::Node)
    {
      bind->setComponent(target->getnstId());
      bind->setComponentUid(target->getnstUid());
    }
    else if(target->getncgType()  == Qncg::Interface)
    {
      QnstGraphicsEntity* targetParent = target->getnstGraphicsParent();

      bind->setComponent(targetParent->getnstId());
      bind->setComponentUid(targetParent->getnstUid());
      bind->setInterface(target->getnstId());
      bind->setInterfaceUid(target->getnstUid());
    }

    ((QnstGraphicsEntityWithEdges*)target)->addnstGraphicsEdge(bind);
  }

  connect(bind,
          SIGNAL(bindParamAdded(QString,
                                QString,
                                QMap<QString, QString>)),

          SLOT(requestBindParamAdjust(QString,
                                      QString,
                                      QMap<QString, QString>)));

  connect(bind,
          SIGNAL(bindParamUpdated(QString,
                                  QMap<QString,QString>,
                                  QMap<QString,QString>)),

          SLOT(updateBindParams(QString,
                                QMap<QString,QString>,
                                QMap<QString,QString>)));

  if(notifyCreation)
    requestEntityAddition(bind);

  adjustBind(bind);

  return bind;
}

void QnstView::keyPressEvent(QKeyEvent *event)
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
    if (selected != NULL)
    {
      selected->setSelected(false);
      selected->adjust();
    }

    selected = NULL;
    modified = true;

    event->accept();
  }
  else if(event->key() == Qt::Key_Control)
  {
//      modified = true;
    QnstGraphicsEntity *entity;
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
  else if(event->modifiers() == Qt::ControlModifier &&
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
  }

  if(!event->isAccepted())
    QGraphicsView::keyPressEvent(event);
}

void QnstView::keyReleaseEvent(QKeyEvent *event)
{
  // SHIFT - Disabling linking
  if (event->key() == Qt::Key_Shift)
  {
    modified = false;
  }
  else if(event->key() == Qt::Key_Control)
  {
    QnstGraphicsEntity *entity;
    foreach(entity, entities.values())
    {
      entity->setDraggable(false);
    }
  }

  QGraphicsView::keyReleaseEvent(event);
}

void QnstView::wheelEvent(QWheelEvent * event)
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

void QnstView::focusOutEvent(QFocusEvent *event)
{
  QnstGraphicsEntity *entity;
  foreach(entity, entities.values())
  {
    entity->setDraggable(false);
  }
}

void QnstView::adjustAngles(QnstGraphicsEdge* edge)
{
  QnstGraphicsEntityWithEdges *entitya = (QnstGraphicsEntityWithEdges*)edge->getEntityA();
  QnstGraphicsEntityWithEdges *entityb = (QnstGraphicsEntityWithEdges*)edge->getEntityB();

  if(entitya == NULL || entityb == NULL) return; // do nothing

  int angle = 0;
  bool par = false;
  foreach (QnstGraphicsEdge *edge, entitya->getnstGraphicsEdges())
  {
    if(edge->getEntityB() == entityb)
    {
      if(par)
      {
        edge->setAngle(angle);
        par = false;
      }
      else
      {
        edge->setAngle(-angle);
        angle += 60;
        par = true;
      }

      edge->adjust();
    }
  }
}

void QnstView::requestBindParamAdjust(QString uid, QString parent,
                                      QMap<QString, QString> properties)
{
  qWarning() << "Request bind param adjust";
  properties["TYPE"] = "bindParam";

  if (!properties["name"].isEmpty() && !properties["value"].isEmpty())
  {
    if (bindParamUIDToBindUID.contains(uid))
    {
      qWarning() << "Request bind param changing entity";
      emit entityChanged(uid, properties);
    }
    else
    {
      bindParamUIDToBindUID[uid] = parent;
      emit entityAdded(uid, parent, properties );
    }
  }
  else if (!properties["name"].isEmpty())
  {
    bindParamUIDToBindUID.remove(uid);

    emit entityRemoved(uid);
  }
}

void QnstView::updateBindParams(QString bindUID, QMap<QString, QString> params,
                                QMap<QString, QString> name_uids)
{
  if (binds.contains(bindUID))
  {
    binds[bindUID]->setParams(params);
    binds[bindUID]->setNamesUIDs(name_uids);
  }
}

void QnstView::markError(QString uid)
{
  if(entities.contains(uid))
  {
    QnstGraphicsEntity *entity = entities[uid];
    assert(entity != NULL);
    entity->setError(true);
  }
}

void QnstView::clearValidationErrors()
{
  foreach(QnstGraphicsEntity *entity, entities.values())
  {
    assert(entity != NULL);
    entity->setError(false);
  }
}

void QnstView::clearAllData()
{
  if(scene->getRoots().size())
    scene->removeRoot(scene->getRoots().at(0));

  entities.clear();
  links.clear();

  refers.clear();
  importBases.clear();

  bindParamUIDToBindUID.clear();

  connectors.clear();
  connectors2.clear();

  deletePendingEntities();
}

void QnstView::traceRefers()
{
  qDebug() << "#### TRACING REFER ####";
  foreach(QString key, refers.keys())
  {
    assert(refers[key] != NULL);
    if(entities.contains(key))
      qDebug() << key << refers[key];
    else
      qDebug() << "WARNING! This entity " + key + " is inside refer, but not in the entities map.";
  }
  qDebug() << "#### END TRACING REFER ####" << endl;
}

void QnstView::traceEntities()
{
  qDebug() << "#### TRACING ENTITIES ####";
  foreach(QString key, entities.keys())
  {
    assert(entities[key] != NULL);
    assert(entities[key] != entities[key]->getnstGraphicsParent());
    qDebug() << key << (int)entities[key] << entities[key]->getnstType()
             << QnstUtil::getStrFromNstType(entities[key]->getnstType());

    if(entities[key]->getnstGraphicsParent())
      qDebug()<< QnstUtil::getStrFromNstType(entities[key]->getnstGraphicsParent()->getnstType());

    foreach(QnstGraphicsEntity * e, entities[key]->getnstGraphicsEntities())
      qDebug() << "\t" << (int)e << e->getnstType() << QnstUtil::getStrFromNstType(e->getnstType());
  }
  qDebug() << "#### END TRACING ENTITIES ####" << endl;
}

void QnstView::deletePendingEntities()
{
  foreach(QnstGraphicsEntity* e, toDelete)
  {
    e->deleteLater();
  }

  toDelete.clear();
}

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

  nlink = 0;

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

  for (unsigned int i=0;i<list.length();i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      read(e, parent);
    }
  }

  for (unsigned int i=0;i<list.length();i++)
  {
    if (list.item(i).isElement())
    {
      QDomElement e = list.item(i).toElement();

      readLink(e, parent);
    }
  }

  for (unsigned int i=0;i<list.length();i++)
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
}

void QnstView::collapseCompositions(QDomElement element, QDomElement parent)
{
  QDomNodeList list = element.childNodes();

  for (unsigned int i=0;i<list.length();i++)
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
      element.nodeName() == "aggregator" ||
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
    interfaceRefers[element.attribute("interfaceReferUID")] =
        element.attribute("interfaceOriginUID");
  }
  else if (element.nodeName() == "nodeReference")
  {
    refers[element.attribute("nodeReferUID")] = element.attribute("nodeOriginUID");
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
  if (element.nodeName() == "linkdata")
  {
    QnstLink* link = new QnstLink();
    link->setnstId(element.attribute("id"));
    link->setnstUid(element.attribute("uid"));

    if(entities.contains(element.attribute("parent")))
    {
      link->setnstParent(entities[element.attribute("parent")]);

      link->setxConnector(element.attribute("xconnetor"));
      link->setxConnectorUID(element.attribute("xconnetorUID"));
      link->setAggregatorUID(element.attribute("aggregatorUID"));

      links[link->getnstUid()] = link;
      link2conn[link->getnstId()] = link->getxConnector();
    }
    else
      qWarning() << "Error trying to add a link inside" << element.attribute("parent") << "that does not exists.";

  }
  else if (element.nodeName() == "mapping")
  {
    // \fixme Why this is here, and not inside the read method ?
    QMap<QString, QString> properties;

    properties["TYPE"] = element.nodeName();

    properties["uid"] = element.attribute("uid");

    properties["component"] = element.attribute("component");
    properties["componentUid"] = element.attribute("componentUid");

    properties["interface"] = element.attribute("interface");
    properties["interfaceUid"] = element.attribute("interfaceUid");

    addEntity(properties["uid"], element.attribute("switchportUid"), properties, true);

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
          entity->setnstGraphicsParent(entitya->getnstGraphicsParent());
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
        QnstGraphicsInterface* entitya = (QnstGraphicsInterface*) entities[element.attribute("uid")];
        QnstGraphicsNode* entityb = (QnstGraphicsNode*) entities[element.attribute("componentUID")];

        QnstGraphicsReference* entity = new QnstGraphicsReference();
        entity->setnstGraphicsParent(entitya->getnstGraphicsParent());
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
    if (element.attribute("uid") != "" &&
        entities.contains(element.attribute("componentaUID")) &&
        entities.contains(element.attribute("componentbUID")))
    {
      qWarning() << entities.contains(element.attribute("uid"))
                 << links.contains(element.attribute("linkUID"));

      if (!entities.contains(element.attribute("uid")) &&
          links.contains(element.attribute("linkUID")))
      {
        QnstGraphicsEntity* entitya = entities[element.attribute("componentaUID")];
        QnstGraphicsEntity* entityb = entities[element.attribute("componentbUID")];

        QnstGraphicsBind *graphicsBind = NULL;
        QnstBind* bo = NULL;
        QnstLink* lo = NULL;

        int bindType = -1;

        // Get the bindType as Qnst::EntityType
        if(element.attribute("type") == "condition")
          bindType = Qnst::Condition;
        else
          bindType = Qnst::Action;

        if(bindType != -1) // i.e., we have a valid bindType
        {
          graphicsBind = new QnstGraphicsBind();

          // Set action or condition depending of the bindType
          if(bindType == Qnst::Condition)
            graphicsBind->setCondition(Qnst::NoBindType);
          else
            graphicsBind->setAction(Qnst::NoBindType);

          graphicsBind->setnstId(element.attribute("id"));
          graphicsBind->setnstUid(element.attribute("uid"));

          graphicsBind->setEntityA(entitya);
          graphicsBind->setEntityB(entityb);

          // if linking NODE to NODE
          if (entitya->getncgType() == Qncg::Node &&
              entityb->getncgType() == Qncg::Node)
          {
            graphicsBind->setnstGraphicsParent(entitya->getnstGraphicsParent());
            entitya->getnstGraphicsParent()->addnstGraphicsEntity(graphicsBind);
          }
          // if linking NODE to INTERFACE
          else if (entitya->getncgType() == Qncg::Node &&
                   entityb->getncgType() == Qncg::Interface)
          {
            graphicsBind->setnstGraphicsParent(entitya->getnstGraphicsParent());
            entitya->getnstGraphicsParent()->addnstGraphicsEntity(graphicsBind);
          }
          // if linking INTERFACE to NODE
          else if (entitya->getncgType() == Qncg::Interface &&
                   entityb->getncgType() == Qncg::Node)
          {
            graphicsBind->setnstGraphicsParent(entityb->getnstGraphicsParent());
            entityb->getnstGraphicsParent()->addnstGraphicsEntity(graphicsBind);
          }
          // if linking INTERFACE to INTERFACE
          else if (entitya->getncgType() == Qncg::Interface &&
                   entityb->getncgType() == Qncg::Interface)
          {
            graphicsBind->setnstGraphicsParent(entitya->getnstGraphicsParent()->getnstGraphicsParent());
            entitya->getnstGraphicsParent()->getnstGraphicsParent()->addnstGraphicsEntity(graphicsBind);
          }

          // adjusting angle
          adjustAngle(graphicsBind, entitya, entityb);

          graphicsBind->adjust();

          ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(graphicsBind);
          ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(graphicsBind);

          QString roleStr;

          // Set action or condition role
          if(bindType == Qnst::Condition) // Qnst::Condition
          {
            roleStr = element.attribute("condition");
            graphicsBind->setCondition(QnstUtil::getBindTypeFromStr(roleStr));
          }
          else //Qnst::Action
          {
            roleStr = element.attribute("action");
            graphicsBind->setAction(QnstUtil::getBindTypeFromStr(roleStr));
          }

          lo = links[element.attribute("linkUID")];

          bo = new QnstBind();
          bo->setnstParent(lo);
          bo->setnstUid(graphicsBind->getnstUid());
          bo->setRole(roleStr);

          // Qnst::Condition
          if(bindType == Qnst::Condition)
          {
            if (entitya->getncgType() == Qncg::Interface)
            {
              bo->setComponent(entitya->getnstGraphicsParent()->getnstId());
              bo->setComponentUid(entitya->getnstGraphicsParent()->getnstUid());
              bo->setInterface(entitya->getnstId());
              bo->setInterfaceUid(entitya->getnstUid());
            }
            else
            {
              bo->setComponent(entitya->getnstId());
              bo->setComponentUid(entitya->getnstUid());
            }

            lo->addCondition(bo);
          }
          // Qnst::Action
          else
          {
            if (entitya->getncgType() == Qncg::Interface)
            {
              bo->setComponent(entityb->getnstGraphicsParent()->getnstId());
              bo->setComponentUid(entityb->getnstGraphicsParent()->getnstUid());
              bo->setInterface(entityb->getnstId());
              bo->setInterfaceUid(entityb->getnstUid());
            }
            else
            {
              bo->setComponent(entityb->getnstId());
              bo->setComponentUid(entityb->getnstUid());
            }

            lo->addAction(bo);
          }

          binds[bo->getnstUid()] = bo;

          brelations[bo->getnstUid()] = graphicsBind->getnstUid();

          entities[graphicsBind->getnstUid()] = graphicsBind;

          graphicsBind->setConn(connectors[lo->getxConnector()]);

          nlink++;

          // SET THE BIND PARAMS
          QMap<QString, QString> params;
          QMap<QString, QString> name_uid;

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
              }
            }
          }

          bo->setParams(params);
          bo->setNameUIDs(name_uid);

          connect(graphicsBind,
                  SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                  SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

          connect(graphicsBind,
                  SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                  SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
        }

        graphicsBind->setParams(bo->getParams());
        graphicsBind->setNameUids(bo->getNameUIDs());
        //FINISH BIND PARAMS
      }
    }
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

  foreach(QString key, interfaceRefers.keys())
  {
    QDomElement e = dom->createElement("interfaceReference");

    e.setAttribute("interfaceReferUID", key);
    e.setAttribute("interfaceOriginUID", interfaceRefers[key]);

    root.appendChild(e);
  }

  foreach(QString key, refers.keys())
  {
    QDomElement e = dom->createElement("nodeReference");

    e.setAttribute("nodeReferUID", key);
    e.setAttribute("nodeOriginUID", refers[key]);

    root.appendChild(e);
  }

  foreach(QnstLink* link, links.values())
  {
    QDomElement e = dom->createElement("linkdata");

    e.setAttribute("id", link->getnstId());
    e.setAttribute("uid", link->getnstUid());

    e.setAttribute("parent", link->getnstParent()->getnstUid());

    e.setAttribute("xconnetor", link->getxConnector());
    e.setAttribute("xconnetorUID", link->getxConnectorUID());
    e.setAttribute("aggregatorUID", link->getAggregatorUID());

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
      break;

    case Qnst::SwitchPort:
      e = dom->createElement("switchPort");
      break;

    case Qnst::Area:
      e = dom->createElement("area");
      break;

    case Qnst::Aggregator:
      e = dom->createElement("aggregator");
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

    foreach(QnstGraphicsEdge* edge, node->getnstGraphicsEdges())
    {
      //We already hav this link, so skipping
      if(linkWriterAux.contains(edge->getnstUid())) continue;

      QnstBind* bind = NULL;
      QDomElement e; // the element that will be inserted

      if (edge->getnstType() == Qnst::Condition ||
          edge->getnstType() == Qnst::Action)
      {
        QnstGraphicsBind* link = (QnstGraphicsBind*) edge;

        e = dom->createElement("bind");

        e.setAttribute("id", edge->getnstId());
        e.setAttribute("uid", edge->getnstUid());

        // We are writing a condition
        if (edge->getnstType() ==  Qnst::Condition)
        {
          e.setAttribute("type", "condition");

          // Be carefull, this is different for action
          e.setAttribute("linkUID", link->getEntityB()->getnstUid());
          e.setAttribute("condition",
                         QnstUtil::getStrFromBindType(link->getCondition()));
        }
        // We are writing an action
        else if(edge->getnstType() == Qnst::Action)
        {
          e.setAttribute("type", "action");

          // Be carefull, this is different for condition
          e.setAttribute("linkUID", link->getEntityA()->getnstUid());
          e.setAttribute("action",
                         QnstUtil::getStrFromBindType(link->getAction()));
        }

        e.setAttribute("componentaUID", link->getEntityA()->getnstUid());
        e.setAttribute("componentbUID", link->getEntityB()->getnstUid());


        //It will be used to write params
        bind = binds[brelations.key(link->getnstUid())];

        element.appendChild(e);

        linkWriterAux.insert(link->getnstUid());
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

  connect(scene, SIGNAL(entityAdded(QnstGraphicsEntity*)), SLOT(requestEntityAddition(QnstGraphicsEntity*)));
  connect(scene, SIGNAL(entityChanged(QnstGraphicsEntity*)), SLOT(requestEntityChange(QnstGraphicsEntity*)));
  connect(scene, SIGNAL(entityRemoved(QnstGraphicsEntity*)), SLOT(requestEntityRemotion(QnstGraphicsEntity*)));
  connect(scene, SIGNAL(entitySelected(QnstGraphicsEntity*)), SLOT(requestEntitySelection(QnstGraphicsEntity*)));
  connect(scene, SIGNAL(entityAboutToChange(QnstGraphicsEntity*,QMap<QString,QString>)), SLOT(requestEntityPreparation(QnstGraphicsEntity*,QMap<QString,QString>)));
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
  qDebug() << "[QNST]" << ":" << "Adding entity '"+uid+"'";

  bool ok = false;

  Qnst::EntityType type = QnstUtil::getnstTypeFromStr(properties["TYPE"]);
  QnstGraphicsEntity *entityParent = NULL;

  if(entities.count(parent))
    entityParent = (QnstGraphicsNode*) entities[parent];

  QnstGraphicsEntity *entity = NULL;

  // \fixme There are a lot of duplicated code among the cases bellow!
  switch (type)
  {
    case Qnst::Body:
      entity = QnstUtil::makeGraphicsEntity(type);

      entity->setnstUid(uid);
      entity->setnstId(properties["id"]);

      entity->setProperties(properties);

      entity->setTop(scene->height()/2 - DEFAULT_BODY_HEIGHT/2);
      entity->setLeft(scene->width()/2 - DEFAULT_BODY_WIDTH/2);
      entity->setWidth(DEFAULT_BODY_WIDTH);
      entity->setHeight(DEFAULT_BODY_HEIGHT);

      scene->addRoot(entity);

      entities[uid] = entity;

      ok = true;

      break;

    case Qnst::Context:
    case Qnst::Switch:
      if (entityParent != NULL)
      {
        entity = QnstUtil::makeGraphicsEntity(type, entityParent);
        if(entity != NULL)
        {
          entity->setnstUid(uid);

          if (properties["id"] != "") entity->setnstId(properties["id"]);

          // Override the default position with the center of the parent
          entity->setTop(entityParent->getHeight()/2 - DEFAULT_CONTEXT_HEIGHT/2);
          entity->setLeft(entityParent->getWidth()/2 - DEFAULT_CONTEXT_WIDTH/2);

          entity->setProperties(properties);

          entities[parent]->addnstGraphicsEntity(entity);

          entities[uid] = entity;

          entity->adjust();

          ok = true;
        }
      }
      break;

    case Qnst::Media:
      if (entityParent != NULL)
      {
        entity = QnstUtil::makeGraphicsEntity(type, entityParent);

        if(entity != NULL)
        {
          entity->setnstType(QnstUtil::getnstTypeFromMime(properties["type"]));

          entity->setnstUid(uid);

          // Override the default position with the center of the parent
          entity->setTop(entityParent->getHeight()/2 - DEFAULT_MEDIA_HEIGHT/2);
          entity->setLeft(entityParent->getWidth()/2 - DEFAULT_MEDIA_WIDTH/2);

          entities[parent]->addnstGraphicsEntity(entity);
          entities[uid] = entity;

          // Update the entity properties
          changeEntity(uid, properties, adjust);

          entity->adjust();

          ok = true;
        }
      }
      break;

    case Qnst::Port:
      if (entityParent != NULL)
      {
        entity = QnstUtil::makeGraphicsEntity(type, entityParent);

        if(entity != NULL)
        {
          entity->setnstUid(uid);

          entity->setnstId(properties["id"]);

          entities[parent]->addnstGraphicsEntity(entity);
          entities[uid] = entity;

          // Update the entity properties
          changeEntity(uid, properties);

          ok = true;
        }
      }
      break;

    case Qnst::Link:
      // \fixme THIS IS NOT BEING USED
      if(entityParent != NULL)
      {
        if (!links.contains(uid))
        {
          QnstLink* entity = new QnstLink(); // \todo Use makeGraphicsEntity!!
          entity->setnstUid(uid);
          entity->setnstParent(entityParent);

          if (properties["id"] != "")
            entity->setnstId(properties["id"]);

          if (properties["xconnector"] != "")
          {
            entity->setxConnector(properties["xconnector"]);
            entity->setxConnectorUID(properties["xconnectorUID"]);
          }

          links[uid] = entity;

          adjustLink(entity);
        }
      }
      break;

    case Qnst::Bind:
      if(entityParent != NULL)
      {
        if (links.contains(parent))
        {
          if (!binds.contains(uid))
          {
            QnstBind* entity = new QnstBind(); // \todo Use makeGraphicsEntity!
            entity->setnstUid(uid);
            entity->setnstParent(links[parent]);

            entity->setRole(properties["role"]);

            entity->setComponent(properties["component"]);
            entity->setComponentUid(properties["componentUid"]);


            entity->setInterface(properties["interface"]);
            entity->setInterfaceUid(properties["interfaceUid"]);

            binds[uid] = entity;

            adjustBind(entity);
          }
        }
      }
      break;

    case Qnst::BindParam: /* Warning: this is not an QnstEntity subclass */
      if (entities.contains(parent))
      {
        QnstGraphicsEntity* e = entities[parent];

        if (e->getnstType() == Qnst::Action)
        {
          QnstGraphicsBind* action = (QnstGraphicsBind*) e;

          action->addParam(uid, properties["name"], properties["value"]);
        }
        else if (e->getnstType() == Qnst::Condition)
        {
          QnstGraphicsBind* condition = (QnstGraphicsBind*) e;

          condition->addParam(uid, properties["name"], properties["value"]);
        }

        bindParamUIDToBindUID[uid] = brelations.key(parent);
      }
      break;

    case Qnst::SwitchPort:
      if (entityParent != NULL)
      {
        entity = QnstUtil::makeGraphicsEntity(type, entityParent);

        entity->setnstUid(uid);
        entity->setnstGraphicsParent(entities[parent]);

        entity->setnstId(properties["id"]);

        entity->setProperties(properties);

        entities[parent]->addnstGraphicsEntity(entity);
        entities[uid] = entity;

        entity->adjust();

        ok = true;
      }
      break;

    case Qnst::Mapping:
      if (entityParent != NULL)
      {
        QnstGraphicsNode* oparent =
            (QnstGraphicsNode*) entities[parent]->getnstGraphicsParent();

        QnstGraphicsMapping* entity = new QnstGraphicsMapping(oparent);
        entity->setnstUid(uid);

        entity->setSwitchPortUid(parent);

        entity->setProperties(properties);

        oparent->addnstGraphicsEntity(entity);
        entities[entity->getnstUid()] = entity;

        adjustMapping(entity);
      }
      break;

    case Qnst::Aggregator:
      if (entityParent)
      {
        entity = QnstUtil::makeGraphicsEntity(type, entityParent);
        entity->setnstUid(uid);

        if (properties["id"] != "")
          entity->setnstId(properties["id"]);

        entity->setProperties(properties);

        entities[parent]->addnstGraphicsEntity(entity);
        entities[uid] = entity;

        entity->adjust();

        ok = true;
      }
      break;

    case Qnst::Area:
    case Qnst::Property:

      if (entityParent)
      {
        entity = QnstUtil::makeGraphicsEntity(type, entityParent);
        entity->setnstUid(uid);
        entity->setnstGraphicsParent(entityParent);

        entity->setnstId(properties["id"]);

        entity->setProperties(properties);

        entities[parent]->addnstGraphicsEntity(entity);
        entities[uid] = entity;

        entity->adjust();

        if (adjust) // update media
        {
          if(entityParent->isMedia())
            adjustMedia((QnstGraphicsMedia*) entities[parent]);

          // Update the medias that refer to this one.
          foreach (QString key,
                   refers.keys(entity->getnstGraphicsParent()->getnstUid()))
          {
            if (entities.contains(key) && entities[key]->isMedia())
                adjustMedia((QnstGraphicsMedia*) entities[key]);
          }
        }

        ok = true;
      }
      break;

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
}

void QnstView::removeEntity(const QString uid, bool undo, bool rmRef)
{
    qDebug() << "[QNST]" << ":" << "Removing entity '"+uid+"'";

    if (entities.contains(uid)){

        QnstGraphicsEntity* entity = entities[uid];

        if (entity != NULL){
            if (entity->getncgType() == Qncg::Node){
                if (!undo){
                  QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
                  history.push(cmd);
                }

                foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities())
                {
                  if (e->getnstType() != Qnst::Link &&
                      e->getnstType() != Qnst::Edge &&
                      e->getnstType() != Qnst::Condition &&
                      e->getnstType() != Qnst::Mapping &&
                      e->getnstType() != Qnst::Action &&
                      e->getnstType() != Qnst::Reference){

                      if (entity->isMedia())
                      {
                        QnstGraphicsMedia* m = (QnstGraphicsMedia*) entity;

                        if (m->getReferUID() != "")
                           removeEntity(e->getnstUid(), true, false);
                        else
                           removeEntity(e->getnstUid(), true, false);
                      }
                      else
                      {
                        removeEntity(e->getnstUid(), true, false);
                      }
                  }
                }

                foreach(QnstGraphicsEdge* edge, ((QnstGraphicsNode*) entity)->getnstGraphicsEdges()){
                    if (edge->getEntityA()->getncgType() == Qncg::Node){
                        ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                    }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                        ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                    }

                    if (edge->getEntityB()->getncgType() == Qncg::Node){
                        ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                    }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                        ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                    }

                    edge->getEntityA()->removeAngle(edge->getEntityB()->getnstUid(), edge->getAngle());
                    edge->getEntityB()->removeAngle(edge->getEntityA()->getnstUid(), -edge->getAngle());

                    if (edge->getnstType() == Qnst::Condition ||
                        edge->getnstType() == Qnst::Action){

                        QnstBind* bb = binds[edge->getnstUid()];

                        if (bb != NULL){
                            QnstLink* ll = (QnstLink*) bb->getnstParent();

                            if (ll != NULL){
                                if (ll->getConditions().contains(bb->getnstUid())){
                                    ll->removeCondition(bb);
                                }

                                if (ll->getActions().contains(bb->getnstUid())){
                                    ll->removeAction(bb);
                                }

                            }

                            QnstGraphicsEntity* parent = entity->getnstGraphicsParent();
                            parent->removenstGraphicsEntity(edge);

                            binds.remove(bb->getnstUid());
                            brelations.remove(bb->getnstUid());
                        }
                    }

                    QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
                    parent->removenstGraphicsEntity(edge);

                    entities.remove(edge->getnstUid());
                }

                QnstGraphicsEntity* parent = entity->getnstGraphicsParent();

                if (parent != NULL){
                    parent->removenstGraphicsEntity(entity);

                }else{
                    scene->removeRoot(entity);
                }

                if (selected == entity){
                    selected = NULL;
                }

                if (clipboard == entity){
                    clipboard = NULL;
                }

                if (entity->getnstType() == Qnst::Aggregator){
                    if (links.contains(entity->getnstUid())){
                        link2conn.remove(links[entity->getnstUid()]->getnstId());
                        links.remove(entity->getnstUid());
                    }
                }

                entities.remove(uid); delete entity;

            }else if (entity->getncgType() == Qncg::Interface){
                if (!undo){
                    QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
                    history.push(cmd);
                }

                foreach(QnstGraphicsEdge* edge, ((QnstGraphicsInterface*) entity)->getnstGraphicsEdges()){
                    if (edge->getEntityA()->getncgType() == Qncg::Node){
                        ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                    }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                        ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                    }

                    if (edge->getEntityB()->getncgType() == Qncg::Node){
                        ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                    }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                        ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                    }

                    edge->getEntityA()->removeAngle(edge->getEntityB()->getnstUid(), edge->getAngle());
                    edge->getEntityB()->removeAngle(edge->getEntityA()->getnstUid(), -edge->getAngle());

                    if (edge->getnstType() == Qnst::Condition ||
                        edge->getnstType() == Qnst::Action){

                        QnstBind* bb = binds[edge->getnstUid()];

                        if (bb != NULL){
                            QnstLink* ll = (QnstLink*) bb->getnstParent();

                            if (ll != NULL){
                                if (ll->getConditions().contains(bb->getnstUid())){
                                    ll->removeCondition(bb);
                                }

                                if (ll->getActions().contains(bb->getnstUid())){
                                    ll->removeAction(bb);
                                }

                            }

                            binds.remove(bb->getnstUid());
                            brelations.remove(bb->getnstUid());
                        }
                    }

                    QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
                    parent->removenstGraphicsEntity(edge);

                    entities.remove(edge->getnstUid());
                }

                QnstGraphicsEntity* parent = entity->getnstGraphicsParent();

                if (parent != NULL)
                {
                  if (parent->isMedia())
                  {
                    QnstGraphicsMedia* m = (QnstGraphicsMedia*) parent;

                    if (m->getInstance() != "new"){
                      // remove interface tha "I" make a reference
                      if (interfaceRefers.contains(entity->getnstUid())){
                        removeEntity(interfaceRefers[entity->getnstUid()]);
                      }
                    }

//                    // remove interfaces that reference "ME"
//                    foreach(QString rUID, interfaceRefers.keys(entity->getnstUid())){
//                      removeEntity(rUID);
//                    }
                  }

                  parent->removenstGraphicsEntity(entity);

                }

                if (selected == entity){
                    selected = NULL;
                }

                if (clipboard == entity){
                    clipboard = NULL;
                }

                entities.remove(uid); delete entity;

            }else if (entity->getncgType() == Qncg::Edge){
                QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

                if (edge->getEntityA()->getncgType() == Qncg::Node){
                    ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                    ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                }

                if (edge->getEntityB()->getncgType() == Qncg::Node){
                    ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                    ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                }

                edge->getEntityA()->removeAngle(edge->getEntityB()->getnstUid(), edge->getAngle());
                edge->getEntityB()->removeAngle(edge->getEntityA()->getnstUid(), -edge->getAngle());

                if (edge->getnstType() == Qnst::Condition ||
                    edge->getnstType() == Qnst::Action){

                    QnstBind* bb = binds[edge->getnstUid()];

                    if (bb != NULL){
                        QnstLink* ll = (QnstLink*) bb->getnstParent();

                        if (ll != NULL){
                            if (ll->getConditions().contains(bb->getnstUid())){
                                ll->removeCondition(bb);
                            }

                            if (ll->getActions().contains(bb->getnstUid())){
                                ll->removeAction(bb);
                            }

                        }

                        binds.remove(bb->getnstUid());
                        brelations.remove(bb->getnstUid());
                    }
                }

                QnstGraphicsEntity* parent = entity->getnstGraphicsParent();
                parent->removenstGraphicsEntity(edge);

                entities.remove(edge->getnstUid());
            }
        }
    }else if (importBases.contains(uid)){

        foreach(QnstConnector* cc, connectors.values()){
            if (cc->getnstUid() == importBases[uid]){
                connectors.remove(cc->getName());
            }
        }

        foreach(QnstConnector* cc, connectors2.values()){
            if (cc->getnstUid() == importBases[uid]){
                connectors2.remove(cc->getnstId());
            }
        }

        importBases.remove(uid);

        foreach(QnstBind* b, binds.values()){
            adjustBind(b);
        }
    }else if (connectors2.contains(uid)){

        connectors.remove(connectors2[uid]->getName());
        connectors2.remove(uid);
    }else if (bindParamUIDToBindUID.contains(uid)){

        QnstGraphicsEntity* e = entities[brelations[bindParamUIDToBindUID[uid]]];

        if (e->getnstType() == Qnst::Action ||
            e->getnstType() == Qnst::Condition)
        {
            QnstGraphicsBind* a = (QnstGraphicsBind*) e;

            a->removeUId(uid);
        }

        bindParamUIDToBindUID.remove(uid);
    }
}

void QnstView::changeEntity(const QString uid,
                            const QMap<QString, QString> &properties,
                            bool adjust)
{
  qDebug() << "[QNST]" << ":" << "Changing entity '"+uid+"'"
           << properties["TYPE"];

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
  else if (binds.contains(uid))
  {
    changeBind(binds[uid], properties);
  }
  else if (entities.contains(uid))
  {
    QnstGraphicsEntity* entity = entities[uid];

    switch(entity->getnstType())
    {
      case Qnst::Body:
        entity->setnstId(properties["id"]);
        entity->setProperties(properties);

          entity->adjust();
          break;

      case Qnst::Context:
      case Qnst::Switch:
          if (properties["id"] != "")
            entity->setnstId(properties["id"]);

          entity->setProperties(properties);
          // \todo refer
          break;

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
              adjustMedia(media);
          }
        }
        break;

      // if the entity type is PORT
      case Qnst::Port:
        {
          entity->setnstId(properties["id"]);

          entity->setProperties(properties);

          foreach (QString key, interfaceRefers.keys(entity->getnstUid()))
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

      // if the entity type is AREA or Property
      case Qnst::Area:
      case Qnst::Property:
        {
          entity->setnstId(properties["id"]);

          entity->setProperties(properties);

          foreach (QString key, interfaceRefers.keys(entity->getnstUid()))
          {
            if (entities.contains(key))
              entities[key]->setnstId(entity->getnstId());
          }
        }
        break;

      // if the entity type is SwitchPort
      case Qnst::SwitchPort:
          if(properties.contains("id"))
            entity->setnstId(properties["id"]);

          entity->setProperties(properties);

          entity->adjust();
          break;

      // if the entity type is Mapping
      case Qnst::Mapping:
        {
          entity->setProperties(properties);

          QnstGraphicsMapping *mapping = dynamic_cast<QnstGraphicsMapping*> (entity);
          adjustMapping(mapping);
        }
        break;

      default:
        // do nothing
        break;
    }
  }
  else if (properties["TYPE"] == "importBase")
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

    if (file->exists()){
        if (file->open(QIODevice::ReadOnly)){
            QDomDocument* domdoc = new QDomDocument();

            if (domdoc->setContent(file)){
                readImportBase(connUID, domdoc->firstChildElement(), properties["alias"]);
            }
        }
    }

    delete file;

    foreach(QnstBind* b, binds.values()){
        adjustBind(b);
    }
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

  foreach(QnstBind* b, binds.values())
  {
    adjustBind(b);
  }
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
    {
      conn->addParam(QUuid::createUuid().toString(), element.attribute("name"));
    }
  }

  if (element.tagName() == "simpleCondition" ||
      element.tagName() == "attributeAssessment")
  {
    if (element.attribute("role") != "")
    {
      conn->addCondition(element.attribute("role"), element.attribute("role"));
    }
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

    if (entity->getRefer() != "" && entity->getReferUID() != "" && entity->getInstance() != ""){

        if (entities.contains(entity->getReferUID())){
            QnstGraphicsEntity* origin = entities[entity->getReferUID()];

            foreach (QnstGraphicsEntity* oe, origin->getnstGraphicsEntities()){

                if (oe->getncgType() == Qncg::Interface){
                    bool contains = false;

                    if (interfaceRefers.contains(oe->getnstUid())){
                        contains = true;

                    }else{
                        foreach (QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
                            if (interfaceRefers.contains(e->getnstUid())){
                                if (interfaceRefers[e->getnstUid()] == oe->getnstUid()){
                                    contains = true;
                                    break;
                                }
                            }
                        }
                    }

                    if (!contains){

                        if (oe->getnstType() == Qnst::Port){
                            QnstGraphicsPort* i = new QnstGraphicsPort(entity);
                            QnstGraphicsPort* p = (QnstGraphicsPort*) oe;


                            i->setnstId(oe->getnstId());

                            i->setTop(oe->getTop());
                            i->setLeft(oe->getLeft());
                            i->setWidth(oe->getWidth());
                            i->setHeight(oe->getHeight());

                            i->adjust();

                            entity->addnstGraphicsEntity(i);

                            entities[i->getnstUid()] = i;

                            interfaceRefers[i->getnstUid()] = oe->getnstUid();

                        }else if (oe->getnstType() == Qnst::Area){
                            QnstGraphicsArea *i = new QnstGraphicsArea(entity);

                            i->setnstId(oe->getnstId());

                            i->setTop(oe->getTop());
                            i->setLeft(oe->getLeft());
                            i->setWidth(oe->getWidth());
                            i->setHeight(oe->getHeight());

                            i->adjust();

                            entity->addnstGraphicsEntity(i);

                            entities[i->getnstUid()] = i;

                            interfaceRefers[i->getnstUid()] = oe->getnstUid();


                        }else if (oe->getnstType() == Qnst::Property){
                            QnstGraphicsProperty *i = new QnstGraphicsProperty(entity);

                            i->setnstId(oe->getnstId());

                            i->setTop(oe->getTop());
                            i->setLeft(oe->getLeft());
                            i->setWidth(oe->getWidth());
                            i->setHeight(oe->getHeight());
                            i->adjust();

                            entity->addnstGraphicsEntity(i);

                            entities[i->getnstUid()] = i;

                            interfaceRefers[i->getnstUid()] = oe->getnstUid();
                        }
                    }
                }
            }

            foreach (QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
                if (e->getncgType() == Qncg::Interface){
                    bool contains = false;

                    foreach (QnstGraphicsEntity* oe, origin->getnstGraphicsEntities()){
                        if (interfaceRefers.contains(oe->getnstUid())){
                            if (interfaceRefers[oe->getnstUid()] == e->getnstUid()){
                                contains = true;
                                break;
                            }
                        }
                    }

                    if (!contains){
                    if (interfaceRefers.contains(e->getnstUid())){
                        contains = true;
                    }
                    }

                    if (!contains && entity->getInstance() != "new"){

                        if (e->getnstType() == Qnst::Port){
                            QnstGraphicsPort *i = new QnstGraphicsPort(origin);

                            i->setnstId(e->getnstId());

                            i->setTop(e->getTop());
                            i->setLeft(e->getLeft());
                            i->setWidth(e->getWidth());
                            i->setHeight(e->getHeight());
                            i->adjust();

                            origin->addnstGraphicsEntity(i);

                            entities[i->getnstUid()] = i;

//                            interfaceRefers[e->getnstUid()] = i->getnstUid();
                            interfaceRefers[i->getnstUid()] = e->getnstUid();

                            origin->adjust();

                        }else if (e->getnstType() == Qnst::Area){
                            QnstGraphicsArea *i = new QnstGraphicsArea(origin);

                            i->setnstId(e->getnstId());

                            i->setTop(e->getTop());
                            i->setLeft(e->getLeft());
                            i->setWidth(e->getWidth());
                            i->setHeight(e->getHeight());
                            i->adjust();

                            origin->addnstGraphicsEntity(i);

                            entities[i->getnstUid()] = i;

//                            interfaceRefers[e->getnstUid()] = i->getnstUid();
                            interfaceRefers[i->getnstUid()] = e->getnstUid();

                            origin->adjust();

                        }else if (e->getnstType() == Qnst::Property){
                            QnstGraphicsProperty *i = new QnstGraphicsProperty(origin);

                            i->setnstId(e->getnstId());

                            i->setTop(e->getTop());
                            i->setLeft(e->getLeft());
                            i->setWidth(e->getWidth());
                            i->setHeight(e->getHeight());
                            i->adjust();

                            origin->addnstGraphicsEntity(i);

                            entities[i->getnstUid()] = i;

//                            interfaceRefers[e->getnstUid()] = i->getnstUid();
                            interfaceRefers[i->getnstUid()] = e->getnstUid();

                            origin->adjust();
                        }
                    }
                }
            }

            refers[entity->getnstUid()] = origin->getnstUid();
//            refers[refer->getnstUid()] = entity->getnstUid();
/*
            foreach (QnstGraphicsEntity* e, refer->getnstGraphicsEntities()){
                if (e->getncgType() == Qncg::Interface){

                    if (e->getnstType() == Qnst::Port){
                        QnstGraphicsPort *i = new QnstGraphicsPort(entity);

                        i->setnstId(e->getnstId());

                        i->setTop(e->getTop());
                        i->setLeft(e->getLeft());
                        i->setWidth(e->getWidth());
                        i->setHeight(e->getHeight());
                        i->adjust();

                        entity->addnstGraphicsEntity(i);

                        entities[i->getnstUid()] = i;

                    }else if (e->getnstType() == Qnst::Area){
                        QnstGraphicsArea *i = new QnstGraphicsArea(entity);

                        i->setnstId(e->getnstId());

                        i->setTop(e->getTop());
                        i->setLeft(e->getLeft());
                        i->setWidth(e->getWidth());
                        i->setHeight(e->getHeight());
                        i->adjust();

                        entity->addnstGraphicsEntity(i);

                        entities[i->getnstUid()] = i;

                    }else if (e->getnstType() == Qnst::Property){
                        QnstGraphicsProperty *i = new QnstGraphicsProperty(entity);

                        i->setnstId(e->getnstId());

                        i->setTop(e->getTop());
                        i->setLeft(e->getLeft());
                        i->setWidth(e->getWidth());
                        i->setHeight(e->getHeight());
                        i->adjust();

                        entity->addnstGraphicsEntity(i);

                        entities[i->getnstUid()] = i;
                    }
                }
            }
            */
        }
    }

    entity->adjust();
}

// \todo refactory
void QnstView::adjustPort(QnstGraphicsPort* entity)
{
    QnstGraphicsEntity* parent = entity->getnstGraphicsParent();

    if (parent != NULL){
        // removing previous edge
        foreach(QnstGraphicsEntity* e, parent->getnstGraphicsEntities()){
            if (e->getnstType() == Qnst::Reference){
                QnstGraphicsEdge* edge = (QnstGraphicsEdge*) e;

                if (entity == edge->getEntityA()){
                    if (edge->getEntityA()->getncgType() == Qncg::Node){
                        ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                    }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                        ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                    }

                    if (edge->getEntityB()->getncgType() == Qncg::Node){
                        ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                    }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                        ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                    }

                    parent->removenstGraphicsEntity(edge); delete (edge);

                    break;
                }
            }
        }

        // adding new edge
        if (entity->getInterfaceUid() != ""){
            // changing
            if (entities.contains(entity->getInterfaceUid())){
                QnstGraphicsEntity* entitya = (QnstGraphicsEntity*) entity;
                QnstGraphicsEntity* entityb = (QnstGraphicsEntity*) entities[entity->getInterfaceUid()];

                QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
                QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

                if (parenta != NULL && parentb != NULL){
                    if (entitya != entityb && parenta == parentb->getnstGraphicsParent()){
                        QnstGraphicsReference* edge = new QnstGraphicsReference();
                        edge->setnstGraphicsParent(parenta);
                        edge->setEntityA(entitya);
                        edge->setEntityB(entityb);
                        edge->adjust();

                        parent->addnstGraphicsEntity(edge);

                        ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(edge);
                        ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(edge);
                    }
                }
            }

        }else if (entity->getComponentUid() != ""){
            // changing
            if (entities.contains(entity->getComponentUid())){
                QnstGraphicsEntity* entitya = (QnstGraphicsEntity*) entity;
                QnstGraphicsEntity* entityb = (QnstGraphicsEntity*) entities[entity->getComponentUid()];

                QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
                QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

                if (parenta != NULL && parentb != NULL){
                    if (entitya != entityb && parenta == parentb){
                        QnstGraphicsReference* edge = new QnstGraphicsReference();
                        edge->setnstGraphicsParent(parenta);
                        edge->setEntityA(entitya);
                        edge->setEntityB(entityb);
                        edge->adjust();

                        parent->addnstGraphicsEntity(edge);

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

    if (entities.contains(entity->getComponentUid()) && entities.contains(entity->getSwitchPortUid())){
        QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

        if (edge->getEntityA() != NULL){
            if (edge->getEntityA()->getncgType() == Qncg::Node){
                ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

            }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
            }
        }

        if (edge->getEntityB() != NULL){
            if (edge->getEntityB()->getncgType() == Qncg::Node){
                ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

            }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
            }
        }

        QnstGraphicsEntity* parent = entities[entity->getSwitchPortUid()]->getnstGraphicsParent();
        edge->setnstGraphicsParent(parent);

        if (!parent->getnstGraphicsEntities().contains(edge)){
            parent->addnstGraphicsEntity(edge);
        }

        edge->setEntityA(entities[entity->getSwitchPortUid()]);

        if (entity->getInterfaceUid() != "" && entities.contains(entity->getInterfaceUid())){
            edge->setEntityB(entities[entity->getInterfaceUid()]);
        }else{
            edge->setEntityB(entities[entity->getComponentUid()]);
        }

        ((QnstGraphicsInterface*) edge->getEntityA())->addnstGraphicsEdge(entity);
        ((QnstGraphicsNode*) edge->getEntityB())->addnstGraphicsEdge(entity);

        edge->adjust();
    }else{
        QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

        if (edge->getEntityA() != NULL){
            if (edge->getEntityA()->getncgType() == Qncg::Node){
                ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

            }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
            }
        }

        if (edge->getEntityB() != NULL){
            if (edge->getEntityB()->getncgType() == Qncg::Node){
                ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

            }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
            }
        }

        QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
        parent->removenstGraphicsEntity(edge);

    }
}

void QnstView::changeLink(QnstLink* entity,
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

void QnstView::adjustLink(QnstLink* entity)
{
  // TODO
}

void QnstView::changeBind(QnstBind* entity,
                          const QMap<QString, QString> &properties)
{
  if (entity != NULL)
  {
    entity->setRole(properties["role"]);
    entity->setComponent(properties["component"]);
    entity->setComponentUid(properties["componentUid"]);
    entity->setInterface(properties["interface"]);
    entity->setInterfaceUid(properties["interfaceUid"]);

    selected = NULL;  // \fixme This fix temporarily a BUG.
                      // The bug is related with the fact that when the bind
                      // is selected, and the adjustBind bellow is called
                      // the value pointed by "selected" is deleted and
                      // it stay points to garbage.

    adjustBind(entity);
  }
}

// \todo refactory
void QnstView::adjustBind(QnstBind* entity)
{
    if (entity != NULL){
        if (entity->getnstParent() != NULL){
            QnstLink* parent = (QnstLink*) entity->getnstParent();

            // is there a graphical bind?
            if (brelations.contains(entity->getnstUid())){

                QnstGraphicsEntity* graphics = entities[brelations[entity->getnstUid()]];

                if (graphics != NULL){

                    // removing graphical bind
                    QnstGraphicsEntity* graparent = graphics->getnstGraphicsParent();

                    foreach(QnstGraphicsEntity* entity, graparent->getnstGraphicsEntities()){
                        if (entity->getnstUid() == graphics->getnstUid()){
                            QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

                            if (edge->getEntityA()->getncgType() == Qncg::Node){
                               ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                            }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                               ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                            }

                            if (edge->getEntityB()->getncgType() == Qncg::Node){
                                ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                            }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                                ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                            }

                            edge->getEntityA()->removeAngle(edge->getEntityB()->getnstUid(), edge->getAngle());
                            edge->getEntityB()->removeAngle(edge->getEntityA()->getnstUid(), -edge->getAngle());

                            graparent->removenstGraphicsEntity(edge); delete (edge);

                            break;
                        }
                    }
                }

                brelations.remove(entity->getnstUid());

                entities.remove(entity->getnstUid());
            }

            bool invalid = true;

            // add bind to link if has a valid type
            if (connectors.contains(parent->getxConnector())){
                QnstConnector* connector = connectors[parent->getxConnector()];

                foreach(QString type, connector->getConditions()){
                    if (type == entity->getRole()){
                        if (parent->getConditions().contains(entity->getnstUid())){
                            parent->removeCondition(entity);
                        }

                        if (parent->getActions().contains(entity->getnstUid())){
                            parent->removeAction(entity);
                        }


                        parent->addCondition(entity);

                        invalid = false;

                        break;
                    }
                }

                foreach(QString type, connector->getActions()){
                    if (type == entity->getRole()){
                        if (parent->getConditions().contains(entity->getnstUid())){
                            parent->removeCondition(entity);
                        }

                        if (parent->getActions().contains(entity->getnstUid())){
                            parent->removeAction(entity);
                        }

                        parent->addAction(entity);

                        invalid = false;

                        break;
                    }
                }
            }

            // creating graphical bind
            if (parent->getConditions().contains(entity->getnstUid())){

                if (parent->getAggregatorUID() == ""){

                    QnstGraphicsEntity* node = (QnstGraphicsEntity*) parent->getnstParent();

                    QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*)node);
                    aggregator->setnstUid(parent->getnstUid());
                    aggregator->setTop(node->getHeight()/2 - DEFAULT_AGGREGATOR_HEIGHT/2);
                    aggregator->setLeft(node->getWidth()/2 - DEFAULT_AGGREGATOR_WIDTH/2);
                    aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
                    aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
                    aggregator->adjust();

                    node->addnstGraphicsEntity(aggregator);

                    entities[aggregator->getnstUid()] = aggregator;

                    parent->setAggregatorUID(aggregator->getnstUid());
                }

                if (entities.contains(entity->getComponentUid()) && entities.contains(parent->getAggregatorUID())){

                    if (entity->getInterface() != ""){
                        if (entities.contains(entity->getInterfaceUid()) && entities[entity->getComponentUid()]->getnstGraphicsEntities().contains(entities[entity->getInterfaceUid()])){
                            QnstGraphicsEntity* entitya = entities[entity->getInterfaceUid()];
                            QnstGraphicsEntity* entityb = entities[parent->getAggregatorUID()];

                            QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
                            QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();



                            if (parenta != NULL && parentb != NULL && parenta->getnstGraphicsParent() == parentb){

                                QnstGraphicsBind* graphics = new QnstGraphicsBind();
                                graphics->setCondition(Qnst::NoBindType);
                                graphics->setnstUid(entity->getnstUid());
                                graphics->setnstGraphicsParent(parentb);
                                graphics->setEntityA(entitya);
                                graphics->setEntityB(entityb);

                                // adjusting angle
                                adjustAngle(graphics, entitya, entityb);

                                graphics->adjust();

                                parentb->addnstGraphicsEntity(graphics);

                                ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(graphics);
                                ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(graphics);

                                if (invalid){
                                    graphics->setInvalid(true);
                                    graphics->setCondition(Qnst::NoBindType);

                                }else{
                                    if (entity->getRole() == "onBegin"){
                                        graphics->setCondition(Qnst::onBegin);

                                    }else if (entity->getRole() == "onEnd"){
                                        graphics->setCondition(Qnst::onEnd);

                                    }else if (entity->getRole() == "onPause"){
                                        graphics->setCondition(Qnst::onPause);

                                    }else if (entity->getRole() == "onResume"){
                                        graphics->setCondition(Qnst::onResume);

                                    }else if (entity->getRole() == "onSelection"){
                                        graphics->setCondition(Qnst::onSelection);

                                    }else{
                                        graphics->setCondition(Qnst::NoBindType);
                                    }
                                }
                                graphics->setConn(connectors[parent->getxConnector()]);

                                graphics->setParams(entity->getParams());
                                graphics->setNameUids(entity->getNameUIDs());


                                connect(graphics,
                                        SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                        SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                                connect(graphics,
                                        SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                        SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                                entities[graphics->getnstUid()] = graphics;
                                brelations[entity->getnstUid()] = graphics->getnstUid();
                            }
                        }

                    }else{

                        QnstGraphicsEntity* entitya = entities[entity->getComponentUid()];
                        QnstGraphicsEntity* entityb = entities[parent->getAggregatorUID()];

                        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
                        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

                        if (parenta == parentb && parenta != NULL && parentb != NULL){

                            QnstGraphicsBind* graphics = new QnstGraphicsBind();
                            graphics->setCondition(Qnst::NoBindType);
                            graphics->setnstUid(entity->getnstUid());
                            graphics->setnstGraphicsParent(parenta);
                            graphics->setEntityA(entitya);
                            graphics->setEntityB(entityb);

                            // adjusting angle
                            adjustAngle(graphics, entitya, entityb);

                            graphics->adjust();

                            parenta->addnstGraphicsEntity(graphics);

                            ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(graphics);
                            ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(graphics);

                            if (invalid){
                                graphics->setInvalid(true);
                                graphics->setCondition(Qnst::NoBindType);

                            }else{
                                if (entity->getRole() == "onBegin"){
                                    graphics->setCondition(Qnst::onBegin);

                                }else if (entity->getRole() == "onEnd"){
                                    graphics->setCondition(Qnst::onEnd);

                                }else if (entity->getRole() == "onPause"){
                                    graphics->setCondition(Qnst::onPause);

                                }else if (entity->getRole() == "onResume"){
                                    graphics->setCondition(Qnst::onResume);

                                }else if (entity->getRole() == "onSelection"){
                                    graphics->setCondition(Qnst::onSelection);

                                }else{
                                    graphics->setCondition(Qnst::NoBindType);
                                }
                            }

                            graphics->setConn(connectors[parent->getxConnector()]);

                            graphics->setParams(entity->getParams());
                            graphics->setNameUids(entity->getNameUIDs());

                            connect(graphics,
                                    SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                    SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                            connect(graphics,
                                    SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                    SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                            entities[graphics->getnstUid()] = graphics;
                            brelations[entity->getnstUid()] = graphics->getnstUid();
                        }
                    }
                }

            }else if (parent->getActions().contains(entity->getnstUid())){

                if (parent->getAggregatorUID() == ""){

                    QnstGraphicsEntity* node = (QnstGraphicsEntity*) parent->getnstParent();

                    QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*)node);
                    aggregator->setnstUid(parent->getnstUid());
                    aggregator->setTop(node->getHeight()/2 - DEFAULT_AGGREGATOR_HEIGHT/2);
                    aggregator->setLeft(node->getWidth()/2 - DEFAULT_AGGREGATOR_WIDTH/2);
                    aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
                    aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
                    aggregator->adjust();

                    node->addnstGraphicsEntity(aggregator);

                    entities[aggregator->getnstUid()] = aggregator;

                    parent->setAggregatorUID(aggregator->getnstUid());
                }

                if (entities.contains(entity->getComponentUid()) && entities.contains(parent->getAggregatorUID())){

                    if (entity->getInterface() != ""){
                        if (entities.contains(entity->getInterfaceUid()) &&  entities[entity->getComponentUid()]->getnstGraphicsEntities().contains(entities[entity->getInterfaceUid()])){


                            QnstGraphicsEntity* entitya = entities[parent->getAggregatorUID()];
                            QnstGraphicsEntity* entityb = entities[entity->getInterfaceUid()];

                            QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
                            QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

                            if (parenta != NULL && parentb != NULL && parenta == parentb->getnstGraphicsParent()){

                                QnstGraphicsBind* graphics = new QnstGraphicsBind();
                                graphics->setAction(Qnst::NoBindType);
                                graphics->setnstUid(entity->getnstUid());
                                graphics->setnstGraphicsParent(parenta);
                                graphics->setEntityA(entitya);
                                graphics->setEntityB(entityb);

                                // adjusting angle
                                adjustAngle(graphics, entitya, entityb);

                                graphics->adjust();

                                parenta->addnstGraphicsEntity(graphics);

                                ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(graphics);
                                ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(graphics);

                                if (invalid){
                                    graphics->setInvalid(true);
                                    graphics->setAction(Qnst::NoBindType);

                                }else{
                                    if (entity->getRole() == "start"){
                                        graphics->setAction(Qnst::Start);

                                    }else if (entity->getRole() == "stop"){
                                        graphics->setAction(Qnst::Stop);

                                    }else if (entity->getRole() == "pause"){
                                        graphics->setAction(Qnst::Pause);

                                    }else if (entity->getRole() == "resume"){
                                        graphics->setAction(Qnst::Resume);

                                    }else if (entity->getRole() == "set"){
                                        graphics->setAction(Qnst::Set);

                                    }else{
                                        graphics->setAction(Qnst::NoBindType);
                                    }
                                }


                                graphics->setConn(connectors[parent->getxConnector()]);

                                graphics->setParams(entity->getParams());
                                graphics->setNameUids(entity->getNameUIDs());

                                connect(graphics,
                                        SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                        SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                                connect(graphics,
                                        SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                        SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                                entities[graphics->getnstUid()] = graphics;
                                brelations[entity->getnstUid()] = graphics->getnstUid();
                            }
                        }

                    }else{


                        QnstGraphicsEntity* entitya = entities[parent->getAggregatorUID()];
                        QnstGraphicsEntity* entityb = entities[entity->getComponentUid()];

                        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
                        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

                        if (parenta == parentb && parenta != NULL && parentb != NULL){

                            QnstGraphicsBind* graphics = new QnstGraphicsBind();
                            graphics->setAction(Qnst::NoBindType);
                            graphics->setnstUid(entity->getnstUid());
                            graphics->setnstGraphicsParent(parentb);
                            graphics->setEntityA(entitya);
                            graphics->setEntityB(entityb);

                            // adjusting angle
                            adjustAngle(graphics, entitya, entityb);

                            graphics->adjust();

                            parentb->addnstGraphicsEntity(graphics);

                            ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(graphics);
                            ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(graphics);

                            if (invalid){
                                graphics->setInvalid(true);
                                graphics->setAction(Qnst::NoBindType);

                            }else{
                                if (entity->getRole() == "start"){
                                    graphics->setAction(Qnst::Start);

                                }else if (entity->getRole() == "stop"){
                                    graphics->setAction(Qnst::Stop);

                                }else if (entity->getRole() == "pause"){
                                    graphics->setAction(Qnst::Pause);

                                }else if (entity->getRole() == "resume"){
                                    graphics->setAction(Qnst::Resume);

                                }else if (entity->getRole() == "set"){
                                    graphics->setAction(Qnst::Set);

                                }else{
                                    graphics->setAction(Qnst::NoBindType);
                                }
                            }

                            graphics->setConn(connectors[parent->getxConnector()]);

                            graphics->setParams(entity->getParams());
                            graphics->setNameUids(entity->getNameUIDs());

                            connect(graphics,
                                    SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                    SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                            connect(graphics,
                                    SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                    SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                            entities[graphics->getnstUid()] = graphics;
                            brelations[entity->getnstUid()] = graphics->getnstUid();
                        }
                    }
                }
            }
        }
    }
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

  if (entity != NULL)
  {
    entities[entity->getnstUid()] = entity;

    switch(entity->getnstType())
    {
      case Qnst::Body:
          if (undo){  scene->addRoot(entity); }

          requestBodyAddition((QnstGraphicsBody*) entity);
          break;

      case Qnst::Context:
          requestContextAddition((QnstGraphicsContext*) entity, undo);
          break;

      case Qnst::Switch:
          requestSwitchAddition((QnstGraphicsSwitch*) entity, undo);
          break;

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
          requestMediaAddition((QnstGraphicsMedia*) entity, undo);
          break;

      case Qnst::Port:
          requestPortAddition((QnstGraphicsPort*) entity, undo);
          break;

      case Qnst::Area:
          requestAreaAddition((QnstGraphicsArea*) entity, undo);
          break;

      case Qnst::Property:
          requestPropertyAddition((QnstGraphicsProperty*) entity);
          break;

      case Qnst::Aggregator:
          requestAggregatorAddition((QnstGraphicsAggregator*) entity);
          break;

      case Qnst::SwitchPort:
          requestSwitchPortAddition((QnstGraphicsSwitchPort*) entity);
          break;

      default:
        // do nothing
        break;
    }

    if (!undo)
    {
      QnstAddCommand* cmd = new QnstAddCommand(this, entity);
      history.push(cmd);
    }
  }
}

// \todo refactory
void QnstView::requestEntityRemotion(QnstGraphicsEntity* entity, bool undo,
                                     bool rmRefs)
{
  qDebug() << "[QNST]" << ":" << "Requesting entity remotion '"+entity->getnstUid()+"'";

  if (entity != NULL)
  {
    if (entity->getncgType() == Qncg::Node)
    {
      if (!undo)
      {
        QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
        history.push(cmd);
      }

            foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
                if (e->getnstType() != Qnst::Link &&
                    e->getnstType() != Qnst::Edge &&
                    e->getnstType() != Qnst::Condition &&
                    e->getnstType() != Qnst::Mapping &&
                    e->getnstType() != Qnst::Action &&
                    e->getnstType() != Qnst::Reference){

                    if (entity->isMedia())
                    {
                      QnstGraphicsMedia* m = (QnstGraphicsMedia*) entity;

                      if (m->getReferUID() != "")
                         requestEntityRemotion(e, true, false);
                      else
                         requestEntityRemotion(e, true, false);

                    }
                    else
                    {
                      requestEntityRemotion(e, true, false);
                    }
                }
            }

            foreach(QnstGraphicsEdge* edge, ((QnstGraphicsNode*) entity)->getnstGraphicsEdges()){
                if (edge->getEntityA()->getncgType() == Qncg::Node){
                    ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                    ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                }

                if (edge->getEntityB()->getncgType() == Qncg::Node){
                    ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                    ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                }

                edge->getEntityA()->removeAngle(edge->getEntityB()->getnstUid(), edge->getAngle());
                edge->getEntityB()->removeAngle(edge->getEntityA()->getnstUid(), -edge->getAngle());

                if (edge->getnstType() == Qnst::Condition ||
                    edge->getnstType() == Qnst::Action){

                    QnstBind* bb = binds[edge->getnstUid()];

                    if (bb != NULL){
                        QnstLink* ll = (QnstLink*) bb->getnstParent();

                        if (ll != NULL){
                            if (ll->getConditions().contains(bb->getnstUid())){
                                ll->removeCondition(bb);
                            }

                            if (ll->getActions().contains(bb->getnstUid())){
                                ll->removeAction(bb);
                            }

                        }

                        binds.remove(bb->getnstUid());
                        brelations.remove(bb->getnstUid());
                    }
                }

                QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
                parent->removenstGraphicsEntity(edge);

                entities.remove(edge->getnstUid());

                if (edge->getnstType() != Qnst::Reference){
                    emit entityRemoved(edge->getnstUid());
                }
            }

            QnstGraphicsEntity* parent = entity->getnstGraphicsParent();

            if (parent != NULL){
                parent->removenstGraphicsEntity(entity);

            }else{
                scene->removeRoot(entity);
            }

            if (selected == entity){
                selected = NULL;
            }

            if (clipboard == entity){
                clipboard = NULL;
            }

            if (entity->getnstType() == Qnst::Aggregator){
                if (links.contains(entity->getnstUid())){
                    link2conn.remove(links[entity->getnstUid()]->getnstId());
                    links.remove(entity->getnstUid());
                }
            }

            entities.remove(entity->getnstUid()); emit entityRemoved(entity->getnstUid()); delete entity;

        }else if (entity->getncgType() == Qncg::Interface){
            if (!undo){
                QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
                history.push(cmd);
            }

            foreach(QnstGraphicsEdge* edge, ((QnstGraphicsInterface*) entity)->getnstGraphicsEdges()){
                if (edge->getEntityA()->getncgType() == Qncg::Node){
                    ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                    ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                }

                if (edge->getEntityB()->getncgType() == Qncg::Node){
                    ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                    ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                }

                edge->getEntityA()->removeAngle(edge->getEntityB()->getnstUid(), edge->getAngle());
                edge->getEntityB()->removeAngle(edge->getEntityA()->getnstUid(), -edge->getAngle());

                if (edge->getnstType() == Qnst::Condition ||
                    edge->getnstType() == Qnst::Action){

                    QnstBind* bb = binds[edge->getnstUid()];

                    if (bb != NULL){
                        QnstLink* ll = (QnstLink*) bb->getnstParent();

                        if (ll != NULL){
                            if (ll->getConditions().contains(bb->getnstUid())){
                                ll->removeCondition(bb);
                            }

                            if (ll->getActions().contains(bb->getnstUid())){
                                ll->removeAction(bb);
                            }

                        }

                        binds.remove(bb->getnstUid());
                        brelations.remove(bb->getnstUid());
                    }
                }

                QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
                parent->removenstGraphicsEntity(edge);

                entities.remove(edge->getnstUid());

                if (edge->getnstType() != Qnst::Reference){
                    emit entityRemoved(edge->getnstUid());
                }
            }

            if (rmRefs){
                // remove interface that "I" make a reference
                if (interfaceRefers.contains(entity->getnstUid())){
                    QString oUID = interfaceRefers[entity->getnstUid()];

                    interfaceRefers.remove(entity->getnstUid());

                    if (entities.contains(oUID)){
                        requestEntityRemotion(entities[oUID]);
                    }


                }
            }

            // remove interfaces that reference "ME"
            foreach(QString rUID, interfaceRefers.keys(entity->getnstUid())){
                interfaceRefers.remove(rUID);

                if (entities.contains(rUID)){
                    requestEntityRemotion(entities[rUID]);
                }
            }

            QnstGraphicsEntity* parent = entity->getnstGraphicsParent();

            if (parent != NULL){
                parent->removenstGraphicsEntity(entity);

            }else{
                scene->removeRoot(entity);
            }

            if (selected == entity){
                selected = NULL;
            }

            if (clipboard == entity){
                clipboard = NULL;
            }

            if (entity->getnstType() == Qnst::Aggregator){
                if (links.contains(entity->getnstUid())){
                    link2conn.remove(links[entity->getnstUid()]->getnstId());
                    links.remove(entity->getnstUid());
                }
            }

            entities.remove(entity->getnstUid()); emit entityRemoved(entity->getnstUid()); delete entity;

        }else if (entity->getncgType() == Qncg::Edge){
            QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

            if (edge->getEntityA()->getncgType() == Qncg::Node){
                ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

            }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
            }

            if (edge->getEntityB()->getncgType() == Qncg::Node){
                ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

            }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
            }

            edge->getEntityA()->removeAngle(edge->getEntityB()->getnstUid(), edge->getAngle());
            edge->getEntityB()->removeAngle(edge->getEntityA()->getnstUid(), -edge->getAngle());

            if (edge->getnstType() == Qnst::Condition || edge->getnstType() == Qnst::Action){

                QnstBind* bb = binds[edge->getnstUid()];

                if (bb != NULL){
                    QnstLink* ll = (QnstLink*) bb->getnstParent();

                    if (ll != NULL){
                        if (ll->getConditions().contains(bb->getnstUid())){
                            ll->removeCondition(bb);
                        }

                        if (ll->getActions().contains(bb->getnstUid())){
                            ll->removeAction(bb);
                        }

                    }

                    QnstGraphicsEntity* parent = entity->getnstGraphicsParent();
                    parent->removenstGraphicsEntity(edge);

                    emit entityRemoved(brelations.key(entity->getnstUid()));

                    binds.remove(bb->getnstUid());
                    brelations.remove(bb->getnstUid());

                    entities.remove(edge->getnstUid());
                }

            }else if (edge->getnstType() == Qnst::Reference){
                QnstGraphicsPort* p = (QnstGraphicsPort*) edge->getEntityA();
                p->setComponent("");
                p->setComponentUid("");

                p->setInterface("");
                p->setInterfaceUid("");

//                adjustPort(p);

                requestPortChange(p);
            }else if (edge->getnstType() == Qnst::Mapping){
                QnstGraphicsEntity* parent = entity->getnstGraphicsParent();
                parent->removenstGraphicsEntity(edge);

                entities.remove(edge->getnstUid()); emit entityRemoved(edge->getnstUid());

            }

            if (selected == entity){
                selected = NULL;
            }

            if (clipboard == entity){
                clipboard = NULL;
            }
        }
    }
}

void QnstView::requestEntityPreparation(QnstGraphicsEntity* entity,
                                        QMap<QString, QString> properties)
{
  QnstChangeCommand* cmd = new QnstChangeCommand(this, entity->getnstUid(), properties);
  history.push(cmd);

  emit viewChanged();
}

void QnstView::requestEntityChange(QnstGraphicsEntity* entity)
{
  qDebug() << "[QNST]" << ":" << "Requesting entity change '"+entity->getnstUid()+"'";

  if (entity != NULL)
  {
    switch(entity->getnstType())
    {
      case Qnst::Body:
        requestBodyChange((QnstGraphicsBody*) entity);
        break;

      case Qnst::Context:
        requestContextChange((QnstGraphicsContext*) entity);
        break;

      case Qnst::Switch:
        requestSwitchChange((QnstGraphicsSwitch*) entity);
        break;

      // if the entity type is MEDIA
      case Qnst::Audio:
      case Qnst::Text:
      case Qnst::Video:
      case Qnst::Image:
      case Qnst::NCLua:
      case Qnst::Settings:
      case Qnst::Media:
        requestMediaChange((QnstGraphicsMedia*) entity);
        break;

      // if the entity type is PORT
      case Qnst::Port:
        requestPortChange((QnstGraphicsPort*) entity);
        break;

      default:
        // do nothing
        break;
    }
  }
}

void QnstView::requestEntitySelection(QnstGraphicsEntity* entity)
{
  qDebug() << "[QNST]" << ":" << "Requesting entity selection '"+entity->getnstUid()+"'";

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

      if (interfaceRefers.contains(entity->getnstUid()))
        emit entitySelected(interfaceRefers[entity->getnstUid()]);
      else
        emit entitySelected(entity->getnstUid());
    }
  }
}

void QnstView::requestBodyAddition(QnstGraphicsBody* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "body";

  properties["id"] = entity->getnstId();

  properties["top"] = QString::number(entity->getTop());
  properties["left"] = QString::number(entity->getLeft());
  properties["width"] = QString::number(entity->getWidth());
  properties["heigh"] = QString::number(entity->getHeight());
  properties["zindex"] = QString::number(entity->getzIndex());

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(), "", properties);
}

void QnstView::requestBodyChange(QnstGraphicsBody* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "body";

  properties["id"] = entity->getnstId();

  properties["top"] = QString::number(entity->getTop());
  properties["left"] = QString::number(entity->getLeft());
  properties["width"] = QString::number(entity->getWidth());
  properties["heigh"] = QString::number(entity->getHeight());
  properties["zindex"] = QString::number(entity->getzIndex());

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestContextAddition(QnstGraphicsContext* entity, bool undo)
{
  if (entity->getnstId() == "" && !undo)
    updateEntityWithUniqueNstId(entity);

  QMap<QString, QString> properties;

  properties["TYPE"] = "context";

  properties["id"] = entity->getnstId();

  properties["refer"] = "";

  properties["top"] = QString::number(entity->getTop());
  properties["left"] = QString::number(entity->getLeft());
  properties["width"] = QString::number(entity->getWidth());
  properties["heigh"] = QString::number(entity->getHeight());
  properties["zindex"] = QString::number(entity->getzIndex());

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(), entity->getnstGraphicsParent()->getnstUid(), properties);
}

void QnstView::requestContextChange(QnstGraphicsContext* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "context";

  properties["id"] = entity->getnstId();

  properties["refer"] = "";

  properties["top"] = QString::number(entity->getTop());
  properties["left"] = QString::number(entity->getLeft());
  properties["width"] = QString::number(entity->getWidth());
  properties["heigh"] = QString::number(entity->getHeight());
  properties["zindex"] = QString::number(entity->getzIndex());

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityChanged(entity->getnstUid(), properties);
  }

void QnstView::requestSwitchAddition(QnstGraphicsSwitch* entity, bool undo)
{
  if (entity->getnstId() == "" && !undo)
    updateEntityWithUniqueNstId(entity);

  QMap<QString, QString> properties;

  properties["TYPE"] = "switch";

  properties["id"] = entity->getnstId();

  properties["refer"] = "";

  properties["top"] = QString::number(entity->getTop());
  properties["left"] = QString::number(entity->getLeft());
  properties["width"] = QString::number(entity->getWidth());
  properties["heigh"] = QString::number(entity->getHeight());
  properties["zindex"] = QString::number(entity->getzIndex());

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(),
                   entity->getnstGraphicsParent()->getnstUid(),
                   properties);
}

void QnstView::requestSwitchChange(QnstGraphicsSwitch* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "switch";

  properties["id"] = entity->getnstId();

  properties["refer"] = "";

  properties["top"] = QString::number(entity->getTop());
  properties["left"] = QString::number(entity->getLeft());
  properties["width"] = QString::number(entity->getWidth());
  properties["heigh"] = QString::number(entity->getHeight());
  properties["zindex"] = QString::number(entity->getzIndex());

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestMediaAddition(QnstGraphicsMedia* entity, bool undo)
{
  if (entity->getnstId() == "" && !undo)
    updateEntityWithUniqueNstId(entity);

  QMap<QString, QString> properties;
  properties["TYPE"] = "media";

  properties["id"] = entity->getnstId();

  properties["src"] = entity->getSource();
  properties["refer"] = entity->getRefer();
  properties["instance"] = entity->getInstance();

  entity->updateToolTip();

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(),
                   entity->getnstGraphicsParent()->getnstUid(),
                   properties);
}

void QnstView::requestMediaChange(QnstGraphicsMedia* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "media";

  properties["id"] = entity->getnstId();
  properties["src"] = entity->getSource();

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
    properties.insert(key, additionalData.value(key));
  }

  emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestAggregatorAddition(QnstGraphicsAggregator* entity)
{
  entities[entity->getnstUid()] = entity;
}

void QnstView::requestPortAddition(QnstGraphicsPort* entity, bool undo)
{
  if (entity->getnstId() == "" && !undo)
    updateEntityWithUniqueNstId(entity);

  QMap<QString, QString> properties;

  properties["TYPE"] = "port";

  properties["id"] = entity->getnstId();

  foreach(QnstGraphicsEdge* edge, entity->getnstGraphicsEdges())
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

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(),
                   entity->getnstGraphicsParent()->getnstUid(),
                   properties);
}

void QnstView::requestPortChange(QnstGraphicsPort* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "port";

  properties["id"] = entity->getnstId();

  foreach(QnstGraphicsEdge* edge, entity->getnstGraphicsEdges())
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

      QMap <QString, QString> additionalData = entity->getUsrData();
      foreach(QString key, additionalData.keys())
      {
        if(!properties.contains(key))
          properties.insert(key, additionalData.value(key));
      }

      break; // there is only one match, so 'break'.
    }
  }

  emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestMappingAddition(QnstGraphicsMapping* entity, bool undo)
{

}

void QnstView::requestMappingChange(QnstGraphicsMapping* entity)
{

}

void QnstView::requestSwitchPortAddition(QnstGraphicsSwitchPort* entity, bool undo)
{
  if (entity->getnstId() == "" && !undo)
    updateEntityWithUniqueNstId(entity);

  QMap<QString, QString> properties;

  properties["TYPE"] = "switchPort";

  properties["id"] = entity->getnstId();

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(),
                   entity->getnstGraphicsParent()->getnstUid(), properties);
}

void QnstView::requestSwitchPortChange(QnstGraphicsSwitchPort* entity)
{
  QMap<QString, QString> properties;

  properties["TYPE"] = "switchPort";

  properties["id"] = entity->getnstId();

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestAreaAddition(QnstGraphicsArea* entity, bool undo)
{
  if (entity->getnstId() == "" && !undo)
    updateEntityWithUniqueNstId(entity);

  QMap<QString, QString> properties;

  properties["TYPE"] = "area";

  properties["id"] = entity->getnstId();

  adjustMedia((QnstGraphicsMedia*) entity->getnstGraphicsParent());

  foreach (QString key,
           refers.keys(entity->getnstGraphicsParent()->getnstUid()))
  {
    if (entities.contains(key))
    {
      if (entities[key]->isMedia())
        adjustMedia((QnstGraphicsMedia*) entities[key]);
    }
  }

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(), entity->getnstGraphicsParent()->getnstUid(), properties);
}

void QnstView::requestAreaChange(QnstGraphicsArea* entity)
{
    // TODO
}

void QnstView::requestPropertyAddition(QnstGraphicsProperty* entity)
{
  QMap<QString, QString> properties;

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

  QMap <QString, QString> additionalData = entity->getUsrData();
  foreach(QString key, additionalData.keys())
  {
    if(!properties.contains(key))
      properties.insert(key, additionalData.value(key));
  }

  emit entityAdded(entity->getnstUid(),
                   entity->getnstGraphicsParent()->getnstUid(), properties);
}

void QnstView::requestPropertyChange(QnstGraphicsProperty* entity)
{
    // TODO
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
    copy->setnstGraphicsParent(parent);

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

    foreach(QnstGraphicsEntity* e, copy->getnstGraphicsEntities())
    {
      performPaste(e, entity, makeRefer, true);
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
  if (selected != NULL){
    requestEntityRemotion(selected);

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
      delete (link); link = NULL;
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
    if (linking){
        QList<QGraphicsItem *> itemsa = scene->items(link->getLine().p1());

        if (itemsa.count() && itemsa.first() == link){
            itemsa.removeFirst();
        }

        QList<QGraphicsItem*> itemsb = scene->items(link->getLine().p2());

        if (itemsb.count() && itemsb.first() == link){
            itemsb.removeFirst();
        }

        if (itemsa.count() && itemsb.count()){
            QnstGraphicsEntity* entitya = (QnstGraphicsEntity*) itemsa.first();
            QnstGraphicsEntity* entityb = (QnstGraphicsEntity*) itemsb.first();

            if (entitya != entityb){
                // if linking NODE to NODE
                if (entitya->getncgType() == Qncg::Node && entityb->getncgType() == Qncg::Node){
                   addNodetoNodeEdge(entitya, entityb);

                // if linking NODE to INTERFACE
                }else if (entitya->getncgType() == Qncg::Node && entityb->getncgType() == Qncg::Interface){
                    addNodetoInterfaceEdge(entitya, entityb);

                // if linking INTERFACE to NODE
                }else if (entitya->getncgType() == Qncg::Interface && entityb->getncgType() == Qncg::Node){
                    addInterfacetoNodeEdge(entitya, entityb);

                // if linking INTERFACE to INTERFACE
                }else if (entitya->getncgType() == Qncg::Interface && entityb->getncgType() == Qncg::Interface){
                   addInterfacetoInterfaceEdge(entitya, entityb);
                }
            }

            entitya->setMouseHover(false);
            entityb->setMouseHover(false);
        }

        if (link != NULL){
            scene->removeItem(link); scene->update(); delete link; link = NULL;
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

void QnstView:: addNodetoNodeEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb)
{
    QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
    QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

    if (parenta != NULL && parentb != NULL){
        if (parenta == parentb){
            qDebug() << "NODE to NODE:" << entitya->getnstUid() << "->" << entityb->getnstUid();

            if (entitya->getnstType() == Qnst::Aggregator){
                actionDialog->init(connectors, links[entitya->getnstUid()]);

                if (actionDialog->exec()){
                    QString link = actionDialog->form.cbLink->currentText();
                    QString act = actionDialog->form.cbAction->currentText();

                    QnstGraphicsBind* entity = new QnstGraphicsBind();
                    entity->setAction(Qnst::NoBindType);
                    entity->setnstGraphicsParent(parenta);
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);

                    // adjusting angle
                    adjustAngle(entity, entitya, entityb);

                    entity->adjust();

                    parenta->addnstGraphicsEntity(entity);

                    ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
                    ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

                    if (act == "start"){
                        entity->setAction(Qnst::Start);

                    }else if (act == "stop"){
                        entity->setAction(Qnst::Stop);

                    }else if (act == "pause"){
                        entity->setAction(Qnst::Pause);

                    }else if (act == "resume"){
                        entity->setAction(Qnst::Resume);

                    }else if (act == "set"){
                        entity->setAction(Qnst::Set);

                    }else{
                        entity->setAction(Qnst::NoBindType);
                    }

                    /////
                    connect(entity,
                            SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                            SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                    connect(entity,
                            SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                            SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
                    /////

                    ///// connector

                    QString connName = actionDialog->form.cbConnector->currentText();;

                    if (actionDialog->form.cbConnector->currentText() == "New..."){
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
                        properties["action"] = act;

                        connName = conn->getName();

                        emit entityAdded(conn->getnstUid(), "", properties);
                    }

                    // associating connector to bind
                    entity->setConn(connectors[connName]);

                    ///// link

                    QMap<QString, QString> properties;

                    properties["TYPE"] = "bind";

                    QnstLink* lo;

                    if (link == "New...")
                    {
                        properties["link"] = "link"+QString::number(++nlink);

                        lo = new QnstLink();
                        lo->setnstParent(entitya->getnstGraphicsParent());
                        lo->setnstId(properties["link"]);
                        lo->setnstUid(entitya->getnstUid());
                        lo->setxConnector(connName);
                        lo->setxConnectorUID(connectors[connName]->getnstUid());
                        lo->setAggregatorUID(entitya->getnstUid());

                        links[lo->getnstUid()] = lo;

                    }else{
                        properties["link"] = link;

                        lo = links[entitya->getnstUid()];
                    }

                    properties["linkUID"] = entitya->getnstUid();

                    properties["connector"] = connName;

                    properties["action"] = act;

                    properties["component"] = entityb->getnstId();

                    properties["role"] = act;

                    QnstBind* bo = new QnstBind();
                    bo->setnstParent(lo);
                    bo->setnstUid(entity->getnstUid());
                    bo->setRole(properties["action"]);
                    bo->setComponent(properties["component"]);
                    bo->setComponentUid(entityb->getnstUid());

                    lo->addAction(bo);

                    binds[bo->getnstUid()] = bo;
                    brelations[bo->getnstUid()] = entity->getnstUid();

                    link2conn[properties["link"]] = connName;

                    entities[entity->getnstUid()] = entity;

                    emit entityAdded(entity->getnstUid(), parenta->getnstUid(), properties);
                }

            }else if (entityb->getnstType() == Qnst::Aggregator){
                conditionDialog->init(connectors, links[entityb->getnstUid()]);

                if (conditionDialog->exec()){
                    QString conn = conditionDialog->form.cbConnector->currentText();
                    QString link = conditionDialog->form.cbLink->currentText();
                    QString cond = conditionDialog->form.cbCondition->currentText();

                    QnstGraphicsBind* entity = new QnstGraphicsBind();
                    entity->setCondition(Qnst::NoBindType);
                    entity->setnstGraphicsParent(parenta);
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);

                    // adjusting angle
                    adjustAngle(entity, entitya, entityb);

                    entity->adjust();

                    parenta->addnstGraphicsEntity(entity);

                    ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
                    ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

                    if (cond == "onBegin"){
                        entity->setCondition(Qnst::onBegin);

                    }else if (cond == "onEnd"){
                        entity->setCondition(Qnst::onEnd);

                    }else if (cond == "onPause"){
                        entity->setCondition(Qnst::onPause);

                    }else if (cond == "onResume"){
                        entity->setCondition(Qnst::onResume);

                    }else if (cond == "onSelection"){
                        entity->setCondition(Qnst::onSelection);

                    }else{
                        entity->setCondition(Qnst::NoBindType);
                    }

                    /////
                    connect(entity,
                            SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                            SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                    connect(entity,
                            SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                            SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
                    /////

                    ///// connector

                    QString connName = conditionDialog->form.cbConnector->currentText();;

                    if (conditionDialog->form.cbConnector->currentText() == "New..."){
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
                        properties["condition"] = cond;

                        connName = conn->getName();

                        emit entityAdded(conn->getnstUid(), "", properties);
                    }

                    // associating connector to bind
                    entity->setConn(connectors[connName]);

                    ///// link

                    QMap<QString, QString> properties;

                    properties["TYPE"] = "bind";

                    QnstLink* lo;

                    if (link == "New..."){
                        properties["link"] = "link"+QString::number(++nlink);


                        lo = new QnstLink();
                        lo->setnstParent(entityb->getnstGraphicsParent());
                        lo->setnstId(properties["link"]);
                        lo->setnstUid(entityb->getnstUid());
                        lo->setxConnector(connName);
                        lo->setxConnectorUID(connectors[connName]->getnstUid());
                        lo->setAggregatorUID(entityb->getnstUid());

                        links[lo->getnstUid()] = lo;

                    }else{
                        properties["link"] = link;

                        lo = links[entityb->getnstUid()];
                    }

                    properties["linkUID"] = entityb->getnstUid();


                    properties["connector"] = connName;
                    properties["condition"] = cond;

                    properties["component"] = entitya->getnstId();

                    properties["role"] = cond;

                    QnstBind* bo = new QnstBind();
                    bo->setnstParent(lo);
                    bo->setnstUid(entity->getnstUid());
                    bo->setRole(properties["condition"]);
                    bo->setComponent(properties["component"]);
                    bo->setComponentUid(entitya->getnstUid());

                    lo->addCondition(bo);

                    binds[bo->getnstUid()] = bo;
                    brelations[bo->getnstUid()] = entity->getnstUid();

                    link2conn[properties["link"]] = connName;

                    entities[entity->getnstUid()] = entity;

                    emit entityAdded(entity->getnstUid(), parenta->getnstUid(), properties);
                }

            }else{
                linkDialog->init(connectors);

                if (linkDialog->exec()){
                    QString con = linkDialog->form.cbCondition->currentText();
                    QString act = linkDialog->form.cbAction->currentText();

                    if (con != "" && act != ""){
                        qreal xcenter = -1;
                        qreal ycenter = -1;

                        QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                                       entitya->getTop() + entitya->getHeight()/2);

                        QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                                       entityb->getTop() + entityb->getHeight()/2);


                        pointb = parenta->mapFromItem(parentb, pointb);

                        qreal wa = entitya->getWidth()/2;
                        qreal wb = entityb->getWidth()/2;

                        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
                        }


                        QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*) parenta);
                        aggregator->setTop(ycenter - DEFAULT_AGGREGATOR_HEIGHT/2);
                        aggregator->setLeft(xcenter - DEFAULT_AGGREGATOR_WIDTH/2);
                        aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
                        aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
                        aggregator->adjust();

                        parenta->addnstGraphicsEntity(aggregator);

                        entities[aggregator->getnstUid()] = aggregator;

                        ///

                        QnstGraphicsBind* condition = new QnstGraphicsBind();
                        condition->setCondition(Qnst::NoBindType);
                        condition->setnstGraphicsParent(parenta);
                        condition->setEntityA(entitya);
                        condition->setEntityB(aggregator);

                        // adjusting angle
                        adjustAngle(condition, entitya, aggregator);

                        condition->adjust();

                        parenta->addnstGraphicsEntity(condition);

                        ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(condition);
                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(condition);

                        if (con == "onBegin"){
                            condition->setCondition(Qnst::onBegin);

                        }else if (con == "onEnd"){
                            condition->setCondition(Qnst::onEnd);

                        }else if (con == "onPause"){
                            condition->setCondition(Qnst::onPause);

                        }else if (con == "onResume"){
                            condition->setCondition(Qnst::onResume);

                        }else if (con == "onSelection"){
                            condition->setCondition(Qnst::onSelection);

                        }else{
                            condition->setCondition(Qnst::NoBindType);
                        }

                        entities[condition->getnstUid()] = condition;

                        ///

                        QnstGraphicsBind* action = new QnstGraphicsBind();
                        action->setAction(Qnst::NoBindType);
                        action->setnstGraphicsParent(parenta);
                        action->setEntityA(aggregator);
                        action->setEntityB(entityb);

                        // adjusting angle
                        adjustAngle(action, aggregator, entityb);

                        action->adjust();

                        parenta->addnstGraphicsEntity(action);

                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(action);
                        ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(action);

                        if (act == "start"){
                            action->setAction(Qnst::Start);

                        }else if (act == "stop"){
                            action->setAction(Qnst::Stop);

                        }else if (act == "pause"){
                            action->setAction(Qnst::Pause);

                        }else if (act == "resume"){
                            action->setAction(Qnst::Resume);

                        }else if (act == "set"){
                            action->setAction(Qnst::Set);

                        }else{
                            action->setAction(Qnst::NoBindType);
                        }

                        entities[action->getnstUid()] = action;

                        /////
                        connect(condition,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(condition,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                        connect(action,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(action,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
                        /////



                        ///// connector

                        QString connName = linkDialog->getCurrentConnector();;

                        if (linkDialog->getCurrentConnector() == "New..."){
                            QnstConnector* conn = new QnstConnector();
                            conn->setName(con+act+QString::number(connectors.size()));
                            conn->addCondition(con, con);
                            conn->addAction(act, act);

                            connectors[conn->getName()] = conn;
                            connectors2[conn->getnstUid()] = conn;

                            QMap<QString, QString> properties;

                            properties["TYPE"] = "connector";

                            properties["id"] = conn->getName();
                            properties["condition"] = con;
                            properties["action"] = act;

                            connName = conn->getName();

                            emit entityAdded(conn->getnstUid(), "", properties);
                        }

                        // associating connector to bind
                        action->setConn(connectors[connName]);
                        condition->setConn(connectors[connName]);

                        ///// link

                        QMap<QString, QString> properties;

                        properties["link"] = "link"+QString::number(++nlink);

                        QnstLink* lo = new QnstLink();
                        lo->setnstParent(aggregator->getnstGraphicsParent());
                        lo->setnstId(properties["link"]);
                        lo->setnstUid(aggregator->getnstUid());
                        lo->setxConnector(connName);
                        lo->setxConnectorUID(connectors[connName]->getnstUid());
                        lo->setAggregatorUID(aggregator->getnstUid());

                        links[lo->getnstUid()] = lo;

                        properties["TYPE"] = "bind";


                        properties["connector"] = connName;
                        properties["condition"] = con;

                        properties["component"] = entitya->getnstId();

                        properties["role"] = con;

                        properties["linkUID"] = aggregator->getnstUid();


                        link2conn[properties["link"]] = connName;

                        QnstBind* bo = new QnstBind();
                        bo->setnstParent(lo);
                        bo->setnstUid(condition->getnstUid());
                        bo->setRole(properties["condition"]);
                        bo->setComponent(properties["component"]);
                        bo->setComponentUid(entitya->getnstUid());

                        lo->addCondition(bo);

                        binds[bo->getnstUid()] = bo;
                        brelations[bo->getnstUid()] = condition->getnstUid();

                        emit entityAdded(condition->getnstUid(), parenta->getnstUid(), properties);

                        properties["condition"] = "";

                        properties["action"] = act;

                        properties["component"] = entityb->getnstId();

                        properties["role"] = act;

                        QnstBind* bo2 = new QnstBind();
                        bo2->setnstParent(lo);
                        bo2->setnstUid(action->getnstUid());
                        bo2->setRole(properties["action"]);
                        bo2->setComponent(properties["component"]);
                        bo2->setComponentUid(entityb->getnstUid());

                        lo->addAction(bo2);

                        binds[bo2->getnstUid()] = bo2;
                        brelations[bo2->getnstUid()] = action->getnstUid();

                        emit entityAdded(action->getnstUid(), parenta->getnstUid(), properties);
                    }
                }
            }

            modified = false;
        }
    }
}

void QnstView::addNodetoInterfaceEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb)
{
    QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
    QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

    if (parenta != NULL && parentb != NULL){
        if (parenta == parentb->getnstGraphicsParent()){
            if (entitya->getnstType() == Qnst::Aggregator){
                ///////////////////
                actionDialog->init(connectors, links[entitya->getnstUid()]);

                if (actionDialog->exec()){
                    QString link = actionDialog->form.cbLink->currentText();
                    QString act = actionDialog->form.cbAction->currentText();

                    QnstGraphicsBind* entity = new QnstGraphicsBind();
                    entity->setAction(Qnst::NoBindType);
                    entity->setnstGraphicsParent(parenta);
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);

                    // adjusting angle
                    adjustAngle(entity, entitya, entityb);

                    entity->adjust();

                    parenta->addnstGraphicsEntity(entity);

                    ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(entity);
                    ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

                    if (act == "start"){
                        entity->setAction(Qnst::Start);

                    }else if (act == "stop"){
                        entity->setAction(Qnst::Stop);

                    }else if (act == "pause"){
                        entity->setAction(Qnst::Pause);

                    }else if (act == "resume"){
                        entity->setAction(Qnst::Resume);

                    }else if (act == "set"){
                        entity->setAction(Qnst::Set);

                    }else{
                        entity->setAction(Qnst::NoBindType);
                    }

                    /////
                    connect(entity,
                            SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                            SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                    connect(entity,
                            SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                            SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
                    /////

                    ///// connector

                    QString connName = actionDialog->form.cbConnector->currentText();;

                    if (actionDialog->form.cbConnector->currentText() == "New..."){
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
                        properties["action"] = act;

                        connName = conn->getName();

                        emit entityAdded(conn->getnstUid(), "", properties);
                    }

                    // associating connector to bind
                    entity->setConn(connectors[connName]);

                    ///// link

                    QMap<QString, QString> properties;

                    properties["TYPE"] = "bind";

                    QnstLink* lo;

                    if (link == "New..."){
                        properties["link"] = "link"+QString::number(++nlink);

                        lo = new QnstLink();
                        lo->setnstParent(entitya->getnstGraphicsParent());
                        lo->setnstId(properties["link"]);
                        lo->setnstUid(entitya->getnstUid());
                        lo->setxConnector(connName);
                        lo->setxConnectorUID(connectors[connName]->getnstUid());
                        lo->setAggregatorUID(entitya->getnstUid());

                        links[lo->getnstUid()] = lo;

                    }else{
                        properties["link"] = link;

                        lo = links[entitya->getnstUid()];
                    }

                    properties["linkUID"] = entitya->getnstUid();

                    properties["connector"] = connName;

                    properties["action"] = act;

                    properties["component"] = parentb->getnstId();
                    properties["interface"] = entityb->getnstId();

                    properties["role"] = act;


                    QnstBind* bo = new QnstBind();
                    bo->setnstParent(lo);
                    bo->setnstUid(entity->getnstUid());
                    bo->setRole(properties["action"]);
                    bo->setComponent(properties["component"]);
                    bo->setComponentUid(parentb->getnstUid());
                    bo->setInterface(properties["interface"]);
                    bo->setInterfaceUid(entityb->getnstUid());

                    lo->addAction(bo);

                    binds[bo->getnstUid()] = bo;
                    brelations[bo->getnstUid()] = entity->getnstUid();

                    link2conn[properties["link"]] = connName;

                    entities[entity->getnstUid()] = entity;

                    emit entityAdded(entity->getnstUid(), parenta->getnstUid(), properties);
                }

                //////////////////
            }else {
                linkDialog->init(connectors);

                if (linkDialog->exec()){
                    QString con = linkDialog->form.cbCondition->currentText();
                    QString act = linkDialog->form.cbAction->currentText();

                    if (con != "" && act != ""){
                        qreal xcenter = -1;
                        qreal ycenter = -1;

                        QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                                       entitya->getTop() + entitya->getHeight()/2);

                        QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                                       entityb->getTop() + entityb->getHeight()/2);


                        pointb = parenta->mapFromItem(parentb, pointb);

                        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
                        }


                        QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*) parenta);
                        aggregator->setTop(ycenter - DEFAULT_AGGREGATOR_HEIGHT/2);
                        aggregator->setLeft(xcenter - DEFAULT_AGGREGATOR_WIDTH/2);
                        aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
                        aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
                        aggregator->adjust();

                        parenta->addnstGraphicsEntity(aggregator);

                        entities[aggregator->getnstUid()] = aggregator;

                        ///

                        QnstGraphicsBind* condition = new QnstGraphicsBind();
                        condition->setCondition(Qnst::NoBindType);
                        condition->setnstGraphicsParent(parenta);
                        condition->setEntityA(entitya);
                        condition->setEntityB(aggregator);

                        // adjusting angle
                        adjustAngle(condition, entitya, aggregator);

                        condition->adjust();

                        parenta->addnstGraphicsEntity(condition);

                        ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(condition);
                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(condition);

                        if (con == "onBegin"){
                            condition->setCondition(Qnst::onBegin);

                        }else if (con == "onEnd"){
                            condition->setCondition(Qnst::onEnd);

                        }else if (con == "onPause"){
                            condition->setCondition(Qnst::onPause);

                        }else if (con == "onResume"){
                            condition->setCondition(Qnst::onResume);

                        }else if (con == "onSelection"){
                            condition->setCondition(Qnst::onSelection);

                        }else{
                            condition->setCondition(Qnst::NoBindType);
                        }

                        entities[condition->getnstUid()] = condition;

                        ///

                        QnstGraphicsBind* action = new QnstGraphicsBind();
                        action->setAction(Qnst::NoBindType);
                        action->setnstGraphicsParent(parenta);
                        action->setEntityA(aggregator);
                        action->setEntityB(entityb);

                        // adjusting angle
                        adjustAngle(action, aggregator, entityb);

                        action->adjust();

                        parenta->addnstGraphicsEntity(action);

                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(action);
                        ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(action);

                        if (act == "start"){
                            action->setAction(Qnst::Start);

                        }else if (act == "stop"){
                            action->setAction(Qnst::Stop);

                        }else if (act == "pause"){
                            action->setAction(Qnst::Pause);

                        }else if (act == "resume"){
                            action->setAction(Qnst::Resume);

                        }else if (act == "set"){
                            action->setAction(Qnst::Set);

                        }else{
                            action->setAction(Qnst::NoBindType);
                        }

                        entities[action->getnstUid()] = action;

                        /////
                        connect(condition,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(condition,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                        connect(action,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(action,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
                        /////

                        ///// connector

                        QString connName = linkDialog->getCurrentConnector();;

                        if (linkDialog->getCurrentConnector() == "New..."){
                            QnstConnector* conn = new QnstConnector();
                            conn->setName(con+act+QString::number(connectors.size()));
                            conn->addCondition(con, con);
                            conn->addAction(act, act);

                            connectors[conn->getName()] = conn;
                            connectors2[conn->getnstUid()] = conn;

                            QMap<QString, QString> properties;

                            properties["TYPE"] = "connector";

                            properties["id"] = conn->getName();
                            properties["condition"] = con;
                            properties["action"] = act;


                            connName = conn->getName();

                            emit entityAdded(conn->getnstUid(), "", properties);
                        }

                        // associating connector to bind
                        action->setConn(connectors[connName]);
                        condition->setConn(connectors[connName]);

                        ///// link

                        QMap<QString, QString> properties;

                        properties["TYPE"] = "bind";
                        properties["link"] = "link"+QString::number(++nlink);
                        properties["linkUID"] = aggregator->getnstUid();

                        QnstLink* lo = new QnstLink();
                        lo->setnstParent(aggregator->getnstGraphicsParent());
                        lo->setnstId(properties["link"]);
                        lo->setnstUid(aggregator->getnstUid());
                        lo->setxConnector(connName);
                        lo->setxConnectorUID(connectors[connName]->getnstUid());
                        lo->setAggregatorUID(aggregator->getnstUid());

                        links[lo->getnstUid()] = lo;

                        link2conn[properties["link"]] = connName;

                        properties["connector"] = connName;
                        properties["condition"] = con;

                        properties["component"] = entitya->getnstId();

                        properties["role"] = con;

                        QnstBind* bo = new QnstBind();
                        bo->setnstParent(lo);
                        bo->setnstUid(condition->getnstUid());
                        bo->setRole(properties["condition"]);
                        bo->setComponent(properties["component"]);
                        bo->setComponentUid(entitya->getnstUid());

                        lo->addCondition(bo);

                        binds[bo->getnstUid()] = bo;
                        brelations[bo->getnstUid()] = condition->getnstUid();

                        emit entityAdded(condition->getnstUid(), parenta->getnstUid(), properties);

                        properties["condition"] = "";

                        properties["action"] = act;

                        properties["component"] = parentb->getnstId();
                        properties["interface"] = entityb->getnstId();

                        properties["role"] = act;


                        QnstBind* bo2 = new QnstBind();
                        bo2->setnstParent(lo);
                        bo2->setnstUid(action->getnstUid());
                        bo2->setRole(properties["action"]);
                        bo2->setComponent(properties["component"]);
                        bo2->setComponentUid(parentb->getnstUid());
                        bo2->setInterface(properties["interface"]);
                        bo2->setInterfaceUid(entityb->getnstUid());

                        lo->addAction(bo2);

                        binds[bo2->getnstUid()] = bo2;
                        brelations[bo2->getnstUid()] = action->getnstUid();

                        emit entityAdded(action->getnstUid(), parenta->getnstUid(), properties);
                    }
                }
            }

            modified = false;
        }
    }
}

void QnstView::addInterfacetoNodeLink(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb)
{
    if (entityb->getnstType() == Qnst::Aggregator){
        conditionDialog->init(connectors, links[entityb->getnstUid()]);

        if (conditionDialog->exec()){
            QString conn = conditionDialog->form.cbConnector->currentText();
            QString link = conditionDialog->form.cbLink->currentText();
            QString cond = conditionDialog->form.cbCondition->currentText();

            QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
            QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

            QnstGraphicsBind* entity = new QnstGraphicsBind();
            entity->setCondition(Qnst::NoBindType);
            entity->setnstGraphicsParent(parentb);
            entity->setEntityA(entitya);
            entity->setEntityB(entityb);

            // adjusting angle
            adjustAngle(entity, entitya, entityb);

            entity->adjust();

            parenta->addnstGraphicsEntity(entity);

            ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
            ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(entity);

            if (cond == "onBegin"){
                entity->setCondition(Qnst::onBegin);

            }else if (cond == "onEnd"){
                entity->setCondition(Qnst::onEnd);

            }else if (cond == "onPause"){
                entity->setCondition(Qnst::onPause);

            }else if (cond == "onResume"){
                entity->setCondition(Qnst::onResume);

            }else if (cond == "onSelection"){
                entity->setCondition(Qnst::onSelection);

            }else{
                entity->setCondition(Qnst::NoBindType);
            }

            /////
            connect(entity,
                    SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                    SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

            connect(entity,
                    SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                    SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
            /////

            ///// connector

            QString connName = conditionDialog->form.cbConnector->currentText();;

            if (conditionDialog->form.cbConnector->currentText() == "New..."){
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
                properties["condition"] = cond;

                connName = conn->getName();

                emit entityAdded(conn->getnstUid(), "", properties);
            }

            // associating connector to bind
            entity->setConn(connectors[connName]);

            ///// link

            QMap<QString, QString> properties;

            properties["TYPE"] = "bind";

            QnstLink* lo;

            if (link == "New..."){
                properties["link"] = "link"+QString::number(++nlink);


                lo = new QnstLink();
                lo->setnstParent(entityb->getnstGraphicsParent());
                lo->setnstId(properties["link"]);
                lo->setnstUid(entityb->getnstUid());
                lo->setxConnector(connName);
                lo->setxConnectorUID(connectors[connName]->getnstUid());
                lo->setAggregatorUID(entityb->getnstUid());

                links[lo->getnstUid()] = lo;

            }else{
                properties["link"] = link;

                lo = links[entityb->getnstUid()];
            }

            properties["linkUID"] = entityb->getnstUid();


            properties["connector"] = connName;
            properties["condition"] = cond;

            properties["component"] = parenta->getnstId();
            properties["interface"] = entitya->getnstId();

            properties["role"] = cond;

            QnstBind* bo = new QnstBind();
            bo->setnstParent(lo);
            bo->setnstUid(entity->getnstUid());
            bo->setRole(properties["condition"]);
            bo->setComponent(properties["component"]);
            bo->setComponentUid(parenta->getnstUid());
            bo->setInterface(properties["interface"]);
            bo->setInterfaceUid(entitya->getnstUid());

            lo->addCondition(bo);

            binds[bo->getnstUid()] = bo;
            brelations[bo->getnstUid()] = entity->getnstUid();

            link2conn[properties["link"]] = connName;

            entities[entity->getnstUid()] = entity;

            emit entityAdded(entity->getnstUid(), parenta->getnstUid(), properties);
        }

    }else{

    linkDialog->init(connectors);

    if (linkDialog->exec()){
        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        QString con = linkDialog->form.cbCondition->currentText();
        QString act = linkDialog->form.cbAction->currentText();

        if (con != "" && act != ""){
            qreal xcenter = -1;
            qreal ycenter = -1;

            QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                           entitya->getTop() + entitya->getHeight()/2);

            QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                           entityb->getTop() + entityb->getHeight()/2);


            pointa = parentb->mapFromItem(parenta, pointa);

            if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

            }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

            }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                ycenter = pointb.y() + (pointa.y() - pointb.y())/2;

            }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
            }


            QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*) parentb);
            aggregator->setTop(ycenter - DEFAULT_AGGREGATOR_HEIGHT/2);
            aggregator->setLeft(xcenter - DEFAULT_AGGREGATOR_WIDTH/2);
            aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
            aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
            aggregator->adjust();

            parentb->addnstGraphicsEntity(aggregator);

            entities[aggregator->getnstUid()] = aggregator;

            ///

            QnstGraphicsBind* condition = new QnstGraphicsBind();
            condition->setCondition(Qnst::NoBindType);
            condition->setnstGraphicsParent(parentb);
            condition->setEntityA(entitya);
            condition->setEntityB(aggregator);

            // adjusting angle
            adjustAngle(condition, entitya, aggregator);

            condition->adjust();

            parentb->addnstGraphicsEntity(condition);

            ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(condition);
            ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(condition);

            if (con == "onBegin"){
                condition->setCondition(Qnst::onBegin);

            }else if (con == "onEnd"){
                condition->setCondition(Qnst::onEnd);

            }else if (con == "onPause"){
                condition->setCondition(Qnst::onPause);

            }else if (con == "onResume"){
                condition->setCondition(Qnst::onResume);

            }else if (con == "onSelection"){
                condition->setCondition(Qnst::onSelection);

            }else{
                condition->setCondition(Qnst::NoBindType);
            }

            entities[condition->getnstUid()] = condition;

            ///

            QnstGraphicsBind* action = new QnstGraphicsBind();
            action->setAction(Qnst::NoBindType);
            action->setnstGraphicsParent(parentb);
            action->setEntityA(aggregator);
            action->setEntityB(entityb);

            // adjusting angle
            adjustAngle(action, aggregator, entityb);

            action->adjust();

            parentb->addnstGraphicsEntity(action);

            ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(action);
            ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(action);

            if (act == "start"){
                action->setAction(Qnst::Start);

            }else if (act == "stop"){
                action->setAction(Qnst::Stop);

            }else if (act == "pause"){
                action->setAction(Qnst::Pause);

            }else if (act == "resume"){
                action->setAction(Qnst::Resume);

            }else if (act == "set"){
                action->setAction(Qnst::Set);

            }else{
                action->setAction(Qnst::NoBindType);
            }

            entities[action->getnstUid()] = action;

            ///
            connect(condition,
                    SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                    SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

            connect(condition,
                    SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                    SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

            connect(action,
                    SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                    SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

            connect(action,
                    SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                    SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
            ///


            ///// connector

            QString connName = linkDialog->getCurrentConnector();;

            if (linkDialog->getCurrentConnector() == "New..."){
                QnstConnector* conn = new QnstConnector();
                conn->setName(con+act+QString::number(connectors.size()));
                conn->addCondition(con, con);
                conn->addAction(act, act);

                connectors[conn->getName()] = conn;
                connectors2[conn->getnstUid()] = conn;

                QMap<QString, QString> properties;

                properties["TYPE"] = "connector";

                properties["id"] = conn->getName();
                properties["condition"] = con;
                properties["action"] = act;


                connName = conn->getName();

                emit entityAdded(conn->getnstUid(), "", properties);
            }

            // associating connector to bind
            action->setConn(connectors[connName]);
            condition->setConn(connectors[connName]);

            ///// link

            QMap<QString, QString> properties;

            properties["TYPE"] = "bind";
            properties["link"] = "link"+QString::number(++nlink);

            properties["linkUID"] = aggregator->getnstUid();

            QnstLink* lo = new QnstLink();
            lo->setnstParent(aggregator->getnstGraphicsParent());
            lo->setnstId(properties["link"]);
            lo->setnstUid(aggregator->getnstUid());
            lo->setxConnector(connName);
            lo->setxConnectorUID(connectors[connName]->getnstUid());
            lo->setAggregatorUID(aggregator->getnstUid());

            links[lo->getnstUid()] = lo;

            link2conn[properties["link"]] = connName;

            properties["connector"] = connName;
            properties["condition"] = con;

            properties["component"] = parenta->getnstId();
            properties["interface"] = entitya->getnstId();

            properties["role"] = con;

            QnstBind* bo = new QnstBind();
            bo->setnstParent(lo);
            bo->setnstUid(condition->getnstUid());
            bo->setRole(properties["condition"]);
            bo->setComponent(properties["component"]);
            bo->setComponentUid(parenta->getnstUid());
            bo->setInterface(properties["interface"]);
            bo->setInterfaceUid(entitya->getnstUid());

            lo->addCondition(bo);

            binds[bo->getnstUid()] = bo;
            brelations[bo->getnstUid()] = condition->getnstUid();

            emit entityAdded(condition->getnstUid(), parentb->getnstUid(), properties);

            properties["condition"] = "";

            properties["action"] = act;

            properties["component"] = entityb->getnstId();
            properties["interface"] = "";

            properties["role"] = act;

            QnstBind* bo2 = new QnstBind();
            bo2->setnstParent(lo);
            bo2->setnstUid(action->getnstUid());
            bo2->setRole(properties["action"]);
            bo2->setComponent(properties["component"]);
            bo2->setComponentUid(parentb->getnstUid());

            lo->addAction(bo2);

            binds[bo2->getnstUid()] = bo2;
            brelations[bo2->getnstUid()] = action->getnstUid();

            emit entityAdded(action->getnstUid(), parentb->getnstUid(), properties);
        }
    }

    }
}

void QnstView::addInterfacetoNodeEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb)
{
    QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
    QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

    if (entitya->getnstType() == Qnst::Port){
        if (parenta != NULL && parentb != NULL){
            if (parenta == parentb){
                qDebug() << "INTERFACE to NODE:" << entitya->getnstUid() << "->" << entityb->getnstUid();

                foreach(QnstGraphicsEntity* entity, parenta->getnstGraphicsEntities()){
                    if (entity->getnstType() == Qnst::Reference){
                        QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

                        if (entitya == edge->getEntityA()){
                            if (edge->getEntityA()->getncgType() == Qncg::Node){
                                ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                            }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                                ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                            }

                            if (edge->getEntityB()->getncgType() == Qncg::Node){
                                ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                            }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                                ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                            }

                            parenta->removenstGraphicsEntity(edge); delete (edge);

                            break;
                        }
                    }
                }


                QnstGraphicsReference* entity = new QnstGraphicsReference();
                entity->setnstGraphicsParent(parenta);
                entity->setEntityA(entitya);
                entity->setEntityB(entityb);
                entity->adjust();

                parenta->addnstGraphicsEntity(entity);

                ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
                ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

                requestEntityChange(entitya);

            }else if (parenta->getnstGraphicsParent() == parentb && parenta != entityb){
                addInterfacetoNodeLink(entitya, entityb);
            }
        }

    }else if (entitya->getnstType() == Qnst::SwitchPort){

        if (parenta != NULL && parentb != NULL){
            if (parenta == parentb){
                qDebug() << "INTERFACE to NODE:" << entitya->getnstUid() << "->" << entityb->getnstUid();

                QnstGraphicsMapping* entity = new QnstGraphicsMapping();
                entity->setnstGraphicsParent(parenta);
                entity->setEntityA(entitya);
                entity->setEntityB(entityb);
                entity->setComponent(entityb->getnstId());
                entity->setComponentUid(entityb->getnstUid());
                entity->adjust();

                parenta->addnstGraphicsEntity(entity);

                ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
                ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(entity);

                entity->setSwitchPortUid(entitya->getnstUid());

                entities[entity->getnstUid()] = entity;

                QMap<QString, QString> properties;

                properties["TYPE"] = "mapping";

                properties["component"] = entityb->getnstId();
                properties["componentUID"] = entityb->getnstUid();

                // parent is the strucUID from the switchPort entity in this case
                emit entityAdded(entity->getnstUid(), entitya->getnstUid(), properties);

            }else if (parenta->getnstGraphicsParent() == parentb && parenta != entityb){
                addInterfacetoNodeLink(entitya, entityb);
            }
        }
    }else if (parenta->getnstGraphicsParent() == parentb && parenta != entityb){
        addInterfacetoNodeLink(entitya, entityb);
    }

    modified = false;
}

void QnstView::addInterfacetoInterfaceEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb)
{
    if (entitya->getnstType() == Qnst::Port){
        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL){
            if (parenta == parentb->getnstGraphicsParent()){

                qDebug() << "INTERFACE to INTERFACE:" << entitya->getnstUid() << "->" << entityb->getnstUid();

                foreach(QnstGraphicsEntity* entity, parenta->getnstGraphicsEntities()){
                    if (entity->getnstType() == Qnst::Reference){
                        QnstGraphicsEdge* edge = (QnstGraphicsEdge*) entity;

                        if (entitya == edge->getEntityA()){
                            if (edge->getEntityA()->getncgType() == Qncg::Node){
                                ((QnstGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);

                            }else if (edge->getEntityA()->getncgType() == Qncg::Interface){
                                ((QnstGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
                            }

                            if (edge->getEntityB()->getncgType() == Qncg::Node){
                                ((QnstGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

                            }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                                ((QnstGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
                            }

                            parenta->removenstGraphicsEntity(edge); delete (edge);

                            break;
                        }
                    }
                }

                QnstGraphicsReference* entity = new QnstGraphicsReference();
                entity->setnstGraphicsParent(parenta);
                entity->setEntityA(entitya);
                entity->setEntityB(entityb);
                entity->adjust();

                parenta->addnstGraphicsEntity(entity);

                ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
                ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

                requestEntityChange(entitya);

            }else if (parenta->getnstGraphicsParent() == parentb->getnstGraphicsParent()){
                linkDialog->init(connectors);

                if (linkDialog->exec()){
                    QnstGraphicsEntity* parents = parenta->getnstGraphicsParent();

                    QString con = linkDialog->form.cbCondition->currentText();
                    QString act = linkDialog->form.cbAction->currentText();

                    if (con != "" && act != ""){
                        qreal xcenter = -1;
                        qreal ycenter = -1;

                        QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                                       entitya->getTop() + entitya->getHeight()/2);

                        QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                                       entityb->getTop() + entityb->getHeight()/2);


                        pointa = parents->mapFromItem(parenta, pointa);
                        pointb = parents->mapFromItem(parentb, pointb);

                        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
                        }


                        QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*) parents);
                        aggregator->setTop(ycenter - DEFAULT_AGGREGATOR_HEIGHT/2);
                        aggregator->setLeft(xcenter - DEFAULT_AGGREGATOR_WIDTH/2);
                        aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
                        aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
                        aggregator->adjust();

                        parents->addnstGraphicsEntity(aggregator);

                        entities[aggregator->getnstUid()] = aggregator;

                        ///

                        QnstGraphicsBind* condition = new QnstGraphicsBind();
                        condition->setCondition(Qnst::NoBindType);
                        condition->setnstGraphicsParent(parents);
                        condition->setEntityA(entitya);
                        condition->setEntityB(aggregator);

                        // adjusting angle
                        adjustAngle(condition, entitya, aggregator);

                        condition->adjust();

                        parents->addnstGraphicsEntity(condition);

                        ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(condition);
                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(condition);

                        if (con == "onBegin"){
                            condition->setCondition(Qnst::onBegin);

                        }else if (con == "onEnd"){
                            condition->setCondition(Qnst::onEnd);

                        }else if (con == "onPause"){
                            condition->setCondition(Qnst::onPause);

                        }else if (con == "onResume"){
                            condition->setCondition(Qnst::onResume);

                        }else if (con == "onSelection"){
                            condition->setCondition(Qnst::onSelection);

                        }else{
                            condition->setCondition(Qnst::NoBindType);
                        }

                        entities[condition->getnstUid()] = condition;

                        ///

                        QnstGraphicsBind* action = new QnstGraphicsBind();
                        action->setAction(Qnst::NoBindType);
                        action->setnstGraphicsParent(parents);
                        action->setEntityA(aggregator);
                        action->setEntityB(entityb);

                        // adjusting angle
                        adjustAngle(action, aggregator, entityb);

                        action->adjust();


                        parents->addnstGraphicsEntity(action);

                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(action);
                        ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(action);

                        if (act == "start"){
                            action->setAction(Qnst::Start);

                        }else if (act == "stop"){
                            action->setAction(Qnst::Stop);

                        }else if (act == "pause"){
                            action->setAction(Qnst::Pause);

                        }else if (act == "resume"){
                            action->setAction(Qnst::Resume);

                        }else if (act == "set"){
                            action->setAction(Qnst::Set);

                        }else{
                            action->setAction(Qnst::NoBindType);
                        }

                        entities[action->getnstUid()] = action;

                        ////
                        connect(condition,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(condition,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                        connect(action,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(action,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));
                        ///


                        ///// connector

                        QString connName = linkDialog->getCurrentConnector();

                        if (linkDialog->getCurrentConnector() == "New..."){
                            QnstConnector* conn = new QnstConnector();
                            conn->setName(con+act+QString::number(connectors.size()));
                            conn->addCondition(con, con);
                            conn->addAction(act, act);

                            connectors[conn->getName()] = conn;
                            connectors2[conn->getnstUid()] = conn;

                            QMap<QString, QString> properties;

                            properties["TYPE"] = "connector";

                            properties["id"] = conn->getName();
                            properties["condition"] = con;
                            properties["action"] = act;


                            connName = conn->getName();

                            emit entityAdded(conn->getnstUid(), "", properties);
                        }

                        // associating connector to bind
                        action->setConn(connectors[connName]);
                        condition->setConn(connectors[connName]);

                        ///// link

                        QMap<QString, QString> properties;

                        properties["TYPE"] = "bind";
                        properties["link"] = "link"+QString::number(++nlink);

                        properties["linkUID"] = aggregator->getnstUid();;

                        QnstLink* lo = new QnstLink();
                        lo->setnstParent(aggregator->getnstGraphicsParent());
                        lo->setnstId(properties["link"]);
                        lo->setnstUid(aggregator->getnstUid());
                        lo->setxConnector(connName);
                        lo->setxConnectorUID(connectors[connName]->getnstUid());
                        lo->setAggregatorUID(aggregator->getnstUid());

                        links[lo->getnstUid()] = lo;

                        link2conn[properties["link"]] = connName;

                        properties["connector"] = connName;
                        properties["condition"] = con;

                        properties["component"] = parenta->getnstId();
                        properties["interface"] = entitya->getnstId();

                        properties["role"] = con;

                        QnstBind* bo = new QnstBind();
                        bo->setnstParent(lo);
                        bo->setnstUid(condition->getnstUid());
                        bo->setRole(properties["condition"]);
                        bo->setComponent(properties["component"]);
                        bo->setComponentUid(parenta->getnstUid());
                        bo->setInterface(properties["interface"]);
                        bo->setInterfaceUid(entitya->getnstUid());

                        lo->addCondition(bo);

                        binds[bo->getnstUid()] = bo;
                        brelations[bo->getnstUid()] = condition->getnstUid();

                        emit entityAdded(condition->getnstUid(), parents->getnstUid(), properties);

                        properties["condition"] = "";

                        properties["action"] = act;

                        properties["component"] = parentb->getnstId();
                        properties["interface"] = entityb->getnstId();

                        properties["role"] = act;

                        QnstBind* bo2 = new QnstBind();
                        bo2->setnstParent(lo);
                        bo2->setnstUid(action->getnstUid());
                        bo2->setRole(properties["action"]);
                        bo2->setComponent(properties["component"]);
                        bo2->setComponentUid(parentb->getnstUid());
                        bo2->setInterface(properties["interface"]);
                        bo2->setInterfaceUid(entityb->getnstUid());

                        lo->addAction(bo2);

                        binds[bo2->getnstUid()] = bo2;
                        brelations[bo2->getnstUid()] = action->getnstUid();

                        emit entityAdded(action->getnstUid(), parents->getnstUid(), properties);
                    }
                }
                modified = false;
            }
        }

    }else if (entitya->getnstType() == Qnst::SwitchPort){
        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL){
            if (parenta == parentb->getnstGraphicsParent()){
                qDebug() << "INTERFACE to INTERFACE:" << entitya->getnstUid() << "->" << entityb->getnstUid();

                QnstGraphicsMapping* entity = new QnstGraphicsMapping();
                entity->setnstGraphicsParent(parenta);
                entity->setEntityA(entitya);
                entity->setEntityB(entityb);
                entity->setComponent(parentb->getnstId());
                entity->setComponentUid(parentb->getnstUid());
                entity->setInterface(entityb->getnstId());
                entity->setInterfaceUid(entityb->getnstUid());
                entity->adjust();

                parenta->addnstGraphicsEntity(entity);

                ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
                ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(entity);

                entity->setSwitchPortUid(entitya->getnstUid());

                entities[entity->getnstUid()] = entity;

                QMap<QString, QString> properties;

                properties["TYPE"] = "mapping";


                properties["component"] = parentb->getnstId();
                properties["componentUID"] = parentb->getnstUid();

                properties["interface"] = entityb->getnstId();
                properties["interfaceUID"] = entityb->getnstUid();

                // parent is has the strucUID from the switchPort entity in this case
                emit entityAdded(entity->getnstUid(), entitya->getnstUid(), properties);

            }else if (parenta->getnstGraphicsParent() == parentb->getnstGraphicsParent()){
                linkDialog->init(connectors);

                if (linkDialog->exec()){
                    QnstGraphicsEntity* parents = parenta->getnstGraphicsParent();

                    QString con = linkDialog->form.cbCondition->currentText();
                    QString act = linkDialog->form.cbAction->currentText();

                    if (con != "" && act != ""){
                        qreal xcenter = -1;
                        qreal ycenter = -1;

                        QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                                       entitya->getTop() + entitya->getHeight()/2);

                        QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                                       entityb->getTop() + entityb->getHeight()/2);


                        pointa = parents->mapFromItem(parenta, pointa);
                        pointb = parents->mapFromItem(parentb, pointb);

                        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                        }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;

                        }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                            xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                            ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
                        }


                        QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*) parents);
                        aggregator->setTop(ycenter - DEFAULT_AGGREGATOR_HEIGHT/2);
                        aggregator->setLeft(xcenter - DEFAULT_AGGREGATOR_WIDTH/2);
                        aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
                        aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
                        aggregator->adjust();

                        parents->addnstGraphicsEntity(aggregator);

                        entities[aggregator->getnstUid()] = aggregator;

                        ///

                        QnstGraphicsBind* condition = new QnstGraphicsBind();
                        condition->setCondition(Qnst::NoBindType);
                        condition->setnstGraphicsParent(parents);
                        condition->setEntityA(entitya);
                        condition->setEntityB(aggregator);

                        // adjusting angle
                        adjustAngle(condition, entitya, aggregator);

                        condition->adjust();

                        parents->addnstGraphicsEntity(condition);

                        ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(condition);
                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(condition);

                        if (con == "onBegin"){
                            condition->setCondition(Qnst::onBegin);

                        }else if (con == "onEnd"){
                            condition->setCondition(Qnst::onEnd);

                        }else if (con == "onPause"){
                            condition->setCondition(Qnst::onPause);

                        }else if (con == "onResume"){
                            condition->setCondition(Qnst::onResume);

                        }else if (con == "onSelection"){
                            condition->setCondition(Qnst::onSelection);

                        }else{
                            condition->setCondition(Qnst::NoBindType);
                        }

                        entities[condition->getnstUid()] = condition;

                        ///

                        QnstGraphicsBind* action = new QnstGraphicsBind();
                        action->setAction(Qnst::NoBindType);
                        action->setnstGraphicsParent(parents);
                        action->setEntityA(aggregator);
                        action->setEntityB(entityb);

                        // adjusting angle
                        adjustAngle(action, aggregator, entityb);

                        action->adjust();


                        parents->addnstGraphicsEntity(action);

                        ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(action);
                        ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(action);

                        if (act == "start"){
                            action->setAction(Qnst::Start);

                        }else if (act == "stop"){
                            action->setAction(Qnst::Stop);

                        }else if (act == "pause"){
                            action->setAction(Qnst::Pause);

                        }else if (act == "resume"){
                            action->setAction(Qnst::Resume);

                        }else if (act == "set"){
                            action->setAction(Qnst::Set);

                        }else{
                            action->setAction(Qnst::NoBindType);
                        }

                        entities[action->getnstUid()] = action;

                        /////

                        connect(condition,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(condition,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                        connect(action,
                                SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                                SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                        connect(action,
                                SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                                SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                        ///

                        ///// connector

                        QString connName = linkDialog->getCurrentConnector();;

                        if (linkDialog->getCurrentConnector() == "New..."){
                            QnstConnector* conn = new QnstConnector();
                            conn->setName(con+act+QString::number(connectors.size()));
                            conn->addCondition(con, con);
                            conn->addAction(act, act);

                            connectors[conn->getName()] = conn;
                            connectors2[conn->getnstUid()] = conn;

                            QMap<QString, QString> properties;

                            properties["TYPE"] = "connector";

                            properties["id"] = conn->getName();
                            properties["condition"] = con;
                            properties["action"] = act;


                            connName = conn->getName();

                            emit entityAdded(conn->getnstUid(), "", properties);
                        }

                        // associating connector to bind
                        action->setConn(connectors[connName]);
                        condition->setConn(connectors[connName]);

                        ///// link

                        QMap<QString, QString> properties;

                        properties["TYPE"] = "bind";
                        properties["link"] = "link"+QString::number(++nlink);

                        properties["linkUID"] = aggregator->getnstUid();;

                        QnstLink* lo = new QnstLink();
                        lo->setnstParent(aggregator->getnstGraphicsParent());
                        lo->setnstId(properties["link"]);
                        lo->setnstUid(aggregator->getnstUid());
                        lo->setxConnector(connName);
                        lo->setxConnectorUID(connectors[connName]->getnstUid());
                        lo->setAggregatorUID(aggregator->getnstUid());

                        links[lo->getnstUid()] = lo;

                        link2conn[properties["link"]] = connName;

                        properties["connector"] = connName;
                        properties["condition"] = con;

                        properties["component"] = parenta->getnstId();
                        properties["interface"] = entitya->getnstId();

                        properties["role"] = con;

                        QnstBind* bo = new QnstBind();
                        bo->setnstParent(lo);
                        bo->setnstUid(condition->getnstUid());
                        bo->setRole(properties["condition"]);
                        bo->setComponent(properties["component"]);
                        bo->setComponentUid(parenta->getnstUid());
                        bo->setInterface(properties["interface"]);
                        bo->setInterfaceUid(entitya->getnstUid());

                        lo->addCondition(bo);

                        binds[bo->getnstUid()] = bo;
                        brelations[bo->getnstUid()] = condition->getnstUid();

                        emit entityAdded(condition->getnstUid(), parents->getnstUid(), properties);

                        properties["condition"] = "";

                        properties["action"] = act;

                        properties["component"] = parentb->getnstId();
                        properties["interface"] = entityb->getnstId();

                        properties["role"] = act;

                        QnstBind* bo2 = new QnstBind();
                        bo2->setnstParent(lo);
                        bo2->setnstUid(action->getnstUid());
                        bo2->setRole(properties["action"]);
                        bo2->setComponent(properties["component"]);
                        bo2->setComponentUid(parentb->getnstUid());
                        bo2->setInterface(properties["interface"]);
                        bo2->setInterfaceUid(entityb->getnstUid());

                        lo->addAction(bo2);

                        binds[bo2->getnstUid()] = bo2;
                        brelations[bo2->getnstUid()] = action->getnstUid();

                        emit entityAdded(action->getnstUid(), parents->getnstUid(), properties);
                    }
                }
                modified = false;
            }
        }
    }else{
        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL){
            linkDialog->init(connectors);

            if (linkDialog->exec()){
                QnstGraphicsEntity* parents = parenta->getnstGraphicsParent();

                QString con = linkDialog->form.cbCondition->currentText();
                QString act = linkDialog->form.cbAction->currentText();

                if (con != "" && act != ""){
                    qreal xcenter = -1;
                    qreal ycenter = -1;

                    QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                                   entitya->getTop() + entitya->getHeight()/2);

                    QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                                   entityb->getTop() + entityb->getHeight()/2);


                    pointa = parents->mapFromItem(parenta, pointa);
                    pointb = parents->mapFromItem(parentb, pointb);

                    if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y()){
                        xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                        ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                    }else if (pointa.x() > pointb.x() && pointa.y() < pointb.y()){
                        xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                        ycenter = pointa.y() + (pointb.y() - pointa.y())/2;

                    }else if (pointa.x() < pointb.x() && pointa.y() > pointb.y()){
                        xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
                        ycenter = pointb.y() + (pointa.y() - pointb.y())/2;

                    }else if (pointa.x() > pointb.x() && pointa.y() > pointb.y()){
                        xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
                        ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
                    }


                    QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*) parents);
                    aggregator->setTop(ycenter - DEFAULT_AGGREGATOR_HEIGHT/2);
                    aggregator->setLeft(xcenter - DEFAULT_AGGREGATOR_WIDTH/2);
                    aggregator->setWidth(DEFAULT_AGGREGATOR_WIDTH);
                    aggregator->setHeight(DEFAULT_AGGREGATOR_HEIGHT);
                    aggregator->adjust();

                    parents->addnstGraphicsEntity(aggregator);

                    entities[aggregator->getnstUid()] = aggregator;


                    ///

                    QnstGraphicsBind* condition = new QnstGraphicsBind();
                    condition->setCondition(Qnst::NoBindType);
                    condition->setnstGraphicsParent(parents);
                    condition->setEntityA(entitya);
                    condition->setEntityB(aggregator);

                    // adjusting angle
                    adjustAngle(condition, entitya, aggregator);

                    condition->adjust();

                    parents->addnstGraphicsEntity(condition);

                    ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(condition);
                    ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(condition);

                    if (con == "onBegin"){
                        condition->setCondition(Qnst::onBegin);

                    }else if (con == "onEnd"){
                        condition->setCondition(Qnst::onEnd);

                    }else if (con == "onPause"){
                        condition->setCondition(Qnst::onPause);

                    }else if (con == "onResume"){
                        condition->setCondition(Qnst::onResume);

                    }else if (con == "onSelection"){
                        condition->setCondition(Qnst::onSelection);

                    }else{
                        condition->setCondition(Qnst::NoBindType);
                    }

                    entities[condition->getnstUid()] = condition;

                    ///

                    QnstGraphicsBind* action = new QnstGraphicsBind();
                    action->setAction(Qnst::NoBindType);
                    action->setnstGraphicsParent(parents);
                    action->setEntityA(aggregator);
                    action->setEntityB(entityb);

                    // adjusting angle
                    adjustAngle(action, aggregator, entityb);

                    action->adjust();


                    parents->addnstGraphicsEntity(action);

                    ((QnstGraphicsNode*) aggregator)->addnstGraphicsEdge(action);
                    ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(action);

                    if (act == "start"){
                        action->setAction(Qnst::Start);

                    }else if (act == "stop"){
                        action->setAction(Qnst::Stop);

                    }else if (act == "pause"){
                        action->setAction(Qnst::Pause);

                    }else if (act == "resume"){
                        action->setAction(Qnst::Resume);

                    }else if (act == "set"){
                        action->setAction(Qnst::Set);

                    }else{
                        action->setAction(Qnst::NoBindType);
                    }

                    entities[action->getnstUid()] = action;

                    ///

                    connect(condition,
                            SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                            SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                    connect(condition,
                            SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                            SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                    connect(action,
                            SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                            SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)));

                    connect(action,
                            SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                            SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)));

                    ///// connector

                    QString connName = linkDialog->getCurrentConnector();;

                    if (linkDialog->getCurrentConnector() == "New..."){
                        QnstConnector* conn = new QnstConnector();
                        conn->setName(con+act+QString::number(connectors.size()));
                        conn->addCondition(con, con);
                        conn->addAction(act, act);

                        connectors[conn->getName()] = conn;
                        connectors2[conn->getnstUid()] = conn;

                        QMap<QString, QString> properties;

                        properties["TYPE"] = "connector";

                        properties["id"] = conn->getName();
                        properties["condition"] = con;
                        properties["action"] = act;


                        connName = conn->getName();

                        emit entityAdded(conn->getnstUid(), "", properties);
                    }

                    // associating connector to bind
                    action->setConn(connectors[connName]);
                    condition->setConn(connectors[connName]);

                    ///// link

                    QMap<QString, QString> properties;

                    properties["TYPE"] = "bind";
                    properties["link"] = "link"+QString::number(++nlink);

                    properties["linkUID"] = aggregator->getnstUid();;

                    QnstLink* lo = new QnstLink();
                    lo->setnstParent(aggregator->getnstGraphicsParent());
                    lo->setnstId(properties["link"]);
                    lo->setnstUid(aggregator->getnstUid());
                    lo->setxConnector(connName);
                    lo->setxConnectorUID(connectors[connName]->getnstUid());
                    lo->setAggregatorUID(aggregator->getnstUid());

                    links[lo->getnstUid()] = lo;

                    link2conn[properties["link"]] = connName;

                    properties["connector"] = connName;
                    properties["condition"] = con;

                    properties["component"] = parenta->getnstId();
                    properties["interface"] = entitya->getnstId();

                    properties["role"] = con;

                    QnstBind* bo = new QnstBind();
                    bo->setnstParent(lo);
                    bo->setnstUid(condition->getnstUid());
                    bo->setRole(properties["condition"]);
                    bo->setComponent(properties["component"]);
                    bo->setComponentUid(parenta->getnstUid());
                    bo->setInterface(properties["interface"]);
                    bo->setInterfaceUid(entitya->getnstUid());

                    lo->addCondition(bo);

                    binds[bo->getnstUid()] = bo;
                    brelations[bo->getnstUid()] = condition->getnstUid();

                    emit entityAdded(condition->getnstUid(), parents->getnstUid(), properties);

                    properties["condition"] = "";

                    properties["action"] = act;

                    properties["component"] = parentb->getnstId();
                    properties["interface"] = entityb->getnstId();

                    properties["role"] = act;

                    QnstBind* bo2 = new QnstBind();
                    bo2->setnstParent(lo);
                    bo2->setnstUid(action->getnstUid());
                    bo2->setRole(properties["action"]);
                    bo2->setComponent(properties["component"]);
                    bo2->setComponentUid(parentb->getnstUid());
                    bo2->setInterface(properties["interface"]);
                    bo2->setInterfaceUid(entityb->getnstUid());

                    lo->addAction(bo2);

                    binds[bo2->getnstUid()] = bo2;
                    brelations[bo2->getnstUid()] = action->getnstUid();

                    emit entityAdded(action->getnstUid(), parents->getnstUid(), properties);
                }
            }
            modified = false;
        }
    }
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

void QnstView::adjustAngle(QnstGraphicsEdge* edge, QnstGraphicsEntity* entitya,
                           QnstGraphicsEntity* entityb)
{
  int angle = 0;

  while(1)
  {
    if (!entitya->getAngles()[entityb->getnstUid()].contains(angle))
    {
      break;
    }
    else if (!entitya->getAngles()[entityb->getnstUid()].contains(-angle))
    {
      angle = -angle;
      break;
    }

    angle += 60;
  }

  entitya->addAngle(entityb->getnstUid(), angle);
  entityb->addAngle(entitya->getnstUid(), -angle);

  edge->setAngle(angle);
}


void QnstView::requestBindParamAdjust(QString uid, QString parent,
                                      QMap<QString, QString> properties)
{
  properties["TYPE"] = "bindParam";

  if (!properties["name"].isEmpty() && !properties["value"].isEmpty())
  {
    if (bindParamUIDToBindUID.contains(uid))
    {
      emit entityChanged(uid, properties);
    }
    else
    {
      bindParamUIDToBindUID[uid] = brelations.key(parent);
      emit entityAdded(uid, parent,properties );
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
  if (binds.contains(brelations.key(bindUID)))
  {
    binds[brelations.key(bindUID)]->setParams(params);
    binds[brelations.key(bindUID)]->setNameUIDs(name_uids);
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

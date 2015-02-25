#include "StructuralView.h"

//
// ATTENTION: This code needs a refactoring.
//

#include <assert.h>
#include <QDebug>

#ifdef SMOOTH_ZOOM
#include <QTimeLine>
#endif

#include "StructuralUtil.h"
#include "StructuralGraphicsBind.h"

std::map <Structural::EntityType, QString> StructuralView::mediaTypeToXMLStr =
    create_map<Structural::EntityType, QString>
      (Structural::Media, "media")
      (Structural::Image, "image")
      (Structural::Audio, "audio")
      (Structural::Video, "video")
      (Structural::Html, "text/html")
      (Structural::NCL, "application/x-ginga-NCL")
      (Structural::Text, "text")
      (Structural::NCLua, "application/x-ginga-NCLua")
      (Structural::Settings, "application/x-ncl-settings");

StructuralView::StructuralView(QWidget* parent)
    : QGraphicsView(parent)
{
  createObjects();
  createConnection();

  zoomStep = 0;

#ifdef SMOOTH_ZOOM
  _numScheduledScalings = 0;
#endif
  linking = false;

  modified = false;

  link = NULL;

  selected = NULL;

  clipboard = NULL;

  hasCutted = false;

  lastLinkMouseOver = NULL;

  // Initialize entity counters
  for(int i = Structural::Node; i < Structural::NoType; i += 1)
    entityCounter[ (Structural::EntityType) i ] = 0;

  setAttribute(Qt::WA_TranslucentBackground);

  minimap = new StructuralMiniMap(this);
  minimap->init(this);
  minimap->setMinimumSize(MINIMAP_DEFAULT_W, MINIMAP_DEFAULT_H);
}

StructuralView::~StructuralView()
{

}

void StructuralView::load(QString data)
{
//  qDebug() << "###########################################";
//  qDebug() << data;
//  qDebug() << "###########################################";

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

  foreach (StructuralGraphicsEntity* entity, entities.values())
  {
    if (entity->isMedia())
    {
      qDebug() << "=========================================" << entity->getnstId();
      qDebug() << "=========================================" << entity->getTop() << entity->getLeft() << entity->getWidth() << entity->getHeight();
      adjustMedia((StructuralGraphicsMedia*) entity);
    }

    entity->adjust();
  }

  deletePendingEntities();
}

void StructuralView::collapseCompositions(QDomElement element, QDomElement parent)
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
      StructuralGraphicsComposition* c =
          ((StructuralGraphicsComposition*) entities[element.attribute("uid")]);

      c->collapse();

      c->setTop(element.attribute("top").toDouble());
      c->setLeft(element.attribute("left").toDouble());
    }
  }
}

void StructuralView::read(QDomElement element, QDomElement parent)
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
      addEntity(uid, parent.attribute("uid"), properties, true, false);
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
    StructuralConnector* conn = new StructuralConnector();
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
          if (e.attribute("uid") != "")
              conn->addCondition(e.attribute("uid"), e.attribute("type"));
          else
              conn->addCondition(e.attribute("type"), e.attribute("type"));
        }
        else if (e.nodeName() == "action")
        {
          if (e.attribute("uid") != "")
            conn->addAction(e.attribute("uid"), e.attribute("type"));
          else
            conn->addAction(e.attribute("type"), e.attribute("type"));
        }
        else if (e.nodeName() == "param")
        {
          if (e.attribute("type") == "action")
          {
            conn->addActionParam(e.attribute("uid"), e.attribute("name"), e.attribute("value"));
          }
          else if (e.attribute("type") == "condition")
          {
            conn->addConditionParam(e.attribute("uid"), e.attribute("name"), e.attribute("value"));
          }
          else
          {
            // maintaining compatibility with old versions (< v0.1.5)
            conn->addActionParam(e.attribute("name"), "", e.attribute("name"));
            conn->addConditionParam(e.attribute("name"), "", e.attribute("name"));
          }
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

void StructuralView::readLink(QDomElement element, QDomElement parent)
{
  Q_UNUSED(parent)

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
        StructuralGraphicsInterface* entitya = (StructuralGraphicsInterface*) entities[element.attribute("uid")];
        StructuralGraphicsInterface* entityb = (StructuralGraphicsInterface*) entities[element.attribute("interfaceUID")];

        assert ( entitya != NULL );
        assert ( entityb != NULL );

        if (entitya != entityb)
        {
          StructuralGraphicsReference* entity = new StructuralGraphicsReference();
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
        StructuralGraphicsInterface* entitya =
            (StructuralGraphicsInterface*)entities[element.attribute("uid")];

        StructuralGraphicsNode* entityb =
            (StructuralGraphicsNode*)entities[element.attribute("componentUID")];

        StructuralGraphicsReference* entity = new StructuralGraphicsReference();
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
        StructuralGraphicsBind *bind =
            dynamic_cast<StructuralGraphicsBind *> (entities[properties["uid"]]);

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

          qDebug() << "=====================================";
          qDebug() << "=====================================";
          qDebug() << "=====================================" << params;
          qDebug() << "=====================================" << name_uid;
          qDebug() << "=====================================";
          qDebug() << "=====================================";

          // Set bindParams
          bind->setParams(params);
          bind->setNamesUIDs(name_uid);

          connect( bind,
                   SIGNAL(bindParamAdded(QString,QString,QMap<QString, QString>)),
                   SLOT(requestBindParamAdjust(QString,QString,QMap<QString, QString>)) );

          connect( bind,
                   SIGNAL(bindParamUpdated(QString,QMap<QString,QString>,QMap<QString,QString>)),
                   SLOT(updateBindParams(QString,QMap<QString,QString>,QMap<QString,QString>)) );

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

QString StructuralView::serialize()
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

  foreach(StructuralConnector* conn, connectors.values())
  {
    assert(conn != NULL);

    QDomElement e = dom->createElement("connector");

    e.setAttribute("id", conn->getName());
    e.setAttribute("uid", conn->getnstUid());

    foreach(QString k, conn->getConditions().keys())
    {
      QDomElement c = dom->createElement("condition");
      c.setAttribute("uid", k); // since version 0.1.5
      c.setAttribute("type", conn->getConditions().value(k));

      e.appendChild(c);
    }

    foreach(QString k, conn->getActions().keys())
    {
      QDomElement a = dom->createElement("action");
      a.setAttribute("uid", k); // since version 0.1.5
      a.setAttribute("type", conn->getActions().value(k));

      e.appendChild(a);
    }

// deprecated since version 0.1.5
//    foreach(QString key, conn->getParams().keys())
//    {
//      QDomElement p = dom->createElement("param");

//      p.setAttribute("uid", key);
//      p.setAttribute("name", conn->getParams()[key]);

//      e.appendChild(p);
//    }

    QPair<QString, QString> key;

    foreach(key, conn->getActionParams().keys())
    {
      QDomElement p = dom->createElement("param");

      p.setAttribute("uid", key.first);
      p.setAttribute("name", key.second);
      p.setAttribute("value", conn->getActionParams().value(key));
      p.setAttribute("type", "action");

      e.appendChild(p);
    }

    foreach(key, conn->getConditionParams().keys())
    {
      QDomElement p = dom->createElement("param");

      p.setAttribute("uid", key.first);
      p.setAttribute("name", key.second);
      p.setAttribute("value", conn->getConditionParams().value(key));
      p.setAttribute("type", "condition");

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

  foreach(StructuralGraphicsLink* link, links.values())
  {
    QDomElement e = dom->createElement("link"); // Previously from 0.1.3 thi was linkdata

    e.setAttribute("id", link->getnstId());
    e.setAttribute("uid", link->getnstUid());

    e.setAttribute("top", QString::number(link->getTop()));
    e.setAttribute("left", QString::number(link->getLeft()));
    e.setAttribute("width", QString::number(link->getWidth()));
    e.setAttribute("height", QString::number(link->getHeight()));

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

  foreach(StructuralGraphicsEntity* entity, scene->getRoots())
  {
    QDomElement e = dom->createElement("body");

    e.setAttribute("id", entity->getnstId());
    e.setAttribute("uid", entity->getnstUid());

    e.setAttribute("top", QString::number(entity->getTop()));
    e.setAttribute("left", QString::number(entity->getLeft()));
    e.setAttribute("width", QString::number(entity->getWidth()));
    e.setAttribute("height", QString::number(entity->getHeight()));

    e.setAttribute("collapsed", ((StructuralGraphicsComposition*) entity)->isCollapsed());

    e.setAttribute("expandWidth", ((StructuralGraphicsComposition*) entity)->getLastW());
    e.setAttribute("expandHeight", ((StructuralGraphicsComposition*) entity)->getLastH());

    foreach(StructuralGraphicsEntity* c, entity->getnstGraphicsEntities())
    {
      write(e, dom, c);
    }

    writeLink(e, dom, entity);

    root.appendChild(e);
  }

  dom->appendChild(root);

  linkWriterAux.clear();

 // qDebug() << dom->toString(4);

  return dom->toString(4);
}

void StructuralView::write(QDomElement element, QDomDocument* dom,
                     StructuralGraphicsEntity* entity)
{
  QDomElement e;

  switch(entity->getnstType())
  {
    case Structural::Body:
      e = dom->createElement("body");

      e.setAttribute("collapsed", ((StructuralGraphicsComposition*) entity)->isCollapsed());

      e.setAttribute("expandWidth", ((StructuralGraphicsComposition*) entity)->getLastW());
      e.setAttribute("expandHeight", ((StructuralGraphicsComposition*) entity)->getLastH());
      break;

    case Structural::Context:
      e = dom->createElement("context");

      e.setAttribute("collapsed", ((StructuralGraphicsComposition*) entity)->isCollapsed());

      e.setAttribute("expandWidth", ((StructuralGraphicsComposition*) entity)->getLastW());
      e.setAttribute("expandHeight", ((StructuralGraphicsComposition*) entity)->getLastH());
      break;

    case Structural::Switch:
      e = dom->createElement("switch");

      e.setAttribute("collapsed", ((StructuralGraphicsComposition*) entity)->isCollapsed());

      e.setAttribute("expandWidth", ((StructuralGraphicsComposition*) entity)->getLastW());
      e.setAttribute("expandHeight", ((StructuralGraphicsComposition*) entity)->getLastH());
      break;

    case Structural::Media:
    case Structural::Image:
    case Structural::Audio:
    case Structural::Video:
    case Structural::Html:
    case Structural::NCL:
    case Structural::Text:
    case Structural::Settings:
    case Structural::NCLua:
      e = dom->createElement("media");

      e.setAttribute("type", mediaTypeToXMLStr[entity->getnstType()]);

      if (((StructuralGraphicsMedia*) entity)->getSource() != "" )
      {
        e.setAttribute("src", ((StructuralGraphicsMedia*) entity)->getSource());
      }

      e.setAttribute("refer", ((StructuralGraphicsMedia*) entity)->getRefer());
      e.setAttribute("referUID", ((StructuralGraphicsMedia*) entity)->getReferUID());
      e.setAttribute("instance", ((StructuralGraphicsMedia*) entity)->getInstance());
      break;

    case Structural::Property:
      e = dom->createElement("property");

      if (((StructuralGraphicsInterface*) entity)->isRefer())
          e.setAttribute("isRefer","true");

      break;

    case Structural::SwitchPort:
      e = dom->createElement("switchPort");
      break;

    case Structural::Area:
      e = dom->createElement("area");

      if (((StructuralGraphicsInterface*) entity)->isRefer())
          e.setAttribute("isRefer","true");

      break;

    case Structural::Aggregator:
      // \fixme This is deprecated!! Now, aggregator is the same of link!!
      // e = dom->createElement("aggregator");
      break;

    case Structural::Port:
    {
      e = dom->createElement("port");

      StructuralGraphicsPort* pentity = ((StructuralGraphicsPort*) entity);

      foreach(StructuralGraphicsEdge* edge, pentity->getnstGraphicsEdges())
      {
        if (edge->getnstType() == Structural::Reference && edge->getEntityA() == entity)
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

  e.setAttribute("top", QString::number(entity->getTop()));
  e.setAttribute("left", QString::number(entity->getLeft()));
  e.setAttribute("width", QString::number(entity->getWidth()));
  e.setAttribute("height",QString::number(entity->getHeight()));

  writeLink(element, dom, entity);

  foreach(StructuralGraphicsEntity* c, entity->getnstGraphicsEntities())
  {
    write(e, dom, c);
  }

  element.appendChild(e);
}

void StructuralView::writeLink(QDomElement element, QDomDocument* dom,
                         StructuralGraphicsEntity* entity)
{
  if (entity->getncgType() == Qncg::Node)
  {
    StructuralGraphicsNode* node = (StructuralGraphicsNode*) entity;

    foreach(StructuralGraphicsEdge* edge, node->getnstGraphicsEdges())
    {
      //We already have this link, so skipping
      if(linkWriterAux.contains(edge->getnstUid())) continue;
      QDomElement e; // the element that will be inserted

      if (edge->getnstType() == Structural::Condition ||
          edge->getnstType() == Structural::Action)
      {
        StructuralGraphicsBind* bind = dynamic_cast <StructuralGraphicsBind*> (edge);

        if(bind)
        {
          e = dom->createElement("bind");
          e.setAttribute("id", bind->getnstId());
          e.setAttribute("uid", bind->getnstUid());

          e.setAttribute("linkUID", bind->getLink()->getnstUid());

          // We are writing a condition
          if (edge->getnstType() ==  Structural::Condition)
          {
            e.setAttribute("type", "condition");

            e.setAttribute("condition",
                           StructuralUtil::getStrFromBindType( bind->getType()) );
          }
          // We are writing an action
          else if(edge->getnstType() == Structural::Action)
          {
            e.setAttribute("type", "action");
            e.setAttribute("action",
                           StructuralUtil::getStrFromBindType(bind->getType()));
          }

          //\fixme Probably this should never get here with NULL
          if(bind->getEntityA() != NULL)
          {
            e.setAttribute("componentaUID", bind->getEntityA()->getnstUid());
          }

          //\fixme Probably this should never get here with NULL
          if(bind->getEntityB() != NULL)
          {
            e.setAttribute("componentbUID", bind->getEntityB()->getnstUid());
          }

          e.setAttribute("component", bind->getComponent());
          e.setAttribute("componentUid", bind->getComponentUid());

          e.setAttribute("interface", bind->getInterface());
          e.setAttribute("interfaceUid", bind->getInterfaceUid());

          element.appendChild(e);

          linkWriterAux.insert( bind->getnstUid() );

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
      else if (edge->getnstType() == Structural::Mapping)
      {
        StructuralGraphicsMapping* link = (StructuralGraphicsMapping*) edge;

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

void StructuralView::createObjects()
{
  scene = new StructuralScene();
  scene->setParent(this);
  scene->setSceneRect(0, 0, 3200, 1800);

  setScene(scene);

  linkDialog = new StructuralGraphicsLinkDialog(this);

  conditionDialog = new StructuralGraphicsConditionDialog(this);

  actionDialog = new StructuralGraphicsActionDialog(this);

  this->resize(scene->itemsBoundingRect().size().width(), scene->itemsBoundingRect().height());
}

void StructuralView::createConnection()
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

  connect(scene, SIGNAL(entityAdded(StructuralGraphicsEntity*)),
                 SLOT(requestEntityAddition(StructuralGraphicsEntity*)));
  connect(scene, SIGNAL(entityChanged(StructuralGraphicsEntity*)),
                 SLOT(requestEntityChange(StructuralGraphicsEntity*)));
  connect(scene, SIGNAL(entityRemoved(StructuralGraphicsEntity*)),
                 SLOT(requestEntityRemotion(StructuralGraphicsEntity*)));
  connect(scene, SIGNAL(entitySelected(StructuralGraphicsEntity*)),
                 SLOT(requestEntitySelection(StructuralGraphicsEntity*)));

  connect(scene,
          SIGNAL(entityAboutToChange(StructuralGraphicsEntity*,
                                     QMap<QString,QString>)),
          SLOT(requestEntityPreparation(StructuralGraphicsEntity*,
                                        QMap<QString,QString>)));
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

void StructuralView::addEntity(const QString uid, const QString parent,
                         const QMap<QString, QString> &properties, bool undo,
                         bool adjust)
{
  qDebug();
  qDebug() << "============================================= ADD ENTITY" << properties;
  qDebug();

  // \fixme Move this logic to Insert
  bool ok = false;

  Structural::EntityType type = StructuralUtil::getnstTypeFromStr(properties["TYPE"]);
  StructuralGraphicsEntity *entityParent = 0;
  StructuralGraphicsEntity *entity = NULL;

  if(entities.count(parent))
  {
    entityParent = dynamic_cast<StructuralGraphicsEntity*> (entities[parent]);
  }

  qDebug() << "[QNST]" << ":" << "Adding entity '" + uid + "'" << properties << "Parent: " << parent;



  if(!entityParent &&
     type != Structural::Body &&
     type != Structural::Bind // for bind we will discover the parent after that
    )
  {
    qWarning() << "[QNST] Trying to add an entity without a parent!!";
    ok = false; // We still can be a special case. So, I can not return here!

    if( properties["TYPE"] == "importBase" ||
        properties["TYPE"] == "causalConnector" ||
        properties["TYPE"] == "connectorParam" ||
        properties["TYPE"] == "simpleAction" ||
        properties["TYPE"] == "simpleCondition" )
    { // In these cases we do not need a parent.
      qWarning() << "[QNST] " << properties["TYPE"] << " is a special case. So, we will not need a parent.";
    }
    else
    {
      qWarning() << "[QNST] The entityAdd is beging ignored!";
      return;
    }
  }
  // \fixme There are a lot of duplicated code among the cases bellow!
  switch (type)
  {
    case Structural::Body:
      entity = StructuralUtil::makeGraphicsEntity(type);
      entity->setnstUid(uid);

      entity->setTop(scene->height()/2 - DEFAULT_BODY_HEIGHT/2);
      entity->setLeft(scene->width()/2 - DEFAULT_BODY_WIDTH/2);
      entity->setWidth(DEFAULT_BODY_WIDTH);
      entity->setHeight(DEFAULT_BODY_HEIGHT);

      scene->addRoot(entity);
      ok = true;
      break;

    case Structural::Context:
    case Structural::Switch:
    {
      entity = StructuralUtil::makeGraphicsEntity(type, entityParent, properties);
      if(entity != NULL) ok = true;
      break;
    }

    case Structural::Media:
    {
      entity = StructuralUtil::makeGraphicsEntity(type, entityParent, properties);

      if(entity != NULL)
      {
        entity->setnstType(StructuralUtil::getnstTypeFromMime(properties["type"]));
        ok = true;
      }
      break;
    }

    case Structural::Port:
    {
      entity = StructuralUtil::makeGraphicsEntity(type, entityParent, properties);
      if(entity != NULL) ok = true;
      break;
    }

    case Structural::Link:
    {
      if (!links.contains(uid))
      {
        entity = StructuralUtil::makeGraphicsEntity(type, entityParent, properties);
        StructuralGraphicsLink *link = dynamic_cast <StructuralGraphicsLink*>(entity);

        if(link)
        {
          link->setnstUid(uid);

          if (properties["id"] != "")
            link->setnstId(properties["id"]);

          links[uid] = link;
          ok = true;
        }
      }
      else
      {
        qWarning() << "[QNST] Warning! Link already added!!";
      }
      break;
    }

    case Structural::Bind:
    case Structural::Action:
    case Structural::Condition:
    {
      if (!binds.contains(uid))
      {
        StructuralGraphicsEntity *entitya = NULL, *entityb = NULL;
        QString action = "", condition = "";

        if(entities.contains(properties["linkUID"]))
          entitya = entities[properties["linkUID"]];
        else
          qWarning() << "[QNST] Trying to make a bind that componentaUID does not exist!";

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
            StructuralGraphicsLink *link = links[ entitya->getnstUid() ];

            StructuralGraphicsBind* bind = createBind( link,
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

    case Structural::BindParam: /* Warning: this is not a QnstEntity subclass */
      if (entities.contains(parent))
      {
        StructuralGraphicsBind *bind = dynamic_cast<StructuralGraphicsBind *>(entityParent);

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

    case Structural::SwitchPort:
    {
      entity = StructuralUtil::makeGraphicsEntity(type, entityParent, properties);
      if(entity != NULL) ok = true;
      break;
    }


    case Structural::Mapping:
    {
      StructuralGraphicsNode* oparent =
          (StructuralGraphicsNode*) entityParent->getnstGraphicsParent();

      entity = new StructuralGraphicsMapping(oparent);
      ((StructuralGraphicsMapping*) entity)->setSwitchPortUid(parent);
      ok = true;

      break;
    }

    case Structural::Aggregator:
    {
      qWarning() << "[QNST] ERROR!!! Trying to add an Aggregator!! This is not \
                    supported anymore!";
      break;
    }


    case Structural::Area:
    case Structural::Property:
    {
      entity = StructuralUtil::makeGraphicsEntity(type, entityParent, properties);

      if (properties.contains("isRefer"))
      {
        ((StructuralGraphicsInterface*) entity)->setRefer(true);
      }

      ok = true;
      break;
    }

    default:
      //do nothing
      break;
  }

  if(ok)
  {

    entity->setnstUid(uid);
    entities[uid] = entity;
    entity->setProperties(properties);

    entity->updateToolTip();
    entity->adjust();

    if (adjust)
      adjustEntity(entity);
 }

  qDebug();
  qDebug() << "============================================= TYPE " << properties["TYPE"];
  qDebug();


  // if the entity type is CONNECTOR
  if (properties["TYPE"] == "causalConnector")
  {
    addConnector(uid, properties);
  }
  // if the entity type is CONDITION
  else if (properties["TYPE"] == "simpleCondition")
  {
    addCondition(uid, properties);
  }
  // if the entity type is ACTION
  else if (properties["TYPE"] == "simpleAction")
  {
    addAction(uid, properties);
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
    Insert* cmd = new Insert(this, entity);
    history.push(cmd);
  }
}

void StructuralView::removeEntity(const QString uid, bool undo, bool rmRef)
{
  Q_UNUSED(rmRef)

  // traceEntities();

  StructuralConnector* cONN = NULL;

  // \fixme Move this logic to RemoveCommand
  if (entities.contains(uid))
  {
    StructuralGraphicsEntity* entity = entities[uid];

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
          Remove* cmd = new Remove(this, entity);
          history.push(cmd);
        }
      }

      // We are removing a Node
      switch(qncgType)
      {
        case Qncg::Node:
        {
          StructuralGraphicsNode *node = dynamic_cast <StructuralGraphicsNode*>(entity);
          if(node)
            removeEdgesOfEntity(node, false);

          // Remove recursively
          foreach(StructuralGraphicsEntity* e, entity->getnstGraphicsEntities())
          {
            if (e->getnstType() != Structural::Edge &&
                e->getnstType() != Structural::Condition &&
                e->getnstType() != Structural::Mapping &&
                e->getnstType() != Structural::Action &&
                e->getnstType() != Structural::Reference)
            {
              removeEntity(e->getnstUid(), true, false);
            }
          }

          // If we are removing a link, also remove it from the links map
          if (entity->getnstType() == Structural::Link ||
              entity->getnstType() == Structural::Aggregator)
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
          StructuralGraphicsInterface *interface =
              dynamic_cast <StructuralGraphicsInterface*>(entity);

          if(interface)
            removeEdgesOfEntity(interface, false); // Remove the edges of this entity
          else
            qWarning() << "Could not make a cast to QnstGraphicsInterface*";

          mustRemoveFromParent = true;
          break;
        }

        case Qncg::Edge:
        {
          StructuralGraphicsEdge *edge = dynamic_cast<StructuralGraphicsEdge *>(entity);

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
        StructuralGraphicsEntity* parent = entity->getnstGraphicsParent();
        if (parent != NULL)
        {
          qDebug() << "parent = "
                   << QString().sprintf("%p", parent)
                   << parent->getnstUid();

          parent->removenstGraphicsEntity(entity);
        }
        else
          scene->removeRoot(entity);

        if (selected == entity)
          selected = NULL;

        if (clipboard == entity)
          clipboard = NULL;

        if (entity->getnstType() == Structural::Link &&
            links.contains(entity->getnstUid()))
        {
          links.remove(entity->getnstUid());
        }

        entities.remove(entity->getnstUid());

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
            StructuralGraphicsMedia* parentMedia
                = dynamic_cast<StructuralGraphicsMedia*>(parent);
            if (parentMedia &&
                entities.contains(parentMedia->getnstUid()))
            {
              // adjustMedia(parentMedia);

              foreach(QString rUid, refers.keys(parentMedia->getnstUid()))
              {
                if (entities.contains(rUid))
                {
                  StructuralGraphicsMedia* referMedia =
                      dynamic_cast <StructuralGraphicsMedia*>(entities.value(rUid));

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
              StructuralGraphicsEntity* e = entities.value(ruid);

              if (e->isMedia())
                adjustMedia((StructuralGraphicsMedia*) e);
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
    foreach(StructuralConnector* cc, connectors.values())
    {
      if (cc->getnstUid() == importBases[uid])
        connectors.remove(cc->getName());
    }

    foreach(StructuralConnector* cc, connectors2.values())
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
  else if ((cONN = getConnectorByRoleUid(uid)) != NULL)
  {
    cONN->removeAction(uid);
    cONN->removeCondition(uid);
  }
  else if (bindParamUIDToBindUID.contains(uid))
  {
    if(bindParamUIDToBindUID.contains(uid))
    {
      if(entities.contains(bindParamUIDToBindUID[uid]))
      {
        StructuralGraphicsEntity* e = entities[bindParamUIDToBindUID[uid]];

        StructuralGraphicsBind *bind = dynamic_cast<StructuralGraphicsBind*>(e);
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

void StructuralView::changeEntity(const QString uid,
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
    StructuralGraphicsEntity* entity = entities[uid];

    switch(entity->getnstType())
    {
      case Structural::Body:
      {
        entity->setnstId(properties["id"]);
        entity->setProperties(properties);

        entity->adjust();
        break;
      }

      case Structural::Context:
      case Structural::Switch:
      {
        if (properties["id"] != "")
          entity->setnstId(properties["id"]);

        entity->setProperties(properties);
        // \todo refer
        break;
      }

      // if the entity type is MEDIA
      case Structural::Audio:
      case Structural::Text:
      case Structural::Video:
      case Structural::Image:
      case Structural::NCLua:
      case Structural::Settings:
      case Structural::Media:
      case Structural::Html:
      case Structural::Stream:
      case Structural::NCL:
      {
        StructuralGraphicsMedia *media = dynamic_cast <StructuralGraphicsMedia *>(entity);
        if(media)
        {
          media->setnstId(properties["id"]);
          media->setProperties(properties);

          if (properties["referUID"] != "")
          {
            if (entities.contains(properties["referUID"]))
              media->setSource(((StructuralGraphicsMedia*)
                                entities[properties["referUID"]])->getSource());
          }

          QString src = media->getSource();
          media->setnstType(StructuralUtil::getnstTypeFromMime(properties["type"]));

          if(src == "")
          {
            // do nothing
          }
          else {
            int url_end = src.indexOf(":");
            if(url_end >= 0)
            {
              QString schema = src.mid(0, url_end);
              media->setnstType(StructuralUtil::getnstTypeFromUrlSchema(schema));
            }
            int start = src.lastIndexOf(".");
            if(start >= 0)
            {
              QString ext = QFileInfo(src).suffix().toLower();
              media->setnstType(StructuralUtil::getnstTypeFromExtension(ext));
            }
          }

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

      case Structural::Port:
      {
        entity->setnstId(properties["id"]);

        entity->setProperties(properties);


        foreach (QString key, refers.keys(entity->getnstUid()))
        {
          if (entities.contains(key))
            entities[key]->setnstId(entity->getnstId());
        }

        StructuralGraphicsPort *port = dynamic_cast<StructuralGraphicsPort*> (entity);

        // \todo updateToolTip!!
        if(port)
          adjustPort(port);
      }
        break;

      case Structural::Area:
      case Structural::Property:
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

      case Structural::SwitchPort:
      {
        if(properties.contains("id"))
          entity->setnstId(properties["id"]);

        entity->setProperties(properties);

        entity->adjust();
        break;
      }

      case Structural::Mapping:
      {
        entity->setProperties(properties);

        StructuralGraphicsMapping *mapping = dynamic_cast<StructuralGraphicsMapping*> (entity);
        adjustMapping(mapping);
        break;
      }

      case Structural::Link:
      {
        entity->setProperties(properties);
        // \todo Update Link!!
        break;
      }

      case Structural::Bind:
      case Structural::Action:
      case Structural::Condition:
      {
        StructuralGraphicsBind *bind = dynamic_cast<StructuralGraphicsBind *> (entity);

        if(bind)
        {
          bind->setProperties(properties);

          adjustBind(bind);
        }
        break;
      }

      default:
        // do nothing
        break;
    }

    entity->updateToolTip();
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

void StructuralView::selectEntity(const QString uid)
{
  qDebug() << "[QNST]" << ":" << "Selecting entity '"+uid+"'";

  if (entities.contains(uid))
  {
    StructuralGraphicsEntity* entity = entities[uid];

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

void StructuralView::addImportBase(const QString &uid,
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


void StructuralView::changeImportBase(const QString &uid,
                                const QMap<QString, QString> &properties)
{
  foreach(StructuralConnector* cc, connectors.values())
  {
    if (cc->getnstUid() == importBases[uid])
      connectors.remove(cc->getName());
  }

  foreach(StructuralConnector* cc, connectors2.values())
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

void StructuralView::readImportBase(const QString &uid,
                              QDomElement element,
                              const QString &alias)
{
  if (element.tagName() == "causalConnector")
  {
    QDomNodeList list = element.childNodes();

    StructuralConnector* conn = new StructuralConnector();
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

void StructuralView::readConnector(QDomElement element, StructuralConnector* conn)
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
    {
      QString UID = QUuid::createUuid().toString();

      conn->addCondition(UID, element.attribute("role"));

      QDomNamedNodeMap attributes = element.attributes();

      for (int i=0;i<attributes.size();i++)
      {
        QString name = attributes.item(i).toAttr().name();
        QString value = attributes.item(i).toAttr().value();

        if (value.contains("$"))
        {
          conn->addConditionParam(UID,name,value.replace('$',""));
        }
      }
    }
  }

  if (element.tagName() == "simpleAction")
  {
    if (element.attribute("role") != "")
    {
      QString UID = QUuid::createUuid().toString();

      conn->addAction(UID, element.attribute("role"));

      QDomNamedNodeMap attributes = element.attributes();

      for (int i=0;i<attributes.size();i++)
      {
        QString name = attributes.item(i).toAttr().name();
        QString value = attributes.item(i).toAttr().value();

        if (value.contains("$"))
        {
          conn->addActionParam(UID,name,value.replace('$',""));
        }
      }
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

void StructuralView::adjustEntity(StructuralGraphicsEntity *entity)
{
  QnstType type = entity->getnstType();
  entity->adjust();

  switch(type)
  {
    case Structural::Link:
    {
      adjustLink((StructuralGraphicsLink*)link);
      break;
    }

    case Structural::Media:
    {
        qDebug() << "======================== ajusta media" ;
      adjustMedia((StructuralGraphicsMedia*)entity);
      break;
    }

    case Structural::Mapping:
    {
      adjustMapping((StructuralGraphicsMapping*)entity);
      break;
    }

    case Structural::Area:
    case Structural::Property:
    {
      StructuralGraphicsEntity *entityParent = entity->getnstGraphicsParent();

      if(entityParent->isMedia())
        adjustMedia((StructuralGraphicsMedia*) entityParent);

      // Update the medias that refer to this one.
      foreach (QString key, refers.keys(entityParent->getnstUid()))
      {
        if (entities.contains(key) && entities[key]->isMedia())
          adjustMedia((StructuralGraphicsMedia*) entities[key]);
      }
      break;
    }

    default:
      // do nothing?
      break;
  }
}

void StructuralView::adjustContext(StructuralGraphicsContext* entity)
{
  Q_UNUSED(entity)
}

// \todo refactory
void StructuralView::adjustMedia(StructuralGraphicsMedia* entity)
{
  QString referUID = entity->getReferUID();
  QString refer = entity->getRefer();

  if (!refer.isEmpty() && !referUID.isEmpty())
  {
    if (entities.contains(referUID)) // media already refer to other media
    {
      StructuralGraphicsMedia* origin =
          dynamic_cast<StructuralGraphicsMedia*>(entities[referUID]);

      if (origin)
      {
        entity->setnstType(origin->getnstType());
        entity->setSource(origin->getSource());

        // cleaning interface without origin
        foreach(StructuralGraphicsEntity* rinterface,
                entity->getnstGraphicsEntities())
        {
          if (rinterface->getncgType() == Qncg::Interface)
          {
            if (refers.contains(rinterface->getnstUid()))
            {
              bool validReference = false;

              foreach(StructuralGraphicsEntity* interface,
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
        foreach (StructuralGraphicsEntity* child, origin->getnstGraphicsEntities())
        {
          bool hasRefer = false;
          StructuralGraphicsInterface *interface = dynamic_cast <StructuralGraphicsInterface*> (child);

          if(interface && !interface->isRefer())
          {
            foreach(StructuralGraphicsEntity* rinterface,
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
              StructuralGraphicsInterface* copy = (StructuralGraphicsInterface*)
                  StructuralUtil::makeGraphicsEntity(interface->getnstType(), entity);

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
          foreach(StructuralGraphicsEntity* interface,
                    origin->getnstGraphicsEntities())
          {
            if (interface->getncgType() == Qncg::Interface)
            {
              if (refers.contains(interface->getnstUid()))
              {
                bool validReference = false;

                foreach(StructuralGraphicsEntity* rinterface,
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

          foreach(StructuralGraphicsEntity* rinterface,
                  entity->getnstGraphicsEntities())
          {
            if (!refers.contains(rinterface->getnstUid()))
            {
              bool containsRef = false;

              foreach(StructuralGraphicsEntity* interface,
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
                StructuralGraphicsInterface* copy = (StructuralGraphicsInterface*)
                    StructuralUtil::makeGraphicsEntity(rinterface->getnstType(), origin);

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
          foreach(StructuralGraphicsEntity* interface, origin->getnstGraphicsEntities())
          {
            if (refers.contains(interface->getnstUid()))
            {
              bool containsRef = false;

              foreach(StructuralGraphicsEntity* rinterface,
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
      foreach(StructuralGraphicsEntity* interface, entity->getnstGraphicsEntities())
      {
        StructuralGraphicsInterface* i =
            dynamic_cast<StructuralGraphicsInterface*>(interface);

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

      entity->setnstType(Structural::Media);
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
      foreach(StructuralGraphicsEntity* interface, entity->getnstGraphicsEntities())
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
                StructuralGraphicsEntity* e = entities.value(rUid);
                StructuralGraphicsEntity* p = e->getnstGraphicsParent();

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
      if (refers.keys(entity->getnstUid()).size() == 0)
      {
        foreach(StructuralGraphicsEntity* interface, entity->getnstGraphicsEntities())
        {
          StructuralGraphicsInterface* i =
              dynamic_cast<StructuralGraphicsInterface*>(interface);

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
      entity->setnstType(Structural::Media);
      entity->setSource("");

      refers.remove(entity->getnstUid());
    }
  }

  entity->adjust();
}

// \todo refactory
void StructuralView::adjustPort(StructuralGraphicsPort* entity)
{
  StructuralGraphicsEntity* parent = entity->getnstGraphicsParent();

  if (parent != NULL)
  {
    // removing previous edge
    foreach(StructuralGraphicsEntity* e, parent->getnstGraphicsEntities())
    {
      if (e->getnstType() == Structural::Reference)
      {
        StructuralGraphicsEdge* edge = (StructuralGraphicsEdge*) e;

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
        StructuralGraphicsEntity* entitya = (StructuralGraphicsEntity*) entity;
        StructuralGraphicsEntity* entityb = (StructuralGraphicsEntity*) entities[entity->getInterfaceUid()];

        StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL)
        {
          if (entitya != entityb && parenta == parentb->getnstGraphicsParent())
          {
              StructuralGraphicsReference* edge = new StructuralGraphicsReference();
              edge->setEntityA(entitya);
              edge->setEntityB(entityb);
              edge->adjust();

              parenta->addnstGraphicsEntity(edge);

              ((StructuralGraphicsInterface*) entitya)->addnstGraphicsEdge(edge);
              ((StructuralGraphicsInterface*) entityb)->addnstGraphicsEdge(edge);
          }
        }
      }
    }
    else if (entity->getComponentUid() != "")
    {
      // changing
      if (entities.contains(entity->getComponentUid()))
      {
        StructuralGraphicsEntity* entitya = (StructuralGraphicsEntity*) entity;
        StructuralGraphicsEntity* entityb = (StructuralGraphicsEntity*) entities[entity->getComponentUid()];

        assert (entitya != NULL);
        assert (entityb != NULL);

        StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL)
        {
          if (entitya != entityb && parenta == parentb)
          {
            StructuralGraphicsReference* edge = new StructuralGraphicsReference();
            edge->setEntityA(entitya);
            edge->setEntityB(entityb);
            edge->adjust();

            parenta->addnstGraphicsEntity(edge);

            ((StructuralGraphicsInterface*) entitya)->addnstGraphicsEdge(edge);
            ((StructuralGraphicsNode*) entityb)->addnstGraphicsEdge(edge);
          }
        }
      }
    }
  }

  entity->adjust();
}

// \todo refactory
void StructuralView::adjustMapping(StructuralGraphicsMapping* entity)
{
  if (entities.contains(entity->getComponentUid()) &&
      entities.contains(entity->getSwitchPortUid()))
  {
    StructuralGraphicsEdge* edge = (StructuralGraphicsEdge*) entity;

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
      StructuralGraphicsEntity* parent =
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

      ((StructuralGraphicsInterface*) edge->getEntityA())->addnstGraphicsEdge(entity);
      ((StructuralGraphicsNode*) edge->getEntityB())->addnstGraphicsEdge(entity);

      edge->adjust();
    }
  }
  // entities does not contains componentUID or switchPortUID
  else
  {
    StructuralGraphicsEdge* edge = (StructuralGraphicsEdge*) entity;

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

    StructuralGraphicsEntity* parent = edge->getnstGraphicsParent();
    parent->removenstGraphicsEntity(edge);
  }
}

void StructuralView::changeLink(StructuralGraphicsLink* entity,
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

void StructuralView::adjustLink(StructuralGraphicsLink* entity)
{
  Q_UNUSED(entity)
  // TODO
}

void StructuralView::adjustBind(StructuralGraphicsBind* bind)
{
  StructuralGraphicsLink *link = NULL;
  StructuralGraphicsEntity *target = NULL;

  link = bind->getLink();

  // set the connector of bind
  if(connectors.contains(link->getxConnector()))
    bind->setConn(connectors[link->getxConnector()]);

  if(bind->getInterfaceUid() != "" && entities.contains(bind->getInterfaceUid()))
    target =  entities[bind->getInterfaceUid()];
  else if(entities.contains(bind->getComponentUid()))
    target = entities[bind->getComponentUid()];

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
    StructuralConnector *connector = connectors[link->getxConnector()];
    if(!connector->getConditions().values().contains(bind->getRole()) &&
       !connector->getActions().values().contains(bind->getRole()))
    {
      bind->setInvalid(true);
    }
    else
      bind->setInvalid(false);
  }
  else
  {
    bind->setInvalid(true);
  }

  bind->adjust();
  // \todo interface
}

void StructuralView::addConnector(const QString &uid,
                            const QMap<QString, QString> &properties)
{
  if (!connectors2.contains(uid))
  {
    StructuralConnector* entity = new StructuralConnector();
    entity->setnstUid(uid);

    if (properties["id"] != "")
    {
      entity->setName(properties["id"]);
      entity->setnstId(properties["id"]);
    }

    connectors2[uid] = entity;

    foreach(StructuralConnector* c, connectors.values())
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

void StructuralView::adjustAll()
{
  traceConnectors();
  traceConnectors2();

  foreach(StructuralGraphicsEntity *entity, entities.values())
  {
    switch(entity->getnstType())
    {
      case Structural::Action:
      case Structural::Condition:
      case Structural::Bind:
      {
        StructuralGraphicsBind *bind = dynamic_cast <StructuralGraphicsBind *>(entity);
        if(bind)
          adjustBind(bind);
        break;
      }
      // if the entity type is MEDIA
      case Structural::Audio:
      case Structural::Text:
      case Structural::Video:
      case Structural::Image:
      case Structural::NCLua:
      case Structural::Settings:
      case Structural::Media:
      case Structural::Html:
      case Structural::NCL:
      {
        StructuralGraphicsMedia *media = dynamic_cast <StructuralGraphicsMedia *>(entity);
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

void StructuralView::changeConnector(StructuralConnector* entity,
                               const QMap<QString, QString> &properties)
{
  qWarning() << "QnstView::changeConnector" << entity << properties;
  if (properties["id"] != "")
  {
    entity->setName(properties["id"]);
    entity->setnstId(properties["id"]);
  }

  foreach(StructuralConnector* c, connectors.values())
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

void StructuralView::addCondition(const QString &uid,
                            const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    StructuralConnector* connector = connectors2[properties["connector"]];

    if (properties["role"] != "")
      connector->addCondition(uid, properties["role"]);

    foreach(QString key, properties.keys())
    {
      if (key.startsWith("value"))
      {
        QString num = key.replace("value","");

        connector->addConditionParam(uid, properties.value("attr"+num), properties.value("value"+num));
      }
    }
  }
  else
    qWarning() << "[QNST] Trying yo add a condition to a connector that does not exist.";
}

void StructuralView::changeCondition(const QString &uid,
                               const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    StructuralConnector* connector = connectors2[properties["connector"]];

    if (connector->getConditions().contains(uid))
      connector->removeCondition(uid);

    if (properties["role"] != "")
    {
      qWarning() << "CHANGING CONDITION" << properties["role"];
      connector->addCondition(uid, properties["role"]);

      foreach(QString key, properties.keys())
      {
        if (key.startsWith("param"))
        {
          QString num = key.replace("param","");

          connector->addConditionParam(uid, properties.value("attr"+num), properties.value(key));
        }
      }
    }
  }
  else
    qWarning() << "[QNST] Trying yo change a condition in a connector that does not exist.";
}

void StructuralView::addAction(const QString &uid,
                         const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    StructuralConnector* connector = connectors2[properties["connector"]];

    if (properties["role"] != "")
      connector->addAction(uid, properties["role"]);

    foreach(QString key, properties.keys())
    {
      if (key.startsWith("value"))
      {
        QString num = key.replace("value","");

        connector->addActionParam(uid, properties.value("attr"+num), properties.value("value"+num));
      }
    }
  }
}

void StructuralView::changeAction(const QString &uid,
                            const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["connector"]))
  {
    StructuralConnector* connector = connectors2[properties["connector"]];

    if (connector->getActions().contains(uid))
      connector->getActions().remove(uid);

    if (properties["role"] != "")
      connector->addAction(uid, properties["role"]);
  }
}

void StructuralView::changeBindParam(const QString &uid,
                               const QMap<QString, QString> &properties)
{
  Q_UNUSED(uid)
  if (entities.contains(properties.value("parent")))
  {
    StructuralGraphicsEntity* e = entities[properties.value("parent")];

    StructuralGraphicsBind* bind = (StructuralGraphicsBind*) e;
    bind->setParam(properties.value("name",""), properties.value("value",""));
  }
}

void StructuralView::addConnectorParam(const QString &uid,
                                 const QString &parent,
                                 const QMap<QString, QString> &properties)
{
  qDebug();
  qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << properties;
  qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << parent;
  qDebug();

  if (connectors2.contains(parent))
  {
    qDebug();
    qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% EXISTE";
    qDebug();

    StructuralConnector* conn = connectors2[parent];
    conn->addParam(uid, properties["name"]);
  }
}

void StructuralView::changeConnectorParam(const QString &uid,
                                    const QMap<QString, QString> &properties)
{
  if (connectors2.contains(properties["parent"]))
  {
    StructuralConnector* conn = connectors2[properties["parent"]];
    conn->addParam(uid, properties["name"]);
  }
}

void StructuralView::requestEntityAddition(StructuralGraphicsEntity* entity, bool undo)
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


    // \fixme Should this come only after the switch/case ?
    entity->getProperties(properties);

    switch(entity->getnstType())
    {
      case Structural::Body:
      {
        if (undo){  scene->addRoot(entity); }

        properties["TYPE"] = "body";
        ok = true;
        break;
      }

      case Structural::Context:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        properties["TYPE"] = "context";
        properties["id"] = entity->getnstId();

        ok = true;
        break;
      }

      case Structural::Switch:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        properties["TYPE"] = "switch";
        properties["id"] = entity->getnstId();
        ok = true;
        break;
      }

      // if the entity type is MEDIA
      case Structural::Audio:
      case Structural::Text:
      case Structural::Video:
      case Structural::Image:
      case Structural::NCLua:
      case Structural::Settings:
      case Structural::Html:
      case Structural::NCL:
      case Structural::Media:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        StructuralGraphicsMedia *media = dynamic_cast<StructuralGraphicsMedia *>(entity);

        if(media)
        {
          properties["TYPE"] = "media";
          properties["id"] = entity->getnstId();

          ok = true;
        }
        break;
      }

      case Structural::Port:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        StructuralGraphicsPort *port = dynamic_cast <StructuralGraphicsPort *> (entity);

        properties["TYPE"] = "port";
        properties["id"] = entity->getnstId();

        // \fixme Do I really need the following code?
        if(port)
        {
          foreach(StructuralGraphicsEdge* edge, port->getnstGraphicsEdges())
          {
            if (edge->getnstType() == Structural::Reference)
            {
              properties["COMPONENT"] = edge->getEntityA()->getnstUid();
              properties["component"] = edge->getEntityA()->getnstId();

              if (edge->getEntityB()->getnstType() == Structural::Port ||
                  edge->getEntityB()->getnstType() == Structural::Property ||
                  edge->getEntityB()->getnstType() == Structural::Area)
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

      case Structural::Area:
      {
        if (entity->getnstId() == "" && !undo)
          updateEntityWithUniqueNstId(entity);

        properties["TYPE"] = "area";
        properties["id"] = entity->getnstId();

        // \fixme Do I really need the following code?
        if(entity->getnstGraphicsParent()->isMedia())
        {
          StructuralGraphicsMedia *media =
            dynamic_cast <StructuralGraphicsMedia *>(entity->getnstGraphicsParent());

          if(media != NULL)
            adjustMedia(media);
        }

        foreach (QString key,
                 refers.keys(entity->getnstGraphicsParent()->getnstUid()))
        {
          if (entities.contains(key))
          {
            if (entities[key]->isMedia())
              adjustMedia((StructuralGraphicsMedia*) entities[key]);
          }
        }

        ok = true;
        break;
      }

      case Structural::Property:
      {
        properties["TYPE"] = "property";

        // \fixme Do I really need the following code?
        if(entity->getnstGraphicsParent()->isMedia())
        {
          StructuralGraphicsMedia *media =
            dynamic_cast <StructuralGraphicsMedia *>(entity->getnstGraphicsParent());

          if(media != NULL)
            adjustMedia(media);
        }

        foreach (QString key,
                 refers.keys(entity->getnstGraphicsParent()->getnstUid()))
        {
          if (entities.contains(key))
          {
            if (entities[key]->isMedia())
              adjustMedia((StructuralGraphicsMedia*) entities[key]);
          }
        }

        ok = true;
        break;
      }

      case Structural::Aggregator:
      {
        qWarning() << "Trying to add an Aggregator!!! This is not supported \
                      anymore!!";

        // \fixme This should be deprecated!!
        // entities[entity->getnstUid()] = entity;
        break;
      }

      case Structural::Link:
      {
        StructuralGraphicsLink *link = dynamic_cast<StructuralGraphicsLink*>(entity);

        // \fixme Do I really need the following code?
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

      case Structural::Bind:
      case Structural::Condition:
      case Structural::Action:
      {
        if (!binds.contains(entity->getnstUid()))
        {
          StructuralGraphicsBind* bind = dynamic_cast <StructuralGraphicsBind*>(entity);
          if(bind)
          {
            properties["TYPE"] = "bind";

            // \fixme Do I really need the following code?
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
                  StructuralUtil::getStrFromBindType(bind->getType());
              properties["condition"] = bind->getRole();
            }
            else
            {
              properties["action"] = bind->getRole();
              properties["role"] =
                  StructuralUtil::getStrFromBindType(bind->getType());
            }

            properties["component"] = bind->getComponent();
            properties["interface"] = bind->getInterface();

            binds[bind->getnstUid()] = bind;

            ok = true;
          }
        }
        break;
      }

      case Structural::SwitchPort:
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
      entity->updateToolTip();

      if(entity->getnstType() == Structural::Body)
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
        Insert* cmd = new Insert(this, entity);
        history.push(cmd);
      }
    }
  }
}

// \todo refactory
void StructuralView::requestEntityRemotion(StructuralGraphicsEntity* entity,
                                           bool undo,
                                           bool rmRefs,
                                           bool notify)
{
  Q_UNUSED(rmRefs)

  qDebug() << "[QNST]" << ":" << "Requesting entity remotion '" +
              entity->getnstUid() + "'" +
              entity->getnstType() + "=" +
              StructuralUtil::getStrFromNstType(entity->getnstType());

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
        Remove* cmd = new Remove(this, entity);
        history.push(cmd);
      }
    }

    QString UID = entity->getnstUid();

    switch (qncgType)
    {
      case Qncg::Node:
      {
        StructuralGraphicsNode *node = dynamic_cast <StructuralGraphicsNode*>(entity);

        if(node)
          removeEdgesOfEntity(node, false); // Remove the edges of this entity
        else
          qWarning() << "Could not make a cast to QnstGraphicsNode*";

        // Remove recursively
        foreach(StructuralGraphicsEntity* e, entity->getnstGraphicsEntities())
        {
          if (e->getnstType() != Structural::Edge &&
              e->getnstType() != Structural::Condition &&
              e->getnstType() != Structural::Mapping &&
              e->getnstType() != Structural::Action &&
              e->getnstType() != Structural::Reference)
          {
            requestEntityRemotion(e, true, false);
          }
        }

        mustRemoveFromParent = true;
        break;
      }

      case Qncg::Interface:
      {
        StructuralGraphicsInterface *interface =
            dynamic_cast <StructuralGraphicsInterface*>(entity);

        if(interface)
          removeEdgesOfEntity(interface, false); // Remove the edges of this entity
        else
          qWarning() << "Could not make a cast to QnstGraphicsInterface*";

        mustRemoveFromParent = true;
        break;
      }

      case Qncg::Edge:
      {
        StructuralGraphicsEdge *edge = dynamic_cast<StructuralGraphicsEdge *>(entity);

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
      StructuralGraphicsEntity* parent = entity->getnstGraphicsParent();
      if (parent != NULL)
        parent->removenstGraphicsEntity(entity);
      else
        scene->removeRoot(entity);

      if (selected == entity)
        selected = NULL;

      if (clipboard == entity)
        clipboard = NULL;

      if (entity->getnstType() == Structural::Link)
      {
        if (links.contains(entity->getnstUid()))
        {
          links.remove(entity->getnstUid());
        }
      }

      entities.remove(entity->getnstUid());

      if (entity->getnstType() == Structural::Link)
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
            adjustMedia((StructuralGraphicsMedia*)parent);

            foreach(QString rUid, refers.keys(parent->getnstUid()))
            {
              if (entities.contains(rUid))
              {
                StructuralGraphicsEntity* e = entities.value(rUid);

                if (e->isMedia())
                  adjustMedia((StructuralGraphicsMedia*)e);
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
            StructuralGraphicsEntity* e = entities.value(ruid);

            if (e->isMedia())
            {
              qDebug() << "=================== un, hello!";
              adjustMedia((StructuralGraphicsMedia*) e);
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

void StructuralView::removeEdgesOfEntity(QnstGraphicsEntityWithEdges *entity,
                                   bool notify)
{
  foreach(StructuralGraphicsEdge* edge, entity->getnstGraphicsEdges())
  {
    requestEntityRemotion(edge, false, false, notify);
  }
}

void StructuralView::removeEdge(StructuralGraphicsEdge *edge, bool notify)
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

  if (edge->getnstType() == Structural::Condition ||
      edge->getnstType() == Structural::Action ||
      edge->getnstType() == Structural::Bind)
  {
    StructuralGraphicsBind* bb = binds[edge->getnstUid()];

    if (bb != NULL)
    {
      StructuralGraphicsLink* ll = (StructuralGraphicsLink*) bb->getLink();
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
  else if (edge->getnstType() == Structural::Reference)
  {
    StructuralGraphicsPort* p = (StructuralGraphicsPort*) edge->getEntityA();
    p->setComponent("");
    p->setComponentUid("");
    p->setInterface("");
    p->setInterfaceUid("");
    // requestEntityChange(p);
  }
  else if (edge->getnstType() == Structural::Mapping)
  {
    // Do nothing ??
    // QnstGraphicsEntity* parent = edge->getnstGraphicsParent();
    // parent->removenstGraphicsEntity(edge);
    // }

    // entities.remove(edge->getnstUid());
    // emit entityRemoved(edge->getnstUid());
  }

  StructuralGraphicsEntity* parent = edge->getnstGraphicsParent();
  if (parent)
  {
    parent->removenstGraphicsEntity(edge);
    entities.remove(edge->getnstUid());
  }

  if (edge->getnstType() != Structural::Reference)
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

void StructuralView::requestEntityPreparation(StructuralGraphicsEntity* entity,
                                        QMap<QString, QString> properties)
{
  Change* cmd = new Change(this, entity->getnstUid(),
                                                 properties);
  history.push(cmd);

  emit viewChanged();
}

void StructuralView::requestEntityChange(StructuralGraphicsEntity* entity)
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

    entity->getProperties(properties);

    switch(entity->getnstType())
    {
      case Structural::Body:
      {
        properties["TYPE"] = "body";
        ok = true;
        break;
      }

      case Structural::Context:
      {
        properties["TYPE"] = "context";
        ok = true;
        break;
      }

      case Structural::Switch:
      {
        properties["TYPE"] = "switch";
        ok = true;
        break;
      }

      // if the entity type is MEDIA
      case Structural::Audio:
      case Structural::Text:
      case Structural::Video:
      case Structural::Image:
      case Structural::NCLua:
      case Structural::Settings:
      case Structural::Html:
      case Structural::NCL:
      case Structural::Media:
      {
        properties["TYPE"] = "media";
        ok = true;
        break;
      }

      // if the entity type is PORT
      case Structural::Port:
      {
        properties["TYPE"] = "port";

        //\fixme DO I really need that?
        StructuralGraphicsPort *port = dynamic_cast <StructuralGraphicsPort *>  (entity);
        foreach(StructuralGraphicsEdge* edge, port->getnstGraphicsEdges())
        {
          if (edge->getnstType() == Structural::Reference && edge->getEntityA() == entity)
          {
            if (edge->getEntityB()->getnstType() == Structural::Port ||
                edge->getEntityB()->getnstType() == Structural::Property ||
                edge->getEntityB()->getnstType() == Structural::Area)
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

      case Structural::Link:
      {
        properties["TYPE"] = "link";
        ok = true;
        break;
      }

      case Structural::Area:
      case Structural::Property:
      case Structural::Aggregator:
      {
        // do nothing ??
        break;
      }

      case Structural::SwitchPort:
      {
        properties["TYPE"] = "switchPort";
        ok = true;
        break;
      }

      case Structural::BindParam:
      {
        // \todo
      }

      default:
        // do nothing
        break;
    }

    if(ok)
      {
      qDebug() << "======== PROPERTIES: " << properties;

      entity->updateToolTip();

      emit entityChanged(entity->getnstUid(), properties);
    }

    // Undo ??
  }
}

void StructuralView::requestEntitySelection(StructuralGraphicsEntity* entity)
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

void StructuralView::performHelp()
{
    // TODO
}

/*void QnstView::performUndo()
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
}*/

void StructuralView::performCut()
{
  if (selected != NULL)
  {
    if (selected->getnstType() != Structural::Body)
    {
      if (clipboard != NULL)
      {
        delete clipboard; clipboard = NULL;
      }

      StructuralGraphicsEntity* entity = selected;

      clipboard = StructuralUtil::makeGraphicsEntity(entity->getnstType());

      if (clipboard != NULL)
      {
        clipboard->setnstId(entity->getnstId());

        clipboard->setTop(entity->getTop());
        clipboard->setLeft(entity->getLeft());
        clipboard->setWidth(entity->getWidth());
        clipboard->setHeight(entity->getHeight());
        clipboard->setnstGraphicsParent(NULL);

        clipboard->setUsrData(entity->getUsrData());

        foreach(StructuralGraphicsEntity* e, entity->getnstGraphicsEntities())
        {
          performCopy(e, clipboard);
        }

        requestEntityRemotion(selected);

        foreach(StructuralGraphicsEntity* e, entities.values())
        {
          if (e->getnstType() == Structural::Body ||
              e->getnstType() == Structural::Context ||
              e->getnstType() == Structural::Switch)
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

void StructuralView::performCopy()
{
  if (selected != NULL)
  {
    if (selected->getnstType() != Structural::Body)
    {
      if (clipboard != NULL)
      {
        delete clipboard; clipboard = NULL;
      }

      StructuralGraphicsEntity* entity = selected;
      clipboard = StructuralUtil::makeGraphicsEntity(entity->getnstType());

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
        foreach(StructuralGraphicsEntity* e, entity->getnstGraphicsEntities())
        {
          performCopy(e, clipboard);
        }

        foreach(StructuralGraphicsEntity* e, entities.values())
        {
          if (e->getnstType() == Structural::Body ||
              e->getnstType() == Structural::Context ||
              e->getnstType() == Structural::Switch)
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

void StructuralView::performCopy(StructuralGraphicsEntity* entity,
                           StructuralGraphicsEntity* parent)
{
  StructuralGraphicsEntity* copy = NULL;

  copy = StructuralUtil::makeGraphicsEntity(entity->getnstType());

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

    foreach(StructuralGraphicsEntity* e, entity->getnstGraphicsEntities())
    {
      performCopy(e, copy);
    }
  }
}

void StructuralView::performPaste()
{
  if (clipboard != NULL)
  {
    StructuralGraphicsEntity* copy = clipboard;
    qDebug() << "[QNST] performPaste() cliboard userdata = "
             << clipboard->getUsrData();

    StructuralGraphicsEntity* parent = selected;

    if (copy != NULL && parent != NULL)
    {
      if (parent->getnstType() == Structural::Context ||
          parent->getnstType() == Structural::Switch ||
          parent->getnstType() == Structural::Body)
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

bool StructuralView::updateEntityWithUniqueNstId(StructuralGraphicsEntity *entity)
{
  if(entity != NULL)
  {
    int n = 0;
    Structural::EntityType type;

    // for any media, we will consider the same identifier prefix
    if(entity->isMedia())
      type = Structural::Media;
    else
      type = entity->getnstType();

    // check if type is in the map
    if(!entityCounter.count(type))
      entityCounter[type] = 0;

    // change this check in future
    QString prefix = StructuralUtil::getPrefixIdFromType(type);

    while(1) //
    {
      bool match = false;

      foreach(StructuralGraphicsEntity* entity, entities.values())
      {
        if (entity->getnstId() == prefix + QString::number(n))
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

    entity->setnstId(StructuralUtil::getPrefixIdFromType(type) +
                     QString::number(n));

    return true;
  }

  return false;
}

void StructuralView::performPaste(StructuralGraphicsEntity* copy,
                            StructuralGraphicsEntity* parent,
                            bool makeRefer, bool keepOriginalPos)
{
  StructuralGraphicsEntity* entity = StructuralUtil::makeGraphicsEntity(copy->getnstType(),
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
      ((StructuralGraphicsMedia*) entity)->setSource(copy->getUsrData()["src"]);
    }

    parent->addnstGraphicsEntity(entity);

    entity->adjust();
    entity->setUsrData(copy->getUsrData());

    requestEntityAddition(entity);

    if(entity->isMedia())
    {
      adjustMedia((StructuralGraphicsMedia*) entity);
    }

    if (!makeRefer)
    {
      foreach(StructuralGraphicsEntity* e, copy->getnstGraphicsEntities())
      {
        performPaste(e, entity, makeRefer, true);
      }
    }
  }
}

void StructuralView::performReference(StructuralGraphicsEntity* entity,
                                StructuralGraphicsEntity* referred)
{
  if(entity != NULL)
  {
    // \fixme We should copy all properties
    if(entity->isMedia())
    {
      //Reference
      ((StructuralGraphicsMedia*) entity)->setInstance("instSame");
      ((StructuralGraphicsMedia*) entity)->setRefer(((StructuralGraphicsMedia*) referred)->getnstId());
      ((StructuralGraphicsMedia*) entity)->setReferUID(((StructuralGraphicsMedia*) referred)->getnstUid());

      adjustMedia((StructuralGraphicsMedia*) entity);
    }

    // \todo Refer for context and switch
  }
}

void StructuralView::performDelete()
{
  if (selected != NULL)
  {
    if(selected->getncgType() == Qncg::Interface &&
       ((StructuralGraphicsInterface*)selected)->isRefer())
    {
      // ignore
    }
    else
    {
      bool mustRemove = true;
      if(selected->getnstType() == Structural::Context ||
         selected->getnstType() == Structural::Body ||
         selected->getnstType() == Structural::Switch)
      {
        mustRemove = false;
        QString strNstType =
            StructuralUtil::getStrFromNstType(selected->getnstType());
        QMessageBox mBox ( QMessageBox::Question,
                           tr("Removing %1 ...").arg(strNstType),
                           tr("Are you sure you want to remove the"
                              " %1 element and all its children?").
                              arg(strNstType),
                           (QMessageBox::Yes | QMessageBox::No),
                           this);
        mBox.setDefaultButton(QMessageBox::No);
        //mBox.setMinimumWidth(300);

        if (mBox.exec() == QMessageBox::Yes)
        {
          mustRemove = true;
        }
      }

      if(mustRemove)
        requestEntityRemotion(selected);
    }

    scene->update();
  }
}

void StructuralView::performExport()
{
  QString location = QFileDialog::getSaveFileName(NULL, "Export...", "", tr("Images (*.png)"));

  if (location != ""){
    qreal top = 0;
    qreal left = 0;
    qreal width = scene->width();
    qreal height = scene->height();

    foreach (StructuralGraphicsEntity* entity, scene->getRoots())
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
  minimap->move(event->size().width() - minimap->width(),
                event->size().height() - minimap->height());

  QGraphicsView::resizeEvent(event);
}

void StructuralView::setMinimapVisible(bool v)
{
  if(v)
    minimap->show();
  else
    minimap->hide();
}

void StructuralView::mouseMoveEvent(QMouseEvent* event)
{
  if (linking)
  {
    if (lastLinkMouseOver != NULL)
    {
      lastLinkMouseOver->setMouseHover(false);

      // \fixme This is not the best place to control this!!
      if(lastLinkMouseOver->getnstType() == Structural::Composition ||
         lastLinkMouseOver->getnstType() == Structural::Context ||
         lastLinkMouseOver->getnstType() == Structural::Switch)
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
      StructuralGraphicsEntity* entitya = (StructuralGraphicsEntity*) itemsa.first();
      entitya->setMouseHover(true);
    }

    QList<QGraphicsItem*> itemsb = scene->items(link->getLine().p2());

    if (itemsb.count() && itemsb.first() == link)
    {
      itemsb.removeFirst();
    }

    if (itemsb.count())
    {
      StructuralGraphicsEntity* entityb = (StructuralGraphicsEntity*) itemsb.first();
      entityb->setMouseHover(true);

      // \fixme This is not the best place to control this!!
      if(entityb->getnstType() == Structural::Composition ||
         entityb->getnstType() == Structural::Context ||
         entityb->getnstType() == Structural::Switch)
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
      StructuralGraphicsEntity* entitya = (StructuralGraphicsEntity*) itemsa.first();
      StructuralGraphicsEntity* entityb = (StructuralGraphicsEntity*) itemsb.first();

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

      // \fixme This is not the best place to control this!!
      if(entityb->getnstType() == Structural::Composition ||
         entityb->getnstType() == Structural::Context ||
         entityb->getnstType() == Structural::Switch)
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

  StructuralGraphicsEntity *entity;
  foreach(entity, entities.values())
  {
    assert(entity != NULL);
    entity->setDraggable(false);
  }

  QGraphicsView::mouseReleaseEvent(event);
}

void StructuralView::addNodetoNodeEdge( StructuralGraphicsEntity* entitya,
                                        StructuralGraphicsEntity* entityb)
{
  StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if ((LINK_WITH_PARENT && ((parenta != NULL) || (parentb != NULL)) ) ||
      ((parenta != NULL) && (parentb != NULL)))
  {
    if ((parenta == parentb) || (parenta == entityb) || (parentb == entitya))
    {
      // aggregator -> node
      if (entitya->getnstType() == Structural::Link)
      {
        createActionWithDialog(entitya, entityb);
      }
      // node -> aggregator
      else if (entityb->getnstType() == Structural::Link)
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

void StructuralView::addNodetoInterfaceEdge(StructuralGraphicsEntity* entitya,
                                      StructuralGraphicsEntity* entityb)
{
  StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if ((LINK_WITH_PARENT && ((parenta != NULL) || (parentb != NULL))) ||
      ((parenta != NULL) && (parentb != NULL)))
  {
    if ((LINK_WITH_PARENT && (parenta == parentb)) ||
        (parenta == parentb->getnstGraphicsParent()))
    {
      // aggregator -> interface
      if (entitya->getnstType() == Structural::Link)
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

void StructuralView::addInterfacetoNodeLink(StructuralGraphicsEntity* entitya,
                                      StructuralGraphicsEntity* entityb)
{
  // interface -> aggreagator
  if (entityb->getnstType() == Structural::Link)
  {
    createConditionWithDialog(entitya, entityb);
  }
  // interface -> node
  else
  {
    createLinkWithDialog(entitya, entityb);
  }
}

void StructuralView::addInterfacetoNodeEdge(StructuralGraphicsEntity* entitya,
                                      StructuralGraphicsEntity* entityb)
{
  StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if ((parenta != NULL) && (parentb != NULL))
  {
    if (parenta == parentb)
    {
      if (entitya->getnstType() == Structural::Port)
      {
        createReference(entitya, entityb);
      }
      else if (entitya->getnstType() == Structural::SwitchPort)
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

void StructuralView::addInterfacetoInterfaceEdge(StructuralGraphicsEntity* entitya,
                                           StructuralGraphicsEntity* entityb)
{
  StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {
    if (parenta == parentb->getnstGraphicsParent())
    {
      if (entitya->getnstType() == Structural::Port)
      {
        createReference(entitya, entityb);
      }
      else if (entitya->getnstType() == Structural::SwitchPort)
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

QString StructuralView::createNewConnector(QString condition, QString action)
{
  StructuralConnector* conn = new StructuralConnector();
  conn->setName("newConnector"+QString::number(connectors.size()));
  conn->addCondition(QUuid::createUuid().toString(), "onBegin");
  conn->addCondition(QUuid::createUuid().toString(), "onEnd");
  conn->addCondition(QUuid::createUuid().toString(), "onSelection");
  conn->addCondition(QUuid::createUuid().toString(), "onResume");
  conn->addCondition(QUuid::createUuid().toString(), "onPause");

  conn->addAction(QUuid::createUuid().toString(), "start");
  conn->addAction(QUuid::createUuid().toString(), "stop");
  conn->addAction(QUuid::createUuid().toString(), "resume");
  conn->addAction(QUuid::createUuid().toString(), "pause");
  conn->addAction(QUuid::createUuid().toString(), "set");

  connectors[conn->getName()] = conn;
  connectors2[conn->getnstUid()] = conn;

  QMap<QString, QString> properties;

  properties["TYPE"] = "complex-connector";

  properties["id"] = conn->getName();

  if (!action.isEmpty())
  {
    properties["action"] = action;

    QString UID = conn->getActions().key(action, QUuid::createUuid().toString());

    properties["actionUID"] = UID;
  }

  if (!condition.isEmpty())
  {
    properties["condition"] = condition;

    QString UID = conn->getConditions().key(action, QUuid::createUuid().toString());

    properties["conditionUID"] = UID;
  }

  emit entityAdded(conn->getnstUid(), "", properties);

  return conn->getName();
}

StructuralGraphicsLink* StructuralView::createLink(StructuralGraphicsEntity* entitya,
                                       StructuralGraphicsEntity* entityb,
                                       QString connName)
{
  StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (( (LINK_WITH_PARENT) && ((parenta != NULL) || (parentb != NULL))) ||
      ((parenta != NULL) && (parentb != NULL)))
  {
      StructuralGraphicsLink* linkDot = NULL;

      if ((LINK_WITH_PARENT) && (parenta == entityb))
      {
        StructuralGraphicsLink* linkDot = new StructuralGraphicsLink((StructuralGraphicsNode*) parenta);
        parenta->addnstGraphicsEntity(linkDot);

        linkDot->adjust();

        updateEntityWithUniqueNstId(linkDot); // set a unique Id

        linkDot->setxConnector(connName);
        if(connectors.contains(connName))
          linkDot->setxConnectorUID(connectors[connName]->getnstUid());

        requestEntityAddition(linkDot);

        return linkDot;
      }
      else if (LINK_WITH_PARENT && (parentb == entitya))
      {
        StructuralGraphicsLink* linkDot = new StructuralGraphicsLink((StructuralGraphicsNode*) parentb);
        parenta->addnstGraphicsEntity(linkDot);

        linkDot->adjust();

        updateEntityWithUniqueNstId(linkDot); // set a unique Id

        linkDot->setxConnector(connName);
        if(connectors.contains(connName))
          linkDot->setxConnectorUID(connectors[connName]->getnstUid());

        requestEntityAddition(linkDot);

        return linkDot;
      }
      else
      {
        qreal xcenter = -1;
        qreal ycenter = -1;

        QPointF pointa(entitya->getLeft() + entitya->getWidth()/2,
                       entitya->getTop() + entitya->getHeight()/2);

        QPointF pointb(entityb->getLeft() + entityb->getWidth()/2,
                       entityb->getTop() + entityb->getHeight()/2);

        if(entitya->getncgType() == Qncg::Interface &&
           entityb->getncgType() == Qncg::Interface)
        {
          StructuralGraphicsEntity* super = parenta->getnstGraphicsParent();

          linkDot = new StructuralGraphicsLink((StructuralGraphicsNode*) super);
          super->addnstGraphicsEntity(linkDot);

          pointa = super->mapFromItem(parenta, pointa);
          pointb = super->mapFromItem(parentb, pointb);
        }
        else if(entitya->getncgType()  == Qncg::Node &&
                entityb->getncgType() == Qncg::Interface)
        {
          linkDot = new StructuralGraphicsLink((StructuralGraphicsNode*) parenta);
          parenta->addnstGraphicsEntity(linkDot);

          pointb = parenta->mapFromItem(parentb, pointb);
        }
        else if(entitya->getncgType()  == Qncg::Interface &&
                entityb->getncgType() == Qncg::Node)
        {
          linkDot = new StructuralGraphicsLink((StructuralGraphicsNode*) parentb);
          parentb->addnstGraphicsEntity(linkDot);

          pointa = parentb->mapFromItem(parenta, pointa);

        }
        else
        {
          linkDot = new StructuralGraphicsLink((StructuralGraphicsNode*) parenta);
          parenta ->addnstGraphicsEntity(linkDot);
        }

        QLineF l(pointa, pointb);

        if (pointa.x() <= pointb.x() && pointa.y() <= pointb.y())
        {
          qreal angle = (l.angle() - 270);
          qreal rangle = angle*PI/180;

          if (angle > 45)
          {
            pointa.setX(pointa.x() + entitya->getWidth()/2);
            pointa.setY(pointa.y() + ((entitya->getWidth()/2)/tan(rangle)));

            pointb.setX(pointb.x() - entityb->getWidth()/2);
            pointb.setY(pointb. y() - ((entityb->getWidth()/2)/tan(rangle)));
          }
          else
          {
            pointa.setX(pointa.x() + ((entitya->getHeight()/2)*tan(rangle)));
            pointa.setY(pointa.y() + entitya->getHeight()/2);

            pointb.setX(pointb.x() - ((entityb->getHeight()/2)*tan(rangle)));
            pointb.setY(pointb.y() - entityb->getHeight()/2);
          }

          xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
          ycenter = pointa.y() + (pointb.y() - pointa.y())/2;
        }
        else if (pointa.x() > pointb.x() && pointa.y() < pointb.y())
        {
          qreal angle = (270 - l.angle());
          qreal rangle = angle*PI/180;

          if (angle > 45)
          {
            pointa.setX(pointa.x() - entitya->getWidth()/2);
            pointa.setY(pointa.y() + ((entitya->getWidth()/2)/tan(rangle)));

            pointb.setX(pointb.x() + entityb->getWidth()/2);
            pointb.setY(pointb.y() - ((entityb->getWidth()/2)/tan(rangle)));
          }
          else
          {
            pointa.setX(pointa.x() - ((entitya->getHeight()/2)*tan(rangle)));
            pointa.setY(pointa.y() + entitya->getHeight()/2);

            pointb.setX(pointb.x() + ((entityb->getHeight()/2)*tan(rangle)));
            pointb.setY(pointb.y() - entityb->getHeight()/2);
          }

          xcenter = pointb.x() + (pointa.x() - pointb.x())/2;
          ycenter = pointa.y() + (pointb.y() - pointa.y())/2;
        }
        else if (pointa.x() < pointb.x() && pointa.y() > pointb.y())
        {
          qreal angle = l.angle();
          qreal rangle = angle*PI/180;

          if (angle < 45)
          {
            pointa.setX(pointa.x() + entitya->getWidth()/2);
            pointa.setY(pointa.y() - ((entitya->getWidth()/2)*tan(rangle)));

            pointb.setX(pointb.x() - entityb->getWidth()/2);
            pointb.setY(pointb.y() + ((entityb->getWidth()/2)*tan(rangle)));
          }
          else
          {
            pointa.setX(pointa.x() + ((entitya->getHeight()/2)/tan(rangle)));
            pointa.setY(pointa.y() - entitya->getHeight()/2);

            pointb.setX(pointb.x() - ((entityb->getHeight()/2)/tan(rangle)));
            pointb.setY(pointb.y() + entityb->getHeight()/2);
          }

          xcenter = pointa.x() + (pointb.x() - pointa.x())/2;
          ycenter = pointb.y() + (pointa.y() - pointb.y())/2;
        }
        else if (pointa.x() > pointb.x() && pointa.y() > pointb.y())
        {
          qreal angle = l.angle() - 90;
          qreal rangle = angle*PI/180;

          if (angle > 45)
          {
            pointa.setX(pointa.x() - entitya->getWidth()/2);
            pointa.setY(pointa.y() - ((entitya->getWidth()/2)/tan(rangle)));

            pointb.setX(pointb.x() + entityb->getWidth()/2);
            pointb.setY(pointb.y() + ((entityb->getWidth()/2)/tan(rangle)));
          }
          else
          {
            pointa.setX(pointa.x() - ((entitya->getHeight()/2)*tan(rangle)));
            pointa.setY(pointa.y() - entitya->getHeight()/2);

            pointb.setX(pointb.x() + ((entityb->getHeight()/2)*tan(rangle)));
            pointb.setY(pointb.y() + entityb->getHeight()/2);
          }

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
  }

  return NULL;
}

void StructuralView::createReference(StructuralGraphicsEntity* entitya,
                               StructuralGraphicsEntity* entityb)
{
  StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  qDebug() << "=================== Creating reference: " << entitya->getnstId() << "->" << entityb->getnstId();

  if (parenta != NULL && parentb != NULL)
  {
    // removing previous reference
    foreach(StructuralGraphicsEntity* entity, parenta->getnstGraphicsEntities())
    {
      if (entity->getnstType() == Structural::Reference)
      {
        StructuralGraphicsEdge* edge = (StructuralGraphicsEdge*) entity;

        if (entitya == edge->getEntityA()){
          if (edge->getEntityA()->getncgType() == Qncg::Node)
          {
            ((StructuralGraphicsNode*) edge->getEntityA())->removenstGraphicsEdge(edge);
          }
          else if (edge->getEntityA()->getncgType() == Qncg::Interface)
          {
            ((StructuralGraphicsInterface*) edge->getEntityA())->removenstGraphicsEdge(edge);
          }

          if (edge->getEntityB()->getncgType() == Qncg::Node)
          {
            ((StructuralGraphicsNode*) edge->getEntityB())->removenstGraphicsEdge(edge);

          }
          else if (edge->getEntityB()->getncgType() == Qncg::Interface)
          {
            ((StructuralGraphicsInterface*) edge->getEntityB())->removenstGraphicsEdge(edge);
          }

          parenta->removenstGraphicsEntity(edge); delete (edge);

          break;
        }
      }
    }

    StructuralGraphicsReference* entity = new StructuralGraphicsReference();
    entity->setEntityA(entitya);
    entity->setEntityB(entityb);
    entity->adjust();

    parenta->addnstGraphicsEntity(entity);

    ((StructuralGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);

    if (entityb->getncgType() == Qncg::Node)
      ((StructuralGraphicsNode*) entityb)->addnstGraphicsEdge(entity);
    else if (entityb->getncgType() == Qncg::Interface)
      ((StructuralGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

    requestEntityChange(entitya);
  }
}

void StructuralView::createMapping(StructuralGraphicsEntity* entitya,
                             StructuralGraphicsEntity* entityb)
{
  StructuralGraphicsEntity* parenta = entitya->getnstGraphicsParent();
  StructuralGraphicsEntity* parentb = entityb->getnstGraphicsParent();

  if (parenta != NULL && parentb != NULL)
  {

    QMap<QString, QString> properties;

    properties["TYPE"] = "mapping";

    StructuralGraphicsMapping* entity = new StructuralGraphicsMapping();
    entity->setEntityA(entitya);
    entity->setEntityB(entityb);

    ((StructuralGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);

    if (entityb->getncgType() == Qncg::Node)
    {
      properties["component"] = entityb->getnstId();
      properties["componentUID"] = entityb->getnstUid();

      entity->setComponent(properties["component"]);
      entity->setComponentUid(properties["componentUID"]);

      ((StructuralGraphicsNode*) entityb)->addnstGraphicsEdge(entity);
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

      ((StructuralGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);
    }

    parenta->addnstGraphicsEntity(entity);

    entity->adjust();

    entity->setSwitchPortUid(entitya->getnstUid());

    entities[entity->getnstUid()] = entity;

    // parent is the strucUID from the switchPort entity in this case
    emit entityAdded(entity->getnstUid(), entitya->getnstUid(), properties);
  }
}

void StructuralView::createActionWithDialog(StructuralGraphicsEntity* entitya,
                                      StructuralGraphicsEntity* entityb)
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
      StructuralGraphicsLink* link = links[entitya->getnstUid()];

      // creating action bind
      StructuralGraphicsBind* bind = createBind(link,
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

void StructuralView::createConditionWithDialog(StructuralGraphicsEntity* entitya,
                                         StructuralGraphicsEntity* entityb)
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
      StructuralGraphicsLink* link = links[entityb->getnstUid()];
      // creating condition bind
      StructuralGraphicsBind* bind = createBind(link,
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

void StructuralView::createLinkWithDialog(StructuralGraphicsEntity* entitya,
                                    StructuralGraphicsEntity* entityb)
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
      StructuralGraphicsLink* link = createLink(entitya, entityb, connName);

      // creating condition bind
      StructuralGraphicsBind* condition = createBind(link,
                                               entitya,
                                               con,
                                               "");
      // creating action bind
      StructuralGraphicsBind* action = createBind(link,
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

StructuralGraphicsBind* StructuralView::createBind(StructuralGraphicsLink* link,
                                       StructuralGraphicsEntity* target,
                                       QString condition,
                                       QString action,
                                       bool notifyCreation)
{
  StructuralGraphicsEntity* parent = link->getnstGraphicsParent();

  StructuralGraphicsBind* bind = new StructuralGraphicsBind(); // \fixme Use makeGraphics
  bind->setLink(link);
  bind->setTarget(target);

  parent->addnstGraphicsEntity(bind);

  // setting type and component/interface
  if (!condition.isEmpty())
  {
    bind->setRole(condition);
    bind->setType(StructuralUtil::getBindTypeFromStr(condition));
  }
  else
  {
    bind->setRole(action);
    bind->setType(StructuralUtil::getBindTypeFromStr(action));
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
      StructuralGraphicsEntity* targetParent = target->getnstGraphicsParent();

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
    StructuralGraphicsEntity *entity;
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
    StructuralGraphicsEntity *entity;
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
#ifdef SMOOTH_ZOOM
    int numDegrees = event->delta() / 8;
    int numSteps = numDegrees / 15;  // see QWheelEvent documentation
    _numScheduledScalings += numSteps;
    if (_numScheduledScalings * numSteps < 0)  // if user moved the wheel in another direction, we reset previously scheduled scalings
        _numScheduledScalings = numSteps;

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(scalingTime(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(animFinished()));
    anim->start();
 #else
    if (event->delta() > 0)
        performZoomIn();
    else
        performZoomOut();
 #endif

    event->accept();
  }
  else
  {
    // call the parent wheelEvent
    QGraphicsView::wheelEvent(event);
  }
}
#ifdef SMOOTH_ZOOM
void QnstView::scalingTime(qreal x)
{
  qreal factor = 1.0 + qreal(_numScheduledScalings) / 500.0;
  scale(factor, factor);
}

void QnstView::animFinished()
{
  if (_numScheduledScalings > 0)
      _numScheduledScalings--;
  else
      _numScheduledScalings++;
  sender()->~QObject();
}
#endif

void StructuralView::focusOutEvent(QFocusEvent *event)
{
  Q_UNUSED(event)

  StructuralGraphicsEntity *entity;
  foreach(entity, entities.values())
  {
    entity->setDraggable(false);
  }
}

void StructuralView::adjustAngles(StructuralGraphicsEdge* edge)
{
  QnstGraphicsEntityWithEdges *entitya = (QnstGraphicsEntityWithEdges*)edge->getEntityA();
  QnstGraphicsEntityWithEdges *entityb = (QnstGraphicsEntityWithEdges*)edge->getEntityB();

  if(entitya == NULL || entityb == NULL) return; // do nothing

  int angle = 0;
  bool par = false;
  foreach (StructuralGraphicsEdge *edge, entitya->getnstGraphicsEdges())
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

void StructuralView::requestBindParamAdjust(QString uid, QString parent,
                                      QMap<QString, QString> properties)
{
  properties["TYPE"] = "bindParam";

  if ( !properties["name"].isEmpty() &&
      (!properties["value"].isEmpty() ||
      (properties["value"].isEmpty() && bindParamUIDToBindUID.contains(uid))))
  {
    if (bindParamUIDToBindUID.contains(uid))
    {
      emit entityChanged(uid, properties);
    }
    else
    {
      bindParamUIDToBindUID[uid] = parent;
      emit entityAdded(uid, parent, properties );
    }
  }
//  else if (!properties["name"].isEmpty())
//  {
//    bindParamUIDToBindUID.remove(uid);
//    emit entityRemoved(uid);
//  }
}

void StructuralView::updateBindParams(QString bindUID,
                                QMap<QString, QString> params,
                                QMap<QString, QString> name_uids)
{
  if (binds.contains(bindUID))
  {
    binds[bindUID]->setParams(params);
    binds[bindUID]->setNamesUIDs(name_uids);
  }
}

void StructuralView::markError(QString uid, QString msg)
{
  StructuralGraphicsEntity *entity = NULL;

  if(entities.contains(uid))
  {
    entity = entities[uid];
  }
  else if (binds.contains(uid))
  {
    entity = (StructuralGraphicsEntity*) binds[uid];
  }

  if (entity != NULL)
  {
    entity->setError(true);
    entity->setErrorMsg(msg);
    entity->updateToolTip();
  }
}

void StructuralView::clearValidationErrors()
{
  foreach(StructuralGraphicsEntity *entity, entities.values())
  {
    assert(entity != NULL);
    entity->setError(false);
  }

  foreach(StructuralGraphicsBind *bind, binds.values())
  {
    assert(bind != NULL);
    bind->setError(false);
  }
}

void StructuralView::clearAllData()
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

void StructuralView::traceRefers()
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

void StructuralView::traceEntities()
{
  qDebug() << "#### TRACING ENTITIES ####";
  foreach(QString key, entities.keys())
  {
    assert(entities[key] != NULL);
    assert(entities[key] != entities[key]->getnstGraphicsParent());

    qDebug() << key
             << QString().sprintf("%p", entities.value(key))
             << entities[key]->getnstType()
             << StructuralUtil::getStrFromNstType(entities[key]->getnstType());

    if(entities[key]->getnstGraphicsParent())
      qDebug()<< StructuralUtil::getStrFromNstType(entities[key]->getnstGraphicsParent()->getnstType());

    foreach(StructuralGraphicsEntity * e, entities[key]->getnstGraphicsEntities())
    {
      qDebug() << "\t"
               << QString().sprintf("%p", e)
               << e->getnstType()
               << StructuralUtil::getStrFromNstType(e->getnstType());
    }
  }
  qDebug() << "#### END TRACING ENTITIES ####" << endl;
}

void StructuralView::traceConnectors()
{
  qDebug() << "#### TRACING CONNECTORS ####";
  foreach(QString key, connectors.keys())
  {
    assert(connectors[key] != NULL);

#ifdef Q_OS_MAC
    qDebug() << key << connectors[key] << connectors[key]->getConditions() << connectors[key]->getActions();
#else
//    qDebug() << key << (int)connectors[key] << connectors[key]->getConditions() << connectors[key]->getActions();
#endif
  }
  qDebug() << "#### END TRACING CONNECTORS ####" << endl;
}

void StructuralView::traceConnectors2()
{
  qDebug() << "#### TRACING CONNECTORS2 ####";
  foreach(QString key, connectors2.keys())
  {
    assert(connectors2[key] != NULL);

#ifdef Q_OS_MAC
    qDebug() << key << connectors2[key] << connectors2[key]->getConditions() << connectors2[key]->getActions();
#else
//    qDebug() << key << (int)connectors2[key] << connectors2[key]->getConditions() << connectors2[key]->getActions();
#endif
  }
  qDebug() << "#### END TRACING CONNECTORS2 ####" << endl;
}

void StructuralView::deletePendingEntities()
{
  foreach(StructuralGraphicsEntity* e, toDelete)
  {
    e->deleteLater();
  }

  toDelete.clear();
}

StructuralConnector* StructuralView::getConnectorByRoleUid(QString uid)
{
  StructuralConnector* conn = NULL;

  foreach(StructuralConnector* c, connectors.values())
  {
    if (c->hasCondition(uid) || c->hasAction(uid))
    {
      conn = c;
    }
  }

  return conn;
}

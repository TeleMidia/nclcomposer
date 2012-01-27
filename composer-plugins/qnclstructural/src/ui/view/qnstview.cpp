#include "qnstview.h"

QnstView::QnstView(QWidget* parent)
    : QGraphicsView(parent)
{
    createObjects();
    createConnection();

    ncontext = 0;

    nswitch = 0;

    nport = 0;

    nmedia = 0;

    nlink = 0;

    zoomStep = 0;

    nproperty = 0;

    narea = 0;

    linking = false;

    modified = false;

    link = NULL;

    selected = NULL;

    clipboard = NULL;
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
}

void QnstView::read(QDomElement element, QDomElement parent)
{
    if (element.nodeName() == "body"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addBody(uid, "", properties, true);

    }else if (element.nodeName() == "media"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;    

        properties["id"] = element.attribute("id");
        properties["type"] = element.attribute("type");
        properties["src"] = element.attribute("src");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addMedia(uid, parent.attribute("uid"), properties, true);

    }else if (element.nodeName() == "context"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addContext(uid, parent.attribute("uid"), properties, true);

    }else if (element.nodeName() == "switch"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addSwitch(uid, parent.attribute("uid"), properties, true);

    }else if (element.nodeName() == "aggregator"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addAggregator(uid, parent.attribute("uid"), properties, true);

    }else if (element.nodeName() == "area"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addArea(uid, parent.attribute("uid"), properties, true);

    }else if (element.nodeName() == "property"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addProperty(uid, parent.attribute("uid"), properties, true);

    }else if (element.nodeName() == "port"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addPort(uid, parent.attribute("uid"), properties, true);

    }else if (element.nodeName() == "connector"){
        QnstConncetor* conn = new QnstConncetor();
        conn->setnstUid(element.attribute("uid"));
        conn->setName(element.attribute("id"));

        QDomNodeList list = element.childNodes();

        for (unsigned int i=0;i<list.length();i++)
        {
            if (list.item(i).isElement())
            {
                QDomElement e = list.item(i).toElement();

                if (e.nodeName() == "condition"){
                    conn->addCondition(e.attribute("type"), e.attribute("type"));

                }else if (e.nodeName() == "action"){
                    conn->addAction(e.attribute("type"), e.attribute("type"));
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
    if (element.nodeName() == "port"){
        if (entities.contains(element.attribute("uid"))){
            if (entities.contains(element.attribute("interfaceUID"))){
                QnstGraphicsInterface* entitya = (QnstGraphicsInterface*) entities[element.attribute("uid")];
                QnstGraphicsInterface* entityb = (QnstGraphicsInterface*) entities[element.attribute("interfaceUID")];

                if (entitya != entityb){
                    QnstGraphicsReference* entity = new QnstGraphicsReference();
                    entity->setnstGraphicsParent(entitya->getnstGraphicsParent());
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);
                    entity->adjust();

                    entitya->getnstGraphicsParent()->addnstGraphicsEntity(entity);

                    entitya->addnstGraphicsEdge(entity);
                    entityb->addnstGraphicsEdge(entity);
                }

            }else if (entities.contains(element.attribute("componentUID"))){
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

    }else if (element.nodeName() == "link"){
        if (element.attribute("uid") != "" && entities.contains(element.attribute("componentaUID"))&& entities.contains(element.attribute("componentbUID"))){
            if (!entities.contains(element.attribute("uid"))){
                QnstGraphicsNode* entitya = (QnstGraphicsNode*) entities[element.attribute("componentaUID")];
                QnstGraphicsNode* entityb = (QnstGraphicsNode*) entities[element.attribute("componentbUID")];

                QnstGraphicsLink* entity = new QnstGraphicsLink();
                entity->setnstId(element.attribute("id"));
                entity->setnstUid(element.attribute("uid"));
                entity->setnstGraphicsParent(entitya->getnstGraphicsParent());
                entity->setEntityA(entitya);
                entity->setEntityB(entityb);
                entity->adjust();

                entitya->getnstGraphicsParent()->addnstGraphicsEntity(entity);

                entitya->addnstGraphicsEdge(entity);
                entityb->addnstGraphicsEdge(entity);

                QString con = element.attribute("condition");
                QString act = element.attribute("action");

                if (con == "onBegin"){
                    entity->setCondition(Qnst::onBegin);

                }else if (con == "onEnd"){
                    entity->setCondition(Qnst::onEnd);

                }else if (con == "onPause"){
                    entity->setCondition(Qnst::onPause);

                }else if (con == "onResume"){
                    entity->setCondition(Qnst::onResume);

                }else if (con == "onSelection"){
                    entity->setCondition(Qnst::onSelection);

                }else{
                    entity->setCondition(Qnst::NoConditionType);
                }

                if (act == "Start"){
                    entity->setAction(Qnst::Start);

                }else if (act == "Stop"){
                    entity->setAction(Qnst::Stop);

                }else if (act == "Pause"){
                    entity->setAction(Qnst::Pause);

                }else if (act == "Resume"){
                    entity->setAction(Qnst::Resume);

                }else if (act == "Set"){
                    entity->setAction(Qnst::Set);

                }else{
                    entity->setAction(Qnst::NoActionType);
                }

                entities[entity->getnstUid()] = entity;

                nlink++;
            }
        }
    }else if (element.nodeName() == "bind"){
        if (element.attribute("uid") != "" && entities.contains(element.attribute("componentaUID"))&& entities.contains(element.attribute("componentbUID"))){
            if (!entities.contains(element.attribute("uid"))){
                QnstGraphicsNode* entitya = (QnstGraphicsNode*) entities[element.attribute("componentaUID")];
                QnstGraphicsNode* entityb = (QnstGraphicsNode*) entities[element.attribute("componentbUID")];

                if (element.attribute("type") == "condition"){
                    QnstGraphicsCondition* entity = new QnstGraphicsCondition();
                    entity->setnstId(element.attribute("id"));
                    entity->setnstUid(element.attribute("uid"));
                    entity->setnstGraphicsParent(entitya->getnstGraphicsParent());
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);
                    entity->adjust();

                    entitya->getnstGraphicsParent()->addnstGraphicsEntity(entity);

                    entitya->addnstGraphicsEdge(entity);
                    entityb->addnstGraphicsEdge(entity);

                    QString con = element.attribute("condition");

                    if (con == "onBegin"){
                        entity->setCondition(Qnst::onBegin);

                    }else if (con == "onEnd"){
                        entity->setCondition(Qnst::onEnd);

                    }else if (con == "onPause"){
                        entity->setCondition(Qnst::onPause);

                    }else if (con == "onResume"){
                        entity->setCondition(Qnst::onResume);

                    }else if (con == "onSelection"){
                        entity->setCondition(Qnst::onSelection);

                    }else{
                        entity->setCondition(Qnst::NoConditionType);
                    }

                    entities[entity->getnstUid()] = entity;

                    nlink++;

                }else if (element.attribute("type") == "action"){
                    QnstGraphicsAction* entity = new QnstGraphicsAction();
                    entity->setnstId(element.attribute("id"));
                    entity->setnstUid(element.attribute("uid"));
                    entity->setnstGraphicsParent(entitya->getnstGraphicsParent());
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);
                    entity->adjust();

                    entitya->getnstGraphicsParent()->addnstGraphicsEntity(entity);

                    entitya->addnstGraphicsEdge(entity);
                    entityb->addnstGraphicsEdge(entity);

                    QString act = element.attribute("action");

                    if (act == "Start"){
                        entity->setAction(Qnst::Start);

                    }else if (act == "Stop"){
                        entity->setAction(Qnst::Stop);

                    }else if (act == "Pause"){
                        entity->setAction(Qnst::Pause);

                    }else if (act == "Resume"){
                        entity->setAction(Qnst::Resume);

                    }else if (act == "Set"){
                        entity->setAction(Qnst::Set);

                    }else{
                        entity->setAction(Qnst::NoActionType);
                    }

                    entities[entity->getnstUid()] = entity;

                    nlink++;
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

            readLink(e, element);
        }
    }
}

QString QnstView::serialize()
{
    QDomDocument* dom = new QDomDocument();

    QDomElement root = dom->createElement("qnst");

    foreach(QnstConncetor* conn, connectors.values()){
        QDomElement e = dom->createElement("connector");

        e.setAttribute("id", conn->getName());
        e.setAttribute("uid", conn->getnstUid());

        foreach(QString condition, conn->getConditions()){
            QDomElement c = dom->createElement("condition");

            c.setAttribute("type", condition);

            e.appendChild(c);
        }

        foreach(QString action, conn->getActions()){
            QDomElement a = dom->createElement("action");

            a.setAttribute("type", action);

            e.appendChild(a);
        }

        root.appendChild(e);
    }

    foreach(QnstGraphicsEntity* entity, scene->getRoots()){
        QDomElement e = dom->createElement("body");

        e.setAttribute("id", entity->getnstId());
        e.setAttribute("uid", entity->getnstUid());

        e.setAttribute("top", entity->getTop());
        e.setAttribute("left", entity->getLeft());
        e.setAttribute("width", entity->getWidth());
        e.setAttribute("height", entity->getHeight());

        foreach(QnstGraphicsEntity* c, entity->getnstGraphicsEntities()){
            write(e, dom, c);
        }

        writeLink(e, dom, entity);

        root.appendChild(e);
    }

    dom->appendChild(root);

    linkWriterAux.clear();

    return dom->toString(4);
}

void QnstView::write(QDomElement element, QDomDocument* dom, QnstGraphicsEntity* entity)
{
    QDomElement e;

    switch(entity->getnstType()){
    case Qnst::Body:
        e = dom->createElement("body");

        break;

    case Qnst::Context:
        e = dom->createElement("context");

        break;

    case Qnst::Switch:
        e = dom->createElement("switch");

        break;

    case Qnst::Media:
        e = dom->createElement("media");

        e.setAttribute("type", "media");

        if (((QnstGraphicsMedia*) entity)->getSource() != "" ){
            e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
        }

        break;

    case Qnst::Image:
        e = dom->createElement("media");

        e.setAttribute("type", "image");

        if (((QnstGraphicsMedia*) entity)->getSource() != "" ){
            e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
        }

        break;

    case Qnst::Video:
        e = dom->createElement("media");

        e.setAttribute("type", "video");

        if (((QnstGraphicsMedia*) entity)->getSource() != "" ){
            e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
        }

        break;

    case Qnst::Text:
        e = dom->createElement("media");

        e.setAttribute("type", "text");

        if (((QnstGraphicsMedia*) entity)->getSource() != "" ){
            e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
        }

        break;

    case Qnst::Settings:
        e = dom->createElement("media");

        e.setAttribute("type", "settings");

        if (((QnstGraphicsMedia*) entity)->getSource() != "" ){
            e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
        }

        break;

    case Qnst::Audio:
        e = dom->createElement("media");

        e.setAttribute("type", "audio");

        if (((QnstGraphicsMedia*) entity)->getSource() != "" ){
            e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
        }

        break;

    case Qnst::Script:
        e = dom->createElement("media");

        e.setAttribute("type", "script");

        if (((QnstGraphicsMedia*) entity)->getSource() != "" ){
            e.setAttribute("src", ((QnstGraphicsMedia*) entity)->getSource());
        }

        break;

    case Qnst::Property:
        e = dom->createElement("property");

        break;

    case Qnst::Area:
        e = dom->createElement("area");

        break;

    case Qnst::Aggregator:
        e = dom->createElement("aggregator");

        break;

    case Qnst::Port:
        e = dom->createElement("port");

        QnstGraphicsPort* pentity = ((QnstGraphicsPort*) entity);

        foreach(QnstGraphicsEdge* edge, pentity->getnstGraphicsEdges()){
            if (edge->getnstType() == Qnst::Reference){
                if (edge->getEntityB()->getncgType() == Qncg::Node){
                    e.setAttribute("componentUID", edge->getEntityB()->getnstUid());

                }else if (edge->getEntityB()->getncgType() == Qncg::Interface){
                    e.setAttribute("componentUID", edge->getEntityB()->getnstParent()->getnstUid());
                    e.setAttribute("interfaceUID", edge->getEntityB()->getnstUid());
                }

                break;
            }
        }

        break;
    }

    e.setAttribute("id", entity->getnstId());
    e.setAttribute("uid", entity->getnstUid());

    e.setAttribute("top", entity->getTop());
    e.setAttribute("left", entity->getLeft());
    e.setAttribute("width", entity->getWidth());
    e.setAttribute("height", entity->getHeight());

    writeLink(element, dom, entity);

    foreach(QnstGraphicsEntity* c, entity->getnstGraphicsEntities()){
        write(e, dom, c);
    }

    element.appendChild(e);
}

void QnstView::writeLink(QDomElement element, QDomDocument* dom, QnstGraphicsEntity* entity)
{
    if (entity->getncgType() == Qncg::Node){
        QnstGraphicsNode* node = (QnstGraphicsNode*) entity;

        foreach(QnstGraphicsEdge* edge, node->getnstGraphicsEdges()){
            if (edge->getnstType() == Qnst::Link && !linkWriterAux.contains(edge->getnstUid())){

                QnstGraphicsLink* link = (QnstGraphicsLink*) edge;

                QDomElement e = dom->createElement("link");

                e.setAttribute("id", edge->getnstId());

                e.setAttribute("uid", edge->getnstUid());

                e.setAttribute("componentaUID", link->getEntityA()->getnstUid());
                e.setAttribute("componentbUID", link->getEntityB()->getnstUid());

                switch(link->getCondition()){
                case Qnst::onBegin:
                    e.setAttribute("condition", "onBegin");
                    break;

                case Qnst::onEnd:
                    e.setAttribute("condition", "onEnd");
                    break;

                case Qnst::onPause:
                    e.setAttribute("condition", "onPause");
                    break;

                case Qnst::onResume:
                    e.setAttribute("condition", "onResume");
                    break;

                case Qnst::onSelection:
                    e.setAttribute("condition", "onSelection");
                    break;

                case Qnst::NoConditionType:
                    e.setAttribute("condition", "NoConditionType");
                    break;
                }

                switch(link->getAction()){
                case Qnst::Start:
                    e.setAttribute("action", "Start");
                    break;

                case Qnst::Stop:
                    e.setAttribute("action", "Stop");
                    break;

                case Qnst::Pause:
                    e.setAttribute("action", "Pause");
                    break;

                case Qnst::Resume:
                    e.setAttribute("action", "Resume");
                    break;

                case Qnst::Set:
                    e.setAttribute("action", "Set");
                    break;

                case Qnst::NoActionType:
                    e.setAttribute("action", "NoActionType");
                    break;
                }

                element.appendChild(e);

                linkWriterAux.insert(link->getnstUid());

            }else if (edge->getnstType() == Qnst::Condition && !linkWriterAux.contains(edge->getnstUid())){
                QnstGraphicsCondition* link = (QnstGraphicsCondition*) edge;

                QDomElement e = dom->createElement("bind");

                e.setAttribute("id", edge->getnstId());

                e.setAttribute("uid", edge->getnstUid());

                e.setAttribute("type", "condition");

                e.setAttribute("componentaUID", link->getEntityA()->getnstUid());
                e.setAttribute("componentbUID", link->getEntityB()->getnstUid());

                switch(link->getCondition()){
                case Qnst::onBegin:
                    e.setAttribute("condition", "onBegin");
                    break;

                case Qnst::onEnd:
                    e.setAttribute("condition", "onEnd");
                    break;

                case Qnst::onPause:
                    e.setAttribute("condition", "onPause");
                    break;

                case Qnst::onResume:
                    e.setAttribute("condition", "onResume");
                    break;

                case Qnst::onSelection:
                    e.setAttribute("condition", "onSelection");
                    break;

                case Qnst::NoConditionType:
                    e.setAttribute("condition", "NoConditionType");
                    break;
                }

                element.appendChild(e);

                linkWriterAux.insert(link->getnstUid());

            }else if (edge->getnstType() == Qnst::Action && !linkWriterAux.contains(edge->getnstUid())){
                QnstGraphicsAction* link = (QnstGraphicsAction*) edge;

                QDomElement e = dom->createElement("bind");

                e.setAttribute("id", edge->getnstId());

                e.setAttribute("uid", edge->getnstUid());

                e.setAttribute("type", "action");

                e.setAttribute("componentaUID", link->getEntityA()->getnstUid());
                e.setAttribute("componentbUID", link->getEntityB()->getnstUid());

                switch(link->getAction()){
                case Qnst::Start:
                    e.setAttribute("action", "Start");
                    break;

                case Qnst::Stop:
                    e.setAttribute("action", "Stop");
                    break;

                case Qnst::Pause:
                    e.setAttribute("action", "Pause");
                    break;

                case Qnst::Resume:
                    e.setAttribute("action", "Resume");
                    break;

                case Qnst::Set:
                    e.setAttribute("action", "Set");
                    break;

                case Qnst::NoActionType:
                    e.setAttribute("action", "NoActionType");
                    break;
                }

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

    connect(scene, SIGNAL(entityAdded(QnstGraphicsEntity*)), SLOT(requestEntityAddition(QnstGraphicsEntity*)));
    connect(scene, SIGNAL(entityChanged(QnstGraphicsEntity*)), SLOT(requestEntityChange(QnstGraphicsEntity*)));
    connect(scene, SIGNAL(entityRemoved(QnstGraphicsEntity*)), SLOT(requestEntityRemotion(QnstGraphicsEntity*)));
    connect(scene, SIGNAL(entitySelected(QnstGraphicsEntity*)), SLOT(requestEntitySelection(QnstGraphicsEntity*)));
    connect(scene, SIGNAL(entityAboutToChange(QnstGraphicsEntity*,QMap<QString,QString>)), SLOT(requestEntityPreparation(QnstGraphicsEntity*,QMap<QString,QString>)));
}

void QnstView::addEntity(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    qDebug() << "[QNST]" << ":" << "Adding entity '"+uid+"'";

    // if the entity type is BODY
    if (properties["TYPE"] == "body"){
        addBody(uid, parent, properties);

    // if the entity type is CONTEXT
    }else if (properties["TYPE"] == "context"){
        addContext(uid, parent, properties);

    // if the entity type is SWITCH
    }else if (properties["TYPE"] == "switch"){
        addSwitch(uid, parent, properties);

    // if the entity type is MEDIA
    }else if (properties["TYPE"] == "media"){
        addMedia(uid, parent, properties);

    // if the entity type is PORT
    }else if (properties["TYPE"] == "port"){
        addPort(uid, parent, properties);

    // if the entity type is LINK
    }else if (properties["TYPE"] == "link"){
        addLink(uid, parent, properties);

    // if the entity type is BIND
    }else if (properties["TYPE"] == "bind"){
        addBind(uid, parent, properties);

    // if the entity type is CONNECTOR
    }else if (properties["TYPE"] == "causalConnector"){
        addConnector(uid, parent, properties);

    // if the entity type is CONDITION
    }else if (properties["TYPE"] == "simpleCondition"){
        addCondition(uid, parent, properties);

    // if the entity type is ACTION
    }else if (properties["TYPE"] == "simpleAction"){
        addAction(uid, parent, properties);

    // if the entity type is IMPORTBASE
    }else if (properties["TYPE"] == "importBase"){
        addImportBase(properties);

    // if the entity type is AREA
    }else if (properties["TYPE"] == "area"){
        addArea(uid, parent, properties);

    // if the entity type is PROPERTY
    }else if (properties["TYPE"] == "property"){
        addProperty(uid, parent, properties);
    }
}

void QnstView::removeEntity(const QString uid, bool undo)
{
    qDebug() << "[QNST]" << ":" << "Removing entity '"+uid+"'";

    if (entities.contains(uid)){
        QnstGraphicsEntity* entity = entities[uid];

        if (entity != NULL){
            if (!undo){
                QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
                history.push(cmd);
            }

            if (entity->getncgType() == Qncg::Node){
                foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
                    if (e->getnstType() != Qnst::Link &&
                        e->getnstType() != Qnst::Edge &&
                        e->getnstType() != Qnst::Condition &&
                        e->getnstType() != Qnst::Action &&
                        e->getnstType() != Qnst::Reference){

                        removeEntity(e->getnstUid(), true);
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

                    QnstGraphicsEntity* parent = edge->getEntityA()->getnstGraphicsParent();

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
                        foreach(QnstBind* b, links[entity->getnstUid()]->getActions()){
                            binds.remove(b->getnstUid());
                            brelations.remove(b->getnstUid());
                        }

                        foreach(QnstBind* b, links[entity->getnstUid()]->getConditions()){
                            binds.remove(b->getnstUid());
                            brelations.remove(b->getnstUid());
                        }

                        link2conn.remove(links[entity->getnstUid()]->getnstId());

                        links.remove(entity->getnstUid());
                    }
                }

                entities.remove(uid); delete entity;

            }else if (entity->getncgType() == Qncg::Interface){
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

                    QnstGraphicsEntity* parent = edge->getEntityA()->getnstGraphicsParent();

                    parent->removenstGraphicsEntity(edge);

                    entities.remove(edge->getnstUid());
                }

                QnstGraphicsEntity* parent = entity->getnstGraphicsParent();

                parent->removenstGraphicsEntity(entity);

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

                QnstGraphicsEntity* parent = edge->getEntityA()->getnstGraphicsParent();

                parent->removenstGraphicsEntity(edge);

                entities.remove(edge->getnstUid());
            }
        }
    }else if (connectors2.contains(uid)){
        connectors.remove(connectors2[uid]->getName());
        connectors2.remove(uid);
    }
}

void QnstView::changeEntity(const QString uid, const QMap<QString, QString> properties)
{
    qDebug() << "[QNST]" << ":" << "Changing entity '"+uid+"'";

    if (links.contains(uid)){
            changeLink(links[uid], properties);

    }else if (binds.contains(uid)){
        changeBind(binds[uid], properties);

    }else if (entities.contains(uid)){
        QnstGraphicsEntity* entity = entities[uid];

        switch(entity->getnstType()){

        // if the entity type is BODY
        case Qnst::Body:
            changeBody((QnstGraphicsBody*) entity, properties);
            break;

        // if the entity type is CONTEXT
        case Qnst::Context:
            changeContext((QnstGraphicsContext*) entity, properties);
            break;

        // if the entity type is SWITCH
        case Qnst::Switch:
            changeSwitch((QnstGraphicsSwitch*) entity, properties);
            break;

        // if the entity type is MEDIA
        case Qnst::Audio:
        case Qnst::Text:
        case Qnst::Video:
        case Qnst::Image:
        case Qnst::Script:
        case Qnst::Settings:
        case Qnst::Media:
        case Qnst::Html:
            changeMedia((QnstGraphicsMedia*) entity, properties);
            break;

        // if the entity type is PORT
        case Qnst::Port:
            changePort((QnstGraphicsPort*) entity, properties);
            break;

        // if the entity type is AREA
        case Qnst::Area:
            changeArea((QnstGraphicsArea*) entity, properties);
            break;

        // if the entity type is PROPERTY
        case Qnst::Property:
            changeProperty((QnstGraphicsProperty*) entity, properties);
            break;
        }

    }else if (connectors2.contains(uid)){
        changeConnector(connectors2[uid], properties);

    }else if (properties["TYPE"] == "simpleCondition"){
        changeCondition(uid, properties);

    }else if (properties["TYPE"] == "simpleAction"){
        changeAction(uid, properties);

    }else if (properties["TYPE"] == "importBase"){
        changeImportBase(properties);
    }
}

void QnstView::selectEntity(const QString uid)
{
    qDebug() << "[QNST]" << ":" << "Selecting entity '"+uid+"'";

    if (entities.contains(uid)){
        QnstGraphicsEntity* entity = entities[uid];

        if (entity != selected){
            if (selected != NULL){
                selected->setSelected(false);
                selected->adjust();
            }

            selected = entity;

            selected->setSelected(true);
            selected->adjust();

            centerOn(selected);
        }
    }
}

void QnstView::addBody(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{
    QnstGraphicsBody* entity = new QnstGraphicsBody();
    entity->setnstUid(uid);

    entity->setTop(scene->height()/2 - 550/2);
    entity->setLeft(scene->width()/2 - 750/2);
    entity->setWidth(750);
    entity->setHeight(550);

    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
    }

    if (properties["top"] != ""){
        entity->setTop(properties["top"].toDouble());
    }

    if (properties["left"] != ""){
        entity->setLeft(properties["left"].toDouble());
    }

    if (properties["width"] != ""){
        entity->setWidth(properties["width"].toDouble());
    }

    if (properties["height"] != ""){
        entity->setHeight(properties["height"].toDouble());
    }

    scene->addRoot(entity); entities[uid] = entity;

    if (!undo){
        QnstAddCommand* cmd = new QnstAddCommand(this, entity);
        history.push(cmd);
    }
}

void QnstView::changeBody(QnstGraphicsBody* entity, const QMap<QString, QString> properties)
{
    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
    }
}

void QnstView::addImportBase(const QMap<QString, QString> properties)
{
    if (properties["documentURI"] != "" && properties["projectURI"] != "" && properties["alias"] != ""){
        int n = properties["projectURI"].lastIndexOf("/");

        QFile* file = new QFile(properties["projectURI"].left(n)+QDir::separator()+properties["documentURI"]);

        if (file->open(QIODevice::ReadOnly)){
            QDomDocument* domdoc = new QDomDocument();

            if (domdoc->setContent(file)){
                readImportBase(domdoc->firstChildElement(), properties["alias"]);
            }
        }

        delete file;
    }
}


void QnstView::changeImportBase(const QMap<QString, QString> properties)
{
    if (properties["documentURI"] != "" && properties["projectURI"] != "" && properties["alias"] != ""){
        connectors2.clear();
        connectors.clear();

        int n = properties["projectURI"].lastIndexOf("/");

        QFile* file = new QFile(properties["projectURI"].left(n)+QDir::separator()+properties["documentURI"]);

        if (file->open(QIODevice::ReadOnly)){
            QDomDocument* domdoc = new QDomDocument();

            if (domdoc->setContent(file)){
                readImportBase(domdoc->firstChildElement(), properties["alias"]);
            }
        }

        delete file;
    }
}


void QnstView::readImportBase(QDomElement element, QString alias)
{
    if (element.tagName() == "causalConnector"){
        QDomNodeList list = element.childNodes();

        QnstConncetor* conn = new QnstConncetor();
        conn->setnstId(alias+"#"+element.attribute("id"));
        conn->setName(alias+"#"+element.attribute("id"));

        for (unsigned int i=0;i<list.length();i++){
            if (list.item(i).isElement()){
                QDomElement e = list.item(i).toElement();

                readConnector(e, conn);
            }
        }

        connectors[conn->getName()] = conn;
        connectors2[conn->getnstId()] = conn;

    }else{
        QDomNodeList list = element.childNodes();

        for (unsigned int i=0;i<list.length();i++){
            if (list.item(i).isElement()){
                QDomElement e = list.item(i).toElement();

                readImportBase(e, alias);
            }
        }
    }
}

void QnstView::readConnector(QDomElement element, QnstConncetor* conn)
{
    if (element.tagName() == "simpleCondition"){
        if (element.attribute("role") != ""){
            conn->addCondition(element.attribute("role"), element.attribute("role"));
        }
    }

    if (element.tagName() == "simpleAction"){
        if (element.attribute("role") != ""){
            conn->addAction(element.attribute("role"), element.attribute("role"));
        }
    }

    QDomNodeList list = element.childNodes();

    for (unsigned int i=0;i<list.length();i++){
        if (list.item(i).isElement()){
            QDomElement e = list.item(i).toElement();

            readConnector(e, conn);
        }
    }
}

void QnstView::addContext(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{
    if (entities.contains(parent)){
        QnstGraphicsContext* entity = new QnstGraphicsContext((QnstGraphicsNode*) entities[parent]);
        entity->setnstUid(uid);

        entity->setTop(entities[parent]->getHeight()/2 - 200/2);
        entity->setLeft(entities[parent]->getWidth()/2 - 250/2);
        entity->setWidth(250);
        entity->setHeight(200);

        if (properties["id"] != ""){
            entity->setnstId(properties["id"]);
        }

        if (properties["top"] != ""){
            entity->setTop(properties["top"].toDouble());
        }

        if (properties["left"] != ""){
            entity->setLeft(properties["left"].toDouble());
        }

        if (properties["width"] != ""){
            entity->setWidth(properties["width"].toDouble());
        }

        if (properties["height"] != ""){
            entity->setHeight(properties["height"].toDouble());
        }

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; entity->adjust();

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);
        }
    }
}

void QnstView::changeContext(QnstGraphicsContext* entity, const QMap<QString, QString> properties)
{
    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
    }

//    TODO:
//    if (properties["refer"] != ""){
//
//    }
}

void QnstView::addSwitch(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{
    if (entities.contains(parent)){
        QnstGraphicsSwitch* entity = new QnstGraphicsSwitch((QnstGraphicsNode*) entities[parent]);
        entity->setnstUid(uid);

        entity->setTop(entities[parent]->getHeight()/2 - 200/2);
        entity->setLeft(entities[parent]->getWidth()/2 - 250/2);
        entity->setWidth(250);
        entity->setHeight(200);

        if (properties["id"] != ""){
            entity->setnstId(properties["id"]);
        }

        if (properties["top"] != ""){
            entity->setTop(properties["top"].toDouble());
        }

        if (properties["left"] != ""){
            entity->setLeft(properties["left"].toDouble());
        }

        if (properties["width"] != ""){
            entity->setWidth(properties["width"].toDouble());
        }

        if (properties["height"] != ""){
            entity->setHeight(properties["height"].toDouble());
        }

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; entity->adjust();

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);
        }
    }
}

void QnstView::changeSwitch(QnstGraphicsSwitch* entity, const QMap<QString, QString> properties)
{
    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
    }

//    TODO:
//    if (properties["refer"] != ""){
//
//    }
}

void QnstView::addMedia(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{
    if (entities.contains(parent)){
        QnstGraphicsMedia* entity;

        if (properties["type"].startsWith("image")){
            entity = new QnstGraphicsImage((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"].startsWith("audio")){
            entity = new QnstGraphicsAudio((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"].startsWith("video")){
            entity = new QnstGraphicsVideo((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"] == "text/html"){
            entity = new QnstGraphicsHTML((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"].startsWith("text")){
            entity = new QnstGraphicsText((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"] == "application/x-ncl-settings"){
            entity = new QnstGraphicsSettings((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"] == "application/x-ginga-NCLua"){
            entity = new QnstGraphicsScript((QnstGraphicsNode*) entities[parent]);

        }else{
            entity = new QnstGraphicsMedia((QnstGraphicsNode*) entities[parent]);
        }

        entity->setnstUid(uid);

        entity->setTop(entities[parent]->getHeight()/2 - 48/2);
        entity->setLeft(entities[parent]->getWidth()/2 - 48/2);
        entity->setWidth(48);
        entity->setHeight(64);

//        if (properties["id"] != ""){
            entity->setnstId(properties["id"]);
//        }

//        if (properties["src"] != ""){
            entity->setSource(properties["src"]);
//        }

        if (properties["top"] != ""){
            entity->setTop(properties["top"].toDouble());
        }

        if (properties["left"] != ""){
            entity->setLeft(properties["left"].toDouble());
        }

        if (properties["width"] != ""){
            entity->setWidth(properties["width"].toDouble());
        }

        if (properties["height"] != ""){
            entity->setHeight(properties["height"].toDouble());
        }

        ++nmedia;

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; entity->adjust();

        adjustMedia(entity);

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);
        }
    }
}

void QnstView::changeMedia(QnstGraphicsMedia* entity, const QMap<QString, QString> properties)
{
//    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
//    }

//    if (properties["src"] != ""){
        entity->setSource(properties["src"]);
//    }

    if (properties["type"] != ""){
        if (properties["type"].startsWith("image")){
            entity->setnstType(Qnst::Image);

            entity->setIcon(":/icon/image");

        }else if (properties["type"].startsWith("audio")){
            entity->setnstType(Qnst::Audio);

            entity->setIcon(":/icon/audio");

        }else if (properties["type"].startsWith("video")){
            entity->setnstType(Qnst::Video);

            entity->setIcon(":/icon/video");

        }else if (properties["type"].startsWith("text/html")){
            entity->setnstType(Qnst::Html);

            entity->setIcon(":/icon/html");

        }else if (properties["type"].startsWith("text")){
            entity->setnstType(Qnst::Text);

            entity->setIcon(":/icon/text");

        }else if (properties["type"] == "application/x-ncl-settings"){
            entity->setnstType(Qnst::Settings);

            entity->setIcon(":/icon/settings");

        }else if (properties["type"] == "application/x-ginga-NCLua"){
            entity->setnstType(Qnst::Script);

            entity->setIcon(":/icon/script");

        }else{
            entity->setnstType(Qnst::Media);

            entity->setIcon(":/icon/media");
        }
    }

    adjustMedia(entity);
}

void QnstView::adjustMedia(QnstGraphicsMedia* entity)
{
  entity->adjust();
}

void QnstView::addPort(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{ 
    if (entities.contains(parent)){
        QnstGraphicsPort* entity = new QnstGraphicsPort((QnstGraphicsNode*) entities[parent]);
        entity->setnstUid(uid);

        entity->setTop(0);
        entity->setLeft(0);
        entity->setWidth(18);
        entity->setHeight(18);

        entity->setnstId(properties["id"]);

        entity->setComponent(properties["component"]);

        entity->setInterface(properties["interface"]);

        if (properties["top"] != ""){
            entity->setTop(properties["top"].toDouble());
        }

        if (properties["left"] != ""){
            entity->setLeft(properties["left"].toDouble());
        }

        if (properties["width"] != ""){
            entity->setWidth(properties["width"].toDouble());
        }

        if (properties["height"] != ""){
            entity->setHeight(properties["height"].toDouble());
        }

        if (properties["interfaceUid"] != ""){
            entity->setInterfaceUid(properties["interfaceUid"]);
        }else{
            entity->setInterfaceUid("");
        }

        if (properties["componentUid"] != ""){
            entity->setComponentUid(properties["componentUid"]);
        }else{
            entity->setComponentUid("");
        }

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; ++nport;

        adjustPort(entity);

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);
        }
    }
}

void QnstView::changePort(QnstGraphicsPort* entity, const QMap<QString, QString> properties)
{
    entity->setnstId(properties["id"]);

    entity->setComponent(properties["component"]);

    entity->setInterface(properties["interface"]);

    if (properties["top"] != ""){
        entity->setTop(properties["top"].toDouble());
    }

    if (properties["left"] != ""){
        entity->setLeft(properties["left"].toDouble());
    }

    if (properties["width"] != ""){
        entity->setWidth(properties["width"].toDouble());
    }

    if (properties["height"] != ""){
        entity->setHeight(properties["height"].toDouble());
    }

    if (properties["interfaceUid"] != ""){
        entity->setInterfaceUid(properties["interfaceUid"]);
    }else{
        entity->setInterfaceUid("");
    }

    if (properties["componentUid"] != ""){
        entity->setComponentUid(properties["componentUid"]);
    }else{
        entity->setComponentUid("");
    }

    adjustPort(entity);
}

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

void QnstView::addArea(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{
    if (entities.contains(parent)){
        QnstGraphicsArea* entity = new QnstGraphicsArea(entities[parent]);
        entity->setnstUid(uid);
        entity->setnstGraphicsParent(entities[parent]);

        entity->setTop(0);
        entity->setLeft(0);
        entity->setWidth(16);
        entity->setHeight(16);

        entity->setnstId(properties["id"]);

        if (properties["top"] != ""){
            entity->setTop(properties["top"].toDouble());
        }

        if (properties["left"] != ""){
            entity->setLeft(properties["left"].toDouble());
        }

        if (properties["width"] != ""){
            entity->setWidth(properties["width"].toDouble());
        }

        if (properties["height"] != ""){
            entity->setHeight(properties["height"].toDouble());
        }

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; ++narea;

        entity->adjust();

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);
        }
    }
}

void QnstView::changeArea(QnstGraphicsArea* entity, const QMap<QString, QString> properties)
{
    entity->setnstId(properties["id"]);

    if (properties["top"] != ""){
        entity->setTop(properties["top"].toDouble());
    }

    if (properties["left"] != ""){
        entity->setLeft(properties["left"].toDouble());
    }

    if (properties["width"] != ""){
        entity->setWidth(properties["width"].toDouble());
    }

    if (properties["height"] != ""){
        entity->setHeight(properties["height"].toDouble());
    }
}

void QnstView::addProperty(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{
    if (entities.contains(parent)){
        QnstGraphicsProperty* entity = new QnstGraphicsProperty(entities[parent]);
        entity->setnstUid(uid);
        entity->setnstGraphicsParent(entities[parent]);

        entity->setTop(0);
        entity->setLeft(0);
        entity->setWidth(16);
        entity->setHeight(16);

        entity->setnstId(properties["id"]);

        if (properties["top"] != ""){
            entity->setTop(properties["top"].toDouble());
        }

        if (properties["left"] != ""){
            entity->setLeft(properties["left"].toDouble());
        }

        if (properties["width"] != ""){
            entity->setWidth(properties["width"].toDouble());
        }

        if (properties["height"] != ""){
            entity->setHeight(properties["height"].toDouble());
        }

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; ++nproperty;

        entity->adjust();

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);
        }
    }
}

void QnstView::changeProperty(QnstGraphicsProperty* entity, const QMap<QString, QString> properties)
{
    entity->setnstId(properties["id"]);

    if (properties["top"] != ""){
        entity->setTop(properties["top"].toDouble());
    }

    if (properties["left"] != ""){
        entity->setLeft(properties["left"].toDouble());
    }

    if (properties["width"] != ""){
        entity->setWidth(properties["width"].toDouble());
    }

    if (properties["height"] != ""){
        entity->setHeight(properties["height"].toDouble());
    }
}

void QnstView::addLink(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    if (!links.contains(uid)){
        QnstLink* entity = new QnstLink();
        entity->setnstUid(uid);
        entity->setnstParent(entities[parent]);

        if (properties["id"] != ""){
            entity->setnstId(properties["id"]);
        }

        if (properties["xconnector"] != ""){
            entity->setxConnector(properties["xconnector"]);
            entity->setxConnectorUID(properties["xconnectorUID"]);
        }

        links[uid] = entity;

        adjustLink(entity);
    }
}

void QnstView::changeLink(QnstLink* entity, const QMap<QString, QString> properties)
{
    if (entity != NULL){
        if (properties["id"] != ""){
            entity->setnstId(properties["id"]);
        }

        if (properties["xconnector"] != ""){
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

void QnstView::addBind(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    if (links.contains(parent)){
        if (!binds.contains(uid)){
            QnstBind* entity = new QnstBind();
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

void QnstView::changeBind(QnstBind* entity, const QMap<QString, QString> properties)
{
    qDebug() << "========================== 001";

    if (entity != NULL){
        entity->setRole(properties["role"]);

        entity->setComponent(properties["component"]);

        entity->setComponentUid(properties["componentUid"]);

        entity->setInterface(properties["interface"]);

        entity->setInterfaceUid(properties["interfaceUid"]);

        adjustBind(entity);
    }
}

void QnstView::adjustBind(QnstBind* entity)
{
    if (entity != NULL){
        if (entity->getnstParent() != NULL){
            QnstLink* parent = (QnstLink*) entity->getnstParent();

            if (brelations.contains(entity->getnstUid())){

                QnstGraphicsEntity* graphics = entities[brelations[entity->getnstUid()]];

                if (graphics != NULL){

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

                            graparent->removenstGraphicsEntity(edge); delete (edge);

                            break;
                        }
                    }
                }

                brelations.remove(entity->getnstUid());
            }

            if (parent->getConditions().contains(entity->getnstUid())){
                parent->removeCondition(entity);
            }

            if (parent->getActions().contains(entity->getnstUid())){
                parent->removeAction(entity);
            }

            if (connectors.contains(parent->getxConnector())){
                QnstConncetor* connector = connectors[parent->getxConnector()];

                foreach(QString type, connector->getConditions()){
                    if (type == entity->getRole()){
                        parent->addCondition(entity);

                        break;
                    }
                }

                foreach(QString type, connector->getActions()){
                    if (type == entity->getRole()){
                        parent->addAction(entity);

                        break;
                    }
                }
            }

            if (parent->getConditions().contains(entity->getnstUid())){
                if (parent->getAggregatorUID() == ""){
                    QnstGraphicsEntity* node = (QnstGraphicsEntity*) parent->getnstParent();

                    QnstGraphicsAggregator* aggregator = new QnstGraphicsAggregator((QnstGraphicsNode*)node);
                    aggregator->setnstUid(parent->getnstUid());
                    aggregator->setTop(node->getHeight()/2 - 14/2);
                    aggregator->setLeft(node->getWidth()/2 - 14/2);
                    aggregator->setWidth(14);
                    aggregator->setHeight(14);
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
                                QnstGraphicsCondition* graphics = new QnstGraphicsCondition();
                                graphics->setnstGraphicsParent(parentb);
                                graphics->setEntityA(entitya);
                                graphics->setEntityB(entityb);
                                graphics->adjust();

                                parentb->addnstGraphicsEntity(graphics);

                                ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(graphics);
                                ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(graphics);

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
                                    graphics->setCondition(Qnst::NoConditionType);
                                }

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
                            QnstGraphicsCondition* graphics = new QnstGraphicsCondition();
                            graphics->setnstGraphicsParent(parenta);
                            graphics->setEntityA(entitya);
                            graphics->setEntityB(entityb);
                            graphics->adjust();

                            parenta->addnstGraphicsEntity(graphics);

                            ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(graphics);
                            ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(graphics);

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
                                graphics->setCondition(Qnst::NoConditionType);
                            }

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
                    aggregator->setTop(node->getHeight()/2 - 14/2);
                    aggregator->setLeft(node->getWidth()/2 - 14/2);
                    aggregator->setWidth(14);
                    aggregator->setHeight(14);
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
                                QnstGraphicsAction* graphics = new QnstGraphicsAction();
                                graphics->setnstGraphicsParent(parenta);
                                graphics->setEntityA(entitya);
                                graphics->setEntityB(entityb);
                                graphics->adjust();

                                parenta->addnstGraphicsEntity(graphics);

                                ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(graphics);
                                ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(graphics);

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
                                    graphics->setAction(Qnst::NoActionType);
                                }

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
                            QnstGraphicsAction* graphics = new QnstGraphicsAction();
                            graphics->setnstGraphicsParent(parentb);
                            graphics->setEntityA(entitya);
                            graphics->setEntityB(entityb);
                            graphics->adjust();

                            parentb->addnstGraphicsEntity(graphics);

                            ((QnstGraphicsNode*) entitya)->addnstGraphicsEdge(graphics);
                            ((QnstGraphicsNode*) entityb)->addnstGraphicsEdge(graphics);

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
                                graphics->setAction(Qnst::NoActionType);
                            }

                            entities[graphics->getnstUid()] = graphics;
                            brelations[entity->getnstUid()] = graphics->getnstUid();
                        }
                    }
                }
            }
        }
    }
}

void QnstView::addConnector(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    if (!connectors2.contains(uid)){
        QnstConncetor* entity = new QnstConncetor();
        entity->setnstUid(uid);

        if (properties["id"] != ""){
            entity->setName(properties["id"]);
            entity->setnstId(properties["id"]);
        }

        connectors2[uid] = entity;

        foreach(QnstConncetor* c, connectors.values()){
            if (c->getnstUid() ==  entity->getnstUid()){
                connectors.remove(connectors.key(c));
                break;
            }
        }

        if (entity->getnstId() != ""){
            connectors[entity->getnstId()] = entity;
        }
    }
}

void QnstView::changeConnector(QnstConncetor* entity, const QMap<QString, QString> properties)
{
    if (properties["id"] != ""){
        entity->setName(properties["id"]);
        entity->setnstId(properties["id"]);
    }

    foreach(QnstConncetor* c, connectors.values()){
        if (c->getnstUid() ==  entity->getnstUid()){
            connectors.remove(connectors.key(c));
            break;
        }
    }

    if (entity->getnstId() != ""){
        connectors[entity->getnstId()] = entity;
    }
}

void QnstView::addCondition(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    if (connectors2.contains(properties["connector"])){
        QnstConncetor* connector = connectors2[properties["connector"]];

        if (properties["role"] != ""){
            connector->addCondition(uid, properties["role"]);
        }
    }
}

void QnstView::changeCondition(QString uid, const QMap<QString, QString> properties)
{
    if (connectors2.contains(properties["connector"])){
        QnstConncetor* connector = connectors2[properties["connector"]];

        if (connector->getConditions().contains(uid)){
            connector->getConditions().remove(uid);
        }

        if (properties["role"] != ""){
            connector->addCondition(uid, properties["role"]);
        }
    }
}

void QnstView::addAction(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    if (connectors2.contains(properties["connector"])){
        QnstConncetor* connector = connectors2[properties["connector"]];

        if (properties["role"] != ""){
            connector->addAction(uid, properties["role"]);
        }
    }
}

void QnstView::changeAction(QString uid, const QMap<QString, QString> properties)
{
    if (connectors2.contains(properties["connector"])){
        QnstConncetor* connector = connectors2[properties["connector"]];

        if (connector->getActions().contains(uid)){
            connector->getActions().remove(uid);
        }

        if (properties["role"] != ""){
            connector->addAction(uid, properties["role"]);
        }
    }
}

void QnstView::addAggregator(const QString uid, const QString parent, const QMap<QString, QString> properties, bool undo)
{
    if (entities.contains(parent)){
        QnstGraphicsAggregator* entity = new QnstGraphicsAggregator((QnstGraphicsNode*) entities[parent]);
        entity->setnstUid(uid);

        entity->setTop(entities[parent]->getHeight()/2 - 14/2);
        entity->setLeft(entities[parent]->getWidth()/2 - 14/2);
        entity->setWidth(14);
        entity->setHeight(14);

        if (properties["id"] != ""){
            entity->setnstId(properties["id"]);
        }

        if (properties["top"] != ""){
            entity->setTop(properties["top"].toDouble());
        }

        if (properties["left"] != ""){
            entity->setLeft(properties["left"].toDouble());
        }

        if (properties["width"] != ""){
            entity->setWidth(properties["width"].toDouble());
        }

        if (properties["height"] != ""){
            entity->setHeight(properties["height"].toDouble());
        }

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; entity->adjust();

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);
        }
    }
}

void QnstView::requestEntityAddition(QnstGraphicsEntity* entity, bool undo)
{
    qDebug() << "[QNST]" << ":" << "Requesting entity addition '"+entity->getnstUid()+"'";

    if (entity != NULL){
        switch(entity->getnstType()){

        // if the entity type is BODY
        case Qnst::Body:
            if (undo){  scene->addRoot(entity); }

            requestBodyAddition((QnstGraphicsBody*) entity);
            break;

        // if the entity type is CONTEXT
        case Qnst::Context:
            requestContextAddition((QnstGraphicsContext*) entity, undo);
            break;

        // if the entity type is SWITCH
        case Qnst::Switch:
            requestSwitchAddition((QnstGraphicsSwitch*) entity, undo);
            break;

        // if the entity type is MEDIA
        case Qnst::Audio:
        case Qnst::Text:
        case Qnst::Video:
        case Qnst::Image:
        case Qnst::Script:
        case Qnst::Settings:
        case Qnst::Html:
        case Qnst::Media:
            requestMediaAddition((QnstGraphicsMedia*) entity, undo);
            break;

        // if the entity type is PORT
        case Qnst::Port:
            requestPortAddition((QnstGraphicsPort*) entity, undo);
            break;

        // if the entity type is AREA
        case Qnst::Area:
            requestAreaAddition((QnstGraphicsArea*) entity, undo);
            break;

        // if the entity type is PROPERTY
        case Qnst::Property:
            requestPropertyAddition((QnstGraphicsProperty*) entity);
            break;

        // if the entity type is AGGREGATOR
        case Qnst::Aggregator:
            requestAggregatorAddition((QnstGraphicsAggregator*) entity);
            break;
        }

        entities[entity->getnstUid()] = entity;

        if (!undo){
            QnstAddCommand* cmd = new QnstAddCommand(this, entity);
            history.push(cmd);

            qDebug() << "===================================" << "add-cmd";
        }
    }
}

void QnstView::requestEntityRemotion(QnstGraphicsEntity* entity, bool undo)
{
    qDebug() << "[QNST]" << ":" << "Requesting entity remotion '"+entity->getnstUid()+"'";

    if (entity != NULL){
        if (!undo){
            QnstRemoveCommand* cmd = new QnstRemoveCommand(this, entity);
            history.push(cmd);
        }

        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
            if (e->getnstType() != Qnst::Link &&
                e->getnstType() != Qnst::Edge &&
                e->getnstType() != Qnst::Condition &&
                e->getnstType() != Qnst::Action &&
                e->getnstType() != Qnst::Reference){

                requestEntityRemotion(e, true);
            }
        }

        if (entity->getncgType() == Qncg::Node){
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

                if (edge->getnstType() == Qnst::Condition || edge->getnstType() == Qnst::Action){
                    entities.remove(edge->getnstUid()); emit entityRemoved(edge->getnstUid());
                }

                QnstGraphicsEntity* parent = edge->getEntityA()->getnstGraphicsParent();

                parent->removenstGraphicsEntity(edge);

                if (entity->getnstType() == Qnst::Aggregator){
                    if (links.contains(entity->getnstUid())){
                        foreach(QnstBind* b, links[entity->getnstUid()]->getActions()){
                            binds.remove(b->getnstUid());
                        }

                        foreach(QnstBind* b, links[entity->getnstUid()]->getConditions()){
                            binds.remove(b->getnstUid());
                        }

                        link2conn.remove(links[entity->getnstUid()]->getnstId());
                        links.remove(entity->getnstUid());
                    }
                }

                entities.remove(edge->getnstUid());
            }

        }else if (entity->getncgType() == Qncg::Interface){
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

                if (edge->getnstType() == Qnst::Condition || edge->getnstType() == Qnst::Action){
                    entities.remove(edge->getnstUid()); emit entityRemoved(edge->getnstUid());
                }

                QnstGraphicsEntity* parent = edge->getEntityA()->getnstGraphicsParent();

                parent->removenstGraphicsEntity(edge);

                entities.remove(edge->getnstUid());
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

        entities.remove(entity->getnstUid());

        emit entityRemoved(entity->getnstUid());

        delete entity;
    }
}

void QnstView::requestEntityPreparation(QnstGraphicsEntity* entity, QMap<QString, QString> properties)
{
    QnstChangeCommand* cmd = new QnstChangeCommand(this, entity->getnstUid(), properties);
    history.push(cmd);

    qDebug() << "===================================" << "pr-cmd";
}

void QnstView::requestEntityChange(QnstGraphicsEntity* entity)
{
    qDebug() << "[QNST]" << ":" << "Requesting entity change '"+entity->getnstUid()+"'";

    if (entity != NULL){
        switch(entity->getnstType()){

        // if the entity type is BODY
        case Qnst::Body:
            requestBodyChange((QnstGraphicsBody*) entity);
            break;

        // if the entity type is CONTEXT
        case Qnst::Context:
            requestContextChange((QnstGraphicsContext*) entity);
            break;

        // if the entity type is SWITCH
        case Qnst::Switch:
            requestSwitchChange((QnstGraphicsSwitch*) entity);
            break;

        // if the entity type is MEDIA
        case Qnst::Audio:
        case Qnst::Text:
        case Qnst::Video:
        case Qnst::Image:
        case Qnst::Script:
        case Qnst::Settings:
        case Qnst::Media:
            requestMediaChange((QnstGraphicsMedia*) entity);
            break;

        // if the entity type is PORT
        case Qnst::Port:
            requestPortChange((QnstGraphicsPort*) entity);
            break;
        }
    }
}

void QnstView::requestEntitySelection(QnstGraphicsEntity* entity)
{
    qDebug() << "[QNST]" << ":" << "Requesting entity selection '"+entity->getnstUid()+"'";

    if (entity != NULL){
        if (entity->isSelected()){
            if (selected != NULL){
                selected->setSelected(false);
                selected->adjust();
            }

            selected = entity;

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

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestContextAddition(QnstGraphicsContext* entity, bool undo)
{
    if (entity->getnstId() == "" && !undo){
        entity->setnstId("ctx"+QString::number(++ncontext));
    }

    QMap<QString, QString> properties;

    properties["TYPE"] = "context";

    properties["id"] = entity->getnstId();

    properties["refer"] = "";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());
    properties["zindex"] = QString::number(entity->getzIndex());

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

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestSwitchAddition(QnstGraphicsSwitch* entity, bool undo)
{
    if (entity->getnstId() == "" && !undo){
        entity->setnstId("swt"+QString::number(++nswitch));
    }

    QMap<QString, QString> properties;

    properties["TYPE"] = "switch";

    properties["id"] = entity->getnstId();

    properties["refer"] = "";

    properties["top"] = QString::number(entity->getTop());
    properties["left"] = QString::number(entity->getLeft());
    properties["width"] = QString::number(entity->getWidth());
    properties["heigh"] = QString::number(entity->getHeight());
    properties["zindex"] = QString::number(entity->getzIndex());

    emit entityAdded(entity->getnstUid(), entity->getnstGraphicsParent()->getnstUid(), properties);
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

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestMediaAddition(QnstGraphicsMedia* entity, bool undo)
{
    if (entity->getnstId() == "" && !undo){
        entity->setnstId("m"+QString::number(++nmedia));
    }

    QMap<QString, QString> properties;

    properties["TYPE"] = "media";

    switch(entity->getnstType()){
    case Qnst::Image:
        properties["SUBTYPE"] = "image";

        properties["type"] = "image/?";

        break;

    case Qnst::Audio:
        properties["SUBTYPE"] = "audio";

        properties["type"] = "audio/?";

        break;

    case Qnst::Video:
        properties["SUBTYPE"] = "video";

        properties["type"] = "video/?";

        break;

    case Qnst::Text:
        properties["SUBTYPE"] = "text";

        properties["type"] = "text/?";

        break;

    case Qnst::Html:
        properties["SUBTYPE"] = "html";

        properties["type"] = "text/html";

        break;

    case Qnst::Script:
        properties["SUBTYPE"] = "script";

        properties["type"] = "application/x-ginga-NCLua";

        break;

    case Qnst::Settings:
        properties["SUBTYPE"] = "settings";

        properties["type"] = "application/x-ncl-settings";

        break;

    case Qnst::Media:
        properties["SUBTYPE"] = "media";
        break;
    }

    properties["id"] = entity->getnstId();

//    TODO:
    properties["src"] = entity->getSource();
//    properties["type"] = "";
//    properties["refer"] = "";
//    properties["instance"] = "";
//    properties["descriptor"] = "";

//    properties["top"] = QString::number(entity->getTop());
//    properties["left"] = QString::number(entity->getLeft());
//    properties["width"] = QString::number(entity->getWidth());
//    properties["heigh"] = QString::number(entity->getHeight());
//    properties["zindex"] = QString::number(entity->getzIndex());

    emit entityAdded(entity->getnstUid(), entity->getnstGraphicsParent()->getnstUid(), properties);
}

void QnstView::requestMediaChange(QnstGraphicsMedia* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "media";

    switch(entity->getnstType()){
    case Qnst::Image:
        properties["SUBTYPE"] = "image";
        break;

    case Qnst::Audio:
        properties["SUBTYPE"] = "audio";
        break;

    case Qnst::Video:
        properties["SUBTYPE"] = "video";
        break;

    case Qnst::Text:
        properties["SUBTYPE"] = "text";
        break;

    case Qnst::Script:
        properties["SUBTYPE"] = "script";
        break;

    case Qnst::Settings:
        properties["SUBTYPE"] = "settings";
        break;

    case Qnst::Media:
        properties["SUBTYPE"] = "media";
        break;

    case Qnst::Html:
        properties["SUBTYPE"] = "html";
        break;
    }

    properties["id"] = entity->getnstId();
    properties["src"] = entity->getSource();

//    properties["type"] = "";
//    properties["refer"] = "";
//    properties["instance"] = "";

//    properties["top"] = QString::number(entity->getTop());
//    properties["left"] = QString::number(entity->getLeft());
//    properties["width"] = QString::number(entity->getWidth());
//    properties["heigh"] = QString::number(entity->getHeight());
//    properties["zindex"] = QString::number(entity->getzIndex());

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestAggregatorAddition(QnstGraphicsAggregator* entity)
{
    entities[entity->getnstUid()] = entity;
}

void QnstView::requestPortAddition(QnstGraphicsPort* entity, bool undo)
{
    if (entity->getnstId() == "" && !undo){
        entity->setnstId("p"+QString::number(++nport));
    }

    QMap<QString, QString> properties;

    properties["TYPE"] = "port";

    properties["id"] = entity->getnstId();

    foreach(QnstGraphicsEdge* edge, entity->getnstGraphicsEdges()){
        if (edge->getnstType() == Qnst::Reference){
            properties["COMPONENT"] = edge->getEntityA()->getnstUid();
            properties["component"] = edge->getEntityA()->getnstId();

            if (edge->getEntityB()->getnstType() == Qnst::Port ||
                edge->getEntityB()->getnstType() == Qnst::Property ||
                edge->getEntityB()->getnstType() == Qnst::Area){

                properties["INTERFACE"] = edge->getEntityB()->getnstUid();;
                properties["interface"] = edge->getEntityB()->getnstId();;
            }
        }
    }

//    TODO:
//    properties["top"] = QString::number(entity->getTop());
//    properties["left"] = QString::number(entity->getLeft());
//    properties["width"] = QString::number(entity->getWidth());
//    properties["heigh"] = QString::number(entity->getHeight());
//    properties["zindex"] = QString::number(entity->getzIndex());

    emit entityAdded(entity->getnstUid(), entity->getnstGraphicsParent()->getnstUid(), properties);
}

void QnstView::requestPortChange(QnstGraphicsPort* entity)
{
    QMap<QString, QString> properties;

    properties["TYPE"] = "port";

    properties["id"] = entity->getnstId();

    foreach(QnstGraphicsEdge* edge, entity->getnstGraphicsEdges()){
        if (edge->getnstType() == Qnst::Reference){
            if (edge->getEntityB()->getnstType() == Qnst::Port ||
                edge->getEntityB()->getnstType() == Qnst::Property ||
                edge->getEntityB()->getnstType() == Qnst::Area){

                properties["INTERFACE"] = edge->getEntityB()->getnstUid();
                properties["interface"] = edge->getEntityB()->getnstId();

                properties["COMPONENT"] = edge->getEntityB()->getnstGraphicsParent()->getnstUid();
                properties["component"] = edge->getEntityB()->getnstGraphicsParent()->getnstId();

            }else{
                properties["COMPONENT"] = edge->getEntityB()->getnstUid();
                properties["component"] = edge->getEntityB()->getnstId();

                properties["INTERFACE"] = "";
                properties["interface"] = "";
            }
        }
    }

//    TODO:
//    properties["top"] = QString::number(entity->getTop());
//    properties["left"] = QString::number(entity->getLeft());
//    properties["width"] = QString::number(entity->getWidth());
//    properties["heigh"] = QString::number(entity->getHeight());
//    properties["zindex"] = QString::number(entity->getzIndex());

    emit entityChanged(entity->getnstUid(), properties);
}

void QnstView::requestAreaAddition(QnstGraphicsArea* entity, bool undo)
{
    if (entity->getnstId() == "" && !undo){
        entity->setnstId("area"+QString::number(++narea));
    }

    QMap<QString, QString> properties;

    properties["TYPE"] = "area";

    properties["id"] = entity->getnstId();

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

    emit entityAdded(entity->getnstUid(), entity->getnstGraphicsParent()->getnstUid(), properties);
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
    qDebug() << "============================ undo";

    if (history.canUndo()){
        history.undo();
    }
}

void QnstView::performRedo()
{
    qDebug() << "============================ redo";

    if (history.canRedo()){
        history.redo();
    }
}

void QnstView::performCut()
{
    if (selected != NULL){
        if (selected->getnstType() != Qnst::Body){
            if (clipboard != NULL){
                delete clipboard; clipboard = NULL;
            }

            QnstGraphicsEntity* entity = selected;

            switch(entity->getnstType()){

            // if the entity type is IMAGE
            case Qnst::Image:
                clipboard = new QnstGraphicsImage();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is AUDIO
            case Qnst::Audio:
                clipboard = new QnstGraphicsAudio();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is VIDEO
            case Qnst::Video:
                clipboard = new QnstGraphicsVideo();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is TEXT
            case Qnst::Text:
                clipboard = new QnstGraphicsText();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SCRIPT
            case Qnst::Script:
                clipboard = new QnstGraphicsScript();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SETTINGS
            case Qnst::Settings:
                clipboard = new QnstGraphicsSettings();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());
                break;

            // if the entity type is MEDIA
            case Qnst::Media:
                clipboard = new QnstGraphicsMedia();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is CONTEXT
            case Qnst::Context:
                clipboard = new QnstGraphicsContext();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SWITCH
            case Qnst::Switch:
                clipboard = new QnstGraphicsSwitch();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is PORT
            case Qnst::Port:
                clipboard = new QnstGraphicsPort();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;
            }

            clipboard->setnstGraphicsParent(NULL);

            foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
                performCopy(e, clipboard);
            }

            requestEntityRemotion(selected);

            foreach(QnstGraphicsEntity* e, entities.values()){
                if (e->getnstType() == Qnst::Body ||
                    e->getnstType() == Qnst::Context ||
                    e->getnstType() == Qnst::Switch){

                    e->menu->actionPaste->setEnabled(true);
                }
            }

        }else{
            QMessageBox::warning(this, "Alert", "You cannot cut a &lt;body&gt; element!");
        }
    }
}

void QnstView::performCopy()
{
    if (selected != NULL){
        if (selected->getnstType() != Qnst::Body){

            if (clipboard != NULL){
                delete clipboard; clipboard = NULL;
            }

            QnstGraphicsEntity* entity = selected;

            switch(entity->getnstType()){

            // if the entity type is IMAGE
            case Qnst::Image:
                clipboard = new QnstGraphicsImage();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is AUDIO
            case Qnst::Audio:
                clipboard = new QnstGraphicsAudio();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is VIDEO
            case Qnst::Video:
                clipboard = new QnstGraphicsVideo();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is TEXT
            case Qnst::Text:
                clipboard = new QnstGraphicsText();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SCRIPT
            case Qnst::Script:
                clipboard = new QnstGraphicsScript();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SETTINGS
            case Qnst::Settings:
                clipboard = new QnstGraphicsSettings();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());
                break;

            // if the entity type is MEDIA
            case Qnst::Media:
                clipboard = new QnstGraphicsMedia();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is CONTEXT
            case Qnst::Context:
                clipboard = new QnstGraphicsContext();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SWITCH
            case Qnst::Switch:
                clipboard = new QnstGraphicsSwitch();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is PORT
            case Qnst::Port:
                clipboard = new QnstGraphicsPort();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is AREA
            case Qnst::Area:
                clipboard = new QnstGraphicsArea();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is PROPERTY
            case Qnst::Property:
                clipboard = new QnstGraphicsProperty();
//                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;
            }

            clipboard->setnstGraphicsParent(NULL);

            foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
                performCopy(e, clipboard);
            }

            foreach(QnstGraphicsEntity* e, entities.values()){
                if (e->getnstType() == Qnst::Body ||
                    e->getnstType() == Qnst::Context ||
                    e->getnstType() == Qnst::Switch){

                    e->menu->actionPaste->setEnabled(true);
                }
            }

        }else{
            QMessageBox::warning(this, "Alert", "You cannot copy a &lt;body&gt; element!");
        }
    }
}

void QnstView::performCopy(QnstGraphicsEntity* entity, QnstGraphicsEntity* parent)
{
    QnstGraphicsEntity* copy;

    switch(entity->getnstType()){

    // if the entity type is IMAGE
    case Qnst::Image:
        copy = new QnstGraphicsImage();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is AUDIO
    case Qnst::Audio:
        copy = new QnstGraphicsAudio();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is VIDEO
    case Qnst::Video:
        copy = new QnstGraphicsVideo();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is TEXT
    case Qnst::Text:
        copy = new QnstGraphicsText();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is SCRIPT
    case Qnst::Script:
        copy = new QnstGraphicsScript();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is SETTINGS
    case Qnst::Settings:
        copy = new QnstGraphicsSettings();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is MEDIA
    case Qnst::Media:
        copy = new QnstGraphicsMedia();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is CONTEXT
    case Qnst::Context:
        copy = new QnstGraphicsContext();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is SWITCH
    case Qnst::Switch:
        copy = new QnstGraphicsSwitch();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is PORT
    case Qnst::Port:
        copy = new QnstGraphicsPort();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is AREA
    case Qnst::Area:
        copy = new QnstGraphicsArea();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;

    // if the entity type is PROPERTY
    case Qnst::Property:
        copy = new QnstGraphicsProperty();
        copy->setnstGraphicsParent(parent);

        copy->setnstId(entity->getnstId());

        copy->setTop(entity->getTop());
        copy->setLeft(entity->getLeft());
        copy->setWidth(entity->getWidth());
        copy->setHeight(entity->getHeight());

        parent->addnstGraphicsEntity(copy);

        break;
    }

    foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
        performCopy(e, copy);
    }
}

void QnstView::performPaste()
{
    QnstGraphicsEntity* copy = clipboard;

    QnstGraphicsEntity* parent = selected;

    if (copy != NULL && parent != NULL){
        if ((parent->getnstType() != Qnst::Image &&
               parent->getnstType() != Qnst::Audio &&
               parent->getnstType() != Qnst::Video &&
               parent->getnstType() != Qnst::Text &&
               parent->getnstType() != Qnst::Script &&
               parent->getnstType() != Qnst::Settings &&
               parent->getnstType() != Qnst::Area &&
               parent->getnstType() != Qnst::Property &&
               parent->getnstType() != Qnst::Media) ||
            (parent->getnstType() != Qnst::Area &&
               parent->getnstType() != Qnst::Property &&
                  (copy->getnstType() == Qnst::Area ||
                      copy->getnstType() == Qnst::Property))){

            QnstGraphicsEntity* entity;

            switch(copy->getnstType()){

            // if the entity type is IMAGE
            case Qnst::Image:
                entity = new QnstGraphicsImage();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 48/2);
                entity->setLeft(parent->getWidth()/2 - 48/2);
                entity->setWidth(48);
                entity->setHeight(64);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is AUDIO
            case Qnst::Audio:
                entity = new QnstGraphicsAudio();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 48/2);
                entity->setLeft(parent->getWidth()/2 - 48/2);
                entity->setWidth(48);
                entity->setHeight(64);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is VIDEO
            case Qnst::Video:
                entity = new QnstGraphicsVideo();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 48/2);
                entity->setLeft(parent->getWidth()/2 - 48/2);
                entity->setWidth(48);
                entity->setHeight(64);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is TEXT
            case Qnst::Text:
                entity = new QnstGraphicsText();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 48/2);
                entity->setLeft(parent->getWidth()/2 - 48/2);
                entity->setWidth(48);
                entity->setHeight(64);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is SCRIPT
            case Qnst::Script:
                entity = new QnstGraphicsScript();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 48/2);
                entity->setLeft(parent->getWidth()/2 - 48/2);
                entity->setWidth(48);
                entity->setHeight(64);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is SETTINGS
            case Qnst::Settings:
                entity = new QnstGraphicsSettings();
                entity->setnstGraphicsParent(parent);

                entity->setnstId(copy->getnstId());

                entity->setTop(parent->getHeight()/2 - 48/2);
                entity->setLeft(parent->getWidth()/2 - 48/2);
                entity->setWidth(48);
                entity->setHeight(64);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is MEDIA
            case Qnst::Media:
                entity = new QnstGraphicsMedia();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 48/2);
                entity->setLeft(parent->getWidth()/2 - 48/2);
                entity->setWidth(48);
                entity->setHeight(64);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is CONTEXT
            case Qnst::Context:
                entity = new QnstGraphicsContext();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("ctx"+QString::number(++ncontext));

                entity->setTop(parent->getHeight()/2 - copy->getWidth()/2);
                entity->setLeft(parent->getWidth()/2 - copy->getHeight()/2);
                entity->setWidth(copy->getWidth());
                entity->setHeight(copy->getHeight());

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is SWITCH
            case Qnst::Switch:
                entity = new QnstGraphicsSwitch();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("swt"+QString::number(++nswitch));

                entity->setTop(parent->getHeight()/2 - copy->getWidth()/2);
                entity->setLeft(parent->getWidth()/2 - copy->getHeight()/2);
                entity->setWidth(copy->getWidth());
                entity->setHeight(copy->getHeight());

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is PORT
            case Qnst::Port:
                entity = new QnstGraphicsPort();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("p"+QString::number(++nport));

                entity->setTop(copy->getTop());
                entity->setLeft(copy->getLeft());
                entity->setWidth(copy->getWidth());
                entity->setHeight(copy->getHeight());
                entity->adjust();

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is AREA
            case Qnst::Area:
                entity = new QnstGraphicsArea();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("area"+QString::number(++narea));

                entity->setTop(copy->getTop());
                entity->setLeft(copy->getLeft());
                entity->setWidth(copy->getWidth());
                entity->setHeight(copy->getHeight());
                entity->adjust();

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is PROPERTY
            case Qnst::Property:
                entity = new QnstGraphicsProperty();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("prop"+QString::number(++nproperty));

                entity->setTop(copy->getTop());
                entity->setLeft(copy->getLeft());
                entity->setWidth(copy->getWidth());
                entity->setHeight(copy->getHeight());
                entity->adjust();

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;
            }

            foreach(QnstGraphicsEntity* e, copy->getnstGraphicsEntities()){
                performPaste(e, entity);
            }
        }
    }
}

void QnstView::performPaste(QnstGraphicsEntity* copy, QnstGraphicsEntity* parent)
{
    QnstGraphicsEntity* entity;

    switch(copy->getnstType()){

    // if the entity type is IMAGE
    case Qnst::Image:
        entity = new QnstGraphicsImage();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("m"+QString::number(++nmedia));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is AUDIO
    case Qnst::Audio:
        entity = new QnstGraphicsAudio();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("m"+QString::number(++nmedia));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is VIDEO
    case Qnst::Video:
        entity = new QnstGraphicsVideo();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("m"+QString::number(++nmedia));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is TEXT
    case Qnst::Text:
        entity = new QnstGraphicsText();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("m"+QString::number(++nmedia));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is SCRIPT
    case Qnst::Script:
        entity = new QnstGraphicsScript();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("m"+QString::number(++nmedia));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is SETTINGS
    case Qnst::Settings:
        entity = new QnstGraphicsSettings();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("m"+QString::number(++nmedia));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is MEDIA
    case Qnst::Media:
        entity = new QnstGraphicsMedia();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("m"+QString::number(++nmedia));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is CONTEXT
    case Qnst::Context:
        entity = new QnstGraphicsContext();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("ctx"+QString::number(++ncontext));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is SWITCH
    case Qnst::Switch:
        entity = new QnstGraphicsSwitch();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("swt"+QString::number(++nswitch));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is PORT
    case Qnst::Port:
        entity = new QnstGraphicsPort();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("p"+QString::number(++nport));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is AREA
    case Qnst::Area:
        entity = new QnstGraphicsArea();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("area"+QString::number(++narea));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());
        entity->adjust();

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;

    // if the entity type is PROPERTY
    case Qnst::Property:
        entity = new QnstGraphicsProperty();
        entity->setnstGraphicsParent(parent);

        entity->setnstId("prop"+QString::number(++nproperty));

        entity->setTop(copy->getTop());
        entity->setLeft(copy->getLeft());
        entity->setWidth(copy->getWidth());
        entity->setHeight(copy->getHeight());
        entity->adjust();

        parent->addnstGraphicsEntity(entity);

        requestEntityAddition(entity);

        break;
    }

    foreach(QnstGraphicsEntity* e, copy->getnstGraphicsEntities()){
        performPaste(e, entity);
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

        foreach (QnstGraphicsEntity* entity, scene->getRoots()) {
            if (entity->getTop() > top){
                top = entity->getTop();
            }

            if (entity->getLeft() > left){
                left = entity->getLeft();
            }

            if (entity->getWidth() < width){
                width = entity->getWidth();
            }

            if (entity->getHeight() < height){
                height = entity->getHeight();
            }
        }

        QImage image(width, height, QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&image);

        scene->render(&painter, QRect(), QRect(left-25,top-25,width+50,height+50));

        painter.end();

        image.save(location);
    }
}

void QnstView::performZoomIn()
{
    if (zoomStep > 0) {
        zoomStep--;

        resetMatrix();

        scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
    }
}

void QnstView::performZoomOut()
{
    if (zoomStep*0.05 < 0.9){
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
    if (linking){
        link->setLine(QLineF(link->getLine().p1(), mapToScene(event->pos())));
    }

    QGraphicsView::mouseMoveEvent(event);
}

void QnstView::mousePressEvent(QMouseEvent* event)
{
    if (modified){
        if (link != NULL){
            delete (link); link = NULL;
        }

        link = new QnstViewLink(); scene->addItem(link);
        link->setLine(QLineF(mapToScene(event->pos()), mapToScene(event->pos())));
        link->adjust();

        linking = true;
    }else{
        QGraphicsView::mousePressEvent(event);

        if (!event->isAccepted()){
            if (event->button() == Qt::LeftButton){
                if (selected != NULL){
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
        }

        if (link != NULL){
            scene->removeItem(link); scene->update(); delete link; link = NULL;
        }

        linking = false;
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void QnstView::addNodetoNodeEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb)
{
    QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
    QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

    if (parenta != NULL && parentb != NULL){
        if (parenta == parentb){
            qDebug() << "NODE to NODE:" << entitya->getnstUid() << "->" << entityb->getnstUid();

            if (entitya->getnstType() == Qnst::Aggregator){
                actionDialog->init(connectors, link2conn);

                if (actionDialog->exec()){
                    QString link = actionDialog->form.cbLink->currentText();
                    QString act = actionDialog->form.cbAction->currentText();

                    QnstGraphicsAction* entity = new QnstGraphicsAction();
                    entity->setnstGraphicsParent(parenta);
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);
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
                        entity->setAction(Qnst::NoActionType);
                    }

                    ///// connector

                    QString connName = actionDialog->form.cbConnector->currentText();;

                    if (actionDialog->form.cbConnector->currentText() == "New..."){
                        QnstConncetor* conn = new QnstConncetor();
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
                conditionDialog->init(connectors, link2conn);

                if (conditionDialog->exec()){
                    QString conn = conditionDialog->form.cbConnector->currentText();
                    QString link = conditionDialog->form.cbLink->currentText();
                    QString cond = conditionDialog->form.cbCondition->currentText();

                    QnstGraphicsCondition* entity = new QnstGraphicsCondition();
                    entity->setnstGraphicsParent(parenta);
                    entity->setEntityA(entitya);
                    entity->setEntityB(entityb);
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
                        entity->setCondition(Qnst::NoConditionType);
                    }

                    ///// connector

                    QString connName = conditionDialog->form.cbConnector->currentText();;

                    if (conditionDialog->form.cbConnector->currentText() == "New..."){
                        QnstConncetor* conn = new QnstConncetor();
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
                        aggregator->setTop(ycenter - 14/2);
                        aggregator->setLeft(xcenter - 14/2);
                        aggregator->setWidth(14);
                        aggregator->setHeight(14);
                        aggregator->adjust();

                        parenta->addnstGraphicsEntity(aggregator);

                        entities[aggregator->getnstUid()] = aggregator;

                        ///

                        QnstGraphicsCondition* condition = new QnstGraphicsCondition();
                        condition->setnstGraphicsParent(parenta);
                        condition->setEntityA(entitya);
                        condition->setEntityB(aggregator);
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
                            condition->setCondition(Qnst::NoConditionType);
                        }

                        entities[condition->getnstUid()] = condition;

                        ///

                        QnstGraphicsAction* action = new QnstGraphicsAction();
                        action->setnstGraphicsParent(parenta);
                        action->setEntityA(aggregator);
                        action->setEntityB(entityb);
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
                            action->setAction(Qnst::NoActionType);
                        }

                        entities[action->getnstUid()] = action;

                        ///// connector

                        QString connName = linkDialog->form.cbConnector->currentText();;

                        if (linkDialog->form.cbConnector->currentText() == "New..."){
                            QnstConncetor* conn = new QnstConncetor();
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
            //////////////////////////////////////////
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
                    aggregator->setTop(ycenter - 14/2);
                    aggregator->setLeft(xcenter - 14/2);
                    aggregator->setWidth(14);
                    aggregator->setHeight(14);
                    aggregator->adjust();

                    parenta->addnstGraphicsEntity(aggregator);

                    entities[aggregator->getnstUid()] = aggregator;

                    ///

                    QnstGraphicsCondition* condition = new QnstGraphicsCondition();
                    condition->setnstGraphicsParent(parenta);
                    condition->setEntityA(entitya);
                    condition->setEntityB(aggregator);
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
                        condition->setCondition(Qnst::NoConditionType);
                    }

                    entities[condition->getnstUid()] = condition;

                    ///

                    QnstGraphicsAction* action = new QnstGraphicsAction();
                    action->setnstGraphicsParent(parenta);
                    action->setEntityA(aggregator);
                    action->setEntityB(entityb);
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
                        action->setAction(Qnst::NoActionType);
                    }

                    entities[action->getnstUid()] = action;

                    ///// connector

                    QString connName = linkDialog->form.cbConnector->currentText();;

                    if (linkDialog->form.cbConnector->currentText() == "New..."){
                        QnstConncetor* conn = new QnstConncetor();
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
            modified = false;
        }
    }
}

void QnstView::addInterfacetoNodeEdge(QnstGraphicsEntity* entitya, QnstGraphicsEntity* entityb)
{
    if (entitya->getnstType() == Qnst::Port){
        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

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
            }
        }

    }else{
        QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
        QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

        if (parenta != NULL && parentb != NULL){
            if (parenta->getnstGraphicsParent() == parentb){
                //////////////////////////////////////////
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
                        aggregator->setTop(ycenter - 14/2);
                        aggregator->setLeft(xcenter - 14/2);
                        aggregator->setWidth(14);
                        aggregator->setHeight(14);
                        aggregator->adjust();

                        parentb->addnstGraphicsEntity(aggregator);

                        entities[aggregator->getnstUid()] = aggregator;

                        ///

                        QnstGraphicsCondition* condition = new QnstGraphicsCondition();
                        condition->setnstGraphicsParent(parentb);
                        condition->setEntityA(entitya);
                        condition->setEntityB(aggregator);
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
                            condition->setCondition(Qnst::NoConditionType);
                        }

                        entities[condition->getnstUid()] = condition;

                        ///

                        QnstGraphicsAction* action = new QnstGraphicsAction();
                        action->setnstGraphicsParent(parentb);
                        action->setEntityA(aggregator);
                        action->setEntityB(entityb);
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
                            action->setAction(Qnst::NoActionType);
                        }

                        entities[action->getnstUid()] = action;

                        ///// connector

                        QString connName = linkDialog->form.cbConnector->currentText();;

                        if (linkDialog->form.cbConnector->currentText() == "New..."){
                            QnstConncetor* conn = new QnstConncetor();
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
                modified = false;
            }
        }
    }
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
                        aggregator->setTop(ycenter - 14/2);
                        aggregator->setLeft(xcenter - 14/2);
                        aggregator->setWidth(14);
                        aggregator->setHeight(14);
                        aggregator->adjust();

                        parents->addnstGraphicsEntity(aggregator);

                        entities[aggregator->getnstUid()] = aggregator;

                        ///

                        QnstGraphicsCondition* condition = new QnstGraphicsCondition();
                        condition->setnstGraphicsParent(parents);
                        condition->setEntityA(entitya);
                        condition->setEntityB(aggregator);
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
                            condition->setCondition(Qnst::NoConditionType);
                        }

                        entities[condition->getnstUid()] = condition;

                        ///

                        QnstGraphicsAction* action = new QnstGraphicsAction();
                        action->setnstGraphicsParent(parents);
                        action->setEntityA(aggregator);
                        action->setEntityB(entityb);
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
                            action->setAction(Qnst::NoActionType);
                        }

                        entities[action->getnstUid()] = action;

                        ///// connector

                        QString connName = linkDialog->form.cbConnector->currentText();;

                        if (linkDialog->form.cbConnector->currentText() == "New..."){
                            QnstConncetor* conn = new QnstConncetor();
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
                    aggregator->setTop(ycenter - 14/2);
                    aggregator->setLeft(xcenter - 14/2);
                    aggregator->setWidth(14);
                    aggregator->setHeight(14);
                    aggregator->adjust();

                    parents->addnstGraphicsEntity(aggregator);

                    entities[aggregator->getnstUid()] = aggregator;

                    ///

                    QnstGraphicsCondition* condition = new QnstGraphicsCondition();
                    condition->setnstGraphicsParent(parents);
                    condition->setEntityA(entitya);
                    condition->setEntityB(aggregator);
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
                        condition->setCondition(Qnst::NoConditionType);
                    }

                    entities[condition->getnstUid()] = condition;

                    ///

                    QnstGraphicsAction* action = new QnstGraphicsAction();
                    action->setnstGraphicsParent(parents);
                    action->setEntityA(aggregator);
                    action->setEntityB(entityb);
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
                        action->setAction(Qnst::NoActionType);
                    }

                    entities[action->getnstUid()] = action;

                    ///// connector

                    QString connName = linkDialog->form.cbConnector->currentText();;

                    if (linkDialog->form.cbConnector->currentText() == "New..."){
                        QnstConncetor* conn = new QnstConncetor();
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
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_X){
        performCut();

    // CTRL+C - Copy
    }else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C){
        performCopy();

    // CTRL+V - Paste
    }else if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_V){
        performPaste();

    // DELETE - Delete
    }else if (event->key() == Qt::Key_Delete){
        performDelete();

    // BACKSPACE - Delete
    }else if (event->key() == Qt::Key_Backspace){
        performDelete();

    // SHIFT - Enabling liking
    }else if (event->key() == Qt::Key_Shift){
        if (selected != NULL){
            selected->setSelected(false);
            selected->adjust();
        }

        selected = NULL;

        modified = true;
    }
    //Ctrl + 0 -> reset to default zoom
    else if( event->modifiers() == Qt::ControlModifier &&
             event->key() == Qt::Key_0)
    {
      performZoomReset();
    }
    //Ctrl + + -> perform zoom in
    else if((event->modifiers() == Qt::ControlModifier ||
             event->modifiers() == Qt::ShiftModifier | Qt::ControlModifier) &&
             event->key() == Qt::Key_Plus)
    {
      performZoomIn();
    }
    //Ctrl + - -> perform zoom out
    else if(event->modifiers() == Qt::ControlModifier &&
            event->key() == Qt::Key_Minus)
    {
      performZoomOut();
    }
    //Ctrl + Z
    else if(event->modifiers() == Qt::ControlModifier &&
            event->key() == Qt::Key_Z)
    {
        performUndo();

    //Ctrl + Shift + Z
    }else if(event->modifiers() == (Qt::ControlModifier + Qt::ShiftModifier) &&
                                    event->key() == Qt::Key_Z){
        performRedo();
    }

    QGraphicsView::keyPressEvent(event);
}

void QnstView::keyReleaseEvent(QKeyEvent *event)
{
    // SHIFT - Disabling liking
    if (event->key() == Qt::Key_Shift){
        modified = false;
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
    // call the father wheelEvent
    QGraphicsView::wheelEvent(event);
  }
}

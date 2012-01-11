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

        addBody(uid, "", properties);

    }else if (element.nodeName() == "media"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");
        properties["type"] = element.attribute("type");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addMedia(uid, parent.attribute("uid"), properties);

    }else if (element.nodeName() == "context"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addContext(uid, parent.attribute("uid"), properties);

    }else if (element.nodeName() == "switch"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addSwitch(uid, parent.attribute("uid"), properties);

    }else if (element.nodeName() == "aggregator"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addAggregator(uid, parent.attribute("uid"), properties);

    }else if (element.nodeName() == "port"){
        QString uid = element.attribute("uid");

        QMap<QString, QString> properties;

        properties["id"] = element.attribute("id");

        properties["top"] = element.attribute("top");
        properties["left"] = element.attribute("left");
        properties["width"] = element.attribute("width");
        properties["height"] = element.attribute("height");

        addPort(uid, parent.attribute("uid"), properties);

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

        break;

    case Qnst::Image:
        e = dom->createElement("media");

        e.setAttribute("type", "image");

        break;

    case Qnst::Video:
        e = dom->createElement("media");

        e.setAttribute("type", "video");

        break;

    case Qnst::Text:
        e = dom->createElement("media");

        e.setAttribute("type", "text");

        break;

    case Qnst::Settings:
        e = dom->createElement("media");

        e.setAttribute("type", "settings");

        break;

    case Qnst::Audio:
        e = dom->createElement("media");

        e.setAttribute("type", "audio");

        break;

    case Qnst::Script:
        e = dom->createElement("media");

        e.setAttribute("type", "script");

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
    }
}

void QnstView::removeEntity(const QString uid)
{
    qDebug() << "[QNST]" << ":" << "Removing entity '"+uid+"'";

    if (entities.contains(uid)){
        QnstGraphicsEntity* entity = entities[uid];

        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
            removeEntity(e->getnstUid());
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

                QnstGraphicsEntity* parenta = edge->getEntityA()->getnstGraphicsParent();
                QnstGraphicsEntity* parentb = edge->getEntityB()->getnstGraphicsParent();

                if (parenta != parentb){
                    parenta->getnstGraphicsParent()->removenstGraphicsEntity(edge);
                }else{
                    parenta->removenstGraphicsEntity(edge);
                }
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

                QnstGraphicsEntity* parenta = edge->getEntityA()->getnstGraphicsParent();
                QnstGraphicsEntity* parentb = edge->getEntityB()->getnstGraphicsParent();

                if (parenta != parentb){
                    parenta->getnstGraphicsParent()->removenstGraphicsEntity(edge);
                }else{
                    parenta->removenstGraphicsEntity(edge);
                }
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

        entities.remove(uid); delete entity;
    }
}

void QnstView::changeEntity(const QString uid, const QMap<QString, QString> properties)
{
    qDebug() << "[QNST]" << ":" << "Changing entity '"+uid+"'";

    if (entities.contains(uid)){
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
            changeMedia((QnstGraphicsMedia*) entity, properties);
            break;

        // if the entity type is PORT
        case Qnst::Port:
            changePort((QnstGraphicsPort*) entity, properties);
            break;

        // if the entity type is LINK
        case Qnst::Link:
            changeLink((QnstLink*) entity, properties);
            break;

        // if the entity type is BIND
        case Qnst::Bind:
            changeBind((QnstBind*) entity, properties);
            break;
        }
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

void QnstView::addBody(const QString uid, const QString parent, const QMap<QString, QString> properties)
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
}

void QnstView::changeBody(QnstGraphicsBody* entity, const QMap<QString, QString> properties)
{
    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
    }
}

void QnstView::addContext(const QString uid, const QString parent, const QMap<QString, QString> properties)
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

void QnstView::addSwitch(const QString uid, const QString parent, const QMap<QString, QString> properties)
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

void QnstView::addMedia(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    if (entities.contains(parent)){
        QnstGraphicsMedia* entity;

        if (properties["type"].startsWith("image")){
            entity = new QnstGraphicsImage((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"].startsWith("audio")){
            entity = new QnstGraphicsAudio((QnstGraphicsNode*) entities[parent]);

        }else if (properties["type"].startsWith("video")){
            entity = new QnstGraphicsVideo((QnstGraphicsNode*) entities[parent]);

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

        entity->setTop(entities[parent]->getHeight()/2 - 56/2);
        entity->setLeft(entities[parent]->getWidth()/2 - 56/2);
        entity->setWidth(56);
        entity->setHeight(72);

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

        ++nmedia;

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; entity->adjust();

        adjustMedia(entity);
    }
}

void QnstView::changeMedia(QnstGraphicsMedia* entity, const QMap<QString, QString> properties)
{
    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
    }

//    TODO:
//    if (properties["src"] != ""){
//
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

//    if (properties["refer"] != ""){
//
//    }

//    if (properties["top"] != ""){
//
//    }

//    if (properties["id"] != ""){
//
//    }

//    if (properties["left"] != ""){
//
//    }

//    if (properties["width"] != ""){
//
//    }

//    if (properties["heigh"] != ""){
//
//    }

//    if (properties["zindex"] != ""){
//
//    }

    adjustMedia(entity);
}

void QnstView::adjustMedia(QnstGraphicsMedia* entity)
{

}

void QnstView::addPort(const QString uid, const QString parent, const QMap<QString, QString> properties)
{ 
    if (entities.contains(parent)){
        QnstGraphicsPort* entity = new QnstGraphicsPort((QnstGraphicsNode*) entities[parent]);
        entity->setnstUid(uid);

        entity->setTop(0);
        entity->setLeft(0);
        entity->setWidth(32);
        entity->setHeight(32);

        if (properties["id"] != ""){
            entity->setnstId(properties["id"]);
        }

        if (properties["component"] != ""){
            entity->setComponent(properties["component"]);
        }

        if (properties["interface"] != ""){
            entity->setInterface(properties["interface"]);
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

        if (properties["interfaceUid"] != ""){
            entity->setInterfaceUid(properties["interfaceUid"]);

        }

        if (properties["componentUid"] != ""){
            entity->setComponentUid(properties["componentUid"]);
        }

        entities[parent]->addnstGraphicsEntity(entity); entities[uid] = entity; ++nport;

        adjustPort(entity);
    }
}

void QnstView::changePort(QnstGraphicsPort* entity, const QMap<QString, QString> properties)
{
    if (properties["id"] != ""){
        entity->setnstId(properties["id"]);
    }

    if (properties["component"] != ""){
        entity->setComponent(properties["component"]);
    }

    if (properties["interface"] != ""){
        entity->setInterface(properties["interface"]);
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

    if (properties["interfaceUid"] != ""){
        entity->setInterfaceUid(properties["interfaceUid"]);

    }

    if (properties["componentUid"] != ""){
        entity->setComponentUid(properties["componentUid"]);
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

void QnstView::addLink(const QString uid, const QString parent, const QMap<QString, QString> properties)
{

}

void QnstView::changeLink(QnstLink* entity, const QMap<QString, QString> properties)
{

}

void QnstView::adjustLink(QnstLink* entity)
{

}

void QnstView::addBind(const QString uid, const QString parent, const QMap<QString, QString> properties)
{

}

void QnstView::changeBind(QnstBind* entity, const QMap<QString, QString> properties)
{

}

void QnstView::adjustBind(QnstBind* entity)
{

}

void QnstView::addAggregator(const QString uid, const QString parent, const QMap<QString, QString> properties)
{
    if (entities.contains(parent)){
        QnstGraphicsAggregator* entity = new QnstGraphicsAggregator((QnstGraphicsNode*) entities[parent]);
        entity->setnstUid(uid);

        entity->setTop(entities[parent]->getHeight()/2 - 28/2);
        entity->setLeft(entities[parent]->getWidth()/2 - 28/2);
        entity->setWidth(28);
        entity->setHeight(28);

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
    }
}

void QnstView::requestEntityAddition(QnstGraphicsEntity* entity)
{
    qDebug() << "[QNST]" << ":" << "Requesting entity addition '"+entity->getnstUid()+"'";

    if (entity != NULL){
        switch(entity->getnstType()){

        // if the entity type is BODY
        case Qnst::Body:
            requestBodyAddition((QnstGraphicsBody*) entity);
            break;

        // if the entity type is CONTEXT
        case Qnst::Context:
            requestContextAddition((QnstGraphicsContext*) entity);
            break;

        // if the entity type is SWITCH
        case Qnst::Switch:
            requestSwitchAddition((QnstGraphicsSwitch*) entity);
            break;

        // if the entity type is MEDIA
        case Qnst::Audio:
        case Qnst::Text:
        case Qnst::Video:
        case Qnst::Image:
        case Qnst::Script:
        case Qnst::Settings:
        case Qnst::Media:
            requestMediaAddition((QnstGraphicsMedia*) entity);
            break;

        // if the entity type is PORT
        case Qnst::Port:
            requestPortAddition((QnstGraphicsPort*) entity);
            break;

        // if the entity type is AGGREGATOR
        case Qnst::Aggregator:
            requestAggregatorAddition((QnstGraphicsAggregator*) entity);
            break;
        }

        entities[entity->getnstUid()] = entity;
    }
}

void QnstView::requestEntityRemotion(QnstGraphicsEntity* entity)
{
    qDebug() << "[QNST]" << ":" << "Requesting entity remotion '"+entity->getnstUid()+"'";

    if (entity != NULL){
        foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
            if (entity->getnstType() != Qnst::Link &&
                entity->getnstType() != Qnst::Edge &&
                entity->getnstType() != Qnst::Reference){

                requestEntityRemotion(e);
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

                if (edge->getnstType() == Qnst::Link){
                    entities.remove(edge->getnstUid()); emit entityRemoved(edge->getnstUid());
                }

                QnstGraphicsEntity* parent = edge->getEntityA()->getnstGraphicsParent();

                parent->removenstGraphicsEntity(edge);
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

                if (edge->getnstType() == Qnst::Link){
                    entities.remove(edge->getnstUid()); emit entityRemoved(edge->getnstUid());
                }

                QnstGraphicsEntity* parent = edge->getEntityA()->getnstGraphicsParent();

                parent->removenstGraphicsEntity(edge);
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

        emit entityRemoved(entity->getnstUid()); delete entity;
    }
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

    properties["id"] = "";

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

void QnstView::requestContextAddition(QnstGraphicsContext* entity)
{
    if (entity->getnstId() == ""){
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

void QnstView::requestSwitchAddition(QnstGraphicsSwitch* entity)
{
    if (entity->getnstId() == ""){
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

void QnstView::requestMediaAddition(QnstGraphicsMedia* entity)
{
    if (entity->getnstId() == ""){
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
//    properties["src"] = "";
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
        properties["TYPE"] = "image";
        break;

    case Qnst::Audio:
        properties["TYPE"] = "audio";
        break;

    case Qnst::Video:
        properties["TYPE"] = "video";
        break;

    case Qnst::Text:
        properties["TYPE"] = "text";
        break;

    case Qnst::Script:
        properties["TYPE"] = "script";
        break;

    case Qnst::Settings:
        properties["TYPE"] = "settings";
        break;

    case Qnst::Media:
        properties["TYPE"] = "media";
        break;
    }

    properties["id"] = entity->getnstId();

//    TODO:
//    properties["src"] = "";
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

void QnstView::requestPortAddition(QnstGraphicsPort* entity)
{
    if (entity->getnstId() == ""){
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

void QnstView::performHelp()
{
    // TODO
}

void QnstView::performUndo()
{
    // TODO
}

void QnstView::performRedo()
{
    // TODO
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
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is AUDIO
            case Qnst::Audio:
                clipboard = new QnstGraphicsAudio();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is VIDEO
            case Qnst::Video:
                clipboard = new QnstGraphicsVideo();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is TEXT
            case Qnst::Text:
                clipboard = new QnstGraphicsText();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SCRIPT
            case Qnst::Script:
                clipboard = new QnstGraphicsScript();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SETTINGS
            case Qnst::Settings:
                clipboard = new QnstGraphicsSettings();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());
                break;

            // if the entity type is MEDIA
            case Qnst::Media:
                clipboard = new QnstGraphicsMedia();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is CONTEXT
            case Qnst::Context:
                clipboard = new QnstGraphicsContext();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SWITCH
            case Qnst::Switch:
                clipboard = new QnstGraphicsSwitch();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is PORT
            case Qnst::Port:
                clipboard = new QnstGraphicsPort();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

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
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is AUDIO
            case Qnst::Audio:
                clipboard = new QnstGraphicsAudio();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is VIDEO
            case Qnst::Video:
                clipboard = new QnstGraphicsVideo();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is TEXT
            case Qnst::Text:
                clipboard = new QnstGraphicsText();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SCRIPT
            case Qnst::Script:
                clipboard = new QnstGraphicsScript();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SETTINGS
            case Qnst::Settings:
                clipboard = new QnstGraphicsSettings();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());
                break;

            // if the entity type is MEDIA
            case Qnst::Media:
                clipboard = new QnstGraphicsMedia();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is CONTEXT
            case Qnst::Context:
                clipboard = new QnstGraphicsContext();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is SWITCH
            case Qnst::Switch:
                clipboard = new QnstGraphicsSwitch();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;

            // if the entity type is PORT
            case Qnst::Port:
                clipboard = new QnstGraphicsPort();
                clipboard->setnstGraphicsParent(entity->getnstGraphicsParent());

                clipboard->setnstId(entity->getnstId());

                clipboard->setTop(entity->getTop());
                clipboard->setLeft(entity->getLeft());
                clipboard->setWidth(entity->getWidth());
                clipboard->setHeight(entity->getHeight());

                break;
            }

            foreach(QnstGraphicsEntity* e, entity->getnstGraphicsEntities()){
                performCopy(e, clipboard);
            }

            clipboard->setnstGraphicsParent(NULL);

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
        if (parent->getnstType() != Qnst::Image &&
            parent->getnstType() != Qnst::Audio &&
            parent->getnstType() != Qnst::Video &&
            parent->getnstType() != Qnst::Text &&
            parent->getnstType() != Qnst::Script &&
            parent->getnstType() != Qnst::Settings &&
            parent->getnstType() != Qnst::Media){

            QnstGraphicsEntity* entity;

            switch(copy->getnstType()){

            // if the entity type is IMAGE
            case Qnst::Image:
                entity = new QnstGraphicsImage();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 56/2);
                entity->setLeft(parent->getWidth()/2 - 56/2);
                entity->setWidth(56);
                entity->setHeight(72);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is AUDIO
            case Qnst::Audio:
                entity = new QnstGraphicsAudio();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 56/2);
                entity->setLeft(parent->getWidth()/2 - 56/2);
                entity->setWidth(56);
                entity->setHeight(72);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is VIDEO
            case Qnst::Video:
                entity = new QnstGraphicsVideo();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 56/2);
                entity->setLeft(parent->getWidth()/2 - 56/2);
                entity->setWidth(56);
                entity->setHeight(72);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is TEXT
            case Qnst::Text:
                entity = new QnstGraphicsText();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 56/2);
                entity->setLeft(parent->getWidth()/2 - 56/2);
                entity->setWidth(56);
                entity->setHeight(72);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is SCRIPT
            case Qnst::Script:
                entity = new QnstGraphicsScript();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 56/2);
                entity->setLeft(parent->getWidth()/2 - 56/2);
                entity->setWidth(56);
                entity->setHeight(72);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is SETTINGS
            case Qnst::Settings:
                entity = new QnstGraphicsSettings();
                entity->setnstGraphicsParent(parent);

                entity->setnstId(copy->getnstId());

                entity->setTop(parent->getHeight()/2 - 56/2);
                entity->setLeft(parent->getWidth()/2 - 56/2);
                entity->setWidth(56);
                entity->setHeight(72);

                parent->addnstGraphicsEntity(entity);

                requestEntityAddition(entity);

                break;

            // if the entity type is MEDIA
            case Qnst::Media:
                entity = new QnstGraphicsMedia();
                entity->setnstGraphicsParent(parent);

                entity->setnstId("m"+QString::number(++nmedia));

                entity->setTop(parent->getHeight()/2 - 56/2);
                entity->setLeft(parent->getWidth()/2 - 56/2);
                entity->setWidth(56);
                entity->setHeight(72);

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
    }

    foreach(QnstGraphicsEntity* e, copy->getnstGraphicsEntities()){
        performPaste(e, entity);
    }
}

void QnstView::performDelete()
{
    if (selected != NULL){
        requestEntityRemotion(selected);
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
    if (zoomStep > 0){
        zoomStep--;

        resetMatrix();

        scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
    }
}

void QnstView::performZoomOut()
{
    if (zoomStep*0.05 < 1){
        zoomStep++;

        resetMatrix();

        scale(1 - zoomStep*0.05, 1 - zoomStep*0.05);
    }
}

void QnstView::performZoomReset()
{
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
                    QnstGraphicsEntity* parenta = entitya->getnstGraphicsParent();
                    QnstGraphicsEntity* parentb = entityb->getnstGraphicsParent();

                    if (parenta != NULL && parentb != NULL){
                        if (parenta == parentb){
                            qDebug() << "NODE to NODE:" << entitya->getnstUid() << "->" << entityb->getnstUid();

                            linkDialog->init(connectors);

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

                                    if (link == "New..."){
                                        properties["link"] = "link"+QString::number(++nlink);
                                    }else{
                                        properties["link"] = link;
                                    }

                                    properties["connector"] = connName;

                                    properties["action"] = act;

                                    properties["component"] = entityb->getnstId();

                                    properties["role"] = act;

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

                                    if (link == "New..."){
                                        properties["link"] = "link"+QString::number(++nlink);


                                    }else{
                                        properties["link"] = link;
                                    }

                                    properties["connector"] = connName;
                                    properties["condition"] = cond;

                                    properties["component"] = entitya->getnstId();

                                    properties["role"] = cond;

                                    link2conn[properties["link"]] = connName;

                                    entities[entity->getnstUid()] = entity;

                                    emit entityAdded(entity->getnstUid(), parenta->getnstUid(), properties);
                                }

                            }else{
                                if (linkDialog->exec()){
                                    QString con = linkDialog->form.cbCondition->currentText();
                                    QString act = linkDialog->form.cbAction->currentText();

                                    if (con != "" && act != ""){
                                        QnstGraphicsLink* entity = new QnstGraphicsLink();
                                        entity->setnstGraphicsParent(parenta);
                                        entity->setEntityA(entitya);
                                        entity->setEntityB(entityb);
                                        entity->adjust();

                                        parenta->addnstGraphicsEntity(entity);

                                        ((QnstGraphicsInterface*) entitya)->addnstGraphicsEdge(entity);
                                        ((QnstGraphicsInterface*) entityb)->addnstGraphicsEdge(entity);

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

                                        QString connName = linkDialog->form.cbConnector->currentText();;

                                        if (linkDialog->form.cbConnector->currentText() == "New..."){
                                            QnstConncetor* conn = new QnstConncetor();
                                            conn->setName(con+act+QString::number(connectors.size()));
                                            conn->addCondition(con, con);
                                            conn->addAction(act, act);

                                            connectors[conn->getName()] = conn;

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

                                        properties["TYPE"] = "link";

                                        properties["id"] = "link"+QString::number(++nlink);

                                        properties["connector"] = connName;

                                        properties["componentcondition"] = entitya->getnstId();
                                        properties["componentaction"] = entityb->getnstId();

                                        properties["condition"] = con;
                                        properties["action"] = act;

                                        entities[entity->getnstUid()] = entity;

                                        emit entityAdded(entity->getnstUid(), parenta->getnstUid(), properties);
                                    }
                                }
                            }

                            modified = false;
                        }
                    }

                // if linking INTERFACE to NODE
                }else if (entitya->getncgType() == Qncg::Interface && entityb->getncgType() == Qncg::Node){
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
                    }

                // if linking INTERFACE to INTERFACE
                }else if (entitya->getncgType() == Qncg::Interface && entityb->getncgType() == Qncg::Interface){
                    if (entitya->getnstType() == Qnst::Port && entityb->getnstType() == Qnst::Port){
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
                                qDebug() << "INTERFACE to INTERFACE:" << entitya->getnstUid() << "->" << entityb->getnstUid();
                            }
                        }
                    }
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
    if (event->delta() > 0){
        performZoomIn();

    }else{
        performZoomOut();
    }

    event->accept();
}

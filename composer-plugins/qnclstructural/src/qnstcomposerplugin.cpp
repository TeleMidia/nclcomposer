/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstcomposerplugin.h"

namespace composer {
    namespace plugin {
        namespace layout {

QnstComposerPlugin::QnstComposerPlugin(QObject* parent)
{
    setParent(parent);

    createWidgets();
    createConnections();

    selectedId = NULL;
}

QnstComposerPlugin::~QnstComposerPlugin()
{
    delete(view);
}

void QnstComposerPlugin::createWidgets()
{
    view = new QnstView();
}

void QnstComposerPlugin::createConnections()
{
    connect(view,SIGNAL(bodyAdded(QString,QString,QMap<QString,QString>)),
            SLOT(requestBodyAddition(QString,QString,QMap<QString,QString>)));

    connect(view,SIGNAL(mediaAdded(QString,QString,QMap<QString,QString>)),
            SLOT(requestMediaAddition(QString,QString,QMap<QString,QString>)));

    connect(view,SIGNAL(contextAdded(QString,QString,QMap<QString,QString>)),
            SLOT(requestContextAddition(QString,QString,QMap<QString,QString>)));

    connect(view,SIGNAL(switchAdded(QString,QString,QMap<QString,QString>)),
            SLOT(requestSwitchAddition(QString,QString,QMap<QString,QString>)));

    connect(view,SIGNAL(portAdded(QString,QString,QMap<QString,QString>)),
            SLOT(requestPortAddition(QString,QString,QMap<QString,QString>)));

     connect(view,SIGNAL(propertyAdded(QString,QString,QMap<QString,QString>)),
             SLOT(requestPropertyAddition(QString,QString,QMap<QString,QString>)));

  connect(view,SIGNAL(areaAdded(QString,QString,QMap<QString,QString>)),
          SLOT(requestAreaAddition(QString,QString,QMap<QString,QString>)));

  connect(view,SIGNAL(linkAdded(QString,QString,QMap<QString,QString>)),
          SLOT(requestLinkAddition(QString,QString,QMap<QString,QString>)));


  connect(view,SIGNAL(entitySelected(QString)),
          SLOT(requestEntitySelection(QString)));
}

void QnstComposerPlugin::init()
{

}

QWidget* QnstComposerPlugin::getWidget()
{
    return view;
}

bool QnstComposerPlugin::saveSubsession()
{

}

void QnstComposerPlugin::onEntityAdded(QString pluginID, Entity *entity)
{
    if (pluginID != getPluginInstanceID()){

    }else{
        if (entity->getType() == "body" && !bodyStack.empty()){
            QString CUID = entity->getUniqueId();
            QString UID = bodyStack.pop();

            m_comp_to_view[CUID] = UID;

            m_view_to_comp[UID] = CUID;


        }else if (entity->getType() == "context" && !contextStack.empty()){
            QString CUID = entity->getUniqueId();
            QString UID = contextStack.pop();

            m_comp_to_view[CUID] = UID;

            m_view_to_comp[UID] = CUID;

        }else if (entity->getType() == "media" && !mediaStack.empty()){
            QString CUID = entity->getUniqueId();
            QString UID = mediaStack.pop();

            m_comp_to_view[CUID] = UID;

            m_view_to_comp[UID] = CUID;

        }else if (entity->getType() == "switch" && !switchStack.empty()){
            QString CUID = entity->getUniqueId();
            QString UID = switchStack.pop();

            m_comp_to_view[CUID] = UID;

            m_view_to_comp[UID] = CUID;

        }else if (entity->getType() == "port" && !portStack.empty()){
            QString CUID = entity->getUniqueId();
            QString UID = portStack.pop();

            m_comp_to_view[CUID] = UID;

            m_view_to_comp[UID] = CUID;
        }

        else if (entity->getType() == "property" && !propertyStack.empty()){
                    QString CUID = entity->getUniqueId();
                    QString UID = propertyStack.pop();

                    m_comp_to_view[CUID] = UID;

                    m_view_to_comp[UID] = CUID;
        }

        else if (entity->getType() == "area" && !areaStack.empty()){
                    QString CUID = entity->getUniqueId();
                    QString UID = areaStack.pop();

                    m_comp_to_view[CUID] = UID;

                    m_view_to_comp[UID] = CUID;
        }

        else if (entity->getType() == "link" && !linkStack.empty()){
                    QString CUID = entity->getUniqueId();
                    QString UID = linkStack.pop();

                    m_comp_to_view[CUID] = UID;

                    m_view_to_comp[UID] = CUID;
        }

        else if (entity->getType() == "bind" && !bindStack.empty()){
                    QString CUID = entity->getUniqueId();
                    QString UID = bindStack.pop();

                    m_comp_to_view[CUID] = UID;

                    m_view_to_comp[UID] = CUID;
        }
    }
}

void QnstComposerPlugin::errorMessage(QString error)
{

}

void QnstComposerPlugin::onEntityChanged(QString pluginID, Entity *entity)
{

}

void QnstComposerPlugin::onEntityRemoved(QString pluginID, QString entityID)
{

}

void QnstComposerPlugin::changeSelectedEntity(QString pluginID, void* param)
{
    if (pluginID != getPluginInstanceID()){
        QString* entityCUID = (QString*) param;

        if(entityCUID != NULL)
        {
            if (m_comp_to_view.contains(*entityCUID)){

                if (*selectedId != *entityCUID){
                    qDebug() << *selectedId  << "!="<< *entityCUID;

                    if(selectedId != NULL)
                    {
                        delete selectedId;
                        selectedId = NULL;
                    }

                    view->selectEntity(m_comp_to_view[*entityCUID]);

                    selectedId = new QString(*entityCUID);
                }
            }
        }
    }
}

void QnstComposerPlugin::requestBodyAddition(const QString entityUID,
                                             const QString parentUID,
                                             const QMap<QString, QString> attributes)
{
    QMap<QString, QString> standard;

    if (getProject()->getEntitiesbyType("ncl").isEmpty())
    {
        QMap<QString, QString> atts;

        emit addEntity("ncl",
                       getProject()->getUniqueId(),
                       atts,
                       false);
    }

    QString nclCUID =
        getProject()->getEntitiesbyType("ncl").at(0)->getUniqueId();

    bodyStack.push(entityUID);

    emit addEntity("body", nclCUID, standard, false);
}

void QnstComposerPlugin::requestContextAddition(const QString entityUID,
                                                const QString parentUID,
                                                const QMap<QString, QString> attributes)
{
    qDebug() << "QnstComposerPlugin::requestContextAddition";

    QString parentCUID = m_view_to_comp.value(parentUID);

    QMap<QString, QString> standard;

    standard["id"] = attributes["id"];

    contextStack.push(entityUID);

    emit addEntity("context",parentCUID,standard, false);
}

void QnstComposerPlugin::requestMediaAddition(const QString entityUID,
                                              const QString parentUID,
                                              const QMap<QString, QString> attributes)
{
    qDebug() << "QnstComposerPlugin::requestMediaAddition";

    QString parentCUID =  m_view_to_comp.value(parentUID);

    QMap<QString, QString> standard;

    standard["id"] = attributes["id"];

    mediaStack.push(entityUID);

    emit addEntity("media",parentCUID,standard, false);
}

void QnstComposerPlugin::requestSwitchAddition(const QString entityUID,
                                               const QString parentUID,
                                               const QMap<QString, QString> attributes)
{
    qDebug() << "QnstComposerPlugin::requestSwitchAddition";

    QString parentCUID = m_view_to_comp.value(parentUID);

    QMap<QString, QString> standard;

    standard["id"] = attributes["id"];

    switchStack.push(entityUID);

    emit addEntity("switch",parentCUID,standard, false);
}

void QnstComposerPlugin::requestPortAddition(const QString entityUID,
                           const QString parentUID,
                           const QMap<QString, QString> attributes)
{
    qDebug() << "QnstComposerPlugin::requestPortAddition";

    QString parentCUID = m_view_to_comp.value(parentUID);

    QMap<QString, QString> standard;

    standard["id"] = attributes["id"];

    portStack.push(entityUID);

    emit addEntity("port",parentCUID,standard, false);
}

void QnstComposerPlugin::requestPropertyAddition(const QString entityUID,
                           const QString parentUID,
                           const QMap<QString, QString> attributes)
{
    qDebug() << "QnstComposerPlugin::requestPropertyAddition";

    QString parentCUID = m_view_to_comp.value(parentUID);

    QMap<QString, QString> standard;

    propertyStack.push(entityUID);

    emit addEntity("property",parentCUID,standard, false);
}

void QnstComposerPlugin::requestAreaAddition(const QString entityUID,
                           const QString parentUID,
                           const QMap<QString, QString> attributes)
{
    qDebug() << "QnstComposerPlugin::requestAreaAddition";

    QString parentCUID = m_view_to_comp.value(parentUID);

    QMap<QString, QString> standard;

    areaStack.push(entityUID);

    emit addEntity("area",parentCUID,standard, false);
}

void QnstComposerPlugin::requestEntitySelection(const QString entityUID)
{
    qDebug() << "QnstComposerPlugin::requestEntitySelection";

    if(selectedId != NULL)
    {
        delete selectedId;
        selectedId = NULL;
    }

    selectedId = new QString(m_view_to_comp[entityUID]);
    emit sendBroadcastMessage("changeSelectedEntity", selectedId);
}

void QnstComposerPlugin::requestLinkAddition(const QString entityUID,
                         const QString parentUID,
                         const QMap<QString, QString> attributes)
{
    qDebug() << "QnstComposerPlugin::requestLinkAddition";

    QString parentCUID = m_view_to_comp.value(parentUID);

    if (attributes["binterface"] != ""){
        QString pCUID = m_view_to_comp[attributes["binterface"]];

        QMap<QString, QString> pattrs;

        pattrs["component"] = getProject()->getEntityById(
                    m_view_to_comp[attributes["enode"]])->getAttribute("id");

        if (attributes["einterface"] != ""){
            pattrs["interface"] = getProject()->getEntityById(
                        m_view_to_comp[attributes["einterface"]])->getAttribute("id");
        }

        emit setAttributes(getProject()->getEntityById(pCUID),pattrs,false);

    }else{

        QString xcon_id = addConnector(attributes["condition"],
                                      attributes["action"]);

        QMap<QString, QString> standard;

        standard["xconnector"] = attributes["id"];

        standard["xconnector"] = xcon_id;

        linkStack.push(entityUID);

        emit addEntity("link",parentCUID,standard, false);

        QString bparentCUID = m_view_to_comp[entityUID];

        QMap<QString, QString> b1attrs;

        if ( attributes["condition"] == "onKeySelection"){
            b1attrs["role"] = "onSelection";

        }else{
             b1attrs["role"] = attributes["condition"];
        }

        b1attrs["role"] = attributes["condition"];



        b1attrs["component"] = getProject()->getEntityById(
                    m_view_to_comp[attributes["bnode"]])->getAttribute("id") ;

        if (attributes["binterface"] != ""){
            b1attrs["interface"] = getProject()->getEntityById(
                        m_view_to_comp[attributes["einterface"]])->getAttribute("id");
        }

        QMap<QString, QString> b2attrs;


        if ( attributes["action"] == "startDelay"){
            b2attrs["role"] = "start";

        }else{
            b2attrs["role"] = attributes["action"];
        }

        b2attrs["component"] = getProject()->getEntityById(
                    m_view_to_comp[attributes["enode"]])->getAttribute("id") ;

        if (attributes["einterface"] != ""){
            b2attrs["interface"] = getProject()->getEntityById(
                        m_view_to_comp[attributes["einterface"]])->getAttribute("id");
        }

        bindStack.push("b1"+entityUID);

        emit addEntity("bind",bparentCUID,b1attrs, false);

        QString b1UID = m_view_to_comp["b1"+entityUID];

        bindStack.push("b2"+entityUID);

        emit addEntity("bind",bparentCUID,b2attrs, false);

        QString b2UID = m_view_to_comp["b2"+entityUID];

        if (attributes["condition"] == "onKeySelection"){
           QMap<QString, QString> bpattrs;

           bpattrs["name"] = "key";

           bpattrs["value"] = attributes["key"];

           emit addEntity("bindParam",b1UID,bpattrs,false);
        }

        if (attributes["action"] == "startDelay"){
           QMap<QString, QString> bpattrs;

           bpattrs["name"] = "delay";

           bpattrs["value"] = attributes["delay"];

           emit addEntity("bindParam",b2UID,bpattrs,false);
        }

        if (attributes["action"] == "set"){
           QMap<QString, QString> bpattrs;

           bpattrs["name"] = "value";

           bpattrs["value"] = attributes["value"];

           emit addEntity("bindParam",b2UID,bpattrs,false);
        }
    }
}

QString QnstComposerPlugin::addConnector(QString condition, QString action)
{
    QString conn_name = "";

    if (condition == "onBegin"){
        conn_name += "OnBegin";

    }else if (condition == "onEnd"){
        conn_name += "OnEnd";

    }else if (condition == "onSet"){
        conn_name += "OnSet";

    }else if (condition == "onResume"){
        conn_name += "OnResume";

    }else if (condition == "onSelection"){
        conn_name += "OnSelection";

    }else if (condition == "onPause"){
        conn_name += "OnPause";

    }else if (condition == "onAbort"){
        conn_name += "OnAbort";

    }else if (condition == "onKeySelection"){
        conn_name += "OnKeySelection";
    }

    if (action == "start"){
        conn_name += "Start";

    }else if (action == "stop"){
        conn_name += "Stop";

    }else if (action == "start"){
        conn_name += "Start";

    }else if (action == "resume"){
        conn_name += "Resume";

    }else if (action == "pause"){
        conn_name += "Pause";

    }else if (action == "abort"){
        conn_name += "Abort";

    }else if (action == "startDelay"){
        conn_name += "StartDelay";
    }

    if (!connectors.contains(conn_name)){

        if (getProject()->getEntitiesbyType("head").isEmpty())
        {
            QMap<QString, QString> atts;

            QString nclCUID =
                    getProject()->getEntitiesbyType("ncl").at(0)->getUniqueId();

            emit addEntity("head",nclCUID,atts,false);
        }

        if (getProject()->getEntitiesbyType("connectorBase").isEmpty())
        {
            QMap<QString, QString> atts;

            QString headCUID =
                    getProject()->getEntitiesbyType("head").at(0)->getUniqueId();

            emit addEntity("connectorBase",headCUID,atts,false);
        }

        QString connBaseCUID =
                getProject()->getEntitiesbyType("connectorBase").at(0)->getUniqueId();

        QMap<QString, QString> ccatts;

        ccatts["id"] = conn_name;

        emit addEntity("causalConnector",connBaseCUID,ccatts,false);

        QString cauConnCUID;

        foreach (Entity* ent, getProject()->getEntitiesbyType("causalConnector")){
            if (ent->getAttribute("id") == conn_name){
                cauConnCUID = ent->getUniqueId();

                break;
            }
        }

        QMap<QString, QString> scatts;

        if (condition == "onKeySelection"){
            scatts["role"] = "onSelection";

            scatts["key"] = "$key";

             QMap<QString, QString> cpatts;

             cpatts["name"] = "key";

             emit addEntity("connectorParam",cauConnCUID,cpatts,false);
        }else{
            scatts["role"] = condition;
        }

        emit addEntity("simpleCondition",cauConnCUID,scatts,false);

        QMap<QString, QString> saatts;


        if (action == "startDelay"){
            saatts["role"] = "start";

            saatts["delay"] = "$delay";

            QMap<QString, QString> cpatts;

            cpatts["name"] = "delay";

            emit addEntity("connectorParam",cauConnCUID,cpatts,false);
        }else if (action == "set"){
            saatts["value"] = "$value";

            QMap<QString, QString> cpatts;

            cpatts["name"] = "value";

            emit addEntity("connectorParam",cauConnCUID,cpatts,false);
        }else{
            saatts["role"] = action;
        }

        emit addEntity("simpleAction",cauConnCUID,saatts,false);

        connectors.insert(conn_name);
    }

    return conn_name;
}

} } } // end namespace

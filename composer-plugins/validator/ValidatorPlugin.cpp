#include "ValidatorPlugin.h"


namespace composer{
    namespace plugin{
        namespace validator{

ValidatorPlugin::ValidatorPlugin()
{
    table = new QTreeWidget;
    table->setColumnCount(3);
    QStringList headers;
    headers << "" << "Elemento" << "Mensagem";
    table->setHeaderLabels(headers);
    table->setColumnWidth(0, 40);

    nclValidator::Langstruct::init();
}

void ValidatorPlugin::init()
{
    qDebug() << "*********************Start Validator init()";

    Project * project = getProject();

    if (!project) return;

    foreach (Entity *entity, project->getChildren())
        updateModel(entity);

//    adapter.getModel().print();

//    adapter.validate();
    adapter.getModel().clearMarkedElements();

    qDebug() << "*********************Endt Validator init()";
}

void ValidatorPlugin::updateModel (Entity *entity)
{
//    qDebug() << entity->getType();
    adapter.addElement(entity);

    foreach (Entity *child, entity->getChildren()){
        updateModel(child);
    }

    std::vector <pair<void *, string> > msgs = adapter.validate();

    updateMessages(msgs);
}

void ValidatorPlugin::updateMessages(std::vector<pair<void *, string> > msgs)
{
    table->clear();
    pairsMessages.clear();
    emit sendBroadcastMessage("clearValidationMessages", 0);


    set <pair <Entity *, QString> > messages;
    for (int i = 0; i < msgs.size(); i++){
        Entity *entity = (Entity *) msgs[i].first;
        if (entity != NULL){
            qDebug() << entity->getUniqueId();
//            first = entity->getType();

            messages.insert(make_pair (entity, QString::fromStdString(msgs [i].second)));
//            pairsMessages.push_back(make_pair (entity, QString::fromStdString(msgs [i].second)));
//            emit sendBroadcastMessage("validationError", &pairsMessages.back());
        }



    }

    for (set <pair <Entity *, QString> >::iterator it = messages.begin(); it != messages.end(); it++){
        pair <Entity *, QString> p = *it;
        pairsMessages.push_back(p);

        QStringList list;
//        QString first = "";

//        if (entity)
//            first = entity->getType();

        list << "" << p.first->getType() << p.second;
        QTreeWidgetItem *item = new QTreeWidgetItem(table, list);
        item->setIcon(0, QIcon(":/images/error.png"));
        table->addTopLevelItem (item);

        emit sendBroadcastMessage("validationError", &pairsMessages.back());
    }

}

void ValidatorPlugin::onEntityAdded(QString ID, Entity *entity)
{
    if (!entity) return;

    qDebug () << "Validator: Entity Added: " << ID;

    adapter.addElement(entity);

//    adapter.getModel().print();


    std::vector <pair<void *, string> > msgs = adapter.validate();

    updateMessages(msgs);
}

void ValidatorPlugin::onEntityChanged(QString ID, Entity * entity)
{
    qDebug () << "Validator: Entity Changed: " << ID;
    adapter.changeElement(entity);
//    adapter.getModel().print();

    std::vector <pair<void *, string> > msgs = adapter.validate();
    updateMessages(msgs);
}

void ValidatorPlugin::onEntityRemoved(QString ID, QString entityID)
{
    qDebug () << "Validator: Entity Removed: " << ID;
    adapter.removeElement(entityID);

//    adapter.getModel().print();

    std::vector <pair<void *, string> > msgs = adapter.validate();
    updateMessages(msgs);

}


void ValidatorPlugin::errorMessage(QString error)
{
    qDebug () << "Validator: Error: " << error;
}


void ValidatorPlugin::clearValidationMessages(QString pluginID, void * param)
{
//    qDebug() << "clearValidationMessages:";
//    qDebug() << pluginID;
//    qDebug() << (param == 0) ;
}


void ValidatorPlugin::validationError(QString pluginID, void * param)
{
//    qDebug() << "validationError:";
//    qDebug() << pluginID;
//    if (param){
//        pair <Entity *, QString> * p = (pair <Entity *, QString> *) param;
//        qDebug () << p->first->getType() << " " << p->second;
//    }

}




        }
    }
}


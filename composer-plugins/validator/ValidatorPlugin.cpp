#include "ValidatorPlugin.h"


namespace composer{
    namespace plugin{
        namespace validator{

ValidatorPlugin::ValidatorPlugin()
{
    edit = new QTextEdit;
    edit->setReadOnly(true);

    nclValidator::Langstruct::init();
}

void ValidatorPlugin::onEntityAdded(QString ID, Entity *entity)
{
    if (!entity) return;

    qDebug () << "Validator: Entity Added: " << ID;

    adapter.addElement(ID, entity);

    edit->clear();

//    for (int i = 0; i < messages.size(); i++)
//        edit->append(messages[i].);

    std::vector <string> msgs = adapter.validate();

    QString txt = "";
    for (int i = 0; i < msgs.size(); i++){
        txt += QString::fromStdString(msgs[i] + "\n");
    }

    qDebug () << txt;

    edit->setText(txt);
}

void ValidatorPlugin::onEntityChanged(QString ID, Entity *)
{
    qDebug () << "Validator: Entity Changed: " << ID;
}

void ValidatorPlugin::onEntityRemoved(QString ID, QString entityID)
{
    qDebug () << "Validator: Removed Added: " << ID;
    adapter.removeElement(ID, entityID);

    std::vector <string> msgs = adapter.validate();

    edit->clear();
    QString txt = "";
    for (int i = 0; i < msgs.size(); i++)
        txt += QString::fromStdString(msgs[i] + "\n");

    edit->setText(txt);
}


void ValidatorPlugin::errorMessage(QString error)
{
    qDebug () << "Validator: Error: " << error;
}



        }
    }
}


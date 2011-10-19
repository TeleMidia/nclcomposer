#ifndef COMPOSERNCLADAPTER_H
#define COMPOSERNCLADAPTER_H

#include "message/message.h"
#include "nclmodel/nclmodel.h"
#include "validation/Validator.h"
#include "message/message.h"
#include <core/model/Entity.h>
#include <QDebug>
#include <QMap>
#include <vector>
#include <string>

using namespace composer::core::model;

class ComposerNCLAdapter
{
public:
    ComposerNCLAdapter();

    void addElement (QString, Entity *);
    void removeElement (QString, QString);
    void changeElement (QString, Entity *);

    std::vector <std::string> validate ();

private:
    nclValidator::Model nclModel;
    QMap <QString, nclValidator::virtualId> idToVirtualId;
};

#endif // COMPOSERNCLADAPTER_H

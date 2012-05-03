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

    void addElement (Entity *);
    void removeElement (QString);
    void changeElement (Entity *);

    inline nclValidator::Model &getModel () { return nclModel; }
    inline void clearModel () { nclModel.clear(); }
    inline void setRelativePath (QString path) { nclModel.setRelativePath(path.toStdString()); }

    std::vector <std::pair<void *, std::string> > validate ();

private:
    std::vector<nclValidator::Attribute> createVectorAttribute (Entity *);
    nclValidator::Model nclModel;
    QMap <QString, nclValidator::virtualId> idToVirtualId;
    QString language;
};

#endif // COMPOSERNCLADAPTER_H

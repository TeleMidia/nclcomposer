#ifndef VALIDATORPLUGIN_H
#define VALIDATORPLUGIN_H

#include <core/extensions/IPlugin.h>
#include <QWidget>
#include <QTreeWidget>
#include <QHBoxLayout>
#include "nclmodel/nclmodel.h"
#include "validation/Validator.h"
#include "composerncladapter.h"

using namespace composer::extension;

namespace composer{
    namespace plugin{
        namespace validator{


class ValidatorPlugin : public IPlugin
{
    Q_OBJECT
public:
    ValidatorPlugin();

    QWidget* getWidget() { return table; }
    bool saveSubsession() {return true;}
    void init();

public slots:
    void onEntityAdded(QString ID, Entity *);
    void onEntityChanged(QString ID, Entity *);
    void onEntityRemoved(QString ID, QString entityID);

    void errorMessage(QString error);

    void clearValidationMessages (QString, void *);
    void validationError (QString, void *);

private:
    void updateModel(Entity *);
    void updateMessages (std::vector<pair<void *, string> >);

    QVector <pair<Entity *, QString> > pairsMessages;

    QWidget* window;
    QTreeWidget *table;
    ComposerNCLAdapter adapter;

};

        }
    }
}


#endif // VALIDATORPLUGIN_H

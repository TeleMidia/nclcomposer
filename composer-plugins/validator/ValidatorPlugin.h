#ifndef VALIDATORPLUGIN_H
#define VALIDATORPLUGIN_H

#include <core/extensions/IPlugin.h>
#include <QWidget>
#include <QTextEdit>
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

    QWidget* getWidget() { return edit; }
    bool saveSubsession() {return true;}
    void init() {};

public slots:
    void onEntityAdded(QString ID, Entity *);
    void onEntityChanged(QString ID, Entity *);
    /*void onEntityAboutToRemove(Entity *);*/
    void onEntityRemoved(QString ID, QString entityID);

    void errorMessage(QString error);

    void sendToAll() {};

private:
    QWidget* window;
    QTextEdit* edit;
    ComposerNCLAdapter adapter;

};

        }
    }
}


#endif // VALIDATORPLUGIN_H

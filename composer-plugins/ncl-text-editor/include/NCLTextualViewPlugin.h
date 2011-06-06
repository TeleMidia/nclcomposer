#ifndef NCLTEXTUALVIEWPLUGIN_H
#define NCLTEXTUALVIEWPLUGIN_H

#include <QObject>

#include "NCLTextEditor.h"
#include "NCLTextEditorMainWindow.h"
#include <core/extensions/IPlugin.h>
using namespace composer::extension;

namespace composer {
    namespace plugin {
        namespace textual {

class NCLTextualViewPlugin : public IPlugin
{
    Q_OBJECT
private:
    NCLTextEditorMainWindow *window;

    QMap <QString, int> startLineOfEntity;
    QMap <QString, int> endLineOfEntity;

public:
    explicit NCLTextualViewPlugin();
    ~NCLTextualViewPlugin();

    QWidget* getWidget();

    bool saveSubsession();
    void updateFromModel();

public slots:
    void onEntityAdded(QString pluginID, Entity *);
    void onEntityChanged(QString pluginID, Entity *);
    void onEntityAboutToRemove(Entity *);
    void onEntityRemoved(QString pluginID, QString entityID);

    void errorMessage(QString error);

    void changeSelectedEntity(void* param);
};

}}} //end namespace

#endif // DEBUGCONSOLEPLUGIN_H

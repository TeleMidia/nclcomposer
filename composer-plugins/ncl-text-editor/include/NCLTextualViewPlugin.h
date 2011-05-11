#ifndef NCLTEXTUALVIEWPLUGIN_H
#define NCLTEXTUALVIEWPLUGIN_H

#include <QObject>

#include "NCLTextEditor.h"
#include "NCLTextEditorMainWindow.h"
#include <core/extensions/IPlugin.h>
using namespace composer::core::extension::plugin;

class NCLTextualViewPlugin : public IPlugin
{
    Q_OBJECT
private:
    NCLTextEditorMainWindow *window;

    QMap <QString, int> lineOfEntity;

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
};

#endif // DEBUGCONSOLEPLUGIN_H

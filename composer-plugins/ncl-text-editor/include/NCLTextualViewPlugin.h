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

public:
    explicit NCLTextualViewPlugin();
    ~NCLTextualViewPlugin();

    QWidget* getWidget();

    bool save();
    void updateFromModel();

public slots:
    void onEntityAdded(QString ID, Entity *);
    void onEntityAddError(QString error);
    void onEntityChanged(QString ID, Entity *);
    void onEntityChangeError(QString error);
    void onEntityAboutToRemove(Entity *);
    void onEntityRemoved(QString ID, QString entityID);
    void onEntityRemoveError(QString error);
};

#endif // DEBUGCONSOLEPLUGIN_H

#ifndef LAYOUTPLUGIN_H
#define LAYOUTPLUGIN_H

#include <core/extensions/IPlugin.h>
using namespace composer::core::extension::plugin;

#include "ui/mainwindow.h"
#include "ui/view/layoutview.h"
#include "ui/view/layoutitem.h"

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutPlugin : public IPlugin
{
    Q_OBJECT

public:
    LayoutPlugin();
    virtual ~LayoutPlugin();

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

private:
    LayoutView* view;

    MainWindow* window;

    QMap<QString, LayoutItem*> items;
};

}
}
}
}
}

#endif // LAYOUTPLUGIN_H

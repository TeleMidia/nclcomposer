#ifndef LAYOUTPLUGIN_H
#define LAYOUTPLUGIN_H

#include <core/extensions/IPlugin.h>
using namespace composer::core::extension::plugin;

#include "ui/mainwindow.h"
#include "ui/view/layoutview.h"
#include "ui/view/layoutitem.h"
#include "ui/view/layoutscene.h"

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
    bool saveSubsession();
    void updateFromModel();

public slots:
    void onEntityAdded(QString ID, Entity *);
    void onEntityChanged(QString ID, Entity *);
    void onEntityAboutToRemove(Entity *);
    void onEntityRemoved(QString ID, QString entityID);

    void errorMessage(QString error);

    void removeRegion(LayoutItem* item);
    void addRegion(LayoutItem* item);

private:
    LayoutView* view;

    MainWindow* window;

    LayoutItem* lasti;
    LayoutScene* scene;


//    QMap<QString, Entity*> idety;
    QMap<QString, LayoutItem*> items;
    QMap<LayoutItem*, QString> items2;
};

}
}
}
}
}

#endif // LAYOUTPLUGIN_H

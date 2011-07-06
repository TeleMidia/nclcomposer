#ifndef QNLYCOMPOSERPLUGIN_H
#define QNLYCOMPOSERPLUGIN_H

#include <QMap>
#include <QStack>
#include <QString>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

#include "ui/view/qnlyview.h"

namespace composer {
namespace plugin {
namespace layout {

class QnlyComposerPlugin : public IPlugin
{
    Q_OBJECT

public:
    QnlyComposerPlugin(QObject* parent = 0);

    ~QnlyComposerPlugin();

    void init();

    virtual QWidget* getWidget();

    virtual bool saveSubsession();

public slots:
    virtual void onEntityAdded(QString pluginID, Entity *entity);

    virtual void errorMessage(QString error);

    virtual void onEntityChanged(QString pluginID, Entity *entity);

    virtual void onEntityRemoved(QString pluginID, QString entityID);

    virtual void changeSelectedEntity (void*);

protected slots:

    virtual void addRegion(Entity* entity);

    virtual void removeRegion(Entity* entity);

    virtual void selectRegion(Entity* entity);

    virtual void changeRegion(Entity* entity);

    virtual void addRegionBase(Entity* entity);

    virtual void removeRegionBase(Entity* entity);

    virtual void selectRegionBase(Entity* entity);

    virtual void changeRegionBase(Entity* entity);


    virtual void addRegion(const QString regionUID, const QString parentUID, const QString regionbaseUID, const QMap<QString, QString> &attributes);

    virtual void removeRegion(const QString regionUID, const QString regionbaseUID);

    virtual void selectRegion(const QString regionUID, const QString regionbaseUID);

    virtual void changeRegion(const QString regionUID, const QString regionbaseUID, const QMap<QString, QString> &attributes);

    virtual void addRegionBase(const QString regionbaseUID, const QMap<QString, QString> &attributes);

    virtual void removeRegionBase(const QString regionbaseUID);

    virtual void selectRegionBase(const QString regionbaseUID);

    virtual void changeRegionBase(const QString regionbaseUID, const QMap<QString, QString> &attributes);

private:
    void createView();

    void createDocument();

    void createConnections();

    QMap<QString, QString> entities;

    QMap<QString, Entity*> regions;

    QMap<QString, Entity*> regionbases;

    QMap<QString, QString> relations;

    QnlyView* view;
};

}
}
}

#endif // QNLYCOMPOSERPLUGIN_H

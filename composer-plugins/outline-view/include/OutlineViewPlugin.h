#ifndef OUTLINEVIEWPLUGIN_H
#define OUTLINEVIEWPLUGIN_H

#include <QObject>

#include "NCLTreeWidget.h"
#include <core/extensions/IPlugin.h>
using namespace composer::core::extension::plugin;

class OutlineViewPlugin : public IPlugin
{
    Q_OBJECT
private:
    NCLTreeWidget *window;

public:
    explicit OutlineViewPlugin();
    ~OutlineViewPlugin();

    QWidget* getWidget();
    QMap <QString, QTreeWidgetItem*> idToItem;

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


private slots:
    void elementRemovedByUser(QString id);
};

#endif // OUTLINEVIEWPLUGIN_H

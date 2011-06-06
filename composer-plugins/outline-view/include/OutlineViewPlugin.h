#ifndef OUTLINEVIEWPLUGIN_H
#define OUTLINEVIEWPLUGIN_H

#include <QObject>

#include "NCLTreeWidget.h"
#include <core/extensions/IPlugin.h>
using namespace composer::extension;


namespace composer {
    namespace plugin {
        namespace outline {

class OutlineViewPlugin : public IPlugin
{
    Q_OBJECT

private:
    NCLTreeWidget *window;
    QString *selectedId;

public:
    explicit OutlineViewPlugin();
    ~OutlineViewPlugin();

    QWidget* getWidget();
    QMap <QString, QTreeWidgetItem*> idToItem;

    bool saveSubsession();
    void updateFromModel();


public slots:
    void onEntityAdded(QString ID, Entity *);
    void onEntityChanged(QString ID, Entity *);
    void onEntityAboutToRemove(Entity *);
    void onEntityRemoved(QString ID, QString entityID);

    void errorMessage(QString error);

    /* Comunication from Debug to me */
    void debugHasSendClearAll(void *obj);

private slots:
    void elementRemovedByUser(QString id);
    void itemSelectionChanged();
};

}}} //end namespace

#endif // OUTLINEVIEWPLUGIN_H

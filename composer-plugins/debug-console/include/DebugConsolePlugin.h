#ifndef DEBUGCONSOLEPLUGIN_H
#define DEBUGCONSOLEPLUGIN_H

#include <QObject>
#include <QGridLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

#include <core/extensions/IPlugin.h>
using namespace composer::extension;

namespace composer {
    namespace plugin {
        namespace debug {

class DebugConsolePlugin : public IPlugin
{
        Q_OBJECT
    private:
        QListWidget *list;
        QWidget *window;
    public:
        explicit DebugConsolePlugin();
        ~DebugConsolePlugin();

        void init();
        QWidget* getWidget();
        bool saveSubsession();

    public slots:
        void onEntityAdded(QString ID, Entity *);
        void onEntityChanged(QString ID, Entity *);
        /*void onEntityAboutToRemove(Entity *);*/
        void onEntityRemoved(QString ID, QString entityID);

        void errorMessage(QString error);

        void sendToAll();
};

} } }//end namespace
#endif // DEBUGCONSOLEPLUGIN_H

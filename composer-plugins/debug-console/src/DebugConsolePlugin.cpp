#include "DebugConsolePlugin.h"

namespace composer {
    namespace plugin {
        namespace debug {

DebugConsolePlugin::DebugConsolePlugin()
{
    window = new QWidget();
    QGridLayout *layout = new QGridLayout(window);
    QPushButton *bt = new QPushButton(window);
    bt->setText(tr("Clear"));
    layout->addWidget(bt);
    list = new QListWidget(window);
    list->setAlternatingRowColors(true);
    layout->addWidget(list);
    connect(bt, SIGNAL(clicked()), list, SLOT(clear()));
    connect(bt, SIGNAL(clicked()), this, SLOT(sendToAll()));
    window->setLayout(layout);
    window->setWindowIcon(QIcon(":/images/icon.png"));
    doc = NULL;

}

DebugConsolePlugin::~DebugConsolePlugin()
{
    if(window != NULL)
        delete window;
    window = NULL;
}

QWidget* DebugConsolePlugin::getWidget()
{
    return window;
}

void DebugConsolePlugin::onEntityAdded(QString ID, Entity *entity)
{
    QString line = "PLUGIN (" + ID + ") added the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    list->insertItem(0, new QListWidgetItem(line));
}

void DebugConsolePlugin::errorMessage(QString error)
{
    list->insertItem(0, new QListWidgetItem(error));
}

void DebugConsolePlugin::onEntityChanged(QString ID, Entity * entity)
{
    QString line = "PLUGIN (" + ID + ") changed the Entity (" +
                    entity->getType() + " - " + entity->getUniqueId() +")";
    list->insertItem(0, new QListWidgetItem(line));
}

void DebugConsolePlugin::onEntityAboutToRemove(Entity *)
{

}

void DebugConsolePlugin::onEntityRemoved(QString ID, QString entityID)
{
    QString line = "PLUGIN (" + ID + ") removed Entity (" +
                   entityID + ")";
    list->insertItem(0, new QListWidgetItem(line));
}

bool DebugConsolePlugin::saveSubsession()
{
    //TODO: All
}

void DebugConsolePlugin::updateFromModel()
{
    //TODO: All
}

void DebugConsolePlugin::sendToAll()
{
   /* Invoker <Result> in;
    in.addArgument<int>(10);
    in.addArgument<int>(20);
    in.addArgument<int>(30);

    in.sendBroadcastMessage("debugHasSendClearAll"); */

    emit sendBroadcastMessage("debugHasSendClearAll", NULL);
}

} } } //end namespace

#include "DebugConsolePlugin.h"

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
    connect(bt,SIGNAL(clicked()),list,SLOT(clear()));
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
    list->addItem(new QListWidgetItem(line));
}

void DebugConsolePlugin::onEntityAddError(QString error)
{
    list->addItem(new QListWidgetItem(error));
}

void DebugConsolePlugin::onEntityChanged(QString ID, Entity * entity)
{
    QString line = "PLUGIN (" + ID + ") changed the Entity (" +
                    entity->getType() + " - " + entity->getUniqueId() +")";
    list->addItem(new QListWidgetItem(line));
}

void DebugConsolePlugin::onEntityChangeError(QString error)
{
    list->addItem(new QListWidgetItem(error));
}

void DebugConsolePlugin::onEntityAboutToRemove(Entity *)
{

}

void DebugConsolePlugin::onEntityRemoved(QString ID, QString entityID)
{
    QString line = "PLUGIN (" + ID + ") removed Entity (" +
                   entityID + ")";
    list->addItem(new QListWidgetItem(line));
}

void DebugConsolePlugin::onEntityRemoveError(QString error)
{
    list->addItem(new QListWidgetItem(error));
}


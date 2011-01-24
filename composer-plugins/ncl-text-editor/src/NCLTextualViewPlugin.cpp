#include "NCLTextualViewPlugin.h"

NCLTextualViewPlugin::NCLTextualViewPlugin()
{
    window = new NCLTextEditorMainWindow();
    /* QGridLayout *layout = new QGridLayout(window);
    QPushButton *bt = new QPushButton(window);
    bt->setMaximumWidth(5);
    bt->setText(tr("Clear"));
    layout->addWidget(bt,0,2);
    list = new QListWidget(window);
    list->setAlternatingRowColors(true);
    layout->addWidget(list);
    connect(bt,SIGNAL(clicked()),list,SLOT(clear()));
    window->setLayout(layout);
    window->setWindowIcon(QIcon(":/images/icon.png")); */
    doc = NULL;

}

NCLTextualViewPlugin::~NCLTextualViewPlugin()
{
    delete window;
    window = NULL;
}

QWidget* NCLTextualViewPlugin::getWidget()
{
    return window;
}

void NCLTextualViewPlugin::onEntityAdded(QString ID, Entity *entity)
{
    QString line = "PLUGIN (" + ID + ") added the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    //TODO: All
}

void NCLTextualViewPlugin::onEntityAddError(QString error)
{
    //TODO: All
}

void NCLTextualViewPlugin::onEntityChanged(QString ID, Entity * entity)
{
    QString line = "PLUGIN (" + ID + ") changed the Entity (" +
                   entity->getType() + " - " + entity->getUniqueId() +")";
    //TODO: All
}

void NCLTextualViewPlugin::onEntityChangeError(QString error)
{
    //TODO: All
}

void NCLTextualViewPlugin::onEntityAboutToRemove(Entity *)
{

}

void NCLTextualViewPlugin::onEntityRemoved(QString ID, QString entityID)
{
    QString line = "PLUGIN (" + ID + ") removed Entity (" +
                   entityID + ")";
    //TODO: All
}

void NCLTextualViewPlugin::onEntityRemoveError(QString error)
{
    //TODO: All
}


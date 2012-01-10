#include "qnstgraphicsbody.h"

QnstGraphicsBody::QnstGraphicsBody(QnstGraphicsNode* parent)
    : QnstGraphicsComposition(parent)
{
    setnstType(Qnst::Body);

    setColor("#CCFFE6");

    createObjects();
    createConnections();
}

QnstGraphicsBody::~QnstGraphicsBody()
{

}

void QnstGraphicsBody::createObjects()
{
    menu = new QnstMenu();
    menu->actionDelete->setEnabled(true);

    menu->actionExport->setEnabled(true);

    menu->menuInsert->setEnabled(true);
    menu->actionImage->setEnabled(true);
    menu->actionVideo->setEnabled(true);
    menu->actionAudio->setEnabled(true);
    menu->actionText->setEnabled(true);
    menu->actionScript->setEnabled(true);
    menu->actionSettings->setEnabled(true);
    menu->actionMedia->setEnabled(true);
    menu->actionContext->setEnabled(true);
    menu->actionSwitch->setEnabled(true);
    menu->actionPort->setEnabled(true);
    menu->actionAggregator->setEnabled(true);
}

void QnstGraphicsBody::createConnections()
{
    connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
    connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

    connect(menu, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
    connect(menu, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
    connect(menu, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

    connect(menu, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

    connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

    connect(menu, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
    connect(menu, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
    connect(menu, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
    connect(menu, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));

    connect(menu, SIGNAL(imageRequested()), SLOT(performImage()));
    connect(menu, SIGNAL(audioRequested()), SLOT(performAudio()));
    connect(menu, SIGNAL(videoRequested()), SLOT(performVideo()));
    connect(menu, SIGNAL(textRequested()), SLOT(performText()));
    connect(menu, SIGNAL(scriptRequested()), SLOT(performScript()));
    connect(menu, SIGNAL(settingsRequested()), SLOT(performSettings()));
    connect(menu, SIGNAL(mediaRequested()), SLOT(performMedia()));
    connect(menu, SIGNAL(contextRequested()), SLOT(performContext()));
    connect(menu, SIGNAL(switchRequested()), SLOT(performSwitch()));
    connect(menu, SIGNAL(portRequested()), SLOT(performPort()));
    connect(menu, SIGNAL(aggregatorRequested()), SLOT(performAggregator()));
}

void QnstGraphicsBody::performImage()
{
    QnstGraphicsImage* entity = new QnstGraphicsImage(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performAudio()
{
    QnstGraphicsAudio* entity = new QnstGraphicsAudio(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performText()
{
    QnstGraphicsText* entity = new QnstGraphicsText(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performVideo()
{
    QnstGraphicsVideo* entity = new QnstGraphicsVideo(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performScript()
{
    QnstGraphicsScript* entity = new QnstGraphicsScript(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performSettings()
{
    QnstGraphicsSettings* entity = new QnstGraphicsSettings(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performMedia()
{
    QnstGraphicsMedia* entity = new QnstGraphicsMedia(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performContext()
{
    QnstGraphicsContext* entity = new QnstGraphicsContext(this);
    entity->setTop(getHeight()/2 - 200/2);
    entity->setLeft(getWidth()/2 - 250/2);
    entity->setWidth(250);
    entity->setHeight(200);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performSwitch()
{
    QnstGraphicsSwitch* entity = new QnstGraphicsSwitch(this);
    entity->setTop(getHeight()/2 - 200/2);
    entity->setLeft(getWidth()/2 - 250/2);
    entity->setWidth(250);
    entity->setHeight(200);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performPort()
{
    QnstGraphicsPort* entity = new QnstGraphicsPort(this);
    entity->setTop(0);
    entity->setLeft(0);
    entity->setWidth(24);
    entity->setHeight(24);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsBody::performAggregator()
{
    QnstGraphicsAggregator* entity = new QnstGraphicsAggregator(this);
    entity->setTop(getHeight()/2 - 28/2);
    entity->setLeft(getWidth()/2 - 28/2);
    entity->setWidth(28);
    entity->setHeight(28);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

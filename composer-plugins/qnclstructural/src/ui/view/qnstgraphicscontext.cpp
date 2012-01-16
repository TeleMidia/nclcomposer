#include "qnstgraphicscontext.h"

QnstGraphicsContext::QnstGraphicsContext(QnstGraphicsNode* parent)
    : QnstGraphicsComposition(parent)
{
    setnstType(Qnst::Context);

    setColor("#EEEEEE");

    createObjects();
    createConnections();
}

QnstGraphicsContext::~QnstGraphicsContext()
{

}

void QnstGraphicsContext::createObjects()
{
    menu = new QnstMenu();
    menu->actionCut->setEnabled(true);
    menu->actionCopy->setEnabled(true);

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

void QnstGraphicsContext::createConnections()
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

void QnstGraphicsContext::performImage()
{
    QnstGraphicsImage* entity = new QnstGraphicsImage(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performAudio()
{
    QnstGraphicsAudio* entity = new QnstGraphicsAudio(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performText()
{
    QnstGraphicsText* entity = new QnstGraphicsText(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performVideo()
{
    QnstGraphicsVideo* entity = new QnstGraphicsVideo(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performScript()
{
    QnstGraphicsScript* entity = new QnstGraphicsScript(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performSettings()
{
    QnstGraphicsSettings* entity = new QnstGraphicsSettings(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performMedia()
{
    QnstGraphicsMedia* entity = new QnstGraphicsMedia(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(72);

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performContext()
{
    QnstGraphicsContext* entity = new QnstGraphicsContext(this);
    entity->setTop(getHeight()/2 - 200/2);
    entity->setLeft(getWidth()/2 - 250/2);
    entity->setWidth(250);
    entity->setHeight(200);

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performSwitch()
{
    QnstGraphicsSwitch* entity = new QnstGraphicsSwitch(this);
    entity->setTop(getHeight()/2 - 200/2);
    entity->setLeft(getWidth()/2 - 250/2);
    entity->setWidth(250);
    entity->setHeight(200);

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performPort()
{
    QnstGraphicsPort* entity = new QnstGraphicsPort(this);
    entity->setTop(0);
    entity->setLeft(0);
    entity->setWidth(28);
    entity->setHeight(28);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performAggregator()
{
    QnstGraphicsAggregator* entity = new QnstGraphicsAggregator(this);
    entity->setTop(getHeight()/2 - 18/2);
    entity->setLeft(getWidth()/2 - 18/2);
    entity->setWidth(18);
    entity->setHeight(18);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

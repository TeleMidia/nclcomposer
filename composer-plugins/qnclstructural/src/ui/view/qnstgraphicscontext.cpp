#include "qnstgraphicscontext.h"

QnstGraphicsContext::QnstGraphicsContext(QnstGraphicsEntity* parent)
    : QnstGraphicsComposition(parent)
{
    setnstType(Qnst::Context);

    setColor("#EEEEEE");

    createObjects();
    createConnections();

    dropsrc = "";
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
    menu->actionHTML->setEnabled(true);
    menu->actionNCL->setEnabled(true);
    menu->actionMedia->setEnabled(true);
    menu->actionContext->setEnabled(true);
    menu->actionSwitch->setEnabled(true);
    menu->actionPort->setEnabled(true);
    menu->actionProperty->setEnabled(true);
    menu->actionAggregator->setEnabled(true);

    menu->menuAdjust->setEnabled(true);
    menu->actionAuto->setEnabled(true);
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
    connect(menu, SIGNAL(htmlRequested()), SLOT(performHtml()));
    connect(menu, SIGNAL(nclRequested()), SLOT(performNCL()));
    connect(menu, SIGNAL(audioRequested()), SLOT(performAudio()));
    connect(menu, SIGNAL(videoRequested()), SLOT(performVideo()));
    connect(menu, SIGNAL(textRequested()), SLOT(performText()));
    connect(menu, SIGNAL(scriptRequested()), SLOT(performScript()));
    connect(menu, SIGNAL(settingsRequested()), SLOT(performSettings()));
    connect(menu, SIGNAL(mediaRequested()), SLOT(performMedia()));
    connect(menu, SIGNAL(contextRequested()), SLOT(performContext()));
    connect(menu, SIGNAL(switchRequested()), SLOT(performSwitch()));
    connect(menu, SIGNAL(portRequested()), SLOT(performPort()));
    connect(menu, SIGNAL(areaRequested()), SLOT(performArea()));
    connect(menu, SIGNAL(propertyRequested()), SLOT(performProperty()));
    connect(menu, SIGNAL(aggregatorRequested()), SLOT(performAggregator()));

    connect(menu, SIGNAL(autoRequested()), SLOT(adjustWithSpring()));
}

void QnstGraphicsContext::performHtml()
{
  createEntity(Qnst::Html);
}

void QnstGraphicsContext::performNCL()
{
  createEntity(Qnst::NCL);
}

void QnstGraphicsContext::performImage()
{
  createEntity(Qnst::Image);
}

void QnstGraphicsContext::performAudio()
{
  createEntity(Qnst::Audio);
}

void QnstGraphicsContext::performText()
{
  createEntity(Qnst::Text);
}

void QnstGraphicsContext::performVideo()
{
  createEntity(Qnst::Video);
}

void QnstGraphicsContext::performScript()
{
  createEntity(Qnst::Script);
}

void QnstGraphicsContext::performSettings()
{
  createEntity(Qnst::Settings);
}

void QnstGraphicsContext::performMedia()
{
  createEntity(Qnst::Media);
}

void QnstGraphicsContext::performContext()
{
  createEntity(Qnst::Context);
}

void QnstGraphicsContext::performSwitch()
{
  createEntity(Qnst::Switch);
}

void QnstGraphicsContext::performPort()
{
    QnstGraphicsPort* entity = new QnstGraphicsPort(this);
    entity->setTop(0);
    entity->setLeft(0);
    entity->setWidth(18);
    entity->setHeight(18);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performArea()
{
    QnstGraphicsArea* entity = new QnstGraphicsArea(this);
    entity->setTop(0);
    entity->setLeft(0);
    entity->setWidth(16);
    entity->setHeight(16);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performProperty()
{
    QnstGraphicsProperty* entity = new QnstGraphicsProperty(this);
    entity->setTop(0);
    entity->setLeft(0);
    entity->setWidth(16);
    entity->setHeight(16);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performAggregator()
{
    QnstGraphicsAggregator* entity = new QnstGraphicsAggregator(this);
    entity->setTop(getHeight()/2 - 14/2);
    entity->setLeft(getWidth()/2 - 14/2);
    entity->setWidth(14);
    entity->setHeight(14);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

    #include "qnstgraphicsbody.h"

QnstGraphicsBody::QnstGraphicsBody(QnstGraphicsNode* parent)
    : QnstGraphicsComposition(parent)
{
    setnstType(Qnst::Body);

    setColor("#EEEEEE");

    createObjects();
    createConnections();

    setAcceptDrops(true);

    dropsrc = "";

    setnstId("");
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
    menu->actionHTML->setEnabled(true);
    menu->actionNCL->setEnabled(true);
    menu->actionMedia->setEnabled(true);
    menu->actionContext->setEnabled(true);
    menu->actionSwitch->setEnabled(true);
    menu->actionPort->setEnabled(true);
    menu->actionArea->setEnabled(true);
    menu->actionProperty->setEnabled(true);
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
    connect(menu, SIGNAL(htmlRequested()), SLOT(performHtml()));
    connect(menu, SIGNAL(nclRequested()), SLOT(performNCL()));
    connect(menu, SIGNAL(mediaRequested()), SLOT(performMedia()));
    connect(menu, SIGNAL(contextRequested()), SLOT(performContext()));
    connect(menu, SIGNAL(switchRequested()), SLOT(performSwitch()));
    connect(menu, SIGNAL(portRequested()), SLOT(performPort()));
    connect(menu, SIGNAL(areaRequested()), SLOT(performArea()));
    connect(menu, SIGNAL(propertyRequested()), SLOT(performProperty()));
    connect(menu, SIGNAL(aggregatorRequested()), SLOT(performAggregator()));
}

void QnstGraphicsBody::performHtml()
{
  createEntity(Qnst::Html);
}

void QnstGraphicsBody::performNCL()
{
  createEntity(Qnst::NCL);
}

void QnstGraphicsBody::performImage()
{
  createEntity(Qnst::Image);
}

void QnstGraphicsBody::performAudio()
{
  createEntity(Qnst::Audio);
}

void QnstGraphicsBody::performText()
{
  createEntity(Qnst::Text);
}

void QnstGraphicsBody::performVideo()
{
  createEntity(Qnst::Video);
}

void QnstGraphicsBody::performScript()
{
  createEntity(Qnst::Script);
}

void QnstGraphicsBody::performSettings()
{
  createEntity(Qnst::Settings);
}

void QnstGraphicsBody::performMedia()
{
  createEntity(Qnst::Media);
}

void QnstGraphicsBody::performContext()
{
  createEntity(Qnst::Context);
}

void QnstGraphicsBody::performSwitch()
{
  createEntity(Qnst::Switch);
}

void QnstGraphicsBody::performPort()
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

void QnstGraphicsBody::performArea()
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

void QnstGraphicsBody::performProperty()
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

void QnstGraphicsBody::performAggregator()
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

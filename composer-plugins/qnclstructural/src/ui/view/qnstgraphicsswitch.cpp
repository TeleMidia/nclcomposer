#include "qnstgraphicsswitch.h"

QnstGraphicsSwitch::QnstGraphicsSwitch(QnstGraphicsEntity* parent)
    : QnstGraphicsComposition(parent)
{
    setnstType(Qnst::Switch);

    setColor("#C6E2FF");

    createObjects();
    createConnections();

    dropsrc = "";
}

QnstGraphicsSwitch::~QnstGraphicsSwitch()
{

}

void QnstGraphicsSwitch::createObjects()
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
    menu->actionHTML->setEnabled(true);
    menu->actionNCL->setEnabled(true);
    menu->actionContext->setEnabled(true);
    menu->actionSwitch->setEnabled(true);
    menu->actionSwitchPort->setEnabled(true);
    menu->actionProperty->setEnabled(true);

    menu->menuAdjust->setEnabled(true);
    menu->actionAuto->setEnabled(true);
}

void QnstGraphicsSwitch::createConnections()
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
    connect(menu, SIGNAL(htmlRequested()), SLOT(performHtml()));
//    connect(menu, SIGNAL(portRequested()), SLOT(performPort()));
    connect(menu, SIGNAL(switchportRequested()), SLOT(performSwitchPort()));
    connect(menu, SIGNAL(nclRequested()), SLOT(performNCL()));
    connect(menu, SIGNAL(videoRequested()), SLOT(performVideo()));
    connect(menu, SIGNAL(textRequested()), SLOT(performText()));
    connect(menu, SIGNAL(scriptRequested()), SLOT(performScript()));
    connect(menu, SIGNAL(settingsRequested()), SLOT(performSettings()));
    connect(menu, SIGNAL(mediaRequested()), SLOT(performMedia()));
    connect(menu, SIGNAL(contextRequested()), SLOT(performContext()));
    connect(menu, SIGNAL(switchRequested()), SLOT(performSwitch()));
    connect(menu, SIGNAL(areaRequested()), SLOT(performArea()));
    connect(menu, SIGNAL(propertyRequested()), SLOT(performProperty()));

    connect(menu, SIGNAL(autoRequested()), SLOT(adjustWithSpring()));
}


void QnstGraphicsSwitch::performHtml()
{
  createEntity(Qnst::Html);
}

void QnstGraphicsSwitch::performNCL()
{
  createEntity(Qnst::NCL);
}

void QnstGraphicsSwitch::performImage()
{
  createEntity(Qnst::Image);
}

void QnstGraphicsSwitch::performAudio()
{
  createEntity(Qnst::Audio);
}

void QnstGraphicsSwitch::performText()
{
  createEntity(Qnst::Text);
}

void QnstGraphicsSwitch::performVideo()
{
  createEntity(Qnst::Video);
}

void QnstGraphicsSwitch::performScript()
{
  createEntity(Qnst::Script);
}

void QnstGraphicsSwitch::performSettings()
{
  createEntity(Qnst::Settings);
}

void QnstGraphicsSwitch::performMedia()
{
  createEntity(Qnst::Media);
}

void QnstGraphicsSwitch::performContext()
{
  createEntity(Qnst::Context);
}

void QnstGraphicsSwitch::performSwitch()
{
  createEntity(Qnst::Switch);
}

void QnstGraphicsSwitch::performSwitchPort()
{
    QnstGraphicsSwitchPort* entity = new QnstGraphicsSwitchPort(this);
    entity->setTop(0);
    entity->setLeft(0);
    entity->setWidth(18);
    entity->setHeight(18);
    entity->adjust();

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performArea()
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

void QnstGraphicsSwitch::performProperty()
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

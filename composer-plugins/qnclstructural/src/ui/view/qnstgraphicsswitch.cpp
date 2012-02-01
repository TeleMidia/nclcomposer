#include "qnstgraphicsswitch.h"

QnstGraphicsSwitch::QnstGraphicsSwitch(QnstGraphicsNode* parent)
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
    menu->actionArea->setEnabled(true);
    menu->actionProperty->setEnabled(true);
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
    connect(menu, SIGNAL(portRequested()), SLOT(performPort()));
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
}

void QnstGraphicsSwitch::performHtml()
{
    QnstGraphicsHTML* entity = new QnstGraphicsHTML(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    if (dropsrc != ""){
        entity->setSource(dropsrc);

        dropsrc = "";
    }

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performNCL()
{
    QnstGraphicsNCL* entity = new QnstGraphicsNCL(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    if (dropsrc != ""){
        entity->setSource(dropsrc);

        dropsrc = "";
    }

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performImage()
{
    QnstGraphicsImage* entity = new QnstGraphicsImage(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
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

void QnstGraphicsSwitch::performAudio()
{
    QnstGraphicsAudio* entity = new QnstGraphicsAudio(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performText()
{
    QnstGraphicsText* entity = new QnstGraphicsText(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performVideo()
{
    QnstGraphicsVideo* entity = new QnstGraphicsVideo(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performScript()
{
    QnstGraphicsScript* entity = new QnstGraphicsScript(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performSettings()
{
    QnstGraphicsSettings* entity = new QnstGraphicsSettings(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performMedia()
{
    QnstGraphicsMedia* entity = new QnstGraphicsMedia(this);
    entity->setTop(getHeight()/2 - 48/2);
    entity->setLeft(getWidth()/2 - 48/2);
    entity->setWidth(48);
    entity->setHeight(64);
    entity->adjust();

    entity->menu->actionPaste->setEnabled(menu->actionPaste->isEnabled());

    addnstGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsSwitch::performContext()
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

void QnstGraphicsSwitch::performSwitch()
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

void QnstGraphicsSwitch::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    foreach(QUrl url, event->mimeData()->urls()){
            event->acceptProposedAction();

            return;
    }
}

void QnstGraphicsSwitch::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    foreach(QUrl url, event->mimeData()->urls())
    {
        QString filename = url.toLocalFile();
        QString suffix = QFileInfo(filename).suffix().toUpper();

        if(suffix=="PNG" ||
           suffix=="JPEG"){
            event->acceptProposedAction();

            dropsrc = filename;

            performImage();

        }else if(suffix=="MP3" ||
                 suffix=="WAV"){
          event->acceptProposedAction();

          dropsrc = filename;

          performAudio();

        }else if(suffix=="AVI" ||
                 suffix=="MPEG4" ||
                 suffix=="MP4" ||
                 suffix=="MPEG"){
          event->acceptProposedAction();

          dropsrc = filename;

          performVideo();

        }else if(suffix=="TXT"){
          event->acceptProposedAction();

          dropsrc = filename;

          performText();

        }else if(suffix=="LUA"){
            event->acceptProposedAction();

            dropsrc = filename;

            performScript();

        }else if(suffix=="HTML"){
            event->acceptProposedAction();

            dropsrc = filename;

            performHtml();

        }else if(suffix=="NCL"){
            event->acceptProposedAction();

            dropsrc = filename;

            performNCL();
         }
    }
}

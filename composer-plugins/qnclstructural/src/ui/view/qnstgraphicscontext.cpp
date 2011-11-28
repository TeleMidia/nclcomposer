#include "qnstgraphicscontext.h"

QnstGraphicsContext::QnstGraphicsContext(QnstGraphicsNode* parent)
    : QnstGraphicsComposition(parent)
{
    setnstType(Qnst::Context);

    createActions();
    createMenus();
    createConnections();

    setColor("#FFD39B");
}

QnstGraphicsContext::~QnstGraphicsContext()
{

}

void QnstGraphicsContext::createActions()
{
    // help action
    helpAction = new QAction(this);
    helpAction->setText(tr("Help"));

    helpAction->setEnabled(false);
    helpAction->setShortcut(QKeySequence("F1"));

    // undo action
    undoAction = new QAction(this);
    undoAction->setText(tr("Undo"));

    undoAction->setEnabled(false);
    undoAction->setShortcut(QKeySequence("Ctrl+Z"));

    // redo action
    redoAction = new QAction(this);
    redoAction->setText(tr("Redo"));

    redoAction->setEnabled(false);
    redoAction->setShortcut(QKeySequence("Ctrl+Shift+Z"));

    // cut action
    cutAction = new QAction(this);
    cutAction->setText(tr("Cut"));

    cutAction->setEnabled(false);
    cutAction->setShortcut(QKeySequence("Ctrl+X"));

    // copy action
    copyAction = new QAction(this);
    copyAction->setText(tr("Copy"));

    copyAction->setEnabled(false);
    copyAction->setShortcut(QKeySequence("Ctrl+C"));

    // paste action
    pasteAction = new QAction(this);
    pasteAction->setText(tr("Paste"));

    pasteAction->setEnabled(false);
    pasteAction->setShortcut(QKeySequence("Ctrl+V"));

    // delete action
    deleteAction = new QAction(this);
    deleteAction->setText(tr("Delete"));

    deleteAction->setEnabled(false);
    deleteAction->setShortcut(QKeySequence("Del"));

    // zoomin action
    zoominAction = new QAction(this);
    zoominAction->setText(tr("Zoom In"));

    zoominAction->setEnabled(false);
    zoominAction->setShortcut(QKeySequence("Ctrl++"));

    // zoomout action
    zoomoutAction = new QAction(this);
    zoomoutAction->setText(tr("Zoom Out"));

    zoomoutAction->setEnabled(false);
    zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

    // reset action
    zoomresetAction = new QAction(this);
    zoomresetAction->setText(tr("Reset"));

    zoomresetAction->setEnabled(false);
    zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

    // fullscreen action
    fullscreenAction = new QAction(this);
    fullscreenAction->setText(tr("Full Screen"));

    fullscreenAction->setEnabled(false);
    fullscreenAction->setShortcut(QKeySequence("Ctrl+F"));

    // export action
    exportAction = new QAction(this);
    exportAction->setText(tr("Export..."));

    exportAction->setEnabled(false);

    // body action
    bodyAction = new QAction(this);
    bodyAction->setText(tr("Body"));
    bodyAction->setIcon(QIcon(":/icon/composition"));

    bodyAction->setEnabled(false);

    // context action
    contextAction = new QAction(this);
    contextAction->setText(tr("Context"));
    contextAction->setIcon(QIcon(":/icon/composition"));

    contextAction->setEnabled(true);

    // switch action
    switchAction = new QAction(this);
    switchAction->setText(tr("Switch"));
    switchAction->setIcon(QIcon(":/icon/composition"));

    switchAction->setEnabled(true);

    // image action
    imageAction = new QAction(this);
    imageAction->setText(tr("Image"));
    imageAction->setIcon(QIcon(":/icon/image"));

    imageAction->setEnabled(true);

    // video action
    videoAction = new QAction(this);
    videoAction->setText(tr("Video"));
    videoAction->setIcon(QIcon(":/icon/video"));

    videoAction->setEnabled(true);

    // audio action
    audioAction = new QAction(this);
    audioAction->setText(tr("Audio"));
    audioAction->setIcon(QIcon(":/icon/audio"));

    audioAction->setEnabled(true);

    // text action
    textAction = new QAction(this);
    textAction->setText(tr("Text"));
    textAction->setIcon(QIcon(":/icon/text"));

    textAction->setEnabled(true);

    // script action
    scriptAction = new QAction(this);
    scriptAction->setText(tr("Script"));
    scriptAction->setIcon(QIcon(":/icon/script"));

    scriptAction->setEnabled(true);

    // settings action
    settingsAction = new QAction(this);
    settingsAction->setText(tr("Settings"));
    settingsAction->setIcon(QIcon(":/icon/settings"));

    settingsAction->setEnabled(true);

    // port action
    portAction = new QAction(this);
    portAction->setText(tr("Port"));
    portAction->setIcon(QIcon(":/icon/port"));

    portAction->setEnabled(true);

    // property action
    propertyAction = new QAction(this);
    propertyAction->setText(tr("Property"));
    propertyAction->setIcon(QIcon(":/icon/property"));

    propertyAction->setEnabled(true);

    // area action
    areaAction = new QAction(this);
    areaAction->setText(tr("Area"));
    areaAction->setIcon(QIcon(":/icon/area"));

    areaAction->setEnabled(true);

    // aggregator action
    aggregatorAction = new QAction(this);
    aggregatorAction->setText(tr("Aggregator"));
    aggregatorAction->setIcon(QIcon(":/icon/aggregator"));

    aggregatorAction->setEnabled(false);

    // bring to front action
    bringfrontAction = new QAction(this);
    bringfrontAction->setText(tr("Bring to Front"));

    bringfrontAction->setEnabled(false);
    bringfrontAction->setShortcut(QKeySequence("Shift+Ctrl+]"));

    // bring forward action
    bringforwardAction = new QAction(this);
    bringforwardAction->setText(tr("Bring Forward"));

    bringforwardAction->setEnabled(false);
    bringforwardAction->setShortcut(QKeySequence("Ctrl+]"));

    // send backward action
    sendbackwardAction = new QAction(this);
    sendbackwardAction->setText(tr("Send Backward"));

    sendbackwardAction->setEnabled(false);
    sendbackwardAction->setShortcut(QKeySequence("Ctrl+["));

    // send to back action
    sendbackAction = new QAction(this);
    sendbackAction->setText(tr("Send to Back"));

    sendbackAction->setEnabled(false);
    sendbackAction->setShortcut(QKeySequence("Shift+Ctrl+["));

    // hide action
    hideAction = new QAction(this);
    hideAction->setText(tr("Hide"));

    hideAction->setEnabled(false);

    // properties action
    propertiesAction = new QAction(this);
    propertiesAction->setText(tr("Properties"));

    propertiesAction->setEnabled(false);
}

void QnstGraphicsContext::createMenus()
{
    // view menu
    viewMenu = new QMenu();
    viewMenu->setTitle(tr("View"));

    viewMenu->setEnabled(false);

    viewMenu->addAction(zoominAction);
    viewMenu->addAction(zoomoutAction);
    viewMenu->addAction(zoomresetAction);
    viewMenu->addSeparator();
    viewMenu->addAction(fullscreenAction);

    // insert menu
    insertMenu = new QMenu();
    insertMenu->setTitle(tr("Insert"));

    insertMenu->addAction(imageAction);
    insertMenu->addAction(videoAction);
    insertMenu->addAction(audioAction);
    insertMenu->addAction(textAction);
    insertMenu->addSeparator();
    insertMenu->addAction(scriptAction);
    insertMenu->addAction(settingsAction);
    insertMenu->addSeparator();
    insertMenu->addAction(contextAction);
    insertMenu->addAction(switchAction);
    insertMenu->addSeparator();
    insertMenu->addAction(bodyAction);
    insertMenu->addSeparator();
    insertMenu->addAction(portAction);
    insertMenu->addAction(areaAction);
    insertMenu->addAction(propertyAction);
    insertMenu->addSeparator();
    insertMenu->addAction(aggregatorAction);

    insertMenu->setEnabled(true);

    // show menu
    showMenu = new QMenu();
    showMenu->setTitle(tr("Show"));

    showMenu->setEnabled(false);

    // arrange menu
    arrangeMenu = new QMenu();
    arrangeMenu->setTitle(tr("Arrange"));

    arrangeMenu->setEnabled(false);

    arrangeMenu->addAction(bringfrontAction);
    arrangeMenu->addAction(bringforwardAction);
    arrangeMenu->addAction(sendbackwardAction);
    arrangeMenu->addAction(sendbackAction);

    // context menu
    contextMenu = new QMenu();
    contextMenu->addAction(helpAction);
    contextMenu->addSeparator();
    contextMenu->addAction(undoAction);
    contextMenu->addAction(redoAction);
    contextMenu->addSeparator();
    contextMenu->addAction(cutAction);
    contextMenu->addAction(copyAction);
    contextMenu->addAction(pasteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(deleteAction);
    contextMenu->addSeparator();
    contextMenu->addAction(exportAction);
    contextMenu->addSeparator();
    contextMenu->addMenu(viewMenu);
    contextMenu->addMenu(insertMenu);
    contextMenu->addMenu(showMenu);
    contextMenu->addMenu(arrangeMenu);
    contextMenu->addSeparator();
    contextMenu->addAction(hideAction);
    contextMenu->addSeparator();
    contextMenu->addAction(propertiesAction);
}

void QnstGraphicsContext::createConnections()
{
    connect(imageAction, SIGNAL(triggered()), SLOT(performImage()));
    connect(audioAction, SIGNAL(triggered()), SLOT(performAudio()));
    connect(textAction, SIGNAL(triggered()), SLOT(performText()));
    connect(videoAction, SIGNAL(triggered()), SLOT(performVideo()));
    connect(scriptAction, SIGNAL(triggered()), SLOT(performScript()));
    connect(settingsAction, SIGNAL(triggered()), SLOT(performSettings()));

    connect(contextAction, SIGNAL(triggered()), SLOT(performContext()));
    connect(switchAction, SIGNAL(triggered()), SLOT(performSwitch()));
}

void QnstGraphicsContext::performContext()
{
    QnstGraphicsContext* entity = new QnstGraphicsContext(this);
    entity->setTop(getHeight()/2 - getHeight()/4);
    entity->setLeft(getWidth()/2 - getWidth()/4);
    entity->setWidth(getWidth()/2);
    entity->setHeight(getHeight()/2);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performSwitch()
{
    QnstGraphicsSwitch* entity = new QnstGraphicsSwitch(this);
    entity->setTop(getHeight()/2 - getHeight()/4);
    entity->setLeft(getWidth()/2 - getWidth()/4);
    entity->setWidth(getWidth()/2);
    entity->setHeight(getHeight()/2);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performImage()
{
    QnstGraphicsImage* entity = new QnstGraphicsImage(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(56);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performAudio()
{
    QnstGraphicsAudio* entity = new QnstGraphicsAudio(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(56);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performVideo()
{
    QnstGraphicsVideo* entity = new QnstGraphicsVideo(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(56);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performText()
{
    QnstGraphicsText* entity = new QnstGraphicsText(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(56);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performScript()
{
    QnstGraphicsScript* entity = new QnstGraphicsScript(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(56);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::performSettings()
{
    QnstGraphicsSettings* entity = new QnstGraphicsSettings(this);
    entity->setTop(getHeight()/2 - 56/2);
    entity->setLeft(getWidth()/2 - 56/2);
    entity->setWidth(56);
    entity->setHeight(56);
    entity->adjust();

    connect(entity, SIGNAL(entityAdded(QnstEntity*)), SIGNAL(entityAdded(QnstEntity*)));
    connect(entity, SIGNAL(entityChanged(QnstEntity*)), SIGNAL(entityChanged(QnstEntity*)));
    connect(entity, SIGNAL(entityRemoved(QnstEntity*)), SIGNAL(entityRemoved(QnstEntity*)));
    connect(entity, SIGNAL(entitySelected(QnstEntity*)), SIGNAL(entitySelected(QnstEntity*)));

    addncgGraphicsEntity(entity);

    emit entityAdded(entity);
}

void QnstGraphicsContext::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QnstGraphicsComposition::contextMenuEvent(event);

    if (!event->isAccepted())
    {
        contextMenu->exec(event->screenPos());

        event->accept();
    }
}

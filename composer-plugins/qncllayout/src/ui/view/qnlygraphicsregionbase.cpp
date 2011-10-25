/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
 */
#include "qnlygraphicsregionbase.h"

QnlyGraphicsRegionBase::QnlyGraphicsRegionBase(QObject* parent,
                                               QMenu* switchMenu)
    : QGraphicsScene(parent)
{
    this->switchMenu = switchMenu;

    this->switchMenu->setEnabled(true);

    createActions();
    createMenus();
    createConnections();

    selectedRegion = NULL;
}

QnlyGraphicsRegionBase::~QnlyGraphicsRegionBase()
{
}

QString QnlyGraphicsRegionBase::getId() const
{
    return id;
}

void QnlyGraphicsRegionBase::setId(const QString &id)
{
    this->id = id;
}

QString QnlyGraphicsRegionBase::getUid() const
{
    return uid;
}

void QnlyGraphicsRegionBase::setUid(const QString &uid)
{
    this->uid = uid;
}

QString QnlyGraphicsRegionBase::getRegion() const
{
    return region;
}

void QnlyGraphicsRegionBase::setRegion(const QString &region)
{
    this->region = region;
}

QString QnlyGraphicsRegionBase::getDevice() const
{
    return device;
}

void QnlyGraphicsRegionBase::setDevice(const QString &device)
{
    this->device = device;
}

void QnlyGraphicsRegionBase::selectRegion(QnlyGraphicsRegion* region)
{
    if (!region->isSelected())
    {
        if (selectedRegion != NULL)
        {
            selectedRegion->setSelected(false);
        }

        region->setSelected(true);

        selectedRegion = region;
    }
}

void QnlyGraphicsRegionBase::changeRegion(QnlyGraphicsRegion* region,
                const QMap<QString, QString> attributes)
{
    if (region != NULL)
    {
        if (!attributes["id"].isEmpty()){
            region->setId(attributes["id"]);
        }

        if (!attributes["title"].isEmpty()){
            region->setTitle(attributes["title"]);
        }

        if (!attributes["color"].isEmpty()){
            region->setColor(attributes["color"]);
        }

        if (!attributes["top"].isEmpty()){
            if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["top"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal top = attribute.toDouble();

                if (top >= 0 && top <= 100){
                    region->setRelativeTop(top/100);
                }
            }else if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["top"];

                qreal top = attribute.toDouble();

                if (top >= 0 && top <= 1){
                    region->setRelativeTop(top);
                }
            }
        }

        if (!attributes["left"].isEmpty()){
            if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["left"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal left = attribute.toDouble();

                if (left >= 0 && left <= 100){
                    region->setRelativeLeft(left/100);
                }

            }else if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["left"];

                qreal left = attribute.toDouble();

                if (left >= 0 && left <= 1){
                    region->setRelativeLeft(left);
                }
            }
        }

        if (!attributes["right"].isEmpty()){
            if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["right"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal right = attribute.toDouble();

                if (right >= 0 && right <= 100){
                    region->setRelativeRight(right/100);
                }

            }else if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["right"];

                qreal right = attribute.toDouble();

                if (right >= 0 && right <= 1){
                    region->setRelativeRight(right);
                }
            }
        }

        if (!attributes["bottom"].isEmpty()){
            if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["bottom"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal bottom = attribute.toDouble();

                if (bottom >= 0 && bottom <= 100){
                    region->setRelativeBottom(bottom/100);
                }

            }else if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["bottom"];

                qreal bottom = attribute.toDouble();

                if (bottom >= 0 && bottom <= 1){
                    region->setRelativeBottom(bottom);
                }
            }
        }

        if (!attributes["width"].isEmpty()){
            if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["width"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal width = attribute.toDouble();

                if (width >= 0 && width <= 100){
                    region->setRelativeWidth(width/100);
                }

            }else if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["width"];

                qreal width = attribute.toDouble();

                if (width >= 0 && width <= 1){
                    region->setRelativeWidth(width);
                }
            }
        }

        if (!attributes["height"].isEmpty()){
            if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["height"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal height = attribute.toDouble();

                if (height >= 0 && height <= 100){
                    region->setRelativeHeight(height/100);
                }

            }else if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["height"];

                qreal height = attribute.toDouble();

                if (height >= 0 && height <= 1){
                    region->setRelativeHeight(height);
                }
            }
        }

        region->adjust();
    }
}

void QnlyGraphicsRegionBase::requestRegionChange(QnlyGraphicsRegion* region,
                            QMap<QString, QString> attributes)
{

    // setting
    QMap<QString, QString> full;

    double value = 0.0;
    if (attributes.contains("id"))
        full["id"] = attributes["id"];
    else if (!region->getId().isEmpty())
        full["id"] = region->getId();

    if (attributes.contains("title"))
        full["title"] = attributes["title"];
    else if (!region->getTitle().isEmpty())
        full["title"] = region->getTitle();

    if (attributes.contains("color"))
        full["color"] = attributes["color"];
    else if (!region->getColor().isEmpty())
        full["color"] = region->getColor();

    if (attributes.contains("top"))
        full["top"] = attributes["top"];
    else
    {
        value = region->getRelativeTop()*100;
        ROUND_DOUBLE(value);
        full["top"] = QString::number(value, 'f', 2) + "%";
    }

    if (attributes.contains("left"))
        full["left"] = attributes["left"];
    else
    {
        value = region->getRelativeLeft()*100;
        ROUND_DOUBLE(value);
        full["left"] = QString::number(value, 'f', 2) + "%";
    }

    if (attributes.contains("right"))
        full["right"] = attributes["right"];
    else
    {
        value = region->getRelativeRight()*100;
        ROUND_DOUBLE(value);
        full["right"] = QString::number(value, 'f', 2) + "%";
    }

    if (attributes.contains("bottom"))
        full["bottom"] = attributes["bottom"];
    else
    {
        value = region->getRelativeBottom()*100;
        ROUND_DOUBLE(value);
        full["bottom"] = QString::number(value, 'f', 2) + "%";
    }

    if (attributes.contains("width"))
        full["width"] = attributes["width"];
    else
    {
        value = region->getRelativeWidth()*100;
        ROUND_DOUBLE(value);
        full["width"] = QString::number(value, 'f', 2) + "%";
    }

    if (attributes.contains("height"))
        full["height"] = attributes["height"];
    else
    {
        value = region->getRelativeHeight()*100;
        ROUND_DOUBLE(value);
        full["height"] = QString::number(value, 'f', 2) + "%";
    }

    // TODO: zIndex

    emit regionChangeRequested( region->getUid(),
                                uid,
                                full);
}


void QnlyGraphicsRegionBase::requestRegionSelection(
        QnlyGraphicsRegion* region)
{
    emit regionSelectionRequested(region->getUid(),uid);
}

void QnlyGraphicsRegionBase::QnlyGraphicsRegionBase::createActions()
{
    // help action
    helpAction = new QAction(this);
    helpAction->setText(tr("Help"));

    helpAction->setEnabled(true);
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

    deleteAction->setEnabled(true);
    deleteAction->setShortcut(QKeySequence("Del"));

    // zoomin action
    zoominAction = new QAction(this);
    zoominAction->setText(tr("Zoom In"));

    zoominAction->setEnabled(true);
    zoominAction->setShortcut(QKeySequence("Ctrl++"));

    // zoomout action
    zoomoutAction = new QAction(this);
    zoomoutAction->setText(tr("Zoom Out"));

    zoomoutAction->setEnabled(true);
    zoomoutAction->setShortcut(QKeySequence("Ctrl+-"));

    // reset action
    zoomresetAction = new QAction(this);
    zoomresetAction->setText(tr("Reset"));

    zoomresetAction->setEnabled(true);
    zoomresetAction->setShortcut(QKeySequence("Ctrl+0"));

    // fullscreen action
    fullscreenAction = new QAction(this);
    fullscreenAction->setText(tr("Full Screen"));

    fullscreenAction->setEnabled(true);
    fullscreenAction->setShortcut(QKeySequence("Ctrl+F"));

    // export action
    exportAction = new QAction(this);
    exportAction->setText(tr("Export..."));

    exportAction->setEnabled(true);

    // region action
    regionAction = new QAction(this);
    regionAction->setText(tr("Region"));

    regionAction->setEnabled(true);

    // regionbase action
    regionbaseAction = new QAction(this);
    regionbaseAction->setText(tr("Regionbase"));

    regionbaseAction->setEnabled(true);

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

    propertiesAction->setEnabled(true);

    regionActionGroup = new QActionGroup(this);
    regionActionGroup->setExclusive(false);
}

void QnlyGraphicsRegionBase::createMenus()
{
    // view menu
    viewMenu = new QMenu();
    viewMenu->setTitle(tr("View"));

    viewMenu->setEnabled(true);

    viewMenu->addAction(zoominAction);
    viewMenu->addAction(zoomoutAction);
    viewMenu->addAction(zoomresetAction);
    viewMenu->addSeparator();
    viewMenu->addAction(fullscreenAction);

    // insert menu
    insertMenu = new QMenu();
    insertMenu->setTitle(tr("Insert"));

    insertMenu->setEnabled(true);

    insertMenu->addAction(regionAction);
    insertMenu->addAction(regionbaseAction);

    // show menu
    showMenu = new QMenu();
    showMenu->setTitle(tr("Show"));

    showMenu->setEnabled(true);

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
    contextMenu->addMenu(switchMenu);
    contextMenu->addSeparator();
    contextMenu->addAction(propertiesAction);
}

void QnlyGraphicsRegionBase::createConnections()
{
    connect(regionAction, SIGNAL(triggered()),
                          SLOT(performRegion()));

    connect(regionbaseAction, SIGNAL(triggered()),
                              SIGNAL(regionbasePerformed()));

    connect(regionActionGroup, SIGNAL(triggered(QAction*)),
            SLOT(performShow(QAction*)));

    connect(deleteAction, SIGNAL(triggered()),
            SLOT(performDelete()));
}

void QnlyGraphicsRegionBase::performShow(QAction* action)
{

    if (!action->isChecked()){

        regions[regionActions.key(action)]->setVisible(false);
    }else{
        regions[regionActions.key(action)]->setVisible(true);
    }
}

void QnlyGraphicsRegionBase::requestAdditionRegion(QnlyGraphicsRegion* parent)
{
    QMap<QString, QString> attributes;

    attributes["top"] = "10%";
    attributes["left"] = "10%";
    attributes["right"] = "10%";
    attributes["bottom"] = "10%";
    attributes["width"] = "80%";
    attributes["height"] = "80%";

    emit regionAdditionRequested("", parent->getUid(),uid,attributes);
}

void QnlyGraphicsRegionBase::requestRegionDeletion(QnlyGraphicsRegion* region)
{
    emit regionDeletionRequested(region->getUid(), uid);
}

void QnlyGraphicsRegionBase::hideRegion(QnlyGraphicsRegion* region)
{
    regionActions[region->getUid()]->trigger();
}

void QnlyGraphicsRegionBase::removeRegion(QnlyGraphicsRegion* region)
{
    if (region != NULL)
    {
        if (region->parentItem() != NULL)
        {
            QnlyGraphicsRegion* parent =
                    (QnlyGraphicsRegion*) region->parentItem();

            foreach(QGraphicsItem* item, region->childItems())
            {
                QnlyGraphicsRegion* child = (QnlyGraphicsRegion*) item;

                regions.remove(child->getUid());
            }

            parent->removeRegion(region);
        }
        else
        {
            foreach(QGraphicsItem* item, region->childItems())
            {
                QnlyGraphicsRegion* child = (QnlyGraphicsRegion*) item;

                regions.remove(child->getUid());
            }

            regionActionGroup->removeAction(regionActions[region->getUid()]);

            regionActions.remove(region->getUid());
        }

        removeItem(region);

        regions.remove(region->getUid());

//        delete(region);

        selectedRegion = NULL;

        emit regionBaseSelectionRequested(uid);
    }
}

void QnlyGraphicsRegionBase::performDelete()
{
    emit regionBaseDeletionRequested(uid);
}

void QnlyGraphicsRegionBase::performRegion()
{
    QMap<QString, QString> attributes;

    attributes["top"] = "10%";
    attributes["left"] = "10%";
    attributes["right"] = "10%";
    attributes["bottom"] = "10%";
    attributes["width"] = "80%";
    attributes["height"] = "80%";

    emit regionAdditionRequested("", "",uid,attributes);
}

void QnlyGraphicsRegionBase::addRegion(QnlyGraphicsRegion* region,
               QnlyGraphicsRegion* parent,
               const QMap<QString, QString> attributes)
{
    if (region != NULL)
    {
        /* changing */
        if (!attributes["id"].isEmpty()){
            region->setId(attributes["id"]);
        }

        if (!attributes["title"].isEmpty()){
            region->setTitle(attributes["title"]);
        }

        if (!attributes["color"].isEmpty()){
            region->setColor(attributes["color"]);
        }

        if (!attributes["top"].isEmpty()){
            if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["top"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal top = attribute.toDouble();

                if (top >= 0 && top <= 100){
                    region->setRelativeTop(top/100);
                }
            }else if (attributes["top"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["top"];

                qreal top = attribute.toDouble();

                if (top >= 0 && top <= 1){
                    region->setRelativeTop(top);
                }
            }
        }

        if (!attributes["left"].isEmpty())
        {
            if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?%")))
            {
                QString attribute = attributes["left"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal left = attribute.toDouble();

                if (left >= 0 && left <= 100){
                    region->setRelativeLeft(left/100);
                }

            }
            else if (attributes["left"].contains(QRegExp("\\d+(.\\d+)?")))
            {
                QString attribute = attributes["left"];

                qreal left = attribute.toDouble();

                if (left >= 0 && left <= 1){
                    region->setRelativeLeft(left);
                }
            }
        }

        if (!attributes["right"].isEmpty()){
            if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?%")))
            {
                QString attribute = attributes["right"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal right = attribute.toDouble();

                if (right >= 0 && right <= 100){
                    region->setRelativeRight(right/100);
                }

            }
            else if (attributes["right"].contains(QRegExp("\\d+(.\\d+)?")))
            {
                QString attribute = attributes["right"];

                qreal right = attribute.toDouble();

                if (right >= 0 && right <= 1){
                    region->setRelativeRight(right);
                }
            }
        }

        if (!attributes["bottom"].isEmpty()){
            if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?%"))){
                QString attribute = attributes["bottom"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal bottom = attribute.toDouble();

                if (bottom >= 0 && bottom <= 100){
                    region->setRelativeBottom(bottom/100);
                }

            }else if (attributes["bottom"].contains(QRegExp("\\d+(.\\d+)?"))){
                QString attribute = attributes["bottom"];

                qreal bottom = attribute.toDouble();

                if (bottom >= 0 && bottom <= 1){
                    region->setRelativeBottom(bottom);
                }
            }
        }

        if (!attributes["width"].isEmpty())
        {
            if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?%")))
            {
                QString attribute = attributes["width"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal width = attribute.toDouble();

                if (width >= 0.0 &&  width <= 100.0)
                {
                    region->setRelativeWidth(width/100);
                }
            }
            else if (attributes["width"].contains(QRegExp("\\d+(.\\d+)?")))
            {
                QString attribute = attributes["width"];

                qreal width = attribute.toDouble();

                if (width >= 0 && width <= 1){
                    region->setRelativeWidth(width);
                }
            }
        }

        if (!attributes["height"].isEmpty()){
            if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?%")))
            {
                QString attribute = attributes["height"];
                attribute.remove(attribute.length()-1,1); // removing '%'

                qreal height = attribute.toDouble();

                if (height >= 0 && height <= 100)
                    region->setRelativeHeight(height/100);

            }
            else if (attributes["height"].contains(QRegExp("\\d+(.\\d+)?")))
            {
                QString attribute = attributes["height"];

                qreal height = attribute.toDouble();

                if (height >= 0 && height <= 1){
                    region->setRelativeHeight(height);
                }
            }
        }

        if (parent != NULL)
        {
           parent->addRegion(region);
        }
        else
        {
            addItem(region);

            QAction* action = new QAction(this);
            action->setText(region->getId());

            showMenu->addAction(action);

            action->setCheckable(true);
            action->setChecked(true);
            action->setEnabled(true);

            regionActionGroup->addAction(action);

            regionActions[region->getUid()] = action;
        }

        region->adjust();

        regions[region->getUid()] = region;

        connect(region,
                SIGNAL(regionSelectionRequested(QnlyGraphicsRegion*)),
                SLOT(requestRegionSelection(QnlyGraphicsRegion*)));

        connect(region,
    SIGNAL(regionChangeRequested(QnlyGraphicsRegion*,QMap<QString,QString>)),
    SLOT(requestRegionChange(QnlyGraphicsRegion*,QMap<QString,QString>)));

        connect(region,
    SIGNAL(regionbasePerformed()),
    SIGNAL(regionbasePerformed()));


        connect(region,
                SIGNAL(regionAdditionRequested(QnlyGraphicsRegion*)),
                SLOT(requestAdditionRegion(QnlyGraphicsRegion*)));

        connect(region,
                SIGNAL(regionDeletionRequested(QnlyGraphicsRegion*)),
                SLOT(requestRegionDeletion(QnlyGraphicsRegion*)));

        // \fixme seg fault on Outline
        QMap<QString, QString> noChangeAtts;

        requestRegionChange(region,noChangeAtts);

        emit requestRegionSelection(region);
    }
}

void QnlyGraphicsRegionBase::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);

    if (!event->isAccepted()){
        if (event->button() == Qt::RightButton){
            event->ignore();

        }else if (event->button() == Qt::LeftButton){
            emit regionBaseSelectionRequested(uid);
        }

       event->accept();
    }
}

void QnlyGraphicsRegionBase::contextMenuEvent(
        QGraphicsSceneContextMenuEvent* event)
{
    QGraphicsScene::contextMenuEvent(event);

    if (!event->isAccepted())
    {
        emit regionBaseSelectionRequested(uid);

        contextMenu->exec(event->screenPos());

        event->accept();
    }
}

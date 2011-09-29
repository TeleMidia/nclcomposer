/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "qnstnode.h"

QnstNode::QnstNode(QnstNode* parent)
    : QncgNode(parent)
{
    setColor("#E4E4E4");
    setBordertColor("#E4E4E4");

    createActions();
    createMenus();
    createConnections();
}

QnstNode::~QnstNode()
{

}

QMap<QString, QVector<qreal> > QnstNode::getAngles() const
{
    return angles;
}

void QnstNode::addAngle(QString uid, qreal angle)
{
    angles[uid].append(angle);
}

void QnstNode::removeAngle(QString uid, qreal angle)
{
    QVector<qreal> v = angles[uid];

    int index = v.indexOf(angle);

    if (index >= 0){
         angles[uid].remove(index);
    }
}

void QnstNode::internalselection()
{
    emit entitySelected(this);
}

QString QnstNode::getColor() const
{
    return color;
}

void QnstNode::setColor(QString color)
{
    this->color = color;
}

QString QnstNode::getBorderColor() const
{
    return borderColor;
}

void QnstNode::setBordertColor(QString borderColor)
{
    this->borderColor = borderColor;
}

void QnstNode::setName(QString name)
{
    QnstEntity::setName(name);

    setToolTip(name);
}

void QnstNode::createConnections()
{
    connect(this,SIGNAL(entitySelected()),SLOT(internalselection()));

    connect(deleteAction,SIGNAL(triggered()),SLOT(deleteEntity()));
}

void QnstNode::deleteEntity()
{
    scene()->removeItem(this);

    emit entityRemoved(this);
}

void QnstNode::createActions()
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

    // body action
    bodyAction = new QAction(this);
    bodyAction->setText(tr("Body"));

    bodyAction->setEnabled(true);

    // context action
    contextAction = new QAction(this);
    contextAction->setText(tr("Context"));

    contextAction->setEnabled(false);

    // switch action
    switchAction = new QAction(this);
    switchAction->setText(tr("Switch"));

    switchAction->setEnabled(false);

    // media action
    mediaAction = new QAction(this);
    mediaAction->setText(tr("Media"));

    mediaAction->setEnabled(false);

    // port action
    portAction = new QAction(this);
    portAction->setText(tr("Port"));

    portAction->setEnabled(false);

    // property action
    propertyAction = new QAction(this);
    propertyAction->setText(tr("Property"));

    propertyAction->setEnabled(false);

    // area action
    areaAction = new QAction(this);
    areaAction->setText(tr("Area"));

    areaAction->setEnabled(false);

    // aggregator action
    aggregatorAction = new QAction(this);
    aggregatorAction->setText(tr("Aggregator"));

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

    propertiesAction->setEnabled(true);
}

void QnstNode::createMenus()
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

    insertMenu->addAction(mediaAction);
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
    contextMenu->addAction(propertiesAction);
}

void QnstNode::draw(QPainter* painter)
{
    painter->setBrush(QColor(color));

    QPen mpen(QBrush(QColor(borderColor)), 3);
    mpen.setCosmetic(true);

    painter->setPen(mpen);
    painter->drawEllipse(4, 4, getWidth()-1.5, getHeight()-1.5);

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawEllipse(getMoveLeft()+4-getLeft(),
                             getMoveTop()+4-getTop(),
                             getWidth()-1,
                             getHeight()-1);

    }else if (isResizing()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->drawEllipse(getResizeLeft()+4-getLeft(),
                             getResizeTop()+4-getTop(),
                             getResizeWidth()-1,
                             getResizeHeight()-1);
    }
}

void QnstNode::delineate(QPainterPath* painter) const
{
    painter->addEllipse(4, 4, getWidth(), getHeight());
}

void QnstNode::link(QGraphicsSceneMouseEvent* event)
{

    QList<QGraphicsItem *> bitems = scene()->items(getSceneEdge()->line().p1());

    if (bitems.count() && bitems.first() == getSceneEdge()){
        bitems.removeFirst();
    }

    QList<QGraphicsItem *> eitems = scene()->items(getSceneEdge()->line().p2());

    if (eitems.count() && eitems.first() == getSceneEdge()){
        eitems.removeFirst();
    }

    if (bitems.count() && eitems.count()){
        QncgEntity* bentity = (QncgEntity*) bitems.first();
        QncgEntity* eentity = (QncgEntity*) eitems.first();

        if (bentity != eentity){
            if ((bentity->getEntityType() == Qncg::Node &&
                eentity->getEntityType() == Qncg::Node)){

                if (bentity->parentItem() != NULL &&
                    eentity->parentItem() != NULL){

                    if (bentity->parentItem() == eentity->parentItem()){
                        QnstNode* bnode = (QnstNode*) bentity;
                        QnstNode* enode = (QnstNode*) eentity;

                        QDialog* d = new QDialog();
                        d->setWindowModality(Qt::ApplicationModal);

                        Ui::QnstDBind dbind;
                        dbind.setupUi(d);

                        d->exec();

                        if (d->result() == QDialog::Accepted){

                        QnstEdge* edge = new QnstEdge();
                        edge->setParentItem(parentItem());
                        edge->setnstParent(getnstParent());
                        edge->setnBegin(bnode);
                        edge->setnEnd(enode);

                        int inv = -1;

                        qreal step = 45;

                        qreal angle = 0;

                        if (angles.contains(enode->getUid())){
                            while (angles.value(enode->getUid()).contains(angle)){

                                if (inv > 0){
                                    angle *= -1;
                                    angle += step;
                                }else{
                                    angle *= inv;
                                }

                                inv *= -1;
                            }

                            angles[enode->getUid()].append(angle);

                            enode->addAngle(getUid(),-angle);

                        }else{
                            angles[enode->getUid()].append(angle);

                            enode->addAngle(getUid(),-angle);
                        }

                        edge->setAngle(angle);

                        if (dbind.ccondition->currentText() == "none"){
                            edge->setConditionType(Qnst::NoCondition);

                        }else if (dbind.ccondition->currentText() == "onBegin"){
                            edge->setConditionType(Qnst::OnBegin);

                        }else if (dbind.ccondition->currentText() == "onEnd"){
                            edge->setConditionType(Qnst::OnEnd);

                        }else if (dbind.ccondition->currentText() == "onPause"){
                            edge->setConditionType(Qnst::OnPause);

                        }else if (dbind.ccondition->currentText() == "onResume"){
                            edge->setConditionType(Qnst::OnResume);

                        }else if (dbind.ccondition->currentText() == "onSelection"){
                            edge->setConditionType(Qnst::OnSelection);

                        }else if (dbind.ccondition->currentText() == "onSet"){
                            edge->setConditionType(Qnst::OnSet);

                        }else if (dbind.ccondition->currentText() == "onAbort"){
                            edge->setConditionType(Qnst::OnAbort);

                        }else if (dbind.ccondition->currentText() == "onKeySelection"){
                            edge->setConditionType(Qnst::OnKeySelection);
                        }


                        if (dbind.caction->currentText() == "none"){
                            edge->setActionType(Qnst::NoAction);

                        }else if (dbind.caction->currentText() == "start"){
                            edge->setActionType(Qnst::Start);

                        }else if (dbind.caction->currentText() == "stop"){
                            edge->setActionType(Qnst::Stop);

                        }else if (dbind.caction->currentText() == "pause"){
                            edge->setActionType(Qnst::Pause);

                        }else if (dbind.caction->currentText() == "resume"){
                            edge->setActionType(Qnst::Resume);

                        }else if (dbind.caction->currentText() == "abort"){
                            edge->setActionType(Qnst::Abort);

                        }else if (dbind.caction->currentText() == "set"){
                            edge->setActionType(Qnst::Set);

                        }else if (dbind.caction->currentText() == "startDelay"){
                            edge->setActionType(Qnst::StartDelay);
                        }

                        edge->adjust();

                        connect(edge, SIGNAL(entityRemoved(QnstEntity*)),
                                SIGNAL(entityRemoved(QnstEntity*)));

                        delete d;

                        bnode->addBeginningEdge(edge);
                        enode->addEndingEdge(edge);

                        emit entityAdded(edge);
                        }
                    }
                }
                // else
                //     parent == scene

            }else if (bentity->getEntityType() == Qncg::Node &&
                      eentity->getEntityType() == Qncg::Interface){

                if (bentity->parentItem() != NULL &&
                    eentity->parentItem() != NULL){

                    if (bentity->parentItem() == eentity->parentItem()->parentItem()){
                        QnstNode* bnode = (QnstNode*) bentity;
                        QnstInterface* einterface = (QnstInterface*) eentity;

                        QDialog* d = new QDialog();
                        d->setWindowModality(Qt::ApplicationModal);

                        Ui::QnstDBind dbind;
                        dbind.setupUi(d);

                        d->exec();

                     if (d->result() == QDialog::Accepted){

                        QnstEdge* edge = new QnstEdge();
                        edge->setParentItem(parentItem());
                        edge->setnstParent(getnstParent());
                        edge->setnBegin(bnode);
                        edge->setnEnd(einterface);

                        int inv = -1;

                        qreal step = 45;

                        qreal angle = 0;

                        if (angles.contains(einterface->getUid())){
                            while (angles.value(einterface->getUid()).contains(angle)){

                                if (inv > 0){
                                    angle *= -1;
                                    angle += step;
                                }else{
                                    angle *= inv;
                                }

                                inv *= -1;
                            }

                            angles[einterface->getUid()].append(angle);

                            einterface->addAngle(getUid(),-angle);

                        }else{
                            angles[einterface->getUid()].append(angle);

                            einterface->addAngle(getUid(),-angle);
                        }

                         edge->setAngle(angle);

                        if (dbind.ccondition->currentText() == "none"){
                            edge->setConditionType(Qnst::NoCondition);

                        }else if (dbind.ccondition->currentText() == "onBegin"){
                            edge->setConditionType(Qnst::OnBegin);

                        }else if (dbind.ccondition->currentText() == "onEnd"){
                            edge->setConditionType(Qnst::OnEnd);

                        }else if (dbind.ccondition->currentText() == "onPause"){
                            edge->setConditionType(Qnst::OnPause);

                        }else if (dbind.ccondition->currentText() == "onResume"){
                            edge->setConditionType(Qnst::OnResume);

                        }else if (dbind.ccondition->currentText() == "onSelection"){
                            edge->setConditionType(Qnst::OnSelection);

                        }else if (dbind.ccondition->currentText() == "onSet"){
                            edge->setConditionType(Qnst::OnSet);

                        }else if (dbind.ccondition->currentText() == "onAbort"){
                            edge->setConditionType(Qnst::OnAbort);

                        }else if (dbind.ccondition->currentText() == "onKeySelection"){
                            edge->setConditionType(Qnst::OnKeySelection);
                        }


                        if (dbind.caction->currentText() == "none"){
                            edge->setActionType(Qnst::NoAction);

                        }else if (dbind.caction->currentText() == "start"){
                            edge->setActionType(Qnst::Start);

                        }else if (dbind.caction->currentText() == "stop"){
                            edge->setActionType(Qnst::Stop);

                        }else if (dbind.caction->currentText() == "pause"){
                            edge->setActionType(Qnst::Pause);

                        }else if (dbind.caction->currentText() == "resume"){
                            edge->setActionType(Qnst::Resume);

                        }else if (dbind.caction->currentText() == "abort"){
                            edge->setActionType(Qnst::Abort);

                        }else if (dbind.caction->currentText() == "set"){
                            edge->setActionType(Qnst::Set);

                        }else if (dbind.caction->currentText() == "startDelay"){
                            edge->setActionType(Qnst::StartDelay);
                        }

                        edge->adjust();

                        delete d;

                        bnode->addBeginningEdge(edge);
                        einterface->addEndingEdge(edge);

                        connect(edge, SIGNAL(entityRemoved(QnstEntity*)),
                                SIGNAL(entityRemoved(QnstEntity*)));

                        emit entityAdded(edge);
                        }
                    }
                }
            }
        }
    }
}

void QnstNode::contextMenuEvent(QGraphicsSceneContextMenuEvent* event)
{
    QncgNode::contextMenuEvent(event);

    if (!event->isAccepted())
    {
        contextMenu->exec(event->screenPos());

        event->accept();
    }
}

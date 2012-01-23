#include "qnstgraphicsmedia.h"

QnstGraphicsMedia::QnstGraphicsMedia(QnstGraphicsNode* parent)
    : QnstGraphicsContent(parent)
{
    setnstType(Qnst::Media);

    setIcon(":/icon/media");

    setResizable(false);

    createObjects();
    createConnections();

    setnstId("");
}

QnstGraphicsMedia::~QnstGraphicsMedia()
{

}

QString QnstGraphicsMedia::getIcon() const
{
    return icon;
}

void QnstGraphicsMedia::setIcon(QString icon)
{
    this->icon = icon;
}

QString QnstGraphicsMedia::getSource() const
{
    return source;
}

void QnstGraphicsMedia::setSource(QString source)
{
    this->source = source;
}

void QnstGraphicsMedia::setnstId(QString id)
{
    QnstGraphicsContent::setnstId(id);

    QString tip = "";
    QString name = (getnstId() != "" ? getnstId() : "?");

    if (getnstType() == Qnst::Image){
        tip += "Image ("+name+")";

    }else if (getnstType() == Qnst::Audio){
        tip += "Audio ("+name+")";

    }else if (getnstType() == Qnst::Text){
        tip += "Text ("+name+")";

    }else if (getnstType() == Qnst::Video){
        tip += "Video ("+name+")";

    }else if (getnstType() == Qnst::Script){
        tip += "Script ("+name+")";

    }else if (getnstType() == Qnst::Html){
        tip += "Html ("+name+")";

    }else if (getnstType() == Qnst::Settings){
        tip += "Settings ("+name+")";

    }else{
        tip += "Media ("+name+")";
    }

    if (source == ""){
        tip += " - Alert: Missing 'src' attribute";
    }

    setToolTip(tip);
}

void QnstGraphicsMedia::createObjects()
{
    menu = new QnstMenu();
    menu->actionCut->setEnabled(true);
    menu->actionCopy->setEnabled(true);

    menu->actionDelete->setEnabled(true);

    menu->actionExport->setEnabled(true);

    menu->menuInsert->setEnabled(true);
    menu->actionArea->setEnabled(true);
    menu->actionProperty->setEnabled(true);
}

void QnstGraphicsMedia::createConnections()
{
    connect(menu, SIGNAL(undoRequested()), SIGNAL(undoRequested()));
    connect(menu, SIGNAL(redoRequested()), SIGNAL(redoRequested()));

    connect(menu, SIGNAL(cutRequested()), SIGNAL(cutRequested()));
    connect(menu, SIGNAL(copyRequested()), SIGNAL(copyRequested()));
    connect(menu, SIGNAL(pasteRequested()), SIGNAL(pasteRequested()));

    connect(menu, SIGNAL(deleteRequested()), SIGNAL(deleteRequested()));

    connect(menu, SIGNAL(exportRequested()), SIGNAL(exportRequested()));

    connect(menu, SIGNAL(areaRequested()), SLOT(performArea()));

    connect(menu, SIGNAL(propertyRequested()), SLOT(performProperty()));

    connect(menu, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
    connect(menu, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
    connect(menu, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
    connect(menu, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));
}

void QnstGraphicsMedia::performArea()
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

void QnstGraphicsMedia::performProperty()
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

void QnstGraphicsMedia::draw(QPainter* painter)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(icon));

    if (!isSelected() && hasMouseHover()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(QColor("#999999")), 0, Qt::DashLine)); // 0px = cosmetic border

        painter->drawRect(4, 4, getWidth(), getHeight());
    }

    painter->setPen(QPen(QBrush(Qt::black),0));

    if (source == ""){
        painter->drawPixmap((getWidth()-8)/2 + 12, (getHeight()-8)/2 + 4, 12, 12, QPixmap(":/icon/alert"));
    }

    if (getnstId().length() > 5){

        QString localid = getnstId().replace(3,getnstId().length()-3,"...");
        painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignLeft, localid);

    }else{
        painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignCenter, getnstId());
    }

    if (isMoving()){
        painter->setBrush(Qt::NoBrush);
        painter->setPen(QPen(QBrush(Qt::black), 0)); // 0px = cosmetic border

        painter->setRenderHint(QPainter::Antialiasing,false);
        painter->drawRect(getMoveLeft()+4-getLeft(), getMoveTop()+4-getTop(), getWidth()-1, getHeight()-1);
    }
}

void QnstGraphicsMedia::delineate(QPainterPath* painter) const
{
    painter->addRect(4, 4, getWidth(), getHeight());
}

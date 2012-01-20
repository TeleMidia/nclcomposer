#include "qnstgraphicsmedia.h"

QnstGraphicsMedia::QnstGraphicsMedia(QnstGraphicsNode* parent)
    : QnstGraphicsContent(parent)
{
    setnstType(Qnst::Media);

    setIcon(":/icon/media");

    setResizable(false);

    source = "";

    createObjects();
    createConnections();
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

void QnstGraphicsMedia::createObjects()
{
    menu = new QnstMenu();
    menu->actionCut->setEnabled(true);
    menu->actionCopy->setEnabled(true);

    menu->actionDelete->setEnabled(true);

    menu->actionExport->setEnabled(true);
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

    connect(menu, SIGNAL(zoominRequested()), SIGNAL(zoominRequested()));
    connect(menu, SIGNAL(zoomoutRequested()), SIGNAL(zoomoutRequested()));
    connect(menu, SIGNAL(zoomresetRequested()), SIGNAL(zoomresetRequested()));
    connect(menu, SIGNAL(fullscreenRequested()), SIGNAL(fullscreenRequested()));
}

void QnstGraphicsMedia::draw(QPainter* painter)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);

    painter->drawPixmap(4 + 8/2, 4 + 8/2, getWidth()-8, getHeight()-16-8, QPixmap(icon));

    if (source == ""){
        painter->drawPixmap((getWidth()-8)/2 + 16, (getHeight()-16-8)/2 + 16, 16, 16, QPixmap(":/icon/alert"));
    }

    painter->drawText(4 + 8/2, 4 + 8/2 + getHeight()-16-8, getWidth()-8, 16, Qt::AlignCenter, getnstId());

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

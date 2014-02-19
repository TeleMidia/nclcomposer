#include "view.h"

View::View(QWidget* parent)
    : QGraphicsView(parent)
{
    createObjects();
    createConnections();

    selected = NULL;
}

View::~View()
{

}

void View::createObjects()
{   
    scene = new Scene();
    scene->setParent(this);
    scene->setSceneRect(0, 0, 2000, 2000);

    setScene(scene);
}

void View::createConnections()
{
    connect(scene, SIGNAL(entityChanged(QncgGraphicsEntity*)), SLOT(alertEntityChange(QncgGraphicsEntity*)));
    connect(scene, SIGNAL(entitySelected(QncgGraphicsEntity*)), SLOT(alertEntitySelection(QncgGraphicsEntity*)));
}

void View::alertEntityChange(QncgGraphicsEntity* entity)
{
    qDebug() << "[QNCG]" << ":" << "Entity changed.";
}

void View::alertEntitySelection(QncgGraphicsEntity* entity)
{
    qDebug() << "[QNCG]" << ":" << "Entity selected.";

    if (selected != NULL){
        selected->setSelected(false);
    }

    selected = entity;
    selected->setSelected(true);
}

void View::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);

    if (event->button() == Qt::LeftButton){
        if (!event->isAccepted()){
            if (selected != NULL){
                selected->setSelected(false);
            }

            selected = NULL;
        }
    }
}

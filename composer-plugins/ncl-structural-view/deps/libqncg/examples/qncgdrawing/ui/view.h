#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsView>
#include <QMouseEvent>

#include <QDebug>

#include "scene.h"

class View : public QGraphicsView
{
    Q_OBJECT

public:
    View(QWidget* parent = 0);

    ~View();

protected:
    virtual void mousePressEvent(QMouseEvent* event);

protected slots:
    void alertEntityChange(QncgGraphicsEntity* entity);

    void alertEntitySelection(QncgGraphicsEntity* entity);

private:
    void createObjects();

    void createConnections();

    Scene* scene;

    QncgGraphicsEntity* selected;
};

#endif // VIEW_H

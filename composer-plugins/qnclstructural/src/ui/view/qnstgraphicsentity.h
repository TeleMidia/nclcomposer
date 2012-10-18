#ifndef QNSTGRAPHICSENTITY_H
#define QNSTGRAPHICSENTITY_H

#include <QMenu>
#include <QAction>
#include <QFileDialog>

#include <QDebug>

#include "qnstentity.h"
#include "qnstmenu.h"
#include "qncggraphicsentity.h"

//
// ATTENTION: This code needs a refactoring.
//

class QnstGraphicsEntity : public QncgGraphicsEntity, public QnstEntity
{
    Q_OBJECT

public:
    QnstGraphicsEntity(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsEntity();

    QnstGraphicsEntity* getnstGraphicsParent() const;

    void setnstGraphicsParent(QnstGraphicsEntity* parent);

    QMap<QString, QSet<int> > getAngles();

    void addAngle(QString uid, int angle);

    void removeAngle(QString uid, int angle);

    QVector<QnstGraphicsEntity*> getnstGraphicsEntities();

    void addnstGraphicsEntity(QnstGraphicsEntity* entity);

    void removenstGraphicsEntity(QnstGraphicsEntity* entity);

    virtual void adjust(bool avoidCollision = false) = 0;

    bool hasMouseHover();

    void setMouseHover(bool hover);

signals:
    void entityAdded(QnstGraphicsEntity* entity);

    void entityRemoved(QnstGraphicsEntity* entity);

    void entityChanged(QnstGraphicsEntity* entity);

    void entityAboutToChange(QnstGraphicsEntity* entity,
                             QMap<QString, QString> properties);

    void entitySelected(QnstGraphicsEntity* entity);

    void undoRequested();

    void redoRequested();

    void cutRequested();

    void copyRequested();

    void pasteRequested();

    void deleteRequested();

    void exportRequested();

    void zoominRequested();

    void zoomoutRequested();

    void zoomresetRequested();

    void fullscreenRequested();

protected:
    virtual void draw(QPainter* painter) = 0;

    virtual void delineate(QPainterPath* painter) const = 0;

    virtual void move(QGraphicsSceneMouseEvent* event) = 0;

    virtual void resize(QGraphicsSceneMouseEvent* event) = 0;

    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);

    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);

protected slots:
    void requestEntityChange();

    void requestEntityPreparation(QMap<QString, QString> properties);

    void requestEntitySelection();

public:
    QnstMenu* menu;
    bool isDraggable();
    void setDraggable(bool isDraggable);
    void setError(bool hasError);

private:
    bool draggable;
    QnstGraphicsEntity* parent;

    QMap<QString, QSet<int> > angles;

    QVector<QnstGraphicsEntity*> entities;

protected:
    bool hover, hasError;
};

#endif // QNSTGRAPHICSENTITY_H

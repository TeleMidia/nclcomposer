#ifndef QNSTGRAPHICSENTITY_H
#define QNSTGRAPHICSENTITY_H

#include <QMenu>
#include <QAction>
#include <QFileDialog>

#include <QDebug>

#include "qnstentity.h"
#include "qnstmenu.h"
#include "qncggraphicsentity.h"

class QnstGraphicsEntity : public QncgGraphicsEntity, public QnstEntity
{
    Q_OBJECT

public:
    QnstGraphicsEntity(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsEntity();

    QnstGraphicsEntity* getnstGraphicsParent() const;

    void setnstGraphicsParent(QnstGraphicsEntity* parent);

    QVector<QnstGraphicsEntity*> getnstGraphicsEntities();

    void addnstGraphicsEntity(QnstGraphicsEntity* entity);

    void removenstGraphicsEntity(QnstGraphicsEntity* entity);

    virtual void adjust() = 0;

signals:
    void entityAdded(QnstGraphicsEntity* entity);

    void entityRemoved(QnstGraphicsEntity* entity);

    void entityChanged(QnstGraphicsEntity* entity);

    void entityAboutToChange(QnstGraphicsEntity* entity, QMap<QString, QString> properties);

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

protected slots:
    void requestEntityChange();

    void requestEntityPreparation(QMap<QString, QString> properties);

    void requestEntitySelection();

public:
    QnstMenu* menu;

private:
    QnstGraphicsEntity* parent;

    QVector<QnstGraphicsEntity*> entities;
};

#endif // QNSTGRAPHICSENTITY_H

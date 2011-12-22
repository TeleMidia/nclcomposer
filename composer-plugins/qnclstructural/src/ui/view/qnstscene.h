#ifndef QNSTSCENE_H
#define QNSTSCENE_H

#include <QGraphicsSceneContextMenuEvent>

#include <QGraphicsScene>

#include <QDebug>

#include "qnstmenu.h"

#include "qnstentity.h"
#include "qnstgraphicsbody.h"

class QnstScene : public QGraphicsScene
{
    Q_OBJECT

public:
    QnstScene(QObject* parent = 0);

    ~QnstScene();

    QVector<QnstGraphicsEntity*> getRoots() const;

    void addRoot(QnstGraphicsEntity* root);

    void removeRoot(QnstGraphicsEntity* root);

signals:
    void entityAdded(QnstGraphicsEntity* entity);

    void entityRemoved(QnstGraphicsEntity* entity);

    void entityChanged(QnstGraphicsEntity* entity);

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
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
    void performBody();

private:
    void createObjects();

    void createConnections();

    QnstMenu* menu;

    QVector<QnstGraphicsEntity*> roots;
};

#endif // QNSTSCENE_H

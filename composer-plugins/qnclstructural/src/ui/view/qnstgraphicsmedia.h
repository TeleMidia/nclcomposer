#ifndef QNSTGRAPHICSMEDIA_H
#define QNSTGRAPHICSMEDIA_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>
#include <QKeyEvent>

#include "qnstgraphicscontent.h"
#include "qnstgraphicsproperty.h"
#include "qnstgraphicsarea.h"

class QnstGraphicsMedia : public QnstGraphicsContent
{
    Q_OBJECT

public:
    QnstGraphicsMedia(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsMedia();

    QString getIcon() const;

    void setIcon(QString icon);

    void setSource(QString source);

    void setnstId(QString id);

    QString getSource() const;

    QString getRefer() const;

    void setRefer(QString refer);

    QString getReferUID() const;

    void setReferUID(QString refetUID);

    QString getInstance() const;

    void setInstance(QString instance);

public slots:
    void performArea();

    void performProperty();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    void createObjects();

    void createConnections();

    QString refer;

    QString referUID;

    QString instance;

    QString icon;

    QString source;

    bool enableDrag;
};

#endif // QNSTGRAPHICSMEDIA_H

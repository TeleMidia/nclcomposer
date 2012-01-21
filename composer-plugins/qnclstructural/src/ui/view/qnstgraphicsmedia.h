#ifndef QNSTGRAPHICSMEDIA_H
#define QNSTGRAPHICSMEDIA_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>

#include "qnstgraphicscontent.h"

class QnstGraphicsMedia : public QnstGraphicsContent
{
public:
    QnstGraphicsMedia(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsMedia();

    QString getIcon() const;

    void setIcon(QString icon);

    void setSource(QString source);

    QString getSource() const;

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

    virtual void dropEvent(QGraphicsSceneDragDropEvent *event);

private:
    void createObjects();

    void createConnections();

    QString icon;

    QString source;
};

#endif // QNSTGRAPHICSMEDIA_H

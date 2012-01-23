#ifndef QNSTGRAPHICSMEDIA_H
#define QNSTGRAPHICSMEDIA_H

#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QList>
#include <QUrl>
#include <QFileInfo>

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

public slots:
    void performArea();

    void performProperty();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    void createObjects();

    void createConnections();

    QString icon;

    QString source;
};

#endif // QNSTGRAPHICSMEDIA_H

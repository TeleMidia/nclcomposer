#ifndef QNSTGRAPHICSMEDIA_H
#define QNSTGRAPHICSMEDIA_H

#include "qnstgraphicscontent.h"

class QnstGraphicsMedia : public QnstGraphicsContent
{
public:
    QnstGraphicsMedia(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsMedia();

    QString getIcon() const;

    void setIcon(QString icon);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    void createObjects();

    void createConnections();

    QString icon;
};

#endif // QNSTGRAPHICSMEDIA_H

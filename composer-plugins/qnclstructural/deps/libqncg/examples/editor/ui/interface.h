#ifndef INTERFACE_H
#define INTERFACE_H

#include "qncggraphicsinterface.h"

class Interface : public QncgGraphicsInterface
{
    Q_OBJECT

public:
    Interface(QncgGraphicsEntity* parent = 0);

    ~Interface();

signals:
    void entityChanged(QncgGraphicsEntity* entity);

    void entitySelected(QncgGraphicsEntity* entity);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

protected slots:
    void requestEntityChange();

    void requestEntitySelection();
};

#endif // INTERFACE_H

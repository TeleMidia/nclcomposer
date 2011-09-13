#ifndef QNSTEDGE_H
#define QNSTEDGE_H

#include "qncgedge.h"

#include "qncgentity.h"
#include "qnstentity.h"
#include "QSvgRenderer"

#include "cmath"

#include "ui_qnstdbind.h"

#include <QBitmap>
#include <QDialog>

class QnstEdge : public QncgEdge, public QnstEntity
{
public:
    QnstEdge(QncgEntity* parent = 0);

    ~QnstEdge();

    void setConditionType(int contitiontype);

    int getConditionType();

    void setActionType(int contitiontype);

    int getActionType();

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    QString conditionimage;

    QString actionimage;

    int conditiontype;

    int actiontype;
};

#endif // QNSTEDGE_H

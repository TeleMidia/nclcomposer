#ifndef QNSTGRAPHICSACTION_H
#define QNSTGRAPHICSACTION_H

#include "qnstgraphicsedge.h"

#include "qnstgraphicsbinddialog.h"

typedef QString QnstActionIcon;
typedef Qnst::ActionType QnstAction;

class QnstGraphicsAction : public QnstGraphicsEdge
{
    Q_OBJECT

public:
    QnstGraphicsAction(QnstGraphicsEntity* parent = 0);

    ~QnstGraphicsAction();

    QnstAction getAction();

    void setAction(QnstAction action);

    QnstActionIcon getActionIcon();

    void setActionIcon(QnstActionIcon actionIcon);

    void adjust();

    void aux_adjust(QPointF pointa, QPointF pointb);

    void setConn(QnstConnector* conn);

    void setNameUids(QMap<QString, QString> nameUids);

    void setParams(QMap<QString, QString> params);

    void addParam(QString uid, QString name, QString value);

    void setParam(QString name, QString value);

    void removeParam(QString name);

    void removeUId(QString uid);

signals:
    void bindParamAdded(QString uid, QString parent, QMap<QString, QString> properties);

    void bindParamUpdated(QString bindUid, QMap<QString, QString> params,  QMap<QString, QString> name_uids);

protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:

    QnstAction action;

    QnstActionIcon actionIcon;

    QnstConnector* conn;

    QMap<QString, QString> name_uid;

    QMap<QString, QString> params;

    QnstGraphicsBindDialog* dialog;
};

#endif // QNSTGRAPHICSACTION_H

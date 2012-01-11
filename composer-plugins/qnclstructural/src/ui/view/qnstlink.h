#ifndef QNSTLINK_H
#define QNSTLINK_H

#include "qnstentity.h"
#include "qnstbind.h"

class QnstLink : public QnstEntity
{
public:
    QnstLink();

    virtual ~QnstLink();

    QString getAggregatorUID() const;

    void setAggregatorUID(QString aggregatorUID) const;

    QString getxConnector() const;

    void setxConnector(QString xconnector) const;

    QString getxConnectorUID() const;

    void setxConnectorUID(QString xconnectorUID) const;

    QVector<QnstBind*> getActions() const;

    void addAction(QnstBind* action);

    void removeAction(QnstBind* action);

    QVector<QnstBind*> getConditions() const;

    void addCondition(QnstBind* condition);

    void removeCondition(QnstBind* condition);

private:
    QString aggregatorUID;

    QString xconnectorUID;

    QVector<QnstBind*> actions;

    QVector<QnstBind*> conditions;
};

#endif // QNSTLINK_H

#ifndef QNSTCONNCETOR_H
#define QNSTCONNCETOR_H

#include  "qnstentity.h"

class QnstConncetor : public QnstEntity
{
public:
    QnstConncetor();

    ~QnstConncetor();

    QString getName();

    void setName(QString name);

    QVector<QString> getConditions();

    void addCondition(QString condition);

    void removeCondition(QString condition);

    QVector<QString> getActions();

    void addAction(QString action);

    void removeAction(QString action);

private:
    QString name;

    QVector<QString> conditions;

    QVector<QString> actions;
};

#endif // QNSTCONNCETOR_H

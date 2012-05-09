#ifndef QNSTCONNCETOR_H
#define QNSTCONNCETOR_H

#include <QMap>

#include  "qnstentity.h"

class QnstConncetor : public QnstEntity
{
public:
    QnstConncetor();

    ~QnstConncetor();

    QString getName();

    void setName(QString name);

    QMap<QString, QString> getConditions();

    void addCondition(QString uid, QString condition);

    void removeCondition(QString uid);

    QMap<QString, QString> getActions();

    void addAction(QString uid, QString action);

    void removeAction(QString uid);

    QMap<QString, QString> getParams();

    void addParam(QString uid, QString name);

    void removeParam(QString uid);

private:
    QString name;

    QMap<QString, QString> params;

    QMap<QString, QString> conditions;

    QMap<QString, QString> actions;
};

#endif // QNSTCONNCETOR_H

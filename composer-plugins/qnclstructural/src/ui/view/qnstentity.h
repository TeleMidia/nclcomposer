#ifndef QNSTENTITY_H
#define QNSTENTITY_H

#include "core/qnst.h"

#include "QUuid"

class QnstEntity
{
public:
    QnstEntity();

    ~QnstEntity();

    int getnstType() const;

    void setnstType(int nsttype);

    QnstEntity* getnstParent() const;

    void setnstParent(QnstEntity* nsttype);

    QString getUid() const;

    void setUid(const QString &uid);

    void setName(QString name);

    QString getName() const;

private:
    int nsttype;

    QString uid;

    QString name;

    QnstEntity* nstparent;
};

#endif // QNSTENTITY_H

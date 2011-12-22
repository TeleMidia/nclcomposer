#ifndef QNSTENTITY_H
#define QNSTENTITY_H

#include <QUuid>
#include <QString>
#include <QVector>

#include "qnst.h"

typedef Qnst::EntityType QnstType;

class QnstEntity
{
public:
    QnstEntity(QnstEntity* parent = 0);

    ~QnstEntity();

    QString getnstId() const;

    void setnstId(QString id);

    QString getnstUid() const;

    void setnstUid(QString uid);

    QnstType getnstType() const;

    void setnstType(QnstType type);

    QnstEntity* getnstParent() const;

    void setnstParent(QnstEntity* parent);

    QVector<QnstEntity*> getnstEntities();

    void addnstEntity(QnstEntity* entity);

    void removenstEntity(QnstEntity* entity);

private:
    QString id;

    QString uid;

    QnstType type;

    QnstEntity* parent;

    QVector<QnstEntity*> entities;
};

#endif // QNSTENTITY_H

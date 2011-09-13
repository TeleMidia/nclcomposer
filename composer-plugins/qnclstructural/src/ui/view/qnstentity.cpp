#include "qnstentity.h"

QnstEntity::QnstEntity()
{
    setUid(QUuid::createUuid().toString());
    setName("");
}

QnstEntity::~QnstEntity()
{
}

QString QnstEntity::getUid() const
{
    return uid;
}

void QnstEntity::setUid(const QString &uid)
{
    this->uid = uid;
}

int QnstEntity::getnstType() const
{
    return nsttype;
}

void QnstEntity::setnstType(int nsttype)
{
    this->nsttype = nsttype;
}

QnstEntity* QnstEntity::getnstParent() const
{
    return nstparent;
}

void QnstEntity::setnstParent(QnstEntity* nstparent)
{
    this->nstparent = nstparent;
}

void QnstEntity::setName(QString name)
{
    this->name = name;
}

QString QnstEntity::getName() const
{
    return name;
}

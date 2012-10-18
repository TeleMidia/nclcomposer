#ifndef QNSTENTITY_H
#define QNSTENTITY_H

#include <QUuid>
#include <QString>
#include <QVector>
#include <QMap>

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

    virtual void setnstType(QnstType type);

    QMap <QString, QString> getUsrData();

    void setUsrData(QMap <QString, QString> userData);

    QnstEntity* getnstParent() const;

    void setnstParent(QnstEntity* parent);

    QVector<QnstEntity*> getnstEntities();

    void addnstEntity(QnstEntity* entity);

    void removenstEntity(QnstEntity* entity);

    bool isMedia();

private:
    QString id;

    QString uid;

    QnstType type;

    QnstEntity* parent;

    QVector<QnstEntity*> entities;

    // FIXME: Maybe, this is not needed!! We could use the
    QMap <QString, QString> userData;
};

#endif // QNSTENTITY_H

/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef QNSTENTITY_H
#define QNSTENTITY_H

#include <QUuid>
#include <QVector>

#include "qnst.h"

typedef Qnst::EntityType QnstType;

class QnstEntity
{
public:
    QnstEntity(QnstEntity* parent = 0);

    ~QnstEntity();

    QString getnstUid() const;

    void setnstUid(QString uid);

    QString getnstId() const;

    void setnstId(QString id);

    QnstType getnstType() const;

    void setnstType(QnstType type);

    QnstEntity* getnstParent() const;

    void setnstParent(QnstEntity* parent);

private:
    QString uid;

    QString id;

    QnstType type;

    QnstEntity* parent;
};

#endif // QNSTENTITY_H

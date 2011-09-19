/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
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

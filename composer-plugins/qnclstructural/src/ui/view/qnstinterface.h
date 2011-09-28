/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef QNSTINTERFACE_H
#define QNSTINTERFACE_H

#include "qncginterface.h"

#include "qnstnode.h"
#include "qnstentity.h"

class QnstNode;

class QnstInterface : public QncgInterface, public QnstEntity
{
    Q_OBJECT

public:
    QnstInterface(QnstNode* parent = 0);

    ~QnstInterface();

    void setName(QString name);

    QMap<QString, QVector<qreal> > getAngles() const;

    void addAngle(QString uid, qreal angle);

    void removeAngle(QString uid);

signals:
    void entitySelected(QnstEntity* e);

    void entityAdded(QnstEntity* e);
protected:
    virtual void draw(QPainter* painter);

    virtual void delineate(QPainterPath* painter) const;

private:
    void createConnections();

    QMap<QString, QVector<qreal> > angles;

private slots:
    void internalselection();
};

#endif // QNSTINTERFACE_H

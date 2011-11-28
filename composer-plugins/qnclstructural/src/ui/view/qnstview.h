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
#ifndef QNSTVIEW_H
#define QNSTVIEW_H

#include <QGraphicsView>
#include <QVector>
#include <QMap>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

#include <QDebug>

#include "qnstscene.h"
#include "qnstentity.h"
#include "qnstgraphicsbody.h"
#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsmedia.h"

typedef int QnstInt;

class QnstView : public QGraphicsView
{
    Q_OBJECT

public:
    QnstView(QWidget* parent = 0);

    ~QnstView();

    QVector<QnstEntity*> getRoots() const;

    void addRoot(QnstEntity* root);

    void removeRoot(QnstEntity* root);

public slots:
    void addEntity(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void removeEntity(const QString uid);

    void changeEntity(const QString uid, const QMap<QString, QString> properties);

    void selectEntity(const QString uid);

    void requestEntityAddition(QnstEntity* entity);

    void requestEntityRemotion(QnstEntity* entity);

    void requestEntityChange(QnstEntity* entity);

    void requestEntitySelection(QnstEntity* entity);

signals:
    void entityAdded(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void entityChanged(const QString uid, const QMap<QString, QString> properties);

    void entityRemoved(const QString uid);

    void entitySelected(const QString uid);

protected slots:
    virtual void mousePressEvent(QMouseEvent* event);

private:
    void addBody(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void changeBody(const QString uid, const QMap<QString, QString> properties);

    void addMedia(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void changeMedia(const QString uid, const QMap<QString, QString> properties);

    void requestBodyAddition(QnstGraphicsBody* entity);

    void requestBodyChange(QnstGraphicsBody* entity);

    void requestContextAddition(QnstGraphicsContext* entity);

    void requestContextChange(QnstGraphicsContext* entity);

    void requestSwitchAddition(QnstGraphicsSwitch* entity);

    void requestSwitchChange(QnstGraphicsSwitch* entity);

    void requestMediaAddition(QnstGraphicsMedia* entity);

    void requestMediaChange(QnstGraphicsMedia* entity);

    void createObjects();

    void createConnection();

    QnstInt n;

    QnstEntity* selected;

    QnstScene* scene;

    QVector<QnstEntity*> roots;

    QMap<QString, QnstEntity*> entities;
};

#endif // QNSTVIEW_H

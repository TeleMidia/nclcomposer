#ifndef QNSTVIEW_H
#define QNSTVIEW_H

#include <QGraphicsView>
#include <QVector>
#include <QMap>
#include <QDomDocument>
#include <QDomElement>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QMessageBox>

#include <QDebug>

#include "qnstscene.h"
#include "qnstviewlink.h"
#include "qnstgraphicsentity.h"
#include "qnstgraphicsbody.h"
#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsmedia.h"
#include "qnstgraphicsport.h"
#include "qnstgraphicsreference.h"
#include "qnstgraphicslink.h"
#include "qnstgraphicslinkdialog.h"

class QnstView : public QGraphicsView
{
    Q_OBJECT

public:
    QnstView(QWidget* parent = 0);

    ~QnstView();

    void load(QString data);

    QString serialize();

    void read(QDomElement element, QDomElement parent);

    void write(QDomElement element, QDomDocument* dom, QnstGraphicsEntity* entity);

public:
    void addEntity(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void removeEntity(const QString uid);

    void changeEntity(const QString uid, const QMap<QString, QString> properties);

    void selectEntity(const QString uid);

public slots:
    void performHelp();

    void performUndo();

    void performRedo();

    void performCut();

    void performCopy();

    void performPaste();

    void performDelete();

    void performExport();

    void performZoomIn();

    void performZoomOut();

    void performZoomReset();

    void performFullscreen();

    void performBringfront();

    void performBringforward();

    void performSendback();

    void performSendbackward();

    void performHide();

    void performProperties();

signals:
    void entityAdded(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void entityChanged(const QString uid, const QMap<QString, QString> properties);

    void entityRemoved(const QString uid);

    void entitySelected(const QString uid);

protected:
    virtual void mouseMoveEvent(QMouseEvent* event);

    virtual void mousePressEvent(QMouseEvent* event);

    virtual void mouseReleaseEvent(QMouseEvent*event);

    virtual void keyPressEvent(QKeyEvent *event);

    virtual void keyReleaseEvent(QKeyEvent *event);

protected slots:
    void requestEntityAddition(QnstGraphicsEntity* entity);

    void requestEntityRemotion(QnstGraphicsEntity* entity);

    void requestEntityChange(QnstGraphicsEntity* entity);

    void requestEntitySelection(QnstGraphicsEntity* entity);

private:
    void addBody(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void changeBody(QnstGraphicsBody* entity, const QMap<QString, QString> properties);

    void addContext(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void changeContext(QnstGraphicsContext* entity, const QMap<QString, QString> properties);

    void addSwitch(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void changeSwitch(QnstGraphicsSwitch* entity, const QMap<QString, QString> properties);

    void addMedia(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void changeMedia(QnstGraphicsMedia* entity, const QMap<QString, QString> properties);

    void addPort(const QString uid, const QString parent, const QMap<QString, QString> properties);

    void changePort(QnstGraphicsPort* entity, const QMap<QString, QString> properties);

    void requestBodyAddition(QnstGraphicsBody* entity);

    void requestBodyChange(QnstGraphicsBody* entity);

    void requestContextAddition(QnstGraphicsContext* entity);

    void requestContextChange(QnstGraphicsContext* entity);

    void requestSwitchAddition(QnstGraphicsSwitch* entity);

    void requestSwitchChange(QnstGraphicsSwitch* entity);

    void requestMediaAddition(QnstGraphicsMedia* entity);

    void requestMediaChange(QnstGraphicsMedia* entity);

    void requestPortAddition(QnstGraphicsPort* entity);

    void requestPortChange(QnstGraphicsPort* entity);

    void performCopy(QnstGraphicsEntity* entity, QnstGraphicsEntity* parent);

    void performPaste(QnstGraphicsEntity* copy, QnstGraphicsEntity* parent);

    void createObjects();

    void createConnection();

    int ncontext;

    int nswitch;

    int nport;

    int nmedia;

    bool modified;

    bool linking;

    QnstScene* scene;

    QnstViewLink* link;

    QnstGraphicsLinkDialog* linkDialog;

    QnstGraphicsEntity* selected;

    QnstGraphicsEntity* clipboard;

    QMap<QString, QnstGraphicsEntity*> entities;
};

#endif // QNSTVIEW_H

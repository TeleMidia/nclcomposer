#ifndef QNLYGRAPHICSVIEW_H
#define QNLYGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QResizeEvent>

#include "qnlygraphicsscene.h"
#include "qnlygraphicsitem.h"

class QnlyGraphicsScene;
class QnlyGraphicsItem;

class QnlyGraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    QnlyGraphicsView(QWidget* parent = 0);

    virtual ~QnlyGraphicsView();

    QString getId() const;

    void setId(const QString &id);

    QString getUid() const;

    void setUid(const QString &uid);

    QString getRegion() const;

    void setRegion(const QString &region);

    QString getDevice() const;

    void setDevice(const QString &device);

public slots:
    virtual void addItem(QnlyGraphicsItem* item, QnlyGraphicsItem* parent);

    virtual void removeItem(QnlyGraphicsItem* item);

    virtual void selectItem(QnlyGraphicsItem* item);

    virtual void changeItem(QnlyGraphicsItem* item, const QMap<QString, QString> &attributes);

signals:
    void fullscreenPerformed(bool status);

    void itemAdded(QnlyGraphicsItem* item, QnlyGraphicsItem* parent, QnlyGraphicsView* view);

    void itemRemoved(QnlyGraphicsItem* item, QnlyGraphicsView* view);

    void itemSelected(QnlyGraphicsItem* item, QnlyGraphicsView* view);

    void itemChanged(QnlyGraphicsItem* item, QnlyGraphicsView* view, const QMap<QString, QString> &attributes);

protected:
    virtual void resizeEvent(QResizeEvent* event);

private:
    void createConnections();

    void createScene();

    QString id;

    QString uid;

    QString region;

    QString device;

    QnlyGraphicsScene* scene;
};

#endif // QNLYGRAPHICSVIEW_H

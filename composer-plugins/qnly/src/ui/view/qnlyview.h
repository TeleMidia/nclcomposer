#ifndef QNLYVIEW_H
#define QNLYVIEW_H

#include <QStackedWidget>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QContextMenuEvent>
#include <QUuid>
#include <QColorDialog>

#include "qnlygraphicsitem.h"
#include "qnlygraphicsview.h"
#include "qnlygraphicsscene.h"

class QnlyView : public QStackedWidget
{
    Q_OBJECT

public:
    QnlyView(QWidget* parent = 0);

    virtual ~QnlyView();

    int getnItem() const;

    void setnItem(const int &value);

    int getnView() const;

    void setnView(const int &value);

    QString getUnit() const;

    void setUnit(const QString &unit);

    QString getResolution() const;

    void setResolution(const QString &resolution);

public slots:
    virtual void addItem(const QString itemUID, const QString parentitemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    virtual void removeItem(const QString itemUID, const QString viewUID);

    virtual void selectItem(const QString itemUID, const QString viewUID);

    virtual void changeItem(const QString itemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    virtual void addRegion(const QString itemUID, const QString parentitemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    virtual void removeRegion(const QString itemUID, const QString viewUID);

    virtual void selectRegion(const QString itemUID, const QString viewUID);

    virtual void changeRegion(const QString itemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    virtual void addView(const QString viewUID, const QMap<QString, QString> &attributes);

    virtual void removeView(const QString viewUID);

    virtual void selectView(const QString viewUID);

    virtual void changeView(const QString viewUID, const QMap<QString, QString> &attributes);

    virtual void addRegionbase(const QString viewUID, const QMap<QString, QString> &attributes);

    virtual void removeRegionbase(const QString viewUID);

    virtual void selectRegionbase(const QString viewUID);

    virtual void changeRegionbase(const QString viewUID, const QMap<QString, QString> &attributes);

signals:
    void itemAdded(const QString itemUID, const QString parentitemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    void itemRemoved(const QString itemUID, const QString viewUID);

    void itemSelected(const QString itemUID, const QString viewUID);

    void itemChanged(const QString itemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    void viewAdded(const QString viewUID, const QMap<QString, QString> &attributes);

    void viewRemoved(const QString viewUID);

    void viewSelected(const QString viewUID);

    void viewChanged(const QString viewUID, const QMap<QString, QString> &attributes);

    void itemAddRequested(const QString itemUID, const QString parentitemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    void itemRemoveRequested(const QString itemUID, const QString viewUID);

    void itemSelectRequested(const QString itemUID, const QString viewUID);

    void itemChangeRequested(const QString itemUID, const QString viewUID, const QMap<QString, QString> &attributes);

    void viewAddRequested(const QString viewUID, const QMap<QString, QString> &attributes);

    void viewRemoveRequested(const QString viewUID);

    void viewSelectRequested(const QString viewUID);

    void viewChangeRequested(const QString viewUID, const QMap<QString, QString> &attributes);

protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    virtual void keyPressEvent(QKeyEvent *event);

protected slots:
    virtual void addItem(QnlyGraphicsItem* item, QnlyGraphicsItem* parentitem, QnlyGraphicsView* view);

    virtual void removeItem(QnlyGraphicsItem* item, QnlyGraphicsView* view);

    virtual void selectItem(QnlyGraphicsItem* item, QnlyGraphicsView* view);

    virtual void changeItem(QnlyGraphicsItem* item, QnlyGraphicsView* view, const QMap<QString, QString> &attributes);

    virtual void addView(QnlyGraphicsView* view);

    virtual void removeView(QnlyGraphicsView* view);

    virtual void selectView(QnlyGraphicsView* view);

    virtual void changeView(QnlyGraphicsView* view, const QMap<QString, QString> &attributes);

private:
    void createActions();

    void createMenus();

    void createConnections();

    int nitem;

    int nview;

    QMenu* viewMenu;

    QMenu* insertMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

    QMenu* switchMenu;

    QMenu* contextMenu;

    QAction* helpAction;

    QAction* undoAction;

    QAction* redoAction;

    QAction* cutAction;

    QAction* copyAction;

    QAction* pasteAction;

    QAction* deleteAction;

    QAction* zoominAction;

    QAction* zoomoutAction;

    QAction* zoomresetAction;

    QAction* fullscreenAction;

    QAction* exportAction;

    QAction* itemAction;

    QAction* viewAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* hideAction;

    QAction* propertiesAction;

    QString unit;

    QString resolution;

    QMap<QString, QnlyGraphicsItem*> items;

    QMap<QString, QnlyGraphicsView*> views;

};

#endif // QNLYVIEW_H

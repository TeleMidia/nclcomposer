#ifndef QNSTGRAPHICSSWITCH_H
#define QNSTGRAPHICSSWITCH_H

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsimage.h"
#include "qnstgraphicsaudio.h"
#include "qnstgraphicsvideo.h"
#include "qnstgraphicstext.h"
#include "qnstgraphicsscript.h"
#include "qnstgraphicssettings.h"

class QnstGraphicsSwitch : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsSwitch(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsSwitch();

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

protected slots:
    void performContext();

    void performSwitch();

    void performImage();

    void performAudio();

    void performVideo();

    void performText();

    void performScript();

    void performSettings();

private:
    void createActions();

    void createMenus();

    void createConnections();

    QMenu* viewMenu;

    QMenu* insertMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

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

    QAction* hideAction;

    QAction* fullscreenAction;

    QAction* exportAction;

    QAction* bodyAction;

    QAction* contextAction;

    QAction* switchAction;

    QAction* imageAction;

    QAction* videoAction;

    QAction* audioAction;

    QAction* textAction;

    QAction* scriptAction;

    QAction* settingsAction;

    QAction* portAction;

    QAction* propertyAction;

    QAction* areaAction;

    QAction* aggregatorAction;

    QAction* bringfrontAction;

    QAction* bringforwardAction;

    QAction* sendbackwardAction;

    QAction* sendbackAction;

    QAction* propertiesAction;
};


#endif // QNSTGRAPHICSSWITCH_H

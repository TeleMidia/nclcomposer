#ifndef QNSTGRAPHICSBODY_H
#define QNSTGRAPHICSBODY_H

#include <QKeyEvent>

#include "qnstgraphicscomposition.h"

#include "qnstgraphicscontext.h"
#include "qnstgraphicsswitch.h"
#include "qnstgraphicsimage.h"
#include "qnstgraphicsaudio.h"
#include "qnstgraphicsvideo.h"
#include "qnstgraphicstext.h"
#include "qnstgraphicsscript.h"
#include "qnstgraphicssettings.h"
#include "qnstgraphicsport.h"

class QnstGraphicsBody : public QnstGraphicsComposition
{
    Q_OBJECT

public:
    QnstGraphicsBody(QnstGraphicsNode* parent = 0);

    ~QnstGraphicsBody();

protected:
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent* event);

    virtual void	keyPressEvent(QKeyEvent* event);

protected slots:
    void performContext();

    void performSwitch();

    void performImage();

    void performAudio();

    void performVideo();

    void performText();

    void performScript();

    void performSettings();

    void performPort();

    void performCompact();

    void performClock();

    void performDelete();

private:
    void createActions();

    void createMenus();

    void createConnections();

    QMenu* viewMenu;

    QMenu* insertMenu;

    QMenu* showMenu;

    QMenu* arrangeMenu;

    QMenu* adjustMenu;

    QMenu* organizeMenu;

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

    QAction* compactAction;

    QAction* clockAction;

    QAction* propertiesAction;
};

#endif // QNSTGRAPHICSBODY_H

#ifndef QNSTMENU_H
#define QNSTMENU_H

#include <QMenu>

class QnstMenu : public QMenu
{
    Q_OBJECT

public:
    QnstMenu(QWidget* parent = 0);

    ~QnstMenu();

signals:
    void helpRequested();

    void undoRequested();

    void redoRequested();

    void cutRequested();

    void copyRequested();

    void pasteRequested();

    void deleteRequested();

    void exportRequested();

    void zoominRequested();

    void zoomoutRequested();

    void zoomresetRequested();

    void fullscreenRequested();

    void imageRequested();

    void audioRequested();

    void textRequested();

    void videoRequested();

    void scriptRequested();

    void settingsRequested();

    void htmlRequested();

    void nclRequested();

    void mediaRequested();

    void contextRequested();

    void switchRequested();

    void bodyRequested();

    void portRequested();

    void switchportRequested();

    void areaRequested();

    void propertyRequested();

    void aggregatorRequested();

    void bringfrontRequested();

    void bringforwardRequested();

    void sendbackwardRequested();

    void sendbackRequested();

    void hideRequested();

    void propertiesRequested();

    void autoRequested();

private:
    void createActions();

    void createMenus();

    void createConnections();

public:
    QMenu* menuView;

    QMenu* menuInsert;

    QMenu* menuShow;

    QMenu* menuArrange;

    QMenu* menuAdjust;

    QAction* actionHelp;

    QAction* actionUndo;

    QAction* actionRedo;

    QAction* actionCut;

    QAction* actionCopy;

    QAction* actionPaste;

    QAction* actionDelete;

    QAction* actionExport;

    QAction* actionZoomIn;

    QAction* actionZoomOut;

    QAction* actionZoomReset;

    QAction* actionFullscreen;

    QAction* actionImage;

    QAction* actionVideo;

    QAction* actionAudio;

    QAction* actionText;

    QAction* actionScript;

    QAction* actionSettings;

    QAction* actionHTML;

    QAction* actionNCL;

    QAction* actionMedia;

    QAction* actionContext;

    QAction* actionSwitch;

    QAction* actionBody;

    QAction* actionPort;

    QAction* actionArea;

    QAction* actionProperty;

    QAction* actionSwitchPort;

    QAction* actionAggregator;

    QAction* actionBringfront;

    QAction* actionBringforward;

    QAction* actionSendbackward;

    QAction* actionSendback;

    QAction* actionHide;

    QAction* actionAuto;

    QAction* actionProperties;
};

#endif // QNSTMENU_H

#ifndef LAYOUTEDITOR_H
#define LAYOUTEDITOR_H

#include <QMainWindow>
#include <QAction>
#include <QToolBar>
#include <QStatusBar>

#include "layoutcontainer.h"
#include "layoutoutline.h"
#include "layoutview.h"

#include <QDockWidget>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

class LayoutEditor : public QMainWindow
{
    Q_OBJECT

public:
    LayoutEditor(QWidget* parent = 0);
    virtual ~LayoutEditor();

public slots:
    void expand();
    void backexpand();
    void exporttoimg();

    void addView(LayoutView* layout);
    void removeView(LayoutView* layout);

protected:
    void mouseMoveEvent ( QMouseEvent * event );

signals:
    void itemAdded(LayoutItem* item);

private:
    void createActions();
    void createWidgets();
    void createConnections();
    void createToolbar();
    void createStatubar();

    QToolBar* fileToolbar;
    QToolBar* editToolbar;
    QToolBar* viewToolbar;
    QToolBar* windowToolbar;

    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;
    QAction* importAction;
    QAction* exportAction;
    QAction* undoAction;
    QAction* redoAction;
    QAction* cutAction;
    QAction* copyAction;
    QAction* pasteAction;
    QAction* plusAction;
    QAction* minusAction;
    QAction* normalAction;
    QAction* expandAction;
    QAction* reduceAction;
    QAction* preferencesAction;

    QDockWidget* docktenso;
    LayoutOutline* layoutOutline;
    LayoutContainer* layoutContainer;
};

}
}
}
}
}

#endif // LAYOUTEDITOR_H

#include "layouteditor.h"

#include "layoutview.h"

#include <QDebug>
#include <QFileDialog>

#include <QKeyEvent>
#include <QMouseEvent>

namespace br{
namespace pucrio{
namespace telemidia{
namespace ncllayout{
namespace ui{

LayoutEditor::LayoutEditor(QWidget* parent)
    : QMainWindow(parent)
{
    setMouseTracking(true);

    createActions();
    createWidgets();
    createToolbar();
    createStatubar();
    createConnections();
}

LayoutEditor::~LayoutEditor()
{
}



void LayoutEditor::createActions()
{
    // New Action
    newAction = new QAction(this);
    newAction->setText(tr("New"));
    newAction->setIcon(QIcon(":/images/new"));

    // Open Action
    openAction =  new QAction(this);
    openAction->setText(tr("Open"));
    openAction->setIcon(QIcon(":/images/open"));

    // Save Action
    saveAction =  new QAction(this);
    saveAction->setText(tr("Save"));
    saveAction->setIcon(QIcon(":/images/save"));

    // Import Action
    importAction = new QAction(this);
    importAction->setText(tr("Import"));
    importAction->setIcon(QIcon(":/images/import"));

    // Export Action
    exportAction = new QAction(this);
    exportAction->setText(tr("Export"));
    exportAction->setIcon(QIcon(":/images/export"));

    // Undo Action
    undoAction = new QAction(this);
    undoAction->setText(tr("Undo"));
    undoAction->setIcon(QIcon(":/images/undo"));

    // Redo Action
    redoAction = new QAction(this);
    redoAction->setText(tr("Redo"));
    redoAction->setIcon(QIcon(":/images/redo"));

    // Cut Action
    cutAction = new QAction(this);
    cutAction->setText(tr("Cut"));
    cutAction->setIcon(QIcon(":/images/cut"));

    // Copy Action
    copyAction = new QAction(this);
    copyAction->setText(tr("Copy"));
    copyAction->setIcon(QIcon(":/images/copy"));

    // Paste Action
    pasteAction = new QAction(this);
    pasteAction->setText(tr("Paste"));
    pasteAction->setIcon(QIcon(":/images/paste"));

    // Plus Action
    plusAction = new QAction(this);
    plusAction->setText(tr("Plus"));
    plusAction->setIcon(QIcon(":/images/plus"));

    // Minus Action
    minusAction = new QAction(this);
    minusAction->setText(tr("Minus"));
    minusAction->setIcon(QIcon(":/images/minus"));

    // Normal Action
    normalAction = new QAction(this);
    normalAction->setText(tr("Normal"));
    normalAction->setIcon(QIcon(":/images/normal"));

    // Expand Action
    expandAction = new QAction(this);
    expandAction->setText(tr("Expand"));
    expandAction->setIcon(QIcon(":/images/expand"));

    // Reduce Action
    reduceAction = new QAction(this);
    reduceAction->setText(tr("Reduce"));
    reduceAction->setIcon(QIcon(":/images/reduce"));

    // Preference Action
    preferencesAction = new QAction(this);
    preferencesAction->setText(tr("Preference"));
    preferencesAction->setIcon(QIcon(":/images/preferences"));
}

void LayoutEditor::createWidgets()
{
    layoutContainer = new LayoutContainer(this);
    docktenso = new QDockWidget("Dock Tenso",this);

//    addDockWidget(Qt ::LeftDockWidgetArea,docktenso);

    setCentralWidget(layoutContainer);
}

void LayoutEditor::createConnections()
{
    connect(expandAction, SIGNAL(triggered()), SLOT(expand()));
    connect(reduceAction, SIGNAL(triggered()), SLOT(backexpand()));
//    connect(exportAction, SIGNAL(triggered()), SLOT(exporttoimg()));
}

void LayoutEditor::createToolbar()
{
    // File Tool Bar
    fileToolbar = addToolBar(tr("File"));
    fileToolbar->setMovable(false);
    fileToolbar->setFloatable(false);
    fileToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    fileToolbar->addAction(newAction);
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(saveAction);

    fileToolbar->addSeparator();

    fileToolbar->addAction(importAction);
    fileToolbar->addAction(exportAction);

    fileToolbar->addSeparator();

    // Edit Tool Bar
    editToolbar = addToolBar(tr("Edit"));
    editToolbar->setMovable(false);
    editToolbar->setFloatable(false);
    editToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    editToolbar->addAction(undoAction);
    editToolbar->addAction(redoAction);

    editToolbar->addSeparator();

    editToolbar->addAction(cutAction);
    editToolbar->addAction(copyAction);
    editToolbar->addAction(pasteAction);

    editToolbar->addSeparator();

    // View Tool Bar
    viewToolbar = addToolBar(tr("View"));
    viewToolbar->setMovable(false);
    viewToolbar->setFloatable(false);
    viewToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    viewToolbar->addAction(plusAction);
    viewToolbar->addAction(minusAction);
    viewToolbar->addAction(normalAction);

    viewToolbar->addSeparator();

    viewToolbar->addAction(expandAction);
    viewToolbar->addAction(reduceAction);

    viewToolbar->addSeparator();

    // Window Tool Bar
    windowToolbar = addToolBar(tr("Window"));
    windowToolbar->setMovable(false);
    windowToolbar->setFloatable(false);
    windowToolbar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    windowToolbar->addAction(preferencesAction);
}

void LayoutEditor::createStatubar()
{

}

void LayoutEditor::exporttoimg()
{
    LayoutView* layoutView = (LayoutView*) layoutContainer->currentWidget();

    QImage img(layoutView->scene()->width(),
               layoutView->scene()->height(),
               QImage::Format_ARGB32_Premultiplied);
    QPainter p(&img);
    layoutView->scene()->render(&p);
//    scene.render(&p);
    p.end();
    img.save(QFileDialog::getSaveFileName(this));
}


void LayoutEditor::mouseMoveEvent ( QMouseEvent * event ){
    QMainWindow::mouseMoveEvent(event);

    qDebug() << "over";

    if (isFullScreen()){
        if (QRect(0,0,30,rect().height()).contains(event->pos())){
            docktenso->show();
        }else{
            docktenso->close();
        }
    }
}


void LayoutEditor::addView(LayoutView* layout)
{
    layoutContainer->addWidget(layout);
}

void LayoutEditor::removeView(LayoutView* layout)
{
    layoutContainer->removeWidget(layout);
}

void LayoutEditor::expand()
{
    qDebug() << "expand()";

    expandAction->setEnabled(false);

    docktenso->close();

    this->setWindowFlags(Qt::Window);
    this->setWindowState(Qt::WindowFullScreen);
    this->show();
}

void LayoutEditor::backexpand()
{

    expandAction->setEnabled(true);

    docktenso->show();;

    this->setWindowState(Qt::WindowNoState);
    this->setWindowFlags(Qt::Widget);
    this->show();
}

}
}
}
}
}

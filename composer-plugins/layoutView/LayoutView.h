#ifndef LAYOUTVIEW_H
#define LAYOUTVIEW_H

#include <QtGui/QMainWindow>
#include "scribblearea.h"
#include "attributes.h"
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QTreeWidget>
#include <QScrollArea>
#include <QDockWidget>
#include <map>

using namespace std;

namespace Ui
{
    class LayoutView;
}

class LayoutView : public QMainWindow
{
    Q_OBJECT

public:
    LayoutView(QWidget *parent = 0);
    ~LayoutView();

public slots:
    void fillFields (NCLRegion *region);
    void addNodeTreeView (NCLRegion *region);
    void selectRegion ();
    void performChanges (NCLRegion *region);
    void selectRegion (NCLRegion *region);
    void removeRegion ();

    void changeLeftRegion (QString,QString);
    void changeRightRegion (QString,QString);
    void changeIdRegion (QString,QString);
    void changeTopRegion (QString,QString);
    void changeBottomRegion (QString,QString);
    void changeWidthRegion (QString,QString);
    void changeHeightRegion (QString,QString);
    void changeZIndexRegion (QString,QString);
    void changeTitleRegion (QString,QString);


private:
    void createContextMenu ();
    void createActions ();


    QTreeWidget *tree;
    Ui::LayoutView *ui;
    ScribbleArea *area;
    QPushButton *button;
    QAction *createRegion;
    QAction *clearAction;
    QAction *deleteRegion;
    QScrollArea *scroll;
    QDockWidget *treeView;
    QDockWidget *principal;
    QRect screenResolution;

    NCLRegion * currentNCLRegion;

    Attributes *att;

    map <NCLRegion *, QTreeWidgetItem *> itens;
    map <QTreeWidgetItem *, NCLRegion *> inverse;

    int width;
    int height;

};

#endif // MAINWINDOW_H

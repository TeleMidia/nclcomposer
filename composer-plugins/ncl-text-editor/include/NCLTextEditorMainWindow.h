/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>

#include "NCLProblemsView.h"

#ifdef NCLEDITOR_STANDALONE
#include "NCLTreeWidget.h"
#endif

#include "NCLTextEditor.h"
//#include "LayoutView.h"
//#include "Preferences.h"

class QAction;
class QMenu;

class NCLTextEditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
        explicit NCLTextEditorMainWindow(QWidget *parent = 0);
        NCLTextEditor *getTextEditor() {return this->textEdit;}
#ifdef NCLEDITOR_STANDALONE
        NCLTreeWidget *getNCLTreeWidget() {return this->outlineView; }
#endif

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    void documentWasModified();
    void showInFullScreen();
    void insertElement();
    void gotoLineOf(QTreeWidgetItem *item, int column);
    void showPreferences();

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createTextView();
    void createOutlineView();
    void createProblemsView();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QDockWidget *dockTextEdit;
    NCLTextEditor *textEdit;
    QString curFile;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;
    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *fullscreenAct;
    QAction *editPreferencesAct;
    QAction *synchronizeAct;

/** VIEWS **/
    /** Outline View */
    QDockWidget *dockOutlineView;
    
#ifdef NCLEDITOR_STANDALONE
    NCLTreeWidget *outlineView;
#endif
    QMenu *nodeMenu;
    QAction *insertNodeChildAct;

    /** Problems View */
    NCLProblemsView *problemsView;
    /*QDockWidget *dockLayoutView;
    LayoutView *layoutView;*/

    /* Preferences *preferences;
    PreferencesPage *textEditorPreferencesPage; */

signals:
    void elementAdded(QString, QString, QMap <QString, QString>&, bool);
};

#endif

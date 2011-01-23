/****************************************************************************
**
** Copyright (C) 2004-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the example classes of the Qt Toolkit.
**
** Licensees holding a valid Qt License Agreement may use this file in
** accordance with the rights, responsibilities and obligations
** contained therein.  Please consult your licensing agreement or
** contact sales@trolltech.com if any conditions of this licensing
** agreement are not clear to you.
**
** Further information about Qt licensing is available at:
** http://www.trolltech.com/products/qt/licensing.html or by
** contacting info@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>

#include "NCLProblemsView.h"
#include "NCLTreeWidget.h"
#include "NCLTextEditor.h"
#include "LayoutView.h"
#include "Preferences.h"

class QAction;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);

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
    void createLayoutView();
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

/** VIEWS **/
    /** Outline View */
    QDockWidget *dockOutlineView;
    NCLTreeWidget *outlineView;
    QMenu *nodeMenu;
    QAction *insertNodeChild;

    /** Problems View */
    NCLProblemsView *problemsView;
    QDockWidget *dockLayoutView;
    LayoutView *layoutView;

    Preferences *preferences;
    PreferencesPage *textEditorPreferencesPage;
};

#endif

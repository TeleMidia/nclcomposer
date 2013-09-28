/*
 * Copyright 2011 TeleMidia/PUC-Rio.
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either 
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public 
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>. 
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include <QApplication>

#include <QMainWindow>
#include <QDockWidget>
#include <QListWidget>
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QStatusBar>
#include <QFileDialog>
#include <QMenuBar>
#include <QGridLayout>

#include "NCLProblemsView.h"

#ifdef NCLEDITOR_STANDALONE
#include "NCLTreeWidget.h"
#endif

#include "NCLTextEditor.h"
//#include "LayoutView.h"
//#include "Preferences.h"

#include "SearchLineEdit.h"

class QAction;
class QMenu;

/*!
 * \brief The main window of the NCL Textual Editor.
 */
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

    void showSearchBox();
    void hideSearchBox();
    void findNext();
    void findNext(QString text);
    void findPrevious();
    void findPrevious(QString text);

private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createTextView();
    void createOutlineView();
    void createProblemsView();
    void createSearchBox();
    void readSettings();
    void writeSettings();
    bool maybeSave();
    void loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    QString strippedName(const QString &fullFileName);

    QDockWidget *dockTextEdit, *dockTextEdit2;
    NCLTextEditor *textEdit, *textEdit2;
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
    QAction *showSearchBoxAct;

    QFrame searchBox;
    QPushButton doSearchButton;
    QDockWidget *dockSearchBox;
    SearchLineEdit searchBoxText;

/** VIEWS **/
    /** Outline View */
#ifdef NCLEDITOR_STANDALONE
    QDockWidget *dockOutlineView;
    NCLTreeWidget *outlineView;
#endif

    QMenu *nodeMenu;
    QAction *insertNodeChildAct;

    /** Problems View */
    NCLProblemsView *problemsView;

    /*  QDockWidget *dockLayoutView;
        LayoutView *layoutView; */

    /* Preferences *preferences;
    PreferencesPage *textEditorPreferencesPage; */

signals:
    void elementAdded(QString, QString, QMap <QString, QString>&, bool);
    void focusLosted();
};

#endif

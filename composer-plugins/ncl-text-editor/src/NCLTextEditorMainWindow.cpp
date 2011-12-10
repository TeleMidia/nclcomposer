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
#include <QtGui>

#include <Qsci/qsciscintilla.h>

#include "NCLTextEditorMainWindow.h"

NCLTextEditorMainWindow::NCLTextEditorMainWindow(QWidget *parent):
        QMainWindow(parent)
{
    // preferences = new Preferences(parent);
    createTextView();
    createActions();
#ifdef NCLEDITOR_STANDALONE
    createMenus();
    createToolBars();
    createProblemsView();
    createStatusBar();
    createOutlineView();
#endif
    setDockOptions(NCLTextEditorMainWindow::AllowNestedDocks
                   | NCLTextEditorMainWindow::AllowTabbedDocks
                   | NCLTextEditorMainWindow::AnimatedDocks);
    //  Set window to fixed size
#ifndef NCLEDITOR_STANDALONE
    this->setWindowFlags(Qt::CustomizeWindowHint);//Set window with no title bar
#endif
    //  this->setWindowFlags(Qt::FramelessWindowHint); //Set a frameless window

    /* setTabPosition(Qt::LeftDockWidgetArea, QTabWidget::North);
    setTabPosition(Qt::RightDockWidgetArea, QTabWidget::North);
    setTabPosition(Qt::BottomDockWidgetArea, QTabWidget::North);
    setTabPosition(Qt::TopDockWidgetArea, QTabWidget::North); */

    readSettings();

    connect(textEdit, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

#ifdef NCLEDITOR_STANDALONE
    connect ( outlineView, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
              SLOT(gotoLineOf(QTreeWidgetItem *, int)) );
#endif

    setCurrentFile("");
    setUnifiedTitleAndToolBarOnMac(true);

}

void NCLTextEditorMainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }
}

void NCLTextEditorMainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }
}

void NCLTextEditorMainWindow::open()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

bool NCLTextEditorMainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}

bool NCLTextEditorMainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}

void NCLTextEditorMainWindow::about()
{
    QMessageBox::about(this, tr("About Application"),
                       tr("The <b>Application</b> example demonstrates how to "
                          "write modern GUI applications using Qt, with a menu "
                          "bar, toolbars, and a status bar."));
}

void NCLTextEditorMainWindow::documentWasModified()
{
    setWindowModified(textEdit->isModified());
}

void NCLTextEditorMainWindow::createActions()
{
    newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
    newAct->setShortcut(tr("Ctrl+N"));
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("Ctrl+O"));
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("Ctrl+S"));
    saveAct->setStatusTip(tr("Save the document to disk"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    cutAct = new QAction(QIcon(":/images/clipboard_cut.png"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                            "clipboard"));
    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                             "clipboard"));
    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                              "selection"));
    connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            cutAct, SLOT(setEnabled(bool)));
    connect(textEdit, SIGNAL(copyAvailable(bool)),
            copyAct, SLOT(setEnabled(bool)));

    fullscreenAct = new QAction( QIcon(":/images/window_fullscreen.png"),
                                 tr("&FullScreen"), this);
    fullscreenAct->setShortcut(tr("F11"));
    fullscreenAct->setStatusTip(tr("Enable/disable fullscreen mode"));
    connect(fullscreenAct, SIGNAL(triggered()), this, SLOT(showInFullScreen()));

    editPreferencesAct = new QAction(tr("&Preferences"), this);
    editPreferencesAct->setStatusTip(tr("Edit Preferences related to "
                                        "editors."));
    connect ( editPreferencesAct, SIGNAL(triggered()),
              this, SLOT(showPreferences()));

    synchronizeAct = new QAction ( QIcon(":/images/synchronize-icon-24.png"),
                                   tr("&Synchronize"), this);
    synchronizeAct->setStatusTip(tr("Synchronize current text with the others"
                                    "plugins."));
}

void NCLTextEditorMainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    editMenu->addSeparator();
    editMenu->addAction(editPreferencesAct);

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
}

void NCLTextEditorMainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->setObjectName(QString("fileToolBar"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    fileToolBar->addAction(fullscreenAct);

    editToolBar = addToolBar(tr("Edit"));
    editToolBar->setObjectName(QString("editToolBar"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    editToolBar->addAction(synchronizeAct);
}

void NCLTextEditorMainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void NCLTextEditorMainWindow::createOutlineView()
{
#ifdef NCLEDITOR_STANDALONE
    outlineView = new NCLTreeWidget(this);
    //outlineView->setMaximumWidth(300);
    outlineView->setColumnCount(4);
    // outlineView->setEditTriggers(QAbstractItemView::AllEditTriggers);

    dockOutlineView = new QDockWidget("Outline", this);
    dockOutlineView->setObjectName(QString("dockOutlineView"));
    dockOutlineView->setFeatures(QDockWidget::DockWidgetMovable /*|
                              QDockWidget::DockWidgetFloatable*/);
    /* dockOutlineView->setAllowedAreas(Qt::LeftDockWidgetArea |
                                  Qt::RightDockWidgetArea); */
    dockOutlineView->setWidget(outlineView);

    addDockWidget(Qt::LeftDockWidgetArea, dockOutlineView);

    outlineView->setContextMenuPolicy(Qt::ActionsContextMenu);

    nodeMenu = new QMenu(outlineView);
    insertNodeChildAct = new QAction( QIcon(":/images/save.png"),
                                      tr("&Add child"), this);
    connect ( insertNodeChildAct, SIGNAL(triggered()),
              this, SLOT(insertElement()));

    outlineView->addAction(insertNodeChildAct);

    connect( outlineView,
             SIGNAL(parserErrorNotify(QString, QString, int, int, int)),
             textEdit, SLOT(markError(QString, QString, int, int, int)) );
#endif
}

void NCLTextEditorMainWindow::createProblemsView()
{
    problemsView = new NCLProblemsView(this);
    problemsView->setObjectName(QString("dockProblemsView"));
    //problemsView->setMaximumHeight(150);
    addDockWidget(Qt::RightDockWidgetArea, problemsView);

#ifdef NCLEDITOR_STANDALONE
    connect( outlineView,
             SIGNAL(parserErrorNotify(QString, QString, int, int, int)),
             problemsView,
             SLOT(addProblem(QString, QString, int, int, int)));
#endif
}

void NCLTextEditorMainWindow::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Trolltech", "Application Example");
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    bool fullscreen = settings.value("fullscreen", true).toBool();

    resize(size);
    move(pos);

    restoreState(settings.value("qDocksState").toByteArray());

    if(fullscreen)
        showFullScreen();

}

void NCLTextEditorMainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Trolltech", "Application Example");
    settings.setValue("pos", pos());
    settings.setValue("size", size());
    settings.setValue("fullscreen", isFullScreen());

    // Save the QDock position state
    settings.setValue("qDocksState", saveState());
}

bool NCLTextEditorMainWindow::maybeSave()
{
    if (textEdit->isModified()) {
        int ret = QMessageBox::warning(this, tr("Application"),
                                       tr("The document has been modified.\n"
                                          "Do you want to save your changes?"),
                                       QMessageBox::Yes | QMessageBox::Default,
                                       QMessageBox::No,
                                       QMessageBox::Cancel|QMessageBox::Escape);
        if (ret == QMessageBox::Yes)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void NCLTextEditorMainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool NCLTextEditorMainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->text();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
#ifdef NCLEDITOR_STANDALONE
    outlineView->updateFromText(textEdit->text());
#endif
    return true;
}

void NCLTextEditorMainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    textEdit->setModified(false);
    setWindowModified(false);

    QString shownName;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = strippedName(curFile);

    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));

#ifdef NCLEDITOR_STANDALONE
    outlineView->updateFromText(textEdit->text());
#endif
}

QString NCLTextEditorMainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void NCLTextEditorMainWindow::showInFullScreen(){
    if(!isFullScreen())
        showFullScreen();
    else
        showNormal();
}

void NCLTextEditorMainWindow::gotoLineOf(QTreeWidgetItem *item, int column)
{
    (void) column;

    bool ok;
    int line = item->text(2).toInt(&ok, 10);
    textEdit->setCursorPosition(line-1, 0);
    textEdit->ensureLineVisible(line-1);
    textEdit->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);
}

//FIXME:    1. fix line element line numbers.
void NCLTextEditorMainWindow::insertElement()
{
#ifdef NCLEDITOR_STANDALONE
    bool ok;
    QList<QTreeWidgetItem*> selecteds = outlineView->selectedItems ();
    QTreeWidgetItem *item = selecteds.at (0);
    QString id = item->text(1);
    int line = item->text(2).toInt ( &ok, 10 );
    QString tagname = item->text(0);
    QMap <QString, QString> empty;

    QStringList strlist;
    map <QString, char> * children =
            NCLStructure::getInstance()->getChildren(tagname);

    if(children != NULL) {
        map <QString, char>::iterator it;
        for(it = children->begin(); it != children->end(); ++it){
            strlist << it->first;
        }
    }

    QString element = QInputDialog::getItem(this,
                                            tr("Add child"),
                                            tr("Element name:"),
                                            strlist,
                                            0,
                                            true,
                                            &ok);

    if(ok && !element.isEmpty())
    {
        //Add new Element to OutlineWidget
        outlineView->addElement(item, 0, element, QString(""), empty, line, 0);

        //Add new Element to texttWidget
        int endLine = textEdit
                      ->SendScintilla( QsciScintilla::SCI_GETLINEENDPOSITION,
                                       line-1);
        int beginLine = textEdit
                        ->SendScintilla(QsciScintilla::SCI_POSITIONFROMLINE,
                                        line-1);
        int end_element_column = item->text(3).toInt(&ok, 10);
        bool fix_next_line_indentation = false;

        //find if we are in a "<tagname/>" then open this tag
        char ch = textEdit->SendScintilla(
                QsciScintilla::SCI_GETCHARAT, beginLine +
                end_element_column-1);
        if (ch == '/') {
            QString endtag = ">";
            endtag += "<";
            textEdit->insertAt(tagname, line-1, end_element_column);
            textEdit->insertAt(endtag,  line-1, end_element_column-1);
        }

        //put all the required attributes
        //TODO: remove from here (create a function)
        map <QString, bool> *attributes =
                NCLStructure::getInstance()->getAttributes(element);
        if(attributes != NULL) {
            map <QString, bool>::iterator it;
            for(it = attributes->begin(); it != attributes->end(); ++it){
                if(it->second) {
                    element += " ";
                    element += it->first + "=\"\"";
                }
            }
        }

        element.prepend("<");
        element.append("/>");
        if(end_element_column != endLine-beginLine) {
            fix_next_line_indentation = true;
            element.append("\n");
        }

        element.prepend("\n");

        // qDebug() << line << " " << beginLine << " " << endLine << " "
        //          << end_element_column << " ";

        textEdit->insertAt(element, line-1, end_element_column);

        //fix indentation
        int lineident = textEdit->SendScintilla(
                QsciScintilla::SCI_GETLINEINDENTATION,
                line-1);
        textEdit->SendScintilla(QsciScintilla::SCI_GETLINEINDENTATION, line-1);

        if(fix_next_line_indentation)
            textEdit->SendScintilla( QsciScintilla::SCI_SETLINEINDENTATION,
                                     line+1, lineident);

        textEdit->SendScintilla(QsciScintilla::SCI_SETLINEINDENTATION,
                                line,
                                lineident+8);

        textEdit->setCursorPosition(line, 0);
        textEdit->ensureLineVisible(line);
        textEdit->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);

        emit elementAdded(tagname, id, empty, false);
    }
#endif

}

void NCLTextEditorMainWindow::createTextView() {
    dockTextEdit = new QDockWidget("Text", this);
    dockTextEdit->setObjectName(QString("dockTextView"));
    dockTextEdit->setFeatures(QDockWidget::DockWidgetMovable /*|
                              QDockWidget::DockWidgetFloatable*/);

    //Remove the title bar if whe are working in the plugin version.
#ifndef NCLEDITOR_STANDALONE
    QWidget* titleWidget = new QWidget(this);/*where this a QMainWindow object*/
    dockTextEdit->setTitleBarWidget( titleWidget );
#endif

//    dockTextEdit2 = new QDockWidget("Text", this);
//    dockTextEdit2->setObjectName(QString("dockTextView2"));
//    dockTextEdit2->setFeatures(QDockWidget::DockWidgetMovable /*|
//                              QDockWidget::DockWidgetFloatable*/);

    // dockTextEdit->setAllowedAreas(Qt::LeftDockWidgetArea |
    //                               Qt::RightDockWidgetArea);

    textEdit = new NCLTextEditor(this);
    textEdit->setTabBehavior(NCLTextEditor::TAB_BEHAVIOR_NEXT_ATTR);
    dockTextEdit->setWidget(textEdit);

//    textEdit2 = new NCLTextEditor(this);
//    textEdit2->setTabBehavior(NCLTextEditor::TAB_BEHAVIOR_NEXT_ATTR);
//    dockTextEdit2->setWidget(textEdit2);
//    textEdit2->setDocument(textEdit->document());

    addDockWidget(Qt::RightDockWidgetArea, dockTextEdit);
//    addDockWidget(Qt::RightDockWidgetArea, dockTextEdit2);

    // setCentralWidget(textEdit);

    /** Initialize Text Preferences Pages*/
    //textEditorPreferencesPage = preferences->addPreferencesPage("Text Editor");
    //textEditorPreferencesPage->addInputString("teste", "teste");
}

void NCLTextEditorMainWindow::showPreferences()
{
    //preferences->show();
}

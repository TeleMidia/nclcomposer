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
  QMainWindow(parent), _searchBox(this)
{
  // preferences = new Preferences(parent);
  createTextView();
  createActions();
  createSearchBox();

#ifdef NCLEDITOR_STANDALONE
  createMenus();
  createToolBars();
  createOutlineView();
  createProblemsView();
  createStatusBar();
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

  connect(_textEdit, SIGNAL(textChanged()), this, SLOT(documentWasModified()));

#ifdef NCLEDITOR_STANDALONE
  connect ( outlineView, SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            SLOT(gotoLineOf(QTreeWidgetItem *, int)) );
#endif

  setCurrentFile("");
  setUnifiedTitleAndToolBarOnMac(true);

}

void NCLTextEditorMainWindow::closeEvent(QCloseEvent *event)
{
  if (maybeSave())
  {
    writeSettings();
    event->accept();
  }
  else
  {
    event->ignore();
  }
}

void NCLTextEditorMainWindow::newFile()
{
  if (maybeSave())
  {
    _textEdit->clear();
    setCurrentFile("");
  }
}

void NCLTextEditorMainWindow::open()
{
  if (maybeSave())
  {
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
      loadFile(fileName);
  }
}

bool NCLTextEditorMainWindow::save()
{
  if (_curFile.isEmpty())
    return saveAs();
  else
    return saveFile(_curFile);
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
  setWindowModified(_textEdit->isModified());
}

void NCLTextEditorMainWindow::createActions()
{
  _newAct = new QAction(QIcon(":/images/new.png"), tr("&New"), this);
  _newAct->setShortcut(tr("Ctrl+N"));
  _newAct->setStatusTip(tr("Create a new file"));
  connect(_newAct, SIGNAL(triggered()), this, SLOT(newFile()));

  _openAct = new QAction(QIcon(":/images/open.png"), tr("&Open..."), this);
  _openAct->setShortcut(tr("Ctrl+O"));
  _openAct->setStatusTip(tr("Open an existing file"));
  connect(_openAct, SIGNAL(triggered()), this, SLOT(open()));

  _saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save"), this);
  _saveAct->setShortcut(tr("Ctrl+S"));
  _saveAct->setStatusTip(tr("Save the document to disk"));
  connect(_saveAct, SIGNAL(triggered()), this, SLOT(save()));

  _saveAsAct = new QAction(tr("Save &As..."), this);
  _saveAsAct->setStatusTip(tr("Save the document under a new name"));
  connect(_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

  _exitAct = new QAction(tr("E&xit"), this);
  _exitAct->setShortcut(tr("Ctrl+Q"));
  _exitAct->setStatusTip(tr("Exit the application"));
  connect(_exitAct, SIGNAL(triggered()), this, SLOT(close()));

  _cutAct = new QAction(QIcon(":/images/clipboard_cut.png"), tr("Cu&t"), this);
  _cutAct->setShortcut(tr("Ctrl+X"));
  _cutAct->setStatusTip(tr("Cut the current selection's contents to the "
                          "clipboard"));
  connect(_cutAct, SIGNAL(triggered()), _textEdit, SLOT(cut()));

  _copyAct = new QAction(QIcon(":/images/copy.png"), tr("&Copy"), this);
  _copyAct->setShortcut(tr("Ctrl+C"));
  _copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                           "clipboard"));
  connect(_copyAct, SIGNAL(triggered()), _textEdit, SLOT(copy()));

  _pasteAct = new QAction(QIcon(":/images/paste.png"), tr("&Paste"), this);
  _pasteAct->setShortcut(tr("Ctrl+V"));
  _pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
                            "selection"));
  connect(_pasteAct, SIGNAL(triggered()), _textEdit, SLOT(paste()));

  _aboutAct = new QAction(tr("&About"), this);
  _aboutAct->setStatusTip(tr("Show the application's About box"));
  connect(_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

  _aboutQtAct = new QAction(tr("About &Qt"), this);
  _aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
  connect(_aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  _cutAct->setEnabled(false);
  _copyAct->setEnabled(false);
  connect(_textEdit, SIGNAL(copyAvailable(bool)),
          _cutAct, SLOT(setEnabled(bool)));
  connect(_textEdit, SIGNAL(copyAvailable(bool)),
          _copyAct, SLOT(setEnabled(bool)));

  _fullscreenAct = new QAction( QIcon(":/images/window_fullscreen.png"),
                               tr("&FullScreen"), this);
  _fullscreenAct->setShortcut(tr("F11"));
  _fullscreenAct->setStatusTip(tr("Enable/disable fullscreen mode"));
  connect(_fullscreenAct, SIGNAL(triggered()), this, SLOT(showInFullScreen()));

  _editPreferencesAct = new QAction(tr("&Preferences"), this);
  _editPreferencesAct->setStatusTip(tr("Edit Preferences related to "
                                      "editors."));
  connect ( _editPreferencesAct, SIGNAL(triggered()),
            this, SLOT(showPreferences()));

  _synchronizeAct = new QAction ( QIcon(":/images/synchronize-icon-24.png"),
                                 tr("&Synchronize"), this);
  _synchronizeAct->setStatusTip(tr("Synchronize current text with the others"
                                  " plugins."));

  _showSearchBoxAct = new QAction(QIcon(), tr("Search"), _textEdit);
  _showSearchBoxAct->setShortcut(tr("Ctrl+F"));
  _showSearchBoxAct->setShortcutContext(Qt::WindowShortcut);
  connect(_showSearchBoxAct, SIGNAL(triggered()), this, SLOT(showSearchBox()));

  addAction(_showSearchBoxAct);
}

void NCLTextEditorMainWindow::createMenus()
{
  _fileMenu = menuBar()->addMenu(tr("&File"));
  _fileMenu->addAction(_newAct);
  _fileMenu->addAction(_openAct);
  _fileMenu->addAction(_saveAct);
  _fileMenu->addAction(_saveAsAct);
  _fileMenu->addSeparator();
  _fileMenu->addAction(_exitAct);

  _editMenu = menuBar()->addMenu(tr("&Edit"));
  _editMenu->addAction(_cutAct);
  _editMenu->addAction(_copyAct);
  _editMenu->addAction(_pasteAct);
  _editMenu->addAction(_showSearchBoxAct);

  _editMenu->addSeparator();
  _editMenu->addAction(_editPreferencesAct);

  menuBar()->addSeparator();

  _helpMenu = menuBar()->addMenu(tr("&Help"));
  _helpMenu->addAction(_aboutAct);
  _helpMenu->addAction(_aboutQtAct);
}

void NCLTextEditorMainWindow::createToolBars()
{
  _fileToolBar = addToolBar(tr("File"));
  _fileToolBar->setObjectName(QString("fileToolBar"));
  _fileToolBar->addAction(_newAct);
  _fileToolBar->addAction(_openAct);
  _fileToolBar->addAction(_saveAct);
  _fileToolBar->addAction(_fullscreenAct);

  _editToolBar = addToolBar(tr("Edit"));
  _editToolBar->setObjectName(QString("editToolBar"));
  _editToolBar->addAction(_cutAct);
  _editToolBar->addAction(_copyAct);
  _editToolBar->addAction(_pasteAct);
  _editToolBar->addAction(_synchronizeAct);
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
  _problemsView = new NCLProblemsView(this);
  _problemsView->setObjectName(QString("dockProblemsView"));
  //problemsView->setMaximumHeight(150);
  addDockWidget(Qt::RightDockWidgetArea, _problemsView);

#ifdef NCLEDITOR_STANDALONE
  connect( outlineView,
           SIGNAL(parserErrorNotify(QString, QString, int, int, int)),
           problemsView,
           SLOT(addProblem(QString, QString, int, int, int)));
#endif
}

void NCLTextEditorMainWindow::createSearchBox()
{
  _dockSearchBox = new QDockWidget("Search", this);
  _dockSearchBox->setVisible(false);
  _dockSearchBox->setMaximumHeight(100);//65 alterado
  _dockSearchBox->setObjectName(QString("dockSearchBox"));
  _dockSearchBox->setFeatures(QDockWidget::DockWidgetClosable );

  //  doSearchButton.setText(tr("Search"));
  _searchBox.setWindowTitle(tr("Search..."));

  QGridLayout *layout = new QGridLayout(&_searchBox);
  layout->addWidget(&_searchBoxText, 0, 0);
  // layout->addWidget(&doSearchButton, 0, 1);
  layout->addWidget(&_replaceBoxText, 1, 0);
  _replaceBoxText.setPlaceholderText(tr("Replace"));
  _searchBox.setLayout(layout);
  layout->setMargin(0);

  QPushButton *nextButton = new QPushButton(QIcon(":/images/next_icon.png"), "");
  nextButton->setFlat(true);
  connect(nextButton, SIGNAL(pressed()), SLOT(findNext()));

  QPushButton *previousButton = new QPushButton(QIcon(":/images/previous_icon.png"), "");
  previousButton->setFlat(true);
  connect(previousButton, SIGNAL(pressed()), SLOT(findPrevious()));

  QPushButton *replaceButton = new QPushButton(QString("Replace"));
  //replaceButton->setFlat(true);
  connect(replaceButton, SIGNAL(pressed()),SLOT(replaceWord()));

  QPushButton *replaceAndNextButton = new QPushButton(QString("Replace and Find"));
  //replaceButton->setFlat(true);
  connect(replaceAndNextButton, SIGNAL(pressed()),SLOT(replaceAndFind()));

  QPushButton *replaceAll = new QPushButton(QString("Replace all"));
  //replaceButton->setFlat(true);
  connect(replaceAll, SIGNAL(pressed()),SLOT(replaceAll()));

  nextButton->setMaximumSize(16, 16);
  previousButton->setMaximumSize(16, 16);
  layout->addWidget(previousButton, 0, 1);
  layout->addWidget(nextButton, 0, 2);
  layout->addWidget(replaceButton, 1, 2);
  layout->addWidget(replaceAndNextButton, 1, 3);
  layout->addWidget(replaceAll, 1, 4);

  //  connect(&doSearchButton, SIGNAL(pressed()), this, SLOT(findNext()));
  connect( &_searchBoxText,
           SIGNAL(textChanged(const QString &)),
           SLOT(findNext(const QString &)) );

  connect(&_searchBoxText, SIGNAL(returnPressed()), SLOT(findNext()));

  connect(&_searchBoxText, SIGNAL(shiftReturnPressed()), SLOT(findPrevious()));

  connect(&_searchBoxText, SIGNAL(escPressed()), SLOT(hideSearchBox()));

  _dockSearchBox->setWidget(&_searchBox);
  addDockWidget(Qt::RightDockWidgetArea, _dockSearchBox);
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
  if (_textEdit->isModified()) {
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
  _textEdit->setText(in.readAll());
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
  out << _textEdit->text();
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
  _curFile = fileName;
  _textEdit->setModified(false);
  setWindowModified(false);

  QString shownName;
  if (_curFile.isEmpty())
    shownName = "untitled.txt";
  else
    shownName = strippedName(_curFile);

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
  _textEdit->setCursorPosition(line-1, 0);
  _textEdit->ensureLineVisible(line-1);
  _textEdit->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);
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
                                          tr("&Add child"),
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

void NCLTextEditorMainWindow::createTextView()
{
  _dockTextEdit = new QDockWidget("Text", this);
  _dockTextEdit->setObjectName(QString("dockTextView"));
  _dockTextEdit->setFeatures(QDockWidget::DockWidgetMovable);

  //Remove the title bar if whe are working in the plugin version.
#ifndef NCLEDITOR_STANDALONE
  QWidget* titleWidget = new QWidget(this);/*where this a QMainWindow object*/
  _dockTextEdit->setTitleBarWidget( titleWidget );
#endif

  //    dockTextEdit2 = new QDockWidget("Text", this);
  //    dockTextEdit2->setObjectName(QString("dockTextView2"));
  //    dockTextEdit2->setFeatures(QDockWidget::DockWidgetMovable /*|
  //                              QDockWidget::DockWidgetFloatable*/);

  // dockTextEdit->setAllowedAreas(Qt::LeftDockWidgetArea |
  //                               Qt::RightDockWidgetArea);

  _textEdit = new NCLTextEditor(this);
  _textEdit->setTabBehavior(NCLTextEditor::TAB_BEHAVIOR_NEXT_ATTR);
  _dockTextEdit->setWidget(_textEdit);

  //    textEdit2 = new NCLTextEditor(this);
  //    textEdit2->setTabBehavior(NCLTextEditor::TAB_BEHAVIOR_NEXT_ATTR);
  //    dockTextEdit2->setWidget(textEdit2);
  //    textEdit2->setDocument(textEdit->document());

  addDockWidget(Qt::RightDockWidgetArea, _dockTextEdit);
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

void NCLTextEditorMainWindow::showSearchBox()
{
  _dockSearchBox->show();
  _searchBoxText.setFocus();
}

void NCLTextEditorMainWindow::hideSearchBox()
{
  _dockSearchBox->hide();
  _textEdit->setFocus();
}

void NCLTextEditorMainWindow::findNext()
{
  QString text = _searchBoxText.text();
  findNext(text);
}

bool NCLTextEditorMainWindow::findNext(const QString &text)
{
  return _textEdit->findFirst(text, true, false, true, true);
}

void NCLTextEditorMainWindow::findPrevious()
{
  QString text = _searchBoxText.text();
  findPrevious(text);
}

void NCLTextEditorMainWindow::findPrevious(const QString &text)
{
  int line, index;
  _textEdit->getCursorPosition(&line, &index);
  index -= text.size();
  if(index < 0) line--;

  _textEdit->findFirst(text, true, false, true, true, false, line, index);
}

void NCLTextEditorMainWindow::replaceWord()
{
  QString text = _replaceBoxText.text();
  replaceWord(text);
}


void NCLTextEditorMainWindow::replaceWord(const QString &text)
{
  _textEdit->replace(text);
}

void NCLTextEditorMainWindow::replaceAndFind()
{
  QString textSearch = _searchBoxText.text();
  QString textReplace = _replaceBoxText.text();
  replaceAndFind(textSearch, textReplace);
}

void NCLTextEditorMainWindow::replaceAndFind(const QString &textSearch, const QString &textReplace)
{
  //todo:verify if highlightedtext == textsearch before replace
  //_textEdit->
  replaceWord(textReplace);
  findNext(textSearch);
}

void NCLTextEditorMainWindow::replaceAll()
{
  QString textSearch = _searchBoxText.text();
  QString textReplace = _replaceBoxText.text();
  replaceAll(textSearch, textReplace);
}

void NCLTextEditorMainWindow::replaceAll(const QString &textSearch, const QString &textReplace)
{
  while(findNext(textSearch))
    replaceWord(textReplace);
}

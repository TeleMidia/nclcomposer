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
#include "NCLTextEditor.h"

#include <QDir>
#include <QUrl>
#include <QFileInfo>
#include <QStack>
#include <Qsci/qscistyledtext.h>

NCLTextEditor::NCLTextEditor(QWidget *parent) :
  QsciScintilla(parent)
{
  initParameters();
  nclexer = NULL;
  apis = NULL;
  textWithoutUserInter = "";
  focusInIgnoringCurrentText = false;

  setAcceptDrops(true);
}

NCLTextEditor::~NCLTextEditor()
{
//    if(nclexer != NULL) delete nclexer;
//    if(apis != NULL) delete apis;
}

void NCLTextEditor::initParameters()
{
  tabBehavior = TAB_BEHAVIOR_DEFAULT;

  setAutoIndent(true);
  setFolding(QsciScintilla::CircledTreeFoldStyle);
  setFoldMarginColors(PREF_FOLD_MARGIN_FORE_COLOR, PREF_FOLD_MARGIN_BACK_COLOR);

  setMarginLineNumbers(1, true);
  setMarginWidth(1, 25);
  setMarginsBackgroundColor(MARGINS_BACKGROUND_COLOR);

  setCaretWidth(20);
  setCaretLineBackgroundColor(QColor("#e6fff0"));
  setCaretLineVisible(true);

  ensureLineVisible(1);
  setUtf8(true);
  setWrapMode(WrapWord);
  setWrapVisualFlags(WrapFlagByText);

  /* AutoCompletetion parameters */
  setAutoCompletionThreshold(-1);
  setAutoCompletionFillupsEnabled(false);
  setAutoIndent(true);
  setIndentationWidth(2);
  //  setIndentationGuides(true);
  setIndentationsUseTabs(true);


  setAnnotationDisplay(QsciScintilla::AnnotationBoxed);
  //adding text partitions
  /*MyLexer *mylexer = new MyLexer(this);

    QRegExp attrValueRegex ("\".*\"");
    QsciStyle attrValueStyle ( 4, "ATTRIBUTE_VALUE", QColor(0x0, 0x0, 0xFF),
                               mylexer->defaultPaper(1),
                               mylexer->defaultFont(1));
    mylexer->addTextPartition(4, attrValueRegex, attrValueStyle);

    QRegExp startTagRegex ("<.[^><.]+>");
    QsciStyle startTagStyle (10, "START_TAG",  QColor(0x0, 0x0, 0xFF),
                             mylexer->defaultPaper(1), mylexer->defaultFont(1));
    mylexer->addTextPartition(10, startTagRegex, startTagStyle);
    */

  nclexer = new QsciLexerNCL (0);
  setLexer(nclexer);

  //APIS
  apis = new QsciNCLAPIs(lexer());

  QFont font;
  font.setFamily(QString::fromUtf8(PREF_FONT_FAMILY));
  font.setPointSize(PREF_FONT_SIZE);
  setFont(font);
  setCaretLineBackgroundColor(QColor(PREF_CARET_LINE_BG_COLOR));

  // connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)), this,
  // SLOT(MarkLine(int,int,Qt::KeyboardModifiers)));

  error_marker = markerDefine(QPixmap(":/images/error-icon-16.png"), -1);
  error_indicator = indicatorDefine(SquiggleIndicator, 1);
  setIndicatorForegroundColor(QColor("#FF0000"), error_indicator);
  filling_attribute_indicator = indicatorDefine (RoundBoxIndicator, 2);

  // qDebug() << error_marker << " " << error_indicator;
  // setWhitespaceVisibility(QsciScintilla::WsVisible);
}

void NCLTextEditor::Increasefont()
{
  zoomIn();
}

void NCLTextEditor::Decreasefont()
{
  zoomOut();
}

void NCLTextEditor::clearErrorIndicators()
{
  // clear markers
  markerDeleteAll(error_marker);

  //clearIndicators
  int nr_lines = lines();
  QString tmp2 = this->text(nr_lines);
  clearIndicatorRange (0,
                       0,
                       nr_lines,
                       tmp2.size(),
                       error_indicator);
  clearAnnotations();
}

void NCLTextEditor::markError ( QString description,
                                QString file,
                                int line,
                                int column,
                                int severity)
{
  //TODO: Show Error Messages
  qDebug() << "MarkError line=" << line;
  QString tmp = this->text(line);

  // ADD MARKER
  if (text() != "")
    markerAdd(line, error_marker);

  //ADD INDICATOR
  int indentation = 0;
  while (tmp[indentation].isSpace())
    indentation++;

  fillIndicatorRange(line,
                     indentation,
                     line,
                     tmp.size()-1,
                     error_indicator);
  annotate(line, QsciStyledText(description, 0));
}

void NCLTextEditor::wheelEvent (QWheelEvent *event)
{
  if(event->modifiers() == Qt::ControlModifier){
    if(event->delta() > 0)
      zoomIn();
    else
      zoomOut();
  }
  QsciScintilla::wheelEvent(event);
}

// TODO: Maybe move the mousePressEvent function content, and part of the
// keyPressEvent, to a function connected to selectionChanged signal.
void NCLTextEditor::mousePressEvent(QMouseEvent *event)
{
  int previousLine, previousIndex;
  getCursorPosition(&previousLine, &previousIndex);

  QString strline = text(previousLine);
  clearIndicatorRange ( previousLine,
                        0,
                        previousLine,
                        strline.size(),
                        filling_attribute_indicator);

  QsciScintilla::mousePressEvent(event);

  int line, index, begin, end;
  getCursorPosition(&line, &index);
  int pos, style;
  pos = SendScintilla(SCI_GETCURRENTPOS);
  style = SendScintilla(SCI_GETSTYLEAT, pos);

  //FIXME: Attribute also can be between Single quotes
  if (style == QsciLexerNCL::HTMLDoubleQuotedString)
  {
    //interaction_state = FILLING_ATTRIBUTES_STATE;
    getCursorPosition (&line, &index);
    updateVisualFillingAttributeField(line, index, begin, end);
  }
  else
  {
    interaction_state = DEFAULT_STATE;
  }
}

//\fixme I DONT KNOW WHY (or WHERE), BUT THE UNDO IS NOT WORKING EVERY TIME!
void NCLTextEditor::keyPressEvent(QKeyEvent *event)
{
  //Ctrl + Space == Autocomplete
  if((event->modifiers() & Qt::ControlModifier) &&
     (event->key() == Qt::Key_Space))
  {
    event->accept();
    autoCompleteFromAPIs();
    return;
  }
  //Ctrl + Shift + F == format Text
  if((event->modifiers() & Qt::ControlModifier) &&
     (event->modifiers() & Qt::ShiftModifier) &&
     (event->key() == Qt::Key_F))
  {
    event->accept();
    formatText();
    return;
  }

  int begin, end;
  int line, index;
  getCursorPosition(&line, &index);
  int pos, style, size;
  pos = SendScintilla(SCI_GETCURRENTPOS);
  size = SendScintilla(SCI_GETTEXTLENGTH);

  // If I receive a Qt::Key_Backtab
  // change event to Shift+Tab
  if(event->key() == Qt::Key_Backtab) {
    event->accept();
    event = new QKeyEvent(event->type(), Qt::Key_Tab, Qt::ShiftModifier);
  }

  //check zoomin event.
  //if((event->modifiers() & Qt::ControlModifier) &&
  //   (event->key() == Qt::Key_Plus))
  if(event->key() == Qt::Key_ZoomIn)
  {
    event->accept();
    Increasefont();
    return;
  }

  //check zoomout event.

  //if((event->modifiers() & Qt::ControlModifier) &&
  //   (event->key() == Qt::Key_Minus))
  if(event->key() == Qt::Key_ZoomOut)
  {
    event->accept();
    Decreasefont();
    return;
  }

  if (interaction_state == FILLING_ATTRIBUTES_STATE) {
    QString strline = text(line);
    clearIndicatorRange ( line,
                          0,
                          line,
                          strline.size(),
                          filling_attribute_indicator);

    if(event->key() == Qt::Key_Return){
      interaction_state = DEFAULT_STATE;
      setSelection(line, index, line, index);
      QsciScintilla::keyPressEvent(event);
      return;
    }

    if(event->key() == Qt::Key_Tab){
      QString strline = text(line);
      pos = SendScintilla(SCI_GETCURRENTPOS);
      style = SendScintilla(SCI_GETSTYLEAT, pos);
      bool error = false;

      //SHIFT+TAB -> GO TO PREVIOUS ATRIBUTE
      if(event->modifiers() & Qt::ShiftModifier) {
        while (pos > 0 ) {
          style = SendScintilla(SCI_GETSTYLEAT, pos);
          if(style != QsciLexerNCL::HTMLDoubleQuotedString)
            break;
          pos--;
        }

        if (pos >= 0)
          userFillingPreviousAttribute(pos);
        else
          error = true;

      }
      //JUST TAB -> GO TO NEXT ATTRIBUTE
      else if(event->modifiers() == Qt::NoModifier) {
        while (pos < size){
          style = SendScintilla(SCI_GETSTYLEAT, pos);
          if(style != QsciLexerNCL::HTMLDoubleQuotedString)
            break;
          pos++;
        }

        if(pos < size)
          userFillingNextAttribute(pos);
        else
          error = true;
      }

      if (error) {
        clearIndicatorRange( line,
                             0,
                             line,
                             strline.size(),
                             filling_attribute_indicator );
        interaction_state = DEFAULT_STATE;
      }
      return;

    }
    else
    {
      QsciScintilla::keyPressEvent ( event ) ;
      getCursorPosition (&line, &index);
      pos = SendScintilla(SCI_GETCURRENTPOS);
      style = SendScintilla(SCI_GETSTYLEAT, pos);

      // Test if pos-1 is also inside the attribute, otherwise it will
      // treat a text inside the end of previous Quote and the start of
      // the current one as an attribute
      if (    style == QsciLexerNCL::HTMLDoubleQuotedString
              && pos-1 >=0 ) {
        //TODO: IMPROVE PERFORMANCE
        recolor();
        style = SendScintilla(SCI_GETSTYLEAT, pos-1);

        if(style == QsciLexerNCL::HTMLDoubleQuotedString)
        {
          updateVisualFillingAttributeField(line, index, begin, end);
          return;
        }
      }

      clearIndicatorRange( line,
                           0,
                           line,
                           strline.size(),
                           filling_attribute_indicator);

      interaction_state = DEFAULT_STATE;
    }
  }
  else
  {
    QsciScintilla::keyPressEvent(event);
    pos = SendScintilla(SCI_GETCURRENTPOS);
    style = SendScintilla(SCI_GETSTYLEAT, pos);

    //Test if pos-1 is also inside the attribute, otherwise it will
    // treat a text inside the end of previous Quote and the start of
    // the current one as an attribute
    if (style == QsciLexerNCL::HTMLDoubleQuotedString &&
        pos-1 >=0 ) {
      //TODO: IMPROVE PERFORMANCE
      recolor();
      style = SendScintilla(SCI_GETSTYLEAT, pos-1);

      //FIXME: Attribute also can be between Single quotes
      if (tabBehavior == TAB_BEHAVIOR_NEXT_ATTR &&
          (style == QsciLexerNCL::HTMLDoubleQuotedString)) {

        interaction_state = FILLING_ATTRIBUTES_STATE;
        getCursorPosition (&line, &index);

        updateVisualFillingAttributeField(line, index, begin, end);
      }
    }
  }
}

void NCLTextEditor::keyReleaseEvent(QKeyEvent *event)
{
  QsciScintilla::keyReleaseEvent(event);
}

void NCLTextEditor::AutoCompleteCompleted()
{
  //qDebug() << "NCLTextEditor::AutoCompleteCompleted()";
}

void NCLTextEditor::MarkLine(int margin, int line, Qt::KeyboardModifiers state)
{
  (void) margin;
  (void) line;
  (void) state;
}

void NCLTextEditor::userFillingNextAttribute(int pos)
{
  int begin, end, style, i = pos;
  int size = SendScintilla(SCI_GETTEXTLENGTH);

  interaction_state = FILLING_ATTRIBUTES_STATE;

  qDebug() << pos;

  while( i < size ) {
    style = SendScintilla(SCI_GETSTYLEAT, i);
    if (style == QsciLexerNCL::HTMLDoubleQuotedString)
      break;
    i++;
  }
  i++;

  qDebug() << i;

  if( i >= size ) {
    interaction_state = DEFAULT_STATE;
    return;
  }

  int newline, newindex;
  lineIndexFromPosition(i, &newline, &newindex);
  setCursorPosition(newline, newindex);
  updateVisualFillingAttributeField (newline, newindex, begin, end);
  setSelection(newline, begin, newline, end);
}

void NCLTextEditor::userFillingPreviousAttribute(int pos)
{
  int begin, end, style;
  int i = pos-1;
  interaction_state = FILLING_ATTRIBUTES_STATE;

  while( i >= 0) {
    style = SendScintilla(SCI_GETSTYLEAT, i);
    if (style == QsciLexerHTML::HTMLDoubleQuotedString)
      break;
    i--;
  }

  if( i < 0) {
    interaction_state = DEFAULT_STATE;
    return;
  }

  int newline, newindex;
  lineIndexFromPosition(i, &newline, &newindex);
  setCursorPosition(newline, newindex);
  updateVisualFillingAttributeField (newline, newindex, begin, end);
  setSelection(newline, begin, newline, end);
}

//FIXME: ESTA INSERINDO UM ESPACO NO FINAL
void NCLTextEditor::updateVisualFillingAttributeField( int line,
                                                       int pos,
                                                       int &begin,
                                                       int &end)
{
  QString strline = text(line);
  begin = pos-1;
  end = pos;
  bool inserted_space = false;

  clearIndicatorRange( line,
                       0,
                       line,
                       strline.size(),
                       filling_attribute_indicator);

  while( begin >= 0 && strline[begin] != '\"')
    begin--;

  if(begin < 0) {
    interaction_state = DEFAULT_STATE;
    return;
  }
  begin++;

  while( end < strline.size() && strline[end] != '\"')
    end++;

  if(end >= strline.size()) {
    interaction_state = DEFAULT_STATE;
    return;
  }

  if(begin == end)
  {
    insert(" ");
    end++;
    inserted_space = true;
  }
  fillIndicatorRange(line, begin, line, end, filling_attribute_indicator);

  if(inserted_space) setSelection(line, begin, line, end);
}

void NCLTextEditor::setTabBehavior(TAB_BEHAVIOR tabBehavior)
{
  this->tabBehavior = tabBehavior;
}

void NCLTextEditor::formatText()
{
  qDebug() << "NCLTextEditor::formatText()";
}

void NCLTextEditor::keepFocused()
{
  focusInIgnoringCurrentText = true;
  this->setFocus();
  this->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);
}

void NCLTextEditor::focusOutEvent(QFocusEvent *event)
{
  emit focusLosted(event);
}

void NCLTextEditor::focusInEvent(QFocusEvent *e)
{
#ifndef NCLEDITOR_STANDALONE
  if(!focusInIgnoringCurrentText)
    textWithoutUserInter = text();

  focusInIgnoringCurrentText = false;
  this->SendScintilla(QsciScintilla::SCI_SETFOCUS, true);
#endif
}

QString NCLTextEditor::textWithoutUserInteraction()
{
  return textWithoutUserInter;
}
void NCLTextEditor::setTextWithoutUserInteraction(QString text)
{
  textWithoutUserInter = text;
}

void NCLTextEditor::setDocumentUrl(QString docURL)
{
  this->docURL = docURL;
}

QString NCLTextEditor::getDocumentUrl()
{
  return this->docURL;
}

bool NCLTextEditor::parseDocument(bool recursive)
{
  domDocs.clear();
  domDoc.clear();
  //  docs.clear();
  if(!domDoc.setContent(this->text()))
  {
    // could not parse the document
    return false;
  }
  //  docs.insert("", doc);

  if(recursive)
  {
    domDocs.insert(docURL, domDoc);
    return parseImportedDocuments(docURL, domDoc, true);
  }
  else
    return true;
}

bool NCLTextEditor::parseImportedDocuments(QString currentFileURI,
                                           QDomDocument &doc, bool recursive)
{
  qDebug() << "parseImportedDocument( " << currentFileURI;
  QStack <QDomElement> stack;
  QDomElement current = doc.firstChildElement();
  bool ret = 1;
  stack.push(current);

  while(stack.size())
  {
    current = stack.top();
    stack.pop();
    qDebug() << current.tagName() << "id = " << current.attribute("id");

    if(current.tagName() == "importBase" &&
       current.hasAttribute("documentURI") &&
       current.hasAttribute("alias"))
    {
      QString importedDocumentUri = current.attribute("documentURI");
      QString fullpath = "";

      QFileInfo importedFI(importedDocumentUri), currentFI(currentFileURI);
      if(importedFI.isAbsolute())
      {
        fullpath = importedDocumentUri;
      }
      else
      {
        importedFI.setFile( currentFI.absolutePath() + "/"
                            + importedDocumentUri);

        if(importedFI.exists())
        {
          qDebug() << importedFI.absoluteFilePath();
          fullpath = importedFI.absoluteFilePath();
        }
      }

      if(fullpath != "")
      {
        qDebug() << "I will include the file " << fullpath;
        QFile file(fullpath);
        if (file.open(QIODevice::ReadOnly))
        {
          QDomDocument currentDomDoc;
          if (currentDomDoc.setContent(&file))
          {
            updateElementsIDWithAlias(currentDomDoc,
                                      current.attribute("alias"));
            if(!parseImportedDocuments(fullpath, currentDomDoc, true))
            {
              ret = false;
              qDebug() << "Could not import " << fullpath;
            }
            domDocs.insert(fullpath, currentDomDoc);
          }
        }
      }
    }

    QDomElement child = current.firstChildElement();
    while(!child.isNull())
    {
      stack.push_back(child);
      child = child.nextSiblingElement();
    }
  }
  return ret;
}

void NCLTextEditor::updateElementsIDWithAlias(QDomDocument doc, QString alias)
{
  QStack <QDomElement> stack;
  QDomElement current = doc.firstChildElement();
  stack.push(current);
  while(stack.size())
  {
    current = stack.top();
    stack.pop();
    if(current.hasAttribute("id"))
    {
      current.setAttribute("id", alias + "#" + current.attribute("id"));
    }

    QDomElement child = current.firstChildElement();
    while(!child.isNull())
    {
      stack.push_back(child);
      child = child.nextSiblingElement();
    }
  }
}

QDomElement NCLTextEditor::elementById(const QDomDocument &domDoc, QString id)
{
  QStack <QDomNode> stack;
  stack.push_front(domDoc.firstChildElement());
  while(stack.size())
  {
    QDomNode current = stack.front();
    stack.pop_front();
    if(current.isElement())
    {
      if(current.toElement().attribute("id") == id)
        return current.toElement();
    }

    QDomNode child = current.firstChild();
    while(!child.isNull())
    {
      stack.push_back(child);
      child = child.nextSibling();
    }
  }

  // The function QDomDocument::elementById will always returns a NULL doc.
  // That is why we have to reimplement this function.
  return QDomElement();
}

QDomElement NCLTextEditor::elementById(QString id, bool recursive)
{
  if(!recursive)
    return elementById(domDoc, id);

  QMapIterator<QString, QDomDocument> i(domDocs);
  while (i.hasNext())
  {
    i.next();
    QDomDocument currentDomDoc = i.value();
    QDomElement element = elementById(currentDomDoc, id);
    if(!element.isNull())
      return element;
  }

  return domDoc.elementById(id);
}

QList <QDomElement> NCLTextEditor::elementsByTagname(const QDomDocument &domDoc,
                                                     QString tagname)
{
  QDomNodeList elements = domDoc.elementsByTagName(tagname);
  QList <QDomElement> ret;
  for(int i = 0; i < elements.length(); i++)
  {
    ret.push_back(elements.at(i).toElement());
  }
  return ret;
}

QList <QDomElement> NCLTextEditor::elementsByTagname(QString tagname)
{
  QList <QDomElement> ret;
  QMapIterator<QString, QDomDocument> i(domDocs);
  while (i.hasNext())
  {
    i.next();
    ret.append(elementsByTagname(i.value(), tagname));
  }
  return ret;
}

QList <QDomElement> NCLTextEditor::elementsByTagname( QString tagname,
                                                      QString parentId )
{
  QDomElement current = elementById(parentId);
  QList <QDomElement> ret;

  QStack <QDomElement> stack;
  stack.push(current);

  while(stack.size())
  {
    current = stack.front();
    stack.pop_front();

    QDomElement node = current.toElement();
    if(node.tagName() == tagname)
      ret.push_back(node);

    QDomElement child = current.firstChildElement();
    while(!child.isNull())
    {
      // \todo We must not continue if the current element define a scope
      stack.push_back(child);
      child = child.nextSiblingElement();
    }
  }

  return ret;
}

/*
bool NCLTextEditor::canInsertFromMimeData(const QMimeData *source) const
{
  if(!QsciScintillaBase::canInsertFromMimeData(source))
  {
    foreach(QUrl url, source->urls())
      if (QFileInfo(url.toLocalFile()).exists())
      {
        return true;
      }
  }
}

// Create text from a MIME data object.
QByteArray NCLTextEditor::fromMimeData(const QMimeData *source, bool &rectangular) const
{
  QByteArray array = QsciScintillaBase::fromMimeData(source, rectangular);
  if(!array.size())
  {
    rectangular = false;
    QString media = "<media src=\"";
    media += source->urls().at(0).toString();
    media += "\"></media>";
    return media.toAscii();
  }
}

void NCLTextEditor::dropEvent(QDropEvent *event)
{
  foreach(QUrl url, event->mimeData()->urls())
  {
    QString filename = url.toLocalFile();
    event->acceptProposedAction();
    QString media = "<media src=\"";
    media += filename;
    media += "\">\n</media>";
    insert(media);
  }
}*/

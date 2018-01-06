/*
 * Copyright (C) 2011-2018 TeleMidia/PUC-Rio.
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
#include <QFileInfo>
#include <QStack>
#include <QUrl>
#include <QVariant>
#include <Qsci/qscistyledtext.h>

#include <NCLTextualViewPlugin_global.h>

#include <util/Preferences.h>
using namespace cpr::core;

NCLTextEditor::NCLTextEditor (QWidget *parent) : QsciScintilla (parent)
{
  Preference *tabWidth
      = Preferences::instance ()->getValue ("cpr.textual.tabWidth");
  _nclLexer = nullptr;
  _apis = nullptr;
  _textWithoutUserInter = "";
  _focusInIgnoringCurrentText = false;
  _emitFocusOut = true;

  initParameters ();
  setAcceptDrops (true);

  setTabWidth (tabWidth->value ().toInt ());
  connect (this, SIGNAL (cursorPositionChanged (int, int)), this,
           SLOT (handleCursorPositionChanged (int, int)));
}

NCLTextEditor::~NCLTextEditor () {}

void
NCLTextEditor::initParameters ()
{
  Preferences *prefs = Preferences::instance ();

  prefs->restore ();
  Preference *bgCaretLine = prefs->getValue ("cpr.textual.bgCaretLine");
  Preference *caretLineVisible
      = prefs->getValue ("cpr.textual.caretLineVisible");
  Preference *bgMarginColor = prefs->getValue ("cpr.textual.bgMarginColor");
  Preference *marginForeColor
      = prefs->getValue ("cpr.textual.marginForeColor");
  Preference *marginBackColor
      = prefs->getValue ("cpr.textual.marginBackColor");
  Preference *prefFontSize = prefs->getValue ("cpr.textual.fontSize");
  Preference *whitespaceVisibility
      = prefs->getValue ("cpr.textual.whitespaceVisibility");
  Preference *edgeMode = prefs->getValue ("cpr.textual.edgeMode");
  Preference *edgeColumn = prefs->getValue ("cpr.textual.edgeColumn");

  _tabBehavior = TAB_BEHAVIOR_DEFAULT;

  setAutoIndent (true);
  setFolding (QsciScintilla::CircledTreeFoldStyle);
  setFoldMarginColors (marginForeColor->value ().toString (),
                       marginBackColor->value ().toString ());

  setMarginLineNumbers (1, true);

  setMarginWidth (1, 25);
  setMarginsBackgroundColor (QColor (bgMarginColor->value ().toString ()));

  setCaretWidth (20);
  setCaretLineBackgroundColor (QColor ("#e6fff0"));
  if (caretLineVisible->value ().toInt () == 1)
    setCaretLineVisible (true);

  // whitespacevisibility switch
  if (whitespaceVisibility->value ().toInt () == 0)
    setWhitespaceVisibility (WhitespaceVisibility::WsInvisible);
  else if (whitespaceVisibility->value ().toInt () == 1)
    setWhitespaceVisibility (WhitespaceVisibility::WsVisible);
  else if (whitespaceVisibility->value ().toInt () == 2)
    setWhitespaceVisibility (WhitespaceVisibility::WsVisibleAfterIndent);
  else if (whitespaceVisibility->value ().toInt () == 3)
    setWhitespaceVisibility (WhitespaceVisibility::WsVisibleOnlyInIndent);

  ensureLineVisible (1);
  setUtf8 (true);
  setWrapMode (WrapWord);
  setWrapVisualFlags (WrapFlagByText);

  /* AutoCompletetion parameters */
  setAutoCompletionThreshold (-1);
  setAutoCompletionFillupsEnabled (false);
  // setAutoIndent(true);
  // setIndentationWidth(0);
  // setIndentationGuides(true);
  // setTabIndents(true);
  setTabWidth (4);
  // setIndentationsUseTabs(true);

  setAnnotationDisplay (QsciScintilla::AnnotationBoxed);
  // adding text partitions
  /*MyLexer *mylexer = new MyLexer(this);

    QRegExp attrValueRegex ("\".*\"");
    QsciStyle attrValueStyle ( 4, "ATTRIBUTE_VALUE", QColor(0x0, 0x0, 0xFF),
                               mylexer->defaultPaper(1),
                               mylexer->defaultFont(1));
    mylexer->addTextPartition(4, attrValueRegex, attrValueStyle);

    QRegExp startTagRegex ("<.[^><.]+>");
    QsciStyle startTagStyle (10, "START_TAG",  QColor(0x0, 0x0, 0xFF),
                             mylexer->defaultPaper(1),
    mylexer->defaultFont(1));
    mylexer->addTextPartition(10, startTagRegex, startTagStyle);
    */

  _nclLexer = new QsciLexerNCL (0);
  QFont font;
  font.setFamily (QString::fromUtf8 (PREF_FONT_FAMILY));
  font.setPointSize (prefFontSize->value ().toInt ());
  _nclLexer->setDefaultFont (font);

  setLexer (_nclLexer);

  // APIS
  _apis = new QsciNCLAPIs (lexer ());

  setCaretLineBackgroundColor (QColor (bgCaretLine->value ().toString ()));

  // connect(this, SIGNAL(marginClicked(int,int,Qt::KeyboardModifiers)), this,
  // SLOT(MarkLine(int,int,Qt::KeyboardModifiers)));

  _errorMarker = markerDefine (QPixmap (":/images/error-icon-16.png"), -1);
  _errorIndicator = indicatorDefine (SquiggleIndicator, 1);
  setIndicatorForegroundColor (QColor ("#FF0000"), _errorIndicator);
  _fillingAttributeIndicator = indicatorDefine (RoundBoxIndicator, 2);

  // setWhitespaceVisibility(QsciScintilla::WsVisible);
  if (edgeMode->value ().toInt () == 1)
    setEdgeMode (QsciScintilla::EdgeLine);
  setEdgeColumn (edgeColumn->value ().toInt ());

  prefs->save ();
}

void
NCLTextEditor::Increasefont ()
{
  zoomIn ();
}

void
NCLTextEditor::Decreasefont ()
{
  zoomOut ();
}

void
NCLTextEditor::clearErrorIndicators ()
{
  // clear markers
  markerDeleteAll (_errorMarker);

  // clearIndicators
  int nr_lines = lines ();
  QString tmp2 = this->text (nr_lines);
  clearIndicatorRange (0, 0, nr_lines, tmp2.size (), _errorIndicator);
  clearAnnotations ();
}

void
NCLTextEditor::markError (const QString &description, const QString &file,
                          int line, int column, int severity)
{
  Q_UNUSED (file)
  Q_UNUSED (column)
  Q_UNUSED (severity)

  // TODO: Show Error Messages
  QString tmp = this->text (line);

  // ADD MARKER
  if (text () != "")
    markerAdd (line, _errorMarker);

  // ADD INDICATOR
  int indentation = 0;
  while (tmp[indentation].isSpace ())
    indentation++;

  fillIndicatorRange (line, indentation, line, tmp.size () - 1,
                      _errorIndicator);

  annotate (line, QsciStyledText (description, 0));
}

void
NCLTextEditor::wheelEvent (QWheelEvent *event)
{
  if (event->modifiers () == Qt::ControlModifier)
  {
    if (event->delta () > 0)
      zoomIn ();
    else
      zoomOut ();
  }

  QsciScintilla::wheelEvent (event);
}

// TODO: Maybe move the mousePressEvent function content, and part of the
// keyPressEvent, to a function connected to selectionChanged signal.
void
NCLTextEditor::mousePressEvent (QMouseEvent *event)
{
  int selBegin = SendScintilla (SCI_GETSELECTIONSTART);
  int selEnd = SendScintilla (SCI_GETSELECTIONEND);

  int clearBegin = SendScintilla (
      SCI_POSITIONFROMLINE, SendScintilla (SCI_LINEFROMPOSITION, selBegin));
  int clearEnd = SendScintilla (SCI_GETLINEENDPOSITION,
                                SendScintilla (SCI_LINEFROMPOSITION, selEnd));

  SendScintilla (SCI_INDICATORCLEARRANGE, clearBegin, clearEnd);

  QsciScintilla::mousePressEvent (event);
}

//\fixme I DONT KNOW WHY (or WHERE), BUT THE UNDO IS NOT WORKING EVERY TIME!
void
NCLTextEditor::keyPressEvent (QKeyEvent *event)
{
  // Ctrl + Space == Autocomplete
  if ((event->modifiers () & Qt::ControlModifier)
      && (event->key () == Qt::Key_Space))
  {
    event->accept ();
    autoCompleteFromAPIs ();
    return;
  }

  // Ctrl + Shift + F == format Text
  if ((event->modifiers () & Qt::ControlModifier)
      && (event->modifiers () & Qt::ShiftModifier)
      && (event->key () == Qt::Key_F))
  {
    event->accept ();
    formatText ();
    return;
  }

  // If I receive a Qt::Key_Backtab
  // change event to Shift+Tab
  if (event->key () == Qt::Key_Backtab)
  {
    event->accept ();
    event = new QKeyEvent (event->type (), Qt::Key_Tab, Qt::ShiftModifier);
  }

  // zoomin event.
  if (event->key () == Qt::Key_ZoomIn)
  {
    event->accept ();
    Increasefont ();
    return;
  }

  // zoomout event.
  if (event->key () == Qt::Key_ZoomOut)
  {
    event->accept ();
    Decreasefont ();
    return;
  }

  int pos = SendScintilla (SCI_GETCURRENTPOS);
  int style = SendScintilla (SCI_GETSTYLEAT, pos);
  int size = SendScintilla (SCI_GETTEXTLENGTH);

  // Test if pos-1 is also inside the attribute, otherwise it will
  // treat a text inside the end of previous Quote and the start of
  // the current one as an attribute
  if (style == QsciLexerNCL::HTMLDoubleQuotedString && pos - 1 >= 0)
  {
    if (event->key () == Qt::Key_Tab)
    {
      // SHIFT+TAB -> GO TO PREVIOUS ATRIBUTE
      if (event->modifiers () & Qt::ShiftModifier)
      {
        userFillingPreviousAttribute (pos);
      }
      // JUST TAB -> GO TO NEXT ATTRIBUTE
      else if (event->modifiers () == Qt::NoModifier)
      {
        userFillingNextAttribute (pos);
      }
    }
    else
    {
      QsciScintilla::keyPressEvent (event);
    }
  }
  else
  {
    QsciScintilla::keyPressEvent (event);

    pos = SendScintilla (SCI_GETCURRENTPOS);
    style = SendScintilla (SCI_GETSTYLEAT, pos);

#ifdef AUTOREMOVE_ENDTAG
    char curChar = SendScintilla (SCI_GETCHARAT, pos);

    if (style == QsciLexerNCL::Tag && event->key () == Qt::Key_Slash
        && curChar == Qt::Key_Greater)
    {
      qCWarning (CPR_PLUGIN_TEXTUAL) << curChar
                                     << "Now, I need to close the tag :)";
      pos++;

      int begin_del = pos;
      style = SendScintilla (SCI_GETSTYLEAT, pos);
      while (pos < size - 1 && style != QsciLexerNCL::Tag)
      {
        pos++;
        style = SendScintilla (SCI_GETSTYLEAT, pos);
      }

      if (pos < size)
      {
        curChar = SendScintilla (QsciScintilla::SCI_GETCHARAT, pos);
        char nextChar = SendScintilla (QsciScintilla::SCI_GETCHARAT, pos + 1);
        if (curChar == '<' && nextChar == '/')
        {
          int line, index;
          lineIndexFromPosition (pos + 2, &line, &index);

          // Check if the close tag is the same of the open tag.
          if (_apis->currentTagname (begin_del)
              == wordAtLineIndex (line, index))
          {
            int end_del = pos;
            style = SendScintilla (SCI_GETSTYLEAT, pos);
            while (end_del < size - 1 && style == QsciLexerNCL::Tag)
            {
              end_del++;
              style = SendScintilla (QsciScintilla::SCI_GETSTYLEAT, end_del);
            }

            if (end_del < size)
            {
              qCWarning (CPR_PLUGIN_TEXTUAL)
                  << "Remove from" << curChar
                  << (char)SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                          end_del);

              SendScintilla (QsciScintilla::SCI_DELETERANGE, begin_del,
                             end_del - begin_del);
            }
          }
        }
      }
    }
#endif
  }
}

void
NCLTextEditor::keyReleaseEvent (QKeyEvent *event)
{
  QsciScintilla::keyReleaseEvent (event);
}

void
NCLTextEditor::AutoCompleteCompleted ()
{
  // qCDebug (CPR_PLUGIN_TEXTUAL) << "NCLTextEditor::AutoCompleteCompleted()";
}

void
NCLTextEditor::markLine (int margin, int line, Qt::KeyboardModifiers state)
{
  (void)margin;
  (void)line;
  (void)state;
}

void
NCLTextEditor::userFillingNextAttribute (int pos)
{
  int style, i = pos;
  int size = SendScintilla (SCI_GETTEXTLENGTH);

  style = SendScintilla (SCI_GETSTYLEAT, i);
  if (style == QsciLexerHTML::HTMLDoubleQuotedString)
  {
    // I am in a attribute, so lets get out of it first
    while (i < size)
    {
      style = SendScintilla (SCI_GETSTYLEAT, i);
      if (style != QsciLexerHTML::HTMLDoubleQuotedString)
        break;
      i++;
    }
  }

  while (i < size)
  {
    style = SendScintilla (SCI_GETSTYLEAT, i);
    if (style == QsciLexerNCL::HTMLDoubleQuotedString)
      break;
    i++;
  }

  i++;

  int newline, newindex;
  lineIndexFromPosition (i, &newline, &newindex);
  setCursorPosition (newline, newindex);
  //  setSelection(newline, begin, newline, end);
}

void
NCLTextEditor::userFillingPreviousAttribute (int pos)
{
  int style;
  int i = pos;

  style = SendScintilla (SCI_GETSTYLEAT, i);
  if (style == QsciLexerHTML::HTMLDoubleQuotedString)
  {
    // I am in a attribute, so lets get out of it first
    while (i >= 0)
    {
      style = SendScintilla (SCI_GETSTYLEAT, i);
      if (style != QsciLexerHTML::HTMLDoubleQuotedString)
        break;
      i--;
    }
  }

  while (i >= 0)
  {
    style = SendScintilla (SCI_GETSTYLEAT, i);
    if (style == QsciLexerHTML::HTMLDoubleQuotedString)
      break;
    i--;
  }

  int newline, newindex;
  lineIndexFromPosition (i, &newline, &newindex);
  setCursorPosition (newline, newindex);
  //  setSelection(newline, begin, newline, end);
}

void
NCLTextEditor::updateVisualFillingAttributeField (int line, int index,
                                                  int &begin, int &end)
{
  Q_UNUSED (index);

  int style;
  int pos = SendScintilla (SCI_GETCURRENTPOS);
  int size = SendScintilla (SCI_GETTEXTLENGTH);
  begin = end = pos;

  while (begin >= 0)
  {
    style = SendScintilla (SCI_GETSTYLEAT, begin);
    if (style != QsciLexerHTML::HTMLDoubleQuotedString)
      break;

    begin--;
  }
  begin += 2;

  while (end < size)
  {
    style = SendScintilla (SCI_GETSTYLEAT, end);
    if (style != QsciLexerHTML::HTMLDoubleQuotedString)
      break;
    end++;
  }
  end -= 1;

  int indexbegin, indexend;
  lineIndexFromPosition (begin, &line, &indexbegin);
  lineIndexFromPosition (end, &line, &indexend);

  fillIndicatorRange (line, indexbegin, line, indexend,
                      _fillingAttributeIndicator);
}

void
NCLTextEditor::setTabBehavior (TAB_BEHAVIOR tabBehavior)
{
  this->_tabBehavior = tabBehavior;
}

void
NCLTextEditor::formatText ()
{
  qCDebug (CPR_PLUGIN_TEXTUAL) << "NCLTextEditor::formatText()";
}

void
NCLTextEditor::keepFocused ()
{
  _focusInIgnoringCurrentText = true;
  this->setFocus ();
  this->SendScintilla (QsciScintilla::SCI_SETFOCUS, true);
}

void
NCLTextEditor::focusOutEvent (QFocusEvent *event)
{
  clearFillingAttributeIndicator ();

  if (_emitFocusOut)
    emit focusLosted (event);
}

void
NCLTextEditor::clearFillingAttributeIndicator ()
{
  int nr_lines = lines ();
  int size = this->text (nr_lines).size ();

  clearIndicatorRange (0, 0, nr_lines, size, _fillingAttributeIndicator);
}

void
NCLTextEditor::focusInEvent (QFocusEvent *e)
{
  Q_UNUSED (e)
#ifndef NCLEDITOR_STANDALONE
  if (!_focusInIgnoringCurrentText)
    _textWithoutUserInter = text ();

  _focusInIgnoringCurrentText = false;
  this->SendScintilla (QsciScintilla::SCI_SETFOCUS, true);
#endif
}

void
NCLTextEditor::handleCursorPositionChanged (int line, int index)
{
  clearFillingAttributeIndicator (); // TODO: Improve performance

  int begin, end;
  int pos = SendScintilla (SCI_GETCURRENTPOS);
  int style = SendScintilla (SCI_GETSTYLEAT, pos);

  if (style == QsciLexerNCL::HTMLDoubleQuotedString)
  {
    updateVisualFillingAttributeField (line, index, begin, end);
  }
}

QString
NCLTextEditor::textWithoutUserInteraction ()
{
  return _textWithoutUserInter;
}

void
NCLTextEditor::setTextWithoutUserInteraction (const QString &text)
{
  _textWithoutUserInter = text;
}

void
NCLTextEditor::setDocumentUrl (const QString &docURL)
{
  this->_docURL = docURL;
}

QString
NCLTextEditor::getDocumentUrl ()
{
  return this->_docURL;
}

bool
NCLTextEditor::parseDocument (bool recursive)
{
  _domDocs.clear ();
  _domDoc.clear ();
  //  docs.clear();
  if (!_domDoc.setContent (this->text ()))
  {
    // could not parse the document
    return false;
  }
  //  docs.insert("", doc);

  if (recursive)
  {
    _domDocs.insert (_docURL, _domDoc);
    return parseImportedDocuments (_docURL, _domDoc, true);
  }
  else
    return true;
}

bool
NCLTextEditor::parseImportedDocuments (const QString &currentFileURI,
                                       const QDomDocument &doc,
                                       bool /*recursive*/)
{
  qCDebug (CPR_PLUGIN_TEXTUAL) << "parseImportedDocument( " << currentFileURI;
  QStack<QDomElement> stack;
  QDomElement current = doc.firstChildElement ();
  bool ret = 1;
  stack.push (current);

  while (stack.size ())
  {
    current = stack.top ();
    stack.pop ();

    if (current.tagName () == "importBase"
        && current.hasAttribute ("documentURI")
        && current.hasAttribute ("alias"))
    {
      QString importedDocumentUri = current.attribute ("documentURI");
      QString fullpath = "";

      QFileInfo importedFI (importedDocumentUri), currentFI (currentFileURI);
      if (importedFI.isAbsolute ())
      {
        fullpath = importedDocumentUri;
      }
      else
      {
        importedFI.setFile (currentFI.absolutePath () + "/"
                            + importedDocumentUri);

        if (importedFI.exists ())
        {
          qCDebug (CPR_PLUGIN_TEXTUAL) << importedFI.absoluteFilePath ();
          fullpath = importedFI.absoluteFilePath ();
        }
      }

      if (fullpath != "")
      {
        qCDebug (CPR_PLUGIN_TEXTUAL) << "I will include the file " << fullpath;
        QFile file (fullpath);
        if (file.open (QIODevice::ReadOnly))
        {
          QDomDocument currentDomDoc;
          if (currentDomDoc.setContent (&file))
          {
            updateElementsIDWithAlias (currentDomDoc,
                                       current.attribute ("alias"));
            if (!parseImportedDocuments (fullpath, currentDomDoc, true))
            {
              ret = false;
              qCDebug (CPR_PLUGIN_TEXTUAL) << "Could not import " << fullpath;
            }
            _domDocs.insert (fullpath, currentDomDoc);
          }
        }
      }
    }

    QDomElement child = current.firstChildElement ();
    while (!child.isNull ())
    {
      stack.push_back (child);
      child = child.nextSiblingElement ();
    }
  }
  return ret;
}

void
NCLTextEditor::updateElementsIDWithAlias (const QDomDocument &doc,
                                          const QString &alias)
{
  QStack<QDomElement> stack;
  QDomElement current = doc.firstChildElement ();
  stack.push (current);
  while (stack.size ())
  {
    current = stack.top ();
    stack.pop ();
    if (current.hasAttribute ("id"))
    {
      current.setAttribute ("id", alias + "#" + current.attribute ("id"));
    }

    QDomElement child = current.firstChildElement ();
    while (!child.isNull ())
    {
      stack.push_back (child);
      child = child.nextSiblingElement ();
    }
  }
}

QDomElement
NCLTextEditor::elementById (const QDomDocument &domDoc, const QString &id)
{
  QStack<QDomNode> stack;
  stack.push_front (domDoc.firstChildElement ());
  while (stack.size ())
  {
    QDomNode current = stack.front ();
    stack.pop_front ();
    if (current.isElement ())
    {
      if (current.toElement ().attribute ("id") == id)
        return current.toElement ();
    }

    QDomNode child = current.firstChild ();
    while (!child.isNull ())
    {
      stack.push_back (child);
      child = child.nextSibling ();
    }
  }

  // The function QDomDocument::elementById will always returns a nullptr doc.
  // That is why we have to reimplement this function.

  return QDomElement ();
}

QDomElement
NCLTextEditor::elementById (const QString &id, bool recursive)
{
  if (!recursive)
    return elementById (_domDoc, id);

  QMapIterator<QString, QDomDocument> i (_domDocs);
  while (i.hasNext ())
  {
    i.next ();
    QDomDocument currentDomDoc = i.value ();
    QDomElement element = elementById (currentDomDoc, id);
    if (!element.isNull ())
      return element;
  }

  return _domDoc.elementById (id);
}

QList<QDomElement>
NCLTextEditor::elementsByTagname (const QDomDocument &domDoc,
                                  const QString &tagname)
{
  QDomNodeList elements = domDoc.elementsByTagName (tagname);
  QList<QDomElement> ret;
  for (int i = 0; i < elements.length (); i++)
  {
    ret.push_back (elements.at (i).toElement ());
  }
  return ret;
}

QList<QDomElement>
NCLTextEditor::elementsByTagname (const QString &tagname)
{
  QList<QDomElement> ret;
  QMapIterator<QString, QDomDocument> i (_domDocs);

  while (i.hasNext ())
  {
    i.next ();
    ret.append (elementsByTagname (i.value (), tagname));
  }

  return ret;
}

QList<QDomElement>
NCLTextEditor::elementsByTagname (const QString &tagname,
                                  const QString &parentId)
{
  QDomElement current = elementById (parentId);
  QList<QDomElement> ret;

  QStack<QDomElement> stack;
  stack.push (current);

  bool first = true;
  while (stack.size ())
  {
    current = stack.front ();
    stack.pop_front ();

    QDomElement node = current.toElement ();
    if (node.tagName () == tagname)
      ret.push_back (node);

    if (first || !NCLStructure::instance ()->defineScope (node.tagName ()))
    {
      first = false;
      QDomElement child = current.firstChildElement ();

      while (!child.isNull ())
      {
        // \todo We must not continue if the current element define a scope
        stack.push_back (child);
        child = child.nextSiblingElement ();
      }
    }
  }

  return ret;
}

void
NCLTextEditor::setEmitFocusOut (bool v)
{
  this->_emitFocusOut = v;
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
QByteArray NCLTextEditor::fromMimeData(const QMimeData *source, bool
&rectangular) const
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

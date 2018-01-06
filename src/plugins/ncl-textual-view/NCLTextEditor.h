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
#ifndef NCLTEXTEDITOR_H
#define NCLTEXTEDITOR_H

#include <QColor>
#include <QDomDocument>
#include <QMessageBox>
#include <QShortcut>
#include <QWheelEvent>
#include <Qsci/qsciscintilla.h>

#include "MyLexer.h"

#include "QsciLexerNCL.h"
#include "QsciNCLAPIs.h"

#define PREF_FONT_FAMILY "Courier"
#define PREF_FONT_SIZE 10

#define LIGTHSTYLE 1
#define AUTOREMOVE_ENDTAG 1

#ifdef LIGTHSTYLE
/* Light Style */
#define PREF_CARET_LINE_BG_COLOR QColor ("#B9D3EE")
#define PREF_FOLD_MARGIN_FORE_COLOR QColor ("#B9D3EE")
#define PREF_FOLD_MARGIN_BACK_COLOR QColor ("#FFFFFF")
#define MARGINS_BACKGROUND_COLOR QColor ("#ffffff")
#else
/* Dark Style */
#define PREF_CARET_LINE_BG_COLOR QColor ("#B9D3EE")
#define PREF_FOLD_MARGIN_FORE_COLOR QColor ("#FFFFFF")
#define PREF_FOLD_MARGIN_BACK_COLOR QColor ("#AAAAAA")
#define MARGINS_BACKGROUND_COLOR QColor ("#BBBBBB")
#endif

//! The NCL Text Editor
/*!
 * This is the main class of the NCL Editor program. It inherents from
 * QScintilla.
 *
 * @author Roberto Azevedo
 * @date 24 May 2011
 */
class NCLTextEditor : public QsciScintilla
{
  Q_OBJECT

public:
  typedef enum {
    TAB_BEHAVIOR_DEFAULT = 1,
    TAB_BEHAVIOR_NEXT_ATTR_AFTER_AUTOCOMPLETE,
    TAB_BEHAVIOR_NEXT_ATTR
  } TAB_BEHAVIOR;

  explicit NCLTextEditor (QWidget *parent = 0);
  virtual ~NCLTextEditor ();

  void setTabBehavior (TAB_BEHAVIOR _tabBehavior);
  void userFillingNextAttribute (int pos);
  void userFillingPreviousAttribute (int pos);
  void updateVisualFillingAttributeField (int line, int index, int &begin,
                                          int &end);

  void setEmitFocusOut (bool v);
  void keepFocused ();
  QString textWithoutUserInteraction ();
  void setTextWithoutUserInteraction (const QString &text);

  void setDocumentUrl (const QString &_docURL);
  QString getDocumentUrl ();
  bool parseDocument (bool recursive = true);
  void updateElementsIDWithAlias (const QDomDocument &doc,
                                  const QString &alias);

  QDomElement elementById (const QDomDocument &_domDoc, const QString &id);
  QDomElement elementById (const QString &id, bool recursive = true);
  QList<QDomElement> elementsByTagname (const QString &tagname);
  QList<QDomElement> elementsByTagname (const QDomDocument &_domDoc,
                                        const QString &tagname);
  QList<QDomElement> elementsByTagname (const QString &tagname,
                                        const QString &parentId);

public slots:
  void Increasefont ();
  void Decreasefont ();
  void clearErrorIndicators ();
  void clearFillingAttributeIndicator ();
  void markError (const QString &description, const QString &file, int line,
                  int column = 0, int severity = 0);

  void markLine (int, int, Qt::KeyboardModifiers);

  void formatText ();

  void handleCursorPositionChanged (int line, int pos);

signals:
  void focusLosted (QFocusEvent *event);

protected:
  void AutoCompleteCompleted ();
  void focusInEvent (QFocusEvent *e);
  void focusOutEvent (QFocusEvent *e);
  /*bool canInsertFromMimeData(const QMimeData *source) const;
  QByteArray fromMimeData(const QMimeData *source, bool &rectangular) const;
  void dragEnterEvent(QDragEnterEvent *event);
  void dropEvent(QDropEvent *event);*/

private:
  QsciLexerNCL *_nclLexer;
  QsciNCLAPIs *_apis;
  QDomDocument _domDoc;
  QString _docURL;
  QMap<QString, QDomDocument> _domDocs;

  int _errorIndicator;
  int _errorMarker;
  int _fillingAttributeIndicator;

  TAB_BEHAVIOR _tabBehavior;
  bool _focusInIgnoringCurrentText;
  bool _emitFocusOut;
  QString _textWithoutUserInter;

  void initParameters ();

  /* events */
  void wheelEvent (QWheelEvent *event);
  void keyPressEvent (QKeyEvent *event);
  void keyReleaseEvent (QKeyEvent *event);
  void mousePressEvent (QMouseEvent *e);

  bool parseImportedDocuments (const QString &currentFileURI,
                               const QDomDocument &doc, bool recursive = true);
};

#endif // NCLTEXTEDITOR_H

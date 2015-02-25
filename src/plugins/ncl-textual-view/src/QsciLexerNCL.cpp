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
#include "QsciLexerNCL.h"

#include <QColor>
#include <QFont>
#include <QSettings>

#include <QDebug>

// The ctor.
QsciLexerNCL::QsciLexerNCL(QObject *parent)
  : QsciLexerHTML(parent),
    _scripts(true)
{
}

// The dtor.
QsciLexerNCL::~QsciLexerNCL()
{
}

// Returns the language name.
const char *QsciLexerNCL::language() const
{
  return "NCL";
}


// Returns the lexer name.
const char *QsciLexerNCL::lexer() const
{
  return "ncl";
}


// Returns the foreground colour of the text for a style.
QColor QsciLexerNCL::defaultColor(int style) const
{
  //    return QColor(0xff, 0xff, 0xff);

  switch (style)
  {
    case Default:
      return QColor(0x00, 0x00, 0x00);

    case Tag:
    case UnknownTag:
    case XMLTagEnd:
    case SGMLDefault:
    case SGMLCommand:
      return QColor(0x00, 0x00, 0x80);

    case Attribute:
    case UnknownAttribute:
      return QColor(0x00,  0x00, 0x80);

    case HTMLNumber:
      return QColor(0x00, 0x7f, 0x7f);

    case HTMLDoubleQuotedString:
    case HTMLSingleQuotedString:
      return QColor(0x99, 0x33, 0x00);

    case OtherInTag:
    case Entity:
    case XMLStart:
    case XMLEnd:
      return QColor(0x80, 0x00, 0x80);

    case HTMLComment:
    case SGMLComment:
      return QColor(0x00, 0x7f, 0x7f);

    case CDATA:
    case PHPStart:
    case SGMLDoubleQuotedString:
    case SGMLError:
      return QColor(0x80,0x00,0x00);

    case HTMLValue:
      return QColor(0x00,0xFF,0x00);

    case SGMLParameter:
      return QColor(0x00,0x66,0x00);

    case SGMLSingleQuotedString:
      return QColor(0x99,0x33,0x00);

    case SGMLSpecial:
      return QColor(0x33,0x66,0xff);

    case SGMLEntity:
      return QColor(0x00,0x00,0x33);

    case SGMLBlockDefault:
      return QColor(0x00,0x00,0x66);
  }

  return QsciLexerHTML::defaultColor(style);
}


// Returns the end-of-line fill for a style.
bool QsciLexerNCL::defaultEolFill(int style) const
{
  if (style == CDATA)
    return true;

  return QsciLexerHTML::defaultEolFill(style);
}


// Returns the font of the text for a style.
QFont QsciLexerNCL::defaultFont(int style) const
{
  QFont f;
  switch (style)
  {
    case Default:
    case Entity:
    case CDATA:
#if defined(Q_OS_WIN)
      f = QFont("Times New Roman", 10);
#else
      f = QFont("Monospace", 10);
#endif
      break;

    case XMLStart:
    case XMLEnd:
    case SGMLCommand:
      f = QsciLexerHTML::defaultFont(style);
      f.setBold(true);
      break;

    default:
      f = QFont("Monospace", 10);
      // f = QsciLexerHTML::defaultFont(style);
  }

  // f.setPointSize(8);

  return f;
}


// Returns the set of keywords.
const char *QsciLexerNCL::keywords(int set) const
{
  if (set == 6)
    return QsciLexerHTML::keywords(set);

  return 0;
}


// Returns the background colour of the text for a style.
QColor QsciLexerNCL::defaultPaper(int style) const
{
  //    return QColor(0x88,0x88,0x88);

  switch (style)
  {
    case CDATA:
      return QColor(0xff,0xff,0xff);

    case SGMLDefault:
    case SGMLCommand:
    case SGMLParameter:
    case SGMLDoubleQuotedString:
    case SGMLSingleQuotedString:
    case SGMLSpecial:
    case SGMLEntity:
    case SGMLComment:
      return QColor(0xef,0xef,0xff);

    case SGMLError:
      return QColor(0xff,0x66,0x66);

    case SGMLBlockDefault:
      return QColor(0xcc,0xcc,0xe0);
  }

  return QsciLexerHTML::defaultPaper(style);
}


// Refresh all properties.
void QsciLexerNCL::refreshProperties()
{
  setScriptsProp();
  emit propertyChanged("fold.html","1");
}


// Read properties from the settings.
bool QsciLexerNCL::readProperties(QSettings &qs, const QString &prefix)
{
  int rc = QsciLexerHTML::readProperties(qs, prefix);

  _scripts = qs.value(prefix + "scriptsstyled", true).toBool();

  return rc;
}


// Write properties to the settings.
bool QsciLexerNCL::writeProperties(QSettings &qs, const QString &prefix) const
{
  int rc = QsciLexerHTML::writeProperties(qs, prefix);

  qs.setValue(prefix + "scriptsstyled", _scripts);

  return rc;
}


// Return true if scripts are styled.
bool QsciLexerNCL::scriptsStyled() const
{
  return _scripts;
}


// Set if scripts are styled.
void QsciLexerNCL::setScriptsStyled(bool styled)
{
  _scripts = styled;
  setScriptsProp();
}


// Set the "lexer.ncl.allow.scripts" property.
void QsciLexerNCL::setScriptsProp()
{
  emit propertyChanged("lexer.xml.allow.scripts",(_scripts ? "1" : "0"));
}

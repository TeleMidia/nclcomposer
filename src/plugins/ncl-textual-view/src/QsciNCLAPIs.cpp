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
#include "QsciNCLAPIs.h"

#include "NCLTextEditor.h"
#include "NCLTextualViewPlugin_global.h"

#include <util/Utilities.h>
using namespace cpr::core;

#include <QFileDialog>

QsciNCLAPIs::QsciNCLAPIs (QsciLexer *lexer) : QsciAPIs (lexer)
{
  _nclStructure = NCLStructure::getInstance ();

  add (QString ("ncl30"));
  prepare ();
}

QsciNCLAPIs::~QsciNCLAPIs () {}

void
QsciNCLAPIs::updateAutoCompletionList (const QStringList &context,
                                       QStringList &list)
{
  _suggesting = SUGGESTING_OTHER;

  int pos = lexer ()->editor ()->SendScintilla (
      QsciScintilla::SCI_GETSELECTIONSTART);

  if (isElement (pos))
  {
    _suggesting = SUGGESTING_ELEMENTS;

    for (int i = 0; i < context.count (); ++i)
    {
      QString father = getParentTagName (pos);

      if (father == "")
        father = "NULL";

      map<QString, char> *elements = _nclStructure->getChildren (father);
      map<QString, char>::iterator it;

      if (elements != nullptr)
      {
        it = elements->begin ();
        for (; it != elements->end (); ++it)
        {
          if (it->first.startsWith (context[i]))
          {
            QString str (it->first);
            list.push_back (str);
          }
        }
      }
    }
  }
  else if (isAttribute (pos))
  {
    _suggesting = SUGGESTING_ATTRIBUTES;
    QString tagname = getCurrentTagName (pos);
    QStringList current_attrs;

    getAttributesTyped (pos, current_attrs);
    // for (int i = 0; i < current_attrs.size(); i++)
    //  qCDebug (CPR_PLUGIN_TEXTUAL) << "Already typed: " <<
    //  current_attrs.at(i);

    if (tagname != "")
    {
      map<QString, bool> *attrs = _nclStructure->getAttributes (tagname);
      deque<QString> *attrs_ordered
          = _nclStructure->getAttributesOrdered (tagname);

      if (attrs != nullptr)
      {
        deque<QString>::iterator it;
        for (int i = 0; i < context.count (); ++i)
        {
          it = attrs_ordered->begin ();

          for (; it != attrs_ordered->end (); ++it)
          {
            if (it->startsWith (context[i]) && !current_attrs.contains (*it))
            {
              QString str = *it;
              list.push_back (str);
            }
          }
        }
      }
    }
  }
  else if (isAttributeValue (pos))
  {
    _suggesting = SUGGESTING_ATTRIBUTE_VALUES;
    QString tagname = getCurrentTagName (pos);
    QString attribute = getCurrentAttribute (pos);
    QString datatype
        = _nclStructure->getAttributeDatatype (tagname, attribute);
    NCLTextEditor *nclEditor = ((NCLTextEditor *)lexer ()->editor ());

    // qCDebug (CPR_PLUGIN_TEXTUAL) << tagname << ":" << attribute << " ->
    // datatype=" << datatype;

    QStringList defaultSuggestion
        = _nclStructure->getDatatypeDefaultSuggestions (datatype);

    list.append (defaultSuggestion);

    vector<AttributeReferences *> references
        = _nclStructure->getReferences (tagname, attribute);

    if (datatype == "URI")
    {
      QString filename
          = QFileDialog::getOpenFileName (nclEditor, tr ("Select file"),
                                          // nclEditor->getDocumentUrl()
                                          Utilities::getLastFileDialogPath ());

      if (!filename.isEmpty () && !filename.isNull ())
      {
        Utilities::updateLastFileDialogPath (filename);

        try
        {
          filename
              = relativePath (nclEditor->getDocumentUrl (), filename, true);
        }
        catch (...)
        {
        }
        nclEditor->removeSelectedText ();
        // Removes an empty space automatically inserted by fillingAttributes.
        if (nclEditor->SendScintilla (QsciScintilla::SCI_GETCHARAT, pos)
            == ' ')
        {
          nclEditor->SendScintilla (QsciScintilla::SCI_GOTOPOS, pos + 1);
          nclEditor->SendScintilla (QsciScintilla::SCI_DELETEBACK);
        }

        nclEditor->insert (filename);
      }
    }

    if (references.size ()) // the attribute should be a reference to other
    // attribute
    {
      //      NCLTextEditor *nclEditor = ((NCLTextEditor *)lexer()->editor());
      if (nclEditor->parseDocument ()) // parse our current document (and the
      // included ones).
      {
        for (unsigned int i = 0; i < references.size (); i++)
        {
          // qCDebug (CPR_PLUGIN_TEXTUAL) << "Should refer to " <<
          // references[i]->getRefElement()
          //         << "." << references[i]->getRefAttribute()
          //         << "in the scope: " << references[i]->getScope();

          QList<QDomElement> elements;

          // If we have an ANY_SCOPE, it doesn't need any special treatment.
          if (references[i]->getScope () == AttributeReferences::ANY_SCOPE)
          {
            elements = nclEditor->elementsByTagname (
                references[i]->getRefElement ());
          }
          else if (references[i]->getScope ()
                   == AttributeReferences::SAME_SCOPE)
          {
            int parent_offset = getParentOffset (pos);
            while (parent_offset >= 0
                   && !_nclStructure->defineScope (
                          getCurrentTagName (parent_offset)))
            {
              parent_offset = getParentOffset (parent_offset);
            }

            QString idValue
                = getAttributeValueFromCurrentElement (parent_offset, "id");

            // \todo suggest elements child of an element with no id.
            elements = nclEditor->elementsByTagname (
                references[i]->getRefElement (), idValue);
          }
          else if (references[i]->getScope ()
                   == AttributeReferences::USERDEFINED_SCOPE)
          {
            QString attr;
            QString userDefinedScope = references[i]->getUserDefinedScope ();

            // qCDebug (CPR_PLUGIN_TEXTUAL) << "User defined scope" <<
            // userDefinedScope;
            // \todo The user defined must be recursive, i.e., the user could
            // define $PARENT.$PARENT.$PARENT...
            if (userDefinedScope.startsWith ("$THIS"))
            {
              attr = userDefinedScope.mid (6);
              // qCDebug (CPR_PLUGIN_TEXTUAL) << "$THIS" << attr;
              QString idValue
                  = getAttributeValueFromCurrentElement (pos, attr);
              elements = nclEditor->elementsByTagname (
                  references[i]->getRefElement (), idValue);
            }
            else if (userDefinedScope.startsWith ("$PARENT"))
            {
              attr = userDefinedScope.mid (8);
              // qCDebug (CPR_PLUGIN_TEXTUAL) << "$PARENT" << attr;
              int parent_offset = getParentOffset (pos);
              // qCDebug (CPR_PLUGIN_TEXTUAL) << "parent_offset" <<
              // parent_offset;
              QString idValue
                  = getAttributeValueFromCurrentElement (parent_offset, attr);

              // qCDebug (CPR_PLUGIN_TEXTUAL) << "idValue = " << idValue;
              elements = nclEditor->elementsByTagname (
                  references[i]->getRefElement (), idValue);
            }
            else if (userDefinedScope.startsWith ("$GRANDPARENT"))
            {
              attr = userDefinedScope.mid (13);
              // qCDebug (CPR_PLUGIN_TEXTUAL) << "$GRANDPARENT" << attr;
              int grandparent_offset = getParentOffset (getParentOffset (pos));
              QString idValue = getAttributeValueFromCurrentElement (
                  grandparent_offset, attr);
              elements = nclEditor->elementsByTagname (
                  references[i]->getRefElement (), idValue);
            }
          }

          for (int j = 0; j < elements.length (); j++)
          {
            QDomElement node = elements.at (j).toElement ();

            QString attributeValue
                = node.attribute (references[i]->getRefAttribute ());

            // qCDebug (CPR_PLUGIN_TEXTUAL) << context.count();
            for (int k = 0; k < context.count (); ++k)
            {
              if (attributeValue.startsWith (context[k]))
                list.append (attributeValue);
            }
          }
        }
      }
    }
    else
    {
      qCDebug (CPR_PLUGIN_TEXTUAL) << "Could not parse the document";
    }
  }
  list.removeDuplicates ();
}

// TODO: END ELEMENT, ATTRIBUTE VALUE
void
QsciNCLAPIs::autoCompletionSelected (const QString &selection)
{
  int start = 0, line, pos;
  QString outputStr ("");
  QString strline;
  bool fixidentation = false;
  bool hasAttributes = false;

  // PREPARING TO INSERT TEXT
  lexer ()->editor ()->beginUndoAction ();
  // cancelling autocomplete (avoid scintilla insert the selection)
  lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_AUTOCCANCEL);
  lexer ()->editor ()->getCursorPosition (&line, &pos);
  // delete original word from text
  start = pos - 1;
  strline = lexer ()->editor ()->text (line);

  // if the user already put a word, delete this word
  // (autocomplete will put it entirely)
  if (start >= 0 && strline.at (start).isLetter ())
  {
    lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_DELWORDLEFT);

    // update the start position and the content line
    lexer ()->editor ()->getCursorPosition (&line, &pos);
    strline = lexer ()->editor ()->text (line);
    start = pos - 1;
  }

  if (_suggesting == SUGGESTING_ELEMENTS)
  {
    QString attributes = getRequiredAttributesAsStr (selection);

    if (attributes != "")
      hasAttributes = true;

    if (start < 0 || strline.at (start) != '<')
      outputStr += "<";
    outputStr += selection + " " + attributes;

    map<QString, char> *children = _nclStructure->getChildren (selection);
    if (children != nullptr && children->size ())
    {
      outputStr += ">";
      outputStr += "\n";
      outputStr += "</";
      outputStr += selection;
      outputStr += ">";
      fixidentation = true;
    }
    else
    {
      outputStr += "/>";
    }
  }
  else if (_suggesting == SUGGESTING_ATTRIBUTES)
  {
    outputStr = selection + "=\"\"";

    // if the attribute is just after another word, put a space between then
    if (start >= 0)
    {
      QChar ch = strline.at (start);
      if (!ch.isSpace ())
        outputStr.prepend (' ');
    }
    // if the attribute is just before another word ut a space in the end of
    // the
    // inserted text
    if (start + 1 < strline.size ())
    {
      QChar ch = strline.at (start + 1);
      if (!ch.isSpace () && ch != '>')
        outputStr.append (' ');
    }
  }
  else if (_suggesting == SUGGESTING_ATTRIBUTE_VALUES)
  {
    int ps
        = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCURRENTPOS)
          - 1;
    int pe = lexer ()->editor ()->SendScintilla (
        QsciScintilla::SCI_GETCURRENTPOS);
    bool quote1_found = false, quote2_found = false;
    char ch;

    int startline = lexer ()->editor ()->SendScintilla (
        QsciScintilla::SCI_GETLINESELSTARTPOSITION, ps);
    int endline = lexer ()->editor ()->SendScintilla (
        QsciScintilla::SCI_GETLINEENDPOSITION, pe);
    // find quote
    while (ps >= startline && !quote1_found)
    {
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               ps);
      if (ch == '\'' || ch == '\"')
        quote1_found = true;
      ps--;
    }

    // find quote
    while (pe <= endline && !quote2_found)
    {
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               pe);
      if (ch == '\'' || ch == '\"')
        quote2_found = true;
      pe++;
    }

    if (quote1_found && quote2_found)
    {
      lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_SETSELECTIONSTART,
                                          ps + 2);
      lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_SETSELECTIONEND,
                                          pe - 1);
      lexer ()->editor ()->removeSelectedText ();
    }

    outputStr = selection;
  }

  // insert the new word (already managed)
  pos = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCURRENTPOS);
  lexer ()->editor ()->insert (outputStr);
  // fix identation
  if (fixidentation)
  {
    int lineident = lexer ()->editor ()->SendScintilla (
        QsciScintilla::SCI_GETLINEINDENTATION, line);

    lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_SETLINEINDENTATION,
                                        line + 1, lineident);
  }

  // move cursor to the new position
  lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GOTOPOS, pos);
  lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_WORDRIGHT);
  lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_WORDRIGHT);

  if ((_suggesting == SUGGESTING_ELEMENTS && hasAttributes)
      || _suggesting == SUGGESTING_ATTRIBUTES)
  {
    lexer ()->editor ()->recolor ();
    ((NCLTextEditor *)lexer ()->editor ())->userFillingNextAttribute (pos);
  }

  lexer ()->editor ()->endUndoAction ();
}

QStringList
QsciNCLAPIs::callTips (const QStringList &context, int commas,
                       QsciScintilla::CallTipsStyle style, QList<int> &shifts)
{
  (void)context;
  (void)commas;
  (void)style;
  (void)shifts;

  QStringList list;
  QString str ("TODO");
  list.push_back (str);

  return list;
}

bool
QsciNCLAPIs::event (QEvent *e)
{
  (void)e;
  // qCDebug (CPR_PLUGIN_TEXTUAL) << "QsciNCLAPIs::event" << e;
  return true;
}

QString
QsciNCLAPIs::getRequiredAttributesAsStr (const QString &element)
{
  QString ret ("");
  map<QString, bool> *attributes
      = NCLStructure::getInstance ()->getAttributes (element);
  deque<QString> *attrs_ordered
      = NCLStructure::getInstance ()->getAttributesOrdered (element);

  if (attributes != nullptr && attrs_ordered != nullptr)
  {
    deque<QString>::iterator it;
    bool first = true;
    for (it = attrs_ordered->begin (); it != attrs_ordered->end (); ++it)
    {
      QString str = *it;
      if (attributes->count (str) && (*attributes)[str])
      { // if it is required
        if (!first)
          ret += " ";

        first = false;
        ret += *it + "=\"\"";
      }
    }
  }
  return ret;
}

bool
QsciNCLAPIs::isElement (int pos)
{
  int style = lexer ()->editor ()->SendScintilla (
      QsciScintilla::SCI_GETSTYLEAT, pos);

  qCDebug (CPR_PLUGIN_TEXTUAL) << "Style=" << style;

  if (style == QsciLexerNCL::Default)
  {
    return true;
  }
  else if (style == QsciLexerNCL::Tag || style == QsciLexerNCL::XMLTagEnd
           || style == QsciLexerNCL::XMLStart
           || style == QsciLexerNCL::OtherInTag
           || style == QsciLexerNCL::UnknownTag)
  {
    int p = pos - 1;
    while (p >= 0)
    {
      char ch = lexer ()->editor ()->SendScintilla (
          QsciScintilla::SCI_GETCHARAT, p);
      char previous_ch = lexer ()->editor ()->SendScintilla (
          QsciScintilla::SCI_GETCHARAT, p - 1);
      if (isspace (ch))
        break;
      if (ch == '<')
      {
        if (previous_ch == '/')
          return false; // \todo should return a end_tag
        else
          return true;
      }
      p--;
    }
  }
  return false;
}

// TODO: returning false when just before >
bool
QsciNCLAPIs::isAttribute (int pos)
{
  qCDebug (CPR_PLUGIN_TEXTUAL) << "QsciNCLAPIs::isAttribute";
  int style = lexer ()->editor ()->SendScintilla (
      QsciScintilla::SCI_GETSTYLEAT, pos);
  if (style == QsciLexerNCL::Attribute)
    return true;
  else if (style == QsciLexerNCL::Tag || style == QsciLexerNCL::XMLTagEnd
           || style == QsciLexerNCL::XMLStart
           || style == QsciLexerNCL::OtherInTag
           || style == QsciLexerNCL::UnknownTag)
  {
    int p = pos - 1;
    while (p >= 0)
    {
      char ch = lexer ()->editor ()->SendScintilla (
          QsciScintilla::SCI_GETCHARAT, p);
      if (isspace (ch))
        return true;
      if (ch == '<')
        return true;
      p--;
    }
  }
  return false;
}

QString
QsciNCLAPIs::getCurrentTagName (int pos)
{
  int p = pos;
  char ch
      = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT, p);

  if (isElement (pos))
  {
    while (p >= 0 && ch != '<')
    {
      p--;
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
    }

    if (p >= 0)
    {
      p++;
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
      QString word ("");
      while (isalnum (ch))
      {
        word += ch;
        p++;
        ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                                 p);
      }
      return word;
    }
  }
  else
  {
    while (p >= 0)
    {
      QString word ("");
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
      while (p >= 0 && isalnum (ch))
      {
        word.prepend (ch);
        p--;
        ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                                 p);
      }
      if (ch == '<')
        return word;
      p--;
    }
  }
  return QString ("");
}

QString
QsciNCLAPIs::getCurrentAttribute (int pos)
{
  int p = pos;
  char ch;
  QString current_attribute = "";
  if (isAttributeValue (pos))
  {
    bool quote_found = false, equal_found = false;
    // find quote
    while (p >= 0 && !quote_found)
    {
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
      if (ch == '\'' || ch == '\"')
        quote_found = true;
      p--;
    }
    // find equal
    while (p >= 0 && !equal_found)
    {
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
      if (ch == '=')
        equal_found = true;
      p--;
    }
    // remove any whitespace between = and attribute name
    while (p >= 0)
    {
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
      if (!isspace (ch))
        break;
      p--;
    }
    // get the attribute name
    while (p >= 0)
    {
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
      if (!isalnum (ch))
        break;
      current_attribute.prepend (ch);
      p--;
    }

    current_attribute = current_attribute.trimmed ();
  }

  return current_attribute;
}

// TODO: Eliminate closed tags in the way
int
QsciNCLAPIs::getParentOffset (int pos)
{
  int p = pos;
  char ch
      = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT, p);

  int reading = -1;
  int closed_tags = 0;
  while (p >= 0)
  {
    p--;
    ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT, p);
    if (ch == '>')
    {
      reading = QsciLexerNCL::XMLStart;
      p--;
      if (p < 0)
        break;
      ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT,
                                               p);
      if (ch == '/')
        reading = QsciLexerNCL::XMLTagEnd;
      else if (ch == '-')
        reading = QsciLexerNCL::HTMLComment;
      else
      {
        char lastch = ch;
        while (p >= 0 && ch != '<')
        {
          p--;
          lastch = ch;
          ch = lexer ()->editor ()->SendScintilla (
              QsciScintilla::SCI_GETCHARAT, p);
        }
        if (lastch == '/')
          reading = QsciLexerNCL::XMLEnd;

        if (reading == QsciLexerNCL::XMLEnd)
          closed_tags++;
        else if (reading == QsciLexerNCL::XMLStart)
          closed_tags--;

        // qCDebug (CPR_PLUGIN_TEXTUAL) << "closed_tags = " << closed_tags << "
        // p=" << p;
        if (closed_tags < 0)
          return p + 2;
      }
    }
  }
  return -1;
}

QString
QsciNCLAPIs::getAttributeValueFromCurrentElement (int pos, const QString &attr)
{
  qCDebug (CPR_PLUGIN_TEXTUAL)
      << "QsciNCLAPIs::getAttributeValueFromCurrentElement";
  int start = getStartTagBegin (pos);
  int length = getStartTagLength (pos);
  int end = start + length;
  QString attrValue = "";

  if (start < 0 || end < 0)
    return "";

  QString text;
  char *chars = (char *)malloc ((end - start) * sizeof (char) + 1);
  lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETTEXTRANGE, start,
                                      end, chars);
  text = QString (chars);
  qCDebug (CPR_PLUGIN_TEXTUAL) << "text = " << text;

  QDomDocument domDoc;
  QString tagname = getCurrentTagName (pos);
  // \fixme I am assuming that the text is not closing the tag. This will
  // probably cause some problem.
  text += "</";
  text += tagname;
  text += ">";
  if (domDoc.setContent (text))
  {
    attrValue = domDoc.firstChildElement ().attribute (attr);
  }
  delete chars;
  return attrValue;
}

QString
QsciNCLAPIs::getParentTagName (int pos)
{
  int p_offset = getParentOffset (pos);
  if (p_offset > 0)
    return getCurrentTagName (p_offset);
  return "";
}

// FIXME: By now, it will consider any quoted string as attribute value, but
// this is not true.
bool
QsciNCLAPIs::isAttributeValue (int pos)
{
  qCDebug (CPR_PLUGIN_TEXTUAL) << "QsciNCLAPIs::isAttributeValue";
  int style = lexer ()->editor ()->SendScintilla (
      QsciScintilla::SCI_GETSTYLEAT, pos);

  if (style == QsciLexerNCL::HTMLDoubleQuotedString
      || style == QsciLexerNCL::HTMLSingleQuotedString)
  {
    return true;
  }

  return false;
}

// FIXME: be sure we are in a start tag
int
QsciNCLAPIs::getStartTagBegin (int pos)
{
  int p = pos;
  char ch
      = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT, p);
  while (p >= 0 && ch != '<')
  {
    p--;
    ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT, p);
  }
  return p;
}

// FIXME: Be sure that we are in a start tag
int
QsciNCLAPIs::getStartTagLength (int pos)
{
  int start = getStartTagBegin (pos);
  int p = pos;
  int text_length
      = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETLENGTH);
  char ch
      = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT, p);

  while (p < text_length && ch != '>')
  {
    p++;
    ch = lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETCHARAT, p);
  }
  return p >= text_length ? -1 : (p - start + 1);
}

void
QsciNCLAPIs::getAttributesTyped (int pos, QStringList &attrs)
{
  int start = getStartTagBegin (pos);
  int length = getStartTagLength (pos);
  int end = start + length;

  qCDebug (CPR_PLUGIN_TEXTUAL) << "start=" << start << " end=" << end;

  if (start < 0 || end < 0)
    return;

  QString text;
  char *chars = (char *)malloc ((end - start) * sizeof (char) + 1);
  lexer ()->editor ()->SendScintilla (QsciScintilla::SCI_GETTEXTRANGE, start,
                                      end, chars);
  text = QString (chars);
  qCDebug (CPR_PLUGIN_TEXTUAL) << "text = " << text;

  // FIXME: The following regex is not completely correct. Te text inside
  // the attribute will be matched in some cases.
  QRegExp attrRegex ("\\s[a-zA-Z]+");

  int lastIndex = 0;
  int index = attrRegex.indexIn (text, lastIndex);
  while (index != -1)
  {
    if (index + attrRegex.matchedLength () > end)
      break;
    qCDebug (CPR_PLUGIN_TEXTUAL) << index << " " << attrRegex.matchedLength ();
    attrs << text.mid (index, attrRegex.matchedLength ()).trimmed ();

    lastIndex = (index + attrRegex.matchedLength ());
    index = attrRegex.indexIn (chars, lastIndex);
  }

  delete chars;
}

QString
QsciNCLAPIs::relativePath (const QString &absolutePath,
                           const QString &relativeTo, bool bIsFile /*= false*/)
{
  QStringList absoluteDirectories
      = absolutePath.split ('/', QString::SkipEmptyParts);
  QStringList relativeDirectories
      = relativeTo.split ('/', QString::SkipEmptyParts);

  // Get the shortest of the two paths
  int length = absoluteDirectories.count () < relativeDirectories.count ()
                   ? absoluteDirectories.count ()
                   : relativeDirectories.count ();

  // Use to determine where in the loop we exited
  int lastCommonRoot = -1;
  int index;

  // Find common root
  for (index = 0; index < length; index++)
    if (absoluteDirectories[index] == relativeDirectories[index])
      lastCommonRoot = index;
    else
      break;

  // If we didn't find a common prefix then throw
  if (lastCommonRoot == -1)
    throw QString ("Paths do not have a common base");

  // Build up the relative path
  QString relativePath;

  // Add on the ..
  for (index = lastCommonRoot + 1;
       index < absoluteDirectories.count () - (bIsFile ? 1 : 0); index++)
  {
    if (absoluteDirectories[index].length () > 0)
      relativePath.append ("../");
  }

  // Add on the folders
  for (index = lastCommonRoot + 1; index < relativeDirectories.count () - 1;
       index++)
  {
    relativePath.append (relativeDirectories[index]).append ("/");
  }
  relativePath.append (relativeDirectories[relativeDirectories.count () - 1]);

  return relativePath;
}

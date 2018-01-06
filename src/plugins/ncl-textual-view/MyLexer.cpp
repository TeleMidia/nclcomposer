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
#include "MyLexer.h"

#include "NCLTextualViewPlugin_global.h"

// TODO: MOVE TO OTHER PLACE
int
text_partition_cmp (const text_partition &a, const text_partition &b)
{
  if (a.begin != b.begin)
    return a.begin < b.begin;

  if (a.end != a.end)
    return a.end < b.end;

  return a.id < b.id;
}

MyLexer::MyLexer (QObject *parent) : QsciLexerCustom (parent)
{
  qCDebug (CPR_PLUGIN_TEXTUAL) << __FUNCTION__;
}

MyLexer::~MyLexer () {}

const char *
MyLexer::language () const
{
  return "AsciiDoc";
}

QString
MyLexer::description (int style) const
{
  switch (style)
  {
    case Default:
      return "Default";

    case Comment:
      return "Comment";
  }

  return QString ();
}

void
MyLexer::styleText (int start, int end)
{
  QString source;
  qCDebug (CPR_PLUGIN_TEXTUAL) << __FUNCTION__ << "start =" << start
                               << " end =" << end;

  if (!editor ())
    return;

  char *chars = (char *)malloc ((end - start) * sizeof (char) + 1);
  editor ()->SendScintilla (QsciScintilla::SCI_GETTEXTRANGE, start, end,
                            chars);
  source = QString (chars);

  qCDebug (CPR_PLUGIN_TEXTUAL) << "source =" << source;
  startStyling (start, 0x1f);

  vector<text_partition> parts = makePartitions (chars, 0, source.length ());
  sort (parts.begin (), parts.end (), text_partition_cmp);
  int lastIndex = 0;
  for (unsigned int i = 0; i < parts.size (); i++)
  {
    qCDebug (CPR_PLUGIN_TEXTUAL) << "partition id=" << parts[i].id
                                 << " begin=" << parts[i].begin
                                 << " end=" << parts[i].end;

    setStyling (parts[i].begin - lastIndex, getStyle (Default));
    setStyling (parts[i].end - parts[i].begin, _partitionStyle[parts[i].id]);
    lastIndex = parts[i].end;
  }

  if (source.length () - lastIndex > 0)
    setStyling (source.length () - lastIndex, getStyle (Default));

  free (chars);
}

QColor
MyLexer::defaultColor (int style) const
{
  switch (style)
  {
    case Default:
      return QColor (0x00, 0x0, 0x0);
    case Comment:
      return QColor (0x0, 0xe0, 0x0);
  }

  return QsciLexer::defaultColor (style);
}

QFont
MyLexer::defaultFont (int style) const
{
  Q_UNUSED (style)
  return QFont ("Courier New", 10);
}

QColor
MyLexer::defaultPaper (int style) const
{
  return QsciLexer::defaultPaper (style);
}

QsciStyle
MyLexer::getStyle (int style)
{
  if (style < MaxStyle)
  {
    return QsciStyle (style, description (style), defaultColor (style),
                      defaultPaper (style), defaultFont (style));
  }
  else
  {
    return QsciStyle (style);
  }
}

bool
MyLexer::addTextPartition (int partition_id, const QRegExp &regex,
                           const QsciStyle &style)
{
  _partitionRegex.insert (partition_id, regex);
  _partitionStyle.insert (partition_id, style);

  return true;
}

vector<text_partition>
MyLexer::makePartitions (char *chars, int begin, int end)
{
  vector<text_partition> partitions;
  QRegExp regex;
  int PARTITION_ID = -1;

  // GET FISRT PARTITION
  QMap<int, QRegExp>::const_iterator i = _partitionRegex.constBegin ();
  int lastIndex = begin;
  while (i != _partitionRegex.constEnd ())
  {
    PARTITION_ID = i.key ();
    regex = i.value ();
    ++i;
    if (PARTITION_ID == -1)
      return partitions;

    lastIndex = begin;

    // TODO: IMPROVE PERFORMANCE
    int index = regex.indexIn (chars, lastIndex);
    while (index != -1)
    {
      if (index + regex.matchedLength () > end)
        break;
      text_partition part;
      part.id = PARTITION_ID;
      part.begin = index;
      part.end = index + regex.matchedLength ();
      partitions.push_back (part);

      lastIndex = (index + regex.matchedLength ());
      index = regex.indexIn (chars, lastIndex);
    }
  }

  return partitions;
}

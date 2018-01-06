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
#ifndef MYLEXER_H
#define MYLEXER_H

#include <algorithm>
#include <vector>
using namespace std;

#include <QColor>
#include <QDebug>
#include <QFont>
#include <QObject>
#include <Qsci/qscilexercustom.h>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscistyle.h>

class QsciStyle;

typedef struct
{
  int begin, end, id;
} text_partition;

class MyLexer : public QsciLexerCustom
{
  Q_OBJECT

public:
  enum
  {
    Default = 0,
    Comment = 1,
    MaxStyle
  };

  explicit MyLexer (QObject *parent = 0);
  ~MyLexer ();
  const char *language () const;
  QString description (int) const;
  void styleText (int start, int end);

  // TODO: PARTITIONS INSIDE PARTITION
  bool addTextPartition (int partition_id, const QRegExp &regex,
                         const QsciStyle &style);

  QColor defaultColor (int) const;
  QFont defaultFont (int) const;
  QColor defaultPaper (int) const;

private:
  QMap<int, QRegExp> _partitionRegex;
  QMap<int, QsciStyle> _partitionStyle;

  vector<text_partition> makePartitions (char *chars, int begin, int end);
  QsciStyle getStyle (int);
};

#endif // MYLEXER_H

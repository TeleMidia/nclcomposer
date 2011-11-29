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
#ifndef MYLEXER_H
#define MYLEXER_H

#include <vector>
#include <algorithm>
using namespace std;

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QFont>
#include <telem/Qsci/qscilexercustom.h>
#include <telem/Qsci/qscistyle.h>
#include <telem/Qsci/qsciscintilla.h>

class QsciStyle;


typedef struct {
    int begin, end, id;
} text_partition;

class MyLexer : public QsciLexerCustom {
    Q_OBJECT

private:
    QMap <int, QRegExp > partition_regex;
    QMap <int, QsciStyle > partition_style;

public:
    enum {
        Default = 0,
        Comment = 1,
        MaxStyle
    };

    MyLexer(QObject *parent = 0);
    ~MyLexer();
    const char *language() const;
    QString description(int) const;
    void styleText(int start, int end);

    //TODO: PARTITIONS INSIDE PARTITION
    bool addTextPartition ( int partition_id,
                            const QRegExp &regex,
                            const QsciStyle &style);


    QColor defaultColor(int);
    QFont  defaultFont(int);
    QColor defaultPaper(int);

private:
    vector <text_partition> makePartitions(char * chars, int begin, int end);
    QsciStyle getStyle(int);

};

#endif // MYLEXER_H

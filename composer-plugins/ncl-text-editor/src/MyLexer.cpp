/* Copyright (c) 2011 Telemidia/PUC-Rio.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * Contributors:
 *    Telemidia/PUC-Rio - initial API and implementation
 */
#include "MyLexer.h"

//TODO: MOVE TO OTHER PLACE
int text_partition_cmp (const text_partition &a, const text_partition &b) {
    if(a.begin != b.begin)
        return a.begin < b.begin;

    if(a.end != a.end)
        return a.end < b.end;

    return a.id < b.id;
}

MyLexer::MyLexer(QObject *parent)
    : QsciLexerCustom(parent)
{
    qDebug() << __FUNCTION__;
}

MyLexer::~MyLexer()
{
}

const char* MyLexer::language() const
{
    return "AsciiDoc";
}

QString MyLexer::description(int style) const
{
    switch(style){
    case Default:
        return "Default";

    case Comment:
        return "Comment";
    }

    return QString();
}

void MyLexer::styleText(int start, int end)
{
    QString source;
    qDebug() << __FUNCTION__
            << "start =" << start
            << " end =" << end;

    if (!editor())
        return;

    char *chars = (char *) malloc ((end - start) * sizeof(char) + 1);
    editor()->SendScintilla(QsciScintilla::SCI_GETTEXTRANGE, start, end, chars);
    source = QString(chars);

    qDebug() << "source =" << source;
    startStyling(start, 0x1f);

    vector <text_partition> parts = makePartitions (chars, 0, source.length());
    sort(parts.begin(), parts.end(), text_partition_cmp);
    int lastIndex = 0;
    for (unsigned int i = 0; i < parts.size(); i++) {
        qDebug() << "partition id=" << parts[i].id << " begin=" << parts[i].begin << " end=" << parts[i].end;
        setStyling(parts[i].begin-lastIndex, getStyle(Default));
        setStyling(parts[i].end-parts[i].begin, partition_style[parts[i].id]);
        lastIndex = parts[i].end;

    }
    if (source.length()-lastIndex > 0) {
        setStyling(source.length()-lastIndex, getStyle(Default));
    }
    free(chars);
}

QColor MyLexer::defaultColor(int style)
{
    switch(style){
    case Default:
        return QColor(0x00, 0x0, 0x0);
    case Comment:
        return QColor(0x0, 0xe0, 0x0);
    }
    return QsciLexer::defaultColor(style);
}

QFont  MyLexer::defaultFont(int style)
{
    return QFont("Courier New", 10);
}

QColor MyLexer::defaultPaper(int style)
{
    return QsciLexer::defaultPaper(style);
}

QsciStyle MyLexer::getStyle(int style)
{
    if (style < MaxStyle) {
        return QsciStyle(style, description(style), defaultColor(style),
                         defaultPaper(style), defaultFont(style));
    } else {
        return QsciStyle(style);
    }
}

bool MyLexer::addTextPartition (int partition_id, const QRegExp &regex,
                                const QsciStyle &style) {

    partition_regex.insert(partition_id, regex);
    partition_style.insert(partition_id, style);

    return true;
}

vector <text_partition > MyLexer::makePartitions (char *chars, int begin, int end){
    vector <text_partition> partitions;
    QRegExp regex;
    int PARTITION_ID = -1;
    //GET FISRT PARTITION
    QMap <int, QRegExp>::const_iterator i = partition_regex.constBegin();

    int lastIndex = begin;
    while(i != partition_regex.constEnd()) {
        PARTITION_ID = i.key();
        regex = i.value();
        ++i;
        if(PARTITION_ID == -1)
            return partitions;

        lastIndex = begin;

        //TODO: IMPROVE PERFORMANCE
        int index = regex.indexIn(chars, lastIndex);
        while(index != -1){
            if (index + regex.matchedLength() > end)
                break;
            text_partition part;
            part.id = PARTITION_ID;
            part.begin = index;
            part.end = index + regex.matchedLength();
            partitions.push_back (part);

            lastIndex = (index + regex.matchedLength());
            index = regex.indexIn(chars, lastIndex);
        }
    }
    return partitions;
}

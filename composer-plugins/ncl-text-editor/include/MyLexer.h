#ifndef MYLEXER_H
#define MYLEXER_H

#include <vector>
#include <algorithm>
using namespace std;

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QFont>
#include <Qsci/qscilexercustom.h>
#include <Qsci/qscistyle.h>
#include <Qsci/qsciscintilla.h>

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

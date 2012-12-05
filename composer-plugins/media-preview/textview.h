#ifndef TEXTVIEW_H
#define TEXTVIEW_H
#include<QString>
#include<QTextEdit>


class textview : public QTextEdit
{
public:
    textview(QString Filename);
};

#endif // TEXTVIEW_H

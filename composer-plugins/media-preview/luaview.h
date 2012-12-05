#ifndef LUAVIEW_H
#define LUAVIEW_H
#include<QTextEdit>
#include<QString>

class luaview : public QTextEdit
{
    Q_OBJECT
public:
    luaview(QString filename);
};

#endif // LUAVIEW_H

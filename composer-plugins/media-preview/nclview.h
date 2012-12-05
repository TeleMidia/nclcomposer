#ifndef NCLVIEW_H
#define NCLVIEW_H
#include<QTextEdit>

class nclview :public QTextEdit
{
public:
    nclview(QString filename);
};

#endif // NCLVIEW_H

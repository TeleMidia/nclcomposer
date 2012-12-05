#ifndef IMAGVIEW_H
#define IMAGVIEW_H
#include<QLabel>
#include<QString>



class imagview :public QLabel
{
 Q_OBJECT

public:
    imagview(QString filename);
};

#endif // IMAGVIEW_H

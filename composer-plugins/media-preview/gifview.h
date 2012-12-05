#ifndef GIFVIEW_H
#define GIFVIEW_H
#include<QLabel>
#include<QString>

class gifview : public QLabel
{
    Q_OBJECT
public:
    gifview(QString filename);

};

#endif // GIFVIEW_H

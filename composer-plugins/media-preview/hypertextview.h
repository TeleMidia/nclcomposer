#ifndef HYPERTEXTVIEW_H
#define HYPERTEXTVIEW_H
#include<QtWebKit/QWebView>
#include<QString>

class hypertextview :public QWebView
{
public:
    hypertextview(QString filename);
};

#endif // HYPERTEXTVIEW_H

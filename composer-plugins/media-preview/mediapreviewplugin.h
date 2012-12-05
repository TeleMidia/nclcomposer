#ifndef MEDIAPREVIEWPLUGIN_H
#define MEDIAPREVIEWPLUGIN_H

#include <audioplayer.h>

#include<imagview.h>
#include<gifview.h>
#include<textview.h>
#include<hypertextview.h>
#include<nclview.h>
#include<luaview.h>
//#include<video.h>
#include "videoplayer.h"
#include <composer/core/extensions/IPlugin.h>
#include<VideoPlayer>
#include<MediaObject>
#include<QPushButton>
#include<QLabel>
#include<QWidget>
#include<QMovie>
#include<QtWebKit/QWebView>
#include<QTextEdit>
#include<QMainWindow>



using namespace composer::core;

class MediaPreviewPlugin : public IPlugin
{
    Q_OBJECT

public:
    explicit MediaPreviewPlugin();
    ~MediaPreviewPlugin();

    QWidget *getWidget();

private:
    QString comp;

    QWidget *window;
    QMainWindow *windowg;
    QWidget *windowv;

    QTextEdit *textedit;
    QTextEdit  *texteditncl;
    QTextEdit *texteditlua;

    Phonon::MediaObject *mediaobject;
    Phonon::MediaObject *media;
   // Phonon::VideoPlayer *videoPlayer;



   QPushButton *button1;
   QPushButton *button2;
   QPushButton *button3;
   QWebView *view;
   audioplayer *musicplayer;
   imagview *imageview;
   gifview *picgifview;
   textview *txtview;
   hypertextview *hyperview;
   nclview *ncldocview;
   luaview *luaobjview;
  //videoplayer *videoview;
videoplayer *videoplay;






    QMovie *movie;

public slots:
    void changeSelectedEntity(QString pluginID, void *entity);

};

#endif // MEDIAPREVIEWPLUGIN_H

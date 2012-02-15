#ifndef MEDIAPREVIEWPLUGIN_H
#define MEDIAPREVIEWPLUGIN_H

#include "player.h"
#include <composer/core/extensions/IPlugin.h>
#include<VideoPlayer>
#include<MediaObject>
#include<QPushButton>
#include<QLabel>
#include<QWidget>
#include<QFont>
#include<QMovie>
#include<QtWebKit/QWebView>
#include<QTextEdit>



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
    QWidget *windowg;
    QWidget *windowv;
    QFont *f;
    QTextEdit *textedit;
    QTextEdit  *texteditncl;
    QTextEdit *texteditlua;

    Phonon::MediaObject *mediaobject;
    Phonon::MediaObject *media;
    Phonon::VideoPlayer *videoPlayer;


    QLabel *picture;
    QLabel *picture1;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    QWebView *view;
    player *musicPlayer;
    QMovie *movie;

public slots:
    void changeSelectedEntity(QString pluginID, void *entity);

};

#endif // MEDIAPREVIEWPLUGIN_H

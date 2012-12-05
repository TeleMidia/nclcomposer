#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include<QString>
#include<MediaObject>
#include<AudioOutput>
#include<VideoPlayer>
#include<VideoWidget>

namespace Ui {
class videoplayer;
}

class videoplayer : public QWidget
{
    Q_OBJECT
    
public:

    videoplayer(QString filename);
    //explicit videoplayer(QWidget *parent = 0);
    ~videoplayer();
    
private:
    Ui::videoplayer *ui;
    Phonon::MediaObject *mediaObject;
    Phonon::VideoPlayer *videoObject;
    Phonon::AudioOutput *Audioutput;
    Phonon::Path audioOutputPath;
    Phonon::VideoWidget *videowidget;


public slots:
    void setfile(QString filename);
    void playvideo(QString filename);
private slots:
    void on_playbutton_clicked();
    void on_pausebutton_clicked();
    void on_stopbutton_clicked();
};

#endif // VIDEOPLAYER_H

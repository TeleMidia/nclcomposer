#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H
#include<QObject>
#include<QPushButton>
#include<MediaObject>
#include<QWidget>
#include<QMouseEvent>
#include<QHBoxLayout>
#include<VideoPlayer>
#include<QSlider>
#include<VolumeSlider>
#include<AudioOutput>
#include<SeekSlider>
#include<QLabel>
#include <phonon/videowidget.h>


class audioplayer : public QWidget
{
  Q_OBJECT

public:
    audioplayer(QString filename);



public slots:
  void play();
  void pause();
  void stop();

 //void setVolume(qreal volume);



private:
   audioplayer *mvideoWidget;
   QWidget *infowidget;
  Phonon::MediaObject *mediaobject;
   Phonon::VolumeSlider *volumeslider;
   Phonon::AudioOutput *Audioutput;
   Phonon::Path audioOutputPath;
   Phonon::SeekSlider *slider;

    QPushButton *stopButton;
    QPushButton *playButton;
    QPushButton *pauseButton;
    QLabel *info;
    QMenu *fileMenu;
    QLabel *timeLabel;
    QLabel *progressLabel;


};

#endif // AUDIOPLAYER_H

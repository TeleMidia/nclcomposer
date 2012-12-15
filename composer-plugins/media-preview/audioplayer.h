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
#include<QtTest>
#include <phonon/videowidget.h>

/*!
 \brief Handles the creation of audio objects.
*/

class audioplayer : public QWidget
{
  Q_OBJECT

public:

    /*!
    * \brief Constructor.
    * \param Filename is the variable for specifying the media object location.
    */
    audioplayer(QString filename = "");

    /*!
      \brief Destructor.
     */
  ~audioplayer();



public slots:

    /*!
     \brief Play the media object .
    */
  void play();
  /*!
   \brief Pause the media object .
  */
  void pause();
  /*!
   \brief stop the media object .
  */
  void stop();

  void setVolume(int volume);


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

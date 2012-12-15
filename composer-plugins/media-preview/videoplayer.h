#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include<QString>
#include<MediaObject>
#include<AudioOutput>
#include<VideoPlayer>
#include<VideoWidget>


namespace Ui {
/*!
 \brief Handles the creation of video objects.
*/

class videoplayer;
}

class videoplayer : public QWidget
{
    Q_OBJECT
    
public:
    /*!
     \brief Constructor.
     \param Filename is the variable for specifying the media object location.
    */

    videoplayer(QString filename);

    /*!
      \brief Destructor.
     */

    ~videoplayer();
    
private:
    Ui::videoplayer *ui;
    Phonon::MediaObject *mediaObject;
    Phonon::VideoPlayer *videoObject;
    Phonon::AudioOutput *Audioutput;
    Phonon::Path audioOutputPath;
    Phonon::VideoWidget *videowidget;


private slots:

    /*!
     \brief Play the video media object.
    */
    void on_playbutton_clicked();
    /*!
     \brief Pause the video media object.
    */
    void on_pausebutton_clicked();
    /*!
     \brief Stop the video media object.
    */
    void on_stopbutton_clicked();

};

#endif // VIDEOPLAYER_H

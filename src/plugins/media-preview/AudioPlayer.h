/*
 * Copyright 2011-2013 TeleMidia/PUC-Rio.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include <QPushButton>
#include <MediaObject>
#include <QWidget>
#include <QMouseEvent>
#include <QHBoxLayout>
#include <VideoPlayer>
#include <QSlider>
#include <VolumeSlider>
#include <AudioOutput>
#include <SeekSlider>

#include<QLabel>
#include<QtTest>

#include <phonon/videowidget.h>

/*!
 * \brief Handles the creation of audio objects.
 */
class AudioPlayer : public QWidget
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor.
   * \param Filename is the variable for specifying the media object location.
   */
  explicit AudioPlayer(const QString &filename = "");

  /*!
   * \brief Destructor.
   */
  ~AudioPlayer();

public slots:
  /*!
   *  \brief Play the media object .
   */
  void play();
  /*!
   * \brief Pause the media object .
   */
  void pause();
  /*!
   * \brief stop the media object .
   */
  void stop();

  /*!
   * \brief setVolume
   * \param volume
   */
  // void setVolume(int volume);

private:
  AudioPlayer *mvideoWidget;
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

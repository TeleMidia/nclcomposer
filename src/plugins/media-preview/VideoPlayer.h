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
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include<QString>
#include<MediaObject>
#include<AudioOutput>
#include<VideoPlayer>
#include<VideoWidget>

namespace Ui {
  class VideoPlayer;
}

class VideoPlayer : public QWidget
{
  Q_OBJECT

public:
  /*!
   * \brief Constructor.
   * \param Filename is the variable for specifying the media object location.
   */
  explicit VideoPlayer(const QString &filename);

  /*!
   * \brief Destructor.
   */
  ~VideoPlayer();

private:
  Ui::VideoPlayer *ui;
  Phonon::MediaObject *mediaObject;
  Phonon::VideoPlayer *videoObject;
  Phonon::AudioOutput *audioOutput;
  Phonon::Path audioOutputPath;
  Phonon::VideoWidget *videoWidget;

private slots:
  /*!
   * \brief Play the video media object.
   */
  void on_playbutton_clicked();
  /*!
   * \brief Pause the video media object.
   */
  void on_pausebutton_clicked();
  /*!
   * \brief Stop the video media object.
   */
  void on_stopbutton_clicked();
};

#endif // VIDEOPLAYER_H

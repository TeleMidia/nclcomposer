/*
 * Copyright 2011-2018 TeleMidia/PUC-Rio.
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
#include "AudioPlayer.h"

#include<QObject>
#include<QPushButton>
#include<MediaObject>
#include<QWidget>
#include<QMouseEvent>
#include<QHBoxLayout>
#include<VideoPlayer>
#include<QFont>
#include<AudioOutput>
#include<QSlider>
#include<VolumeSlider>
#include<Path>

AudioPlayer::AudioPlayer(const QString &filename)

{
  setStyleSheet("* { background-color: rgb(220,220,220) }");
  setMinimumWidth(150);
  setMinimumHeight(60);

  mediaobject = Phonon::createPlayer(Phonon::MusicCategory,
                                     Phonon::MediaSource(filename));
  /*Criação dos botões de controle(play, pause, stop, slider de voulme,e
  seek slider*/

  playButton = new QPushButton();
  pauseButton = new QPushButton();
  stopButton = new QPushButton();
  playButton->setFont(QFont("Comic Sans MS",10,QFont::Bold));
  pauseButton->setFont(QFont("Comic Sans MS",10,QFont::Bold));
  stopButton->setFont(QFont("Comic Sans MS",10,QFont::Bold));

  playButton->setIcon(QIcon("play.jpeg"));
  playButton->setFixedWidth(51);
  playButton->setFixedHeight(34);
  playButton->setIconSize(QSize (26,26));
  playButton->setStyleSheet("* { background-color: rgb(173,255,47) }");
  playButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

  pauseButton->setIcon(QIcon("pause.jpeg"));
  pauseButton->setFixedWidth(51);
  pauseButton->setFixedHeight(34);
  pauseButton->setIconSize(QSize (26,26));
  pauseButton->setStyleSheet("* { background-color: rgb(173,255,47) }");
  pauseButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

  stopButton->setIcon(QIcon("stop.jpeg"));
  stopButton->setFixedWidth(51);
  stopButton->setFixedHeight(34);
  stopButton->setIconSize(QSize (26,26));
  stopButton->setStyleSheet("* { background-color: rgb(173,255,47) }");
  stopButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);

  mediaobject = new Phonon::MediaObject(this);
  mediaobject->setCurrentSource(Phonon::MediaSource(filename));
  Audioutput =new Phonon::AudioOutput(Phonon::MusicCategory, this);
  audioOutputPath = Phonon::createPath(mediaobject, Audioutput);
  // Audioutput->setVolume(0.003);

  volumeslider= new Phonon::VolumeSlider();
  volumeslider->setAudioOutput(Audioutput);

  volumeslider->setSingleStep(0.002);
  volumeslider->setMaximumVolume(0.5);
  volumeslider->setTracking(true);
  volumeslider->setMaximumVolume(1);
  volumeslider->setMuteVisible(true);
  volumeslider->setFixedSize(150,31);
  volumeslider->setFixedWidth(170);
  volumeslider->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::MinimumExpanding);

  Phonon::SeekSlider *slider = new Phonon::SeekSlider;
  slider->setMediaObject(mediaobject);
  slider->setFixedWidth(510);
  slider->show();

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(playButton);
  layout->addWidget(pauseButton);
  layout->addWidget(stopButton);
  layout->addWidget(volumeslider);

  QHBoxLayout *layout2=new QHBoxLayout;
  layout2->addWidget(slider);

  QVBoxLayout *layoutf=new QVBoxLayout;

  layoutf->addLayout(layout);
  layoutf->addLayout(layout2);

  setLayout(layoutf);

  setVisible(true);

  /*Slots para o controle dos botões*/
  QObject::connect(playButton,SIGNAL(clicked()),this,SLOT(play()));
  QObject::connect(pauseButton,SIGNAL(clicked()),this,SLOT(pause()));
  QObject::connect(stopButton,SIGNAL(clicked()),this,SLOT(stop()));
}


AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::play()
{
  mediaobject->play();
}

void AudioPlayer::pause()
{
  mediaobject->pause();
}

void AudioPlayer::stop()
{
  mediaobject->stop();
}

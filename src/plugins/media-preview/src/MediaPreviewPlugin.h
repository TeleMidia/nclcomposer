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
#ifndef MEDIAPREVIEWPLUGIN_H
#define MEDIAPREVIEWPLUGIN_H

#include "AudioPlayer.h"
#include "ImageView.h"
#include "GifView.h"
#include "TextView.h"
#include "HypertextView.h"
#include "NCLView.h"
#include "LuaView.h"
#include "VideoPlayer.h"

#include <composer/core/extensions/IPlugin.h>

#include <VideoPlayer>
#include <MediaObject>
#include <QPushButton>
#include <QLabel>
#include <QWidget>
#include <QMovie>
#include <QFileInfo>
#include <QtWebKit/QWebView>
#include <QTextEdit>
#include <QMainWindow>

using namespace composer::core;

/*!
 * \brief MediaPreviewPlugin is a Composer plugin that display a different
 *  media player, which depends of the extension and the media type object.
 *  When a media object is selected (clicked) by the user in the structural
 *  view, a media player is displayed and a media pre-view of the media object
 *  is avaliable.
 */
class MediaPreviewPlugin : public IPlugin
{
  Q_OBJECT

public:
  /*!
   * \brief Handles the creation of a principal Window where the media player
   * is displayed.Constructor
   */
  explicit MediaPreviewPlugin();

  /*!
   * \brief Destructor.
   */
  ~MediaPreviewPlugin();

  /*!
   * \brief Returns the widget of that plugin. This widget will be presentated
   *      to the user.
   *
   *  This function is part of the IPlugin API.
   *
   * \return QWidget* the widget that represents this plugin. If NULL, the
   *      plugin has not a visual representation.
   */
  QWidget *getWidget();

public slots:
  /*!
   * \brief Identify when a media object was selected and though entity
   *  parameter the media object attributes can be accessed. In this case the
   *  source and media type attributes was used.
   *
   * \param PluginID is the plugin instance id that first called the changes to
   *  the entity
   * \param *entity the entity that was selected by the user.
   */
  void changeSelectedEntity(QString pluginID, void *entity);

private:
  QString comp;
  QMainWindow *windowg;
  QWidget *windowv;
  Phonon::MediaObject *mediaobject;
  QWebView *view;
  AudioPlayer *musicplayer;
  ImageView *imageview;
  GifView *picgifview;
  TextView *txtview;
  HypertextView *hyperview;
  NCLView *ncldocview;
  LuaView *luaobjview;
  VideoPlayer *videoplay;
  QMovie *movie;
};

#endif // MEDIAPREVIEWPLUGIN_H















#ifndef MEDIAPREVIEWPLUGIN_H
#define MEDIAPREVIEWPLUGIN_H

#include "player.h"
#include <composer/core/extensions/IPlugin.h>
#include <QFileDialog>
#include<VideoPlayer>
#include<MediaObject>
#include <QDebug>
#include<QPushButton>
#include<QPalette>
#include<QHBoxLayout>
#include<QLabel>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsItem>
#include<QWidget>
#include<VideoWidget>
#include<QFont>
#include<QMovie>

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
  QFont *f;

  Phonon::MediaObject *mediaobject;
  Phonon::VideoWidget *vwidget;
  Phonon::MediaObject *media;
  Phonon::VideoPlayer *videoPlayer;

  QLabel *picture;
  QLabel *picture1;
  QPushButton *button1;

  QPushButton *button2;
  QPushButton *button3;

public slots:
  void changeSelectedEntity(QString pluginID, void *entity);

};

#endif // MEDIAPREVIEWPLUGIN_H

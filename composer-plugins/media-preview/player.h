#ifndef PLAYER_H
#define PLAYER_H
#include<QObject>
#include<QPushButton>
#include<MediaObject>
#include<QWidget>
#include<QMouseEvent>
#include<QHBoxLayout>
#include<VideoPlayer>

class player : public QWidget
{
  Q_OBJECT

public:
  player(QString ue);

public slots:
  void play();
  void pause();
  void stop();

private:
  Phonon::MediaObject *mediaobject;
  Phonon::MediaObject *mediaobject2;
  QWidget *source;
};

#endif // PLAYER_H

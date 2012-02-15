#include <player.h>
#include<QObject>
#include<QPushButton>
#include<MediaObject>
#include<QWidget>
#include<QMouseEvent>
#include<QHBoxLayout>
#include<VideoPlayer>

player::player(QString ue)

{
  mediaobject = Phonon::createPlayer(Phonon::MusicCategory,
                                     Phonon::MediaSource(ue));


}

void player::play()
{
  mediaobject->play();
}

void player::pause()
{
  mediaobject->pause();
}

void player::stop()
{
  mediaobject->stop();
}











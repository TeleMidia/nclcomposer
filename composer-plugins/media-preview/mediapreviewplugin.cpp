#include <mediapreviewplugin.h>
#include<player.h>
#include<QPushButton>
#include<VideoPlayer>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsPathItem>
#include<QtGui>
#include<MediaObject>
#include<VideoWidget>
#include<QMovie>
#include<QIcon>
#include<QPalette>
#include<QHBoxLayout>
#include<QFont>
#include<QString>
#include<wchar.h>
#include<QLine>
#include<QTextStream>
#include<QByteArray>

#include <QHBoxLayout>

using namespace std;


MediaPreviewPlugin::MediaPreviewPlugin()
{
  /*main widget*/
  windowg =  new QWidget(0);
  windowg->setStyleSheet("* { background-color: rgb(220,220,220) }");
  windowg->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);

  /*audio*/
  window= new QWidget(0);
  window->setParent(windowg);

  window->hide();

  /*picture*/
  picture=new QLabel(0);
  picture->setParent(windowg);
  picture->setAlignment(Qt::AlignHCenter);
  picture->hide();

  /*picture1*/
  picture1=new QLabel(0);
  picture1->setParent(windowg);
  picture1->hide();
  picture1->setAlignment(Qt::AlignHCenter);

  /*video*/
  vwidget=new Phonon::VideoWidget(windowg);
  vwidget->hide();

  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(vwidget);

  windowg->setLayout(layout);

  comp = "";

  videoPlayer= new Phonon::VideoPlayer(Phonon::VideoCategory,vwidget);
}

MediaPreviewPlugin::~MediaPreviewPlugin()
{

}


QWidget *MediaPreviewPlugin::getWidget()
{
  return windowg;
}

void MediaPreviewPlugin::changeSelectedEntity(QString plugID, void *param)
{
  QString  *entityUID = (QString*) param;

  if(entityUID != NULL)
  {
    Entity* entity = getProject()->getEntityById(*entityUID);
    if(entity != NULL)
    {
      if(entity->getType() == "media")
      {
        QString filename = entity->getAttribute("src");
        if (comp != entity->getUniqueId())
        {
          comp=entity->getUniqueId();
          if (filename.endsWith(".webm") || (filename.endsWith(".mp4")) ||
              (filename.endsWith(".mpg")) )
          {

            videoPlayer->play(Phonon::MediaSource(filename));
            videoPlayer->setVisible(true);
            videoPlayer->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            videoPlayer->adjustSize();
            if(vwidget->isVisible())
            {
              videoPlayer->stop();
              vwidget->hide();
            }
            else
            {
              vwidget->show();
              videoPlayer->show();
              picture->hide();
              picture1->hide();
              window->hide();
            }
          }
          else if (filename.endsWith(".mp3") || (filename.endsWith(".flv") ||
                                                 (filename.endsWith(".wav"))))
          {
            QPushButton *button1 = new QPushButton("Play");
            button1->setFont(QFont("Comic Sans MS",10,QFont::Bold));

            QPushButton *button2 = new QPushButton("Pause");
            button2->setFont(QFont("Comic Sans MS",10,QFont::Bold));

            QPushButton *button3 = new QPushButton("Stop");
            button3->setFont(QFont("Comic Sans MS",10,QFont::Bold));

            QPalette pal = window->palette();

            pal.setColor(window->backgroundRole(), Qt::blue);
            window->setPalette(pal);


            button1->setIcon(QIcon("play.jpeg"));
            button1->setIconSize(QSize (30,30));
            button1->setStyleSheet("* { background-color: rgb(173,255,47) }");

            button2->setIcon(QIcon("pause.jpeg"));
            button2->setIconSize(QSize (30,30));
            button2->setStyleSheet("* { background-color: rgb(173,255,47) }");

            button3->setIcon(QIcon("stop.jpeg"));
            button3->setIconSize(QSize (30,30));
            button3->setStyleSheet("* { background-color: rgb(173,255,47) }");

            QHBoxLayout *layout = new QHBoxLayout;
            layout->addWidget(button1);
            layout->addWidget(button2);
            layout->addWidget(button3);

            window->setLayout(layout);

            player *p = new player(filename);
            mediaobject = Phonon::createPlayer(Phonon::MusicCategory,
                                               Phonon::MediaSource(filename));

            QObject::connect(button1,SIGNAL(clicked()),p,SLOT(play()));
            QObject::connect(button2,SIGNAL(clicked()),p,SLOT(pause()));
            QObject::connect(button3,SIGNAL(clicked()),p,SLOT(stop()));

            window->resize(400,100);
            window->setVisible(true);

            if(vwidget->isVisible())
            {
              videoPlayer->stop();
              vwidget->hide();
              window->show();

              picture->hide();
              picture1->hide();
            }
            else
            {
              window->show();
              picture->hide();
              picture1->hide();
              vwidget->hide();
            }
          }
          else if (filename.endsWith(".jpeg") || (filename.endsWith(".png")))
          {
            picture->setPixmap(QPixmap(filename));
            picture->adjustSize();
            picture->resize(500,200);
            picture->setAlignment(Qt::AlignHCenter);
            picture->show();
            picture->setVisible(true);

            if (vwidget->isVisible())
            {
              videoPlayer->stop();
              vwidget->hide();
              picture->show();
              picture->setVisible(true);
              picture1->hide();
              window->hide();
            }
            else
            {
              picture1->hide();
              window->hide();
              picture->hide();
              picture->show();
              picture->setVisible(true);

            }
          }
          else if (filename.endsWith(".gif") )
          {
            QMovie *movie = new QMovie(filename);
            picture1->setMovie(movie);
            movie->start();
            picture1->adjustSize();
            picture1->resize(500,150);
            picture1->setAlignment(Qt::AlignCenter);
            picture1->show();

            if(vwidget->isVisible())
            {
              videoPlayer->stop();
              vwidget->hide();
              window->hide();
              picture->hide();
              picture1->show();
            }
            else
            {
              window->hide();
              picture->hide();
              picture1->show();
            }
          }
          /*    else if (filename.endsWith(".lua" || ".ncl" || ".txt"))
                     {

                  QFile file(filename);
                  if (!file.open (IO_ReadOnly))
                       return 1;
                  QTextStream stream ( &file );
                  QString line;
                  while( !stream.eof() ) {
                       line = stream.readLine();

                  }
                  file.close(); // when your done.*/

          /*  QFile file(filename);
                         if(!file.open(QIODevice::ReadOnly)) {
                             QMessageBox::information(0, "error", file.errorString());
                         }




                         QTextStream in(&file);

                         while(!in.atEnd()) {
                             QString line = in.readLine();
                             QStringList fields = line.split(",");
                             model->appendRow(fields);
                         }

                         file.close();*/
        }
      }
    }
  }
}

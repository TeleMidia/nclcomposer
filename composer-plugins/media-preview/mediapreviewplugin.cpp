#include <mediapreviewplugin.h>
#include<player.h>
#include<QMovie>
#include<QIcon>
#include<QPalette>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFont>
#include<QString>
#include<QTextStream>
#include<QIODevice>


using namespace std;


MediaPreviewPlugin::MediaPreviewPlugin()
{
    /*main widget*/
    windowg =  new QWidget(0);
    windowg->setStyleSheet("* { background-color: rgb(220,220,220) }");


    /*audio*/
    window= new QWidget(windowg);
    window->hide();

    /*control video*/
    windowv=new QWidget(windowg);
    windowv->hide();


    /*picture*/
    picture=new QLabel(windowg);
    picture->setAlignment(Qt::AlignHCenter);
    picture->hide();

    /*picture1*/
    picture1=new QLabel(windowg);
    picture1->hide();
    picture1->setAlignment(Qt::AlignHCenter);

    /*webview*/
    view= new QWebView(windowg);
    view->hide();

    /*text*/

    textedit=new QTextEdit(windowg);
    textedit->hide();


    /*ncl*/
    texteditncl =new QTextEdit(windowg);
    texteditncl->hide();
    /*lua*/
    texteditlua =new QTextEdit(windowg);
    texteditlua->hide();


    comp = "";

    /*video*/
    QVBoxLayout *layout2 = new QVBoxLayout();
    videoPlayer= new Phonon::VideoPlayer(Phonon::VideoCategory, windowg);
    videoPlayer->hide();


    layout2->addWidget(videoPlayer);
    layout2->addWidget(textedit);
    layout2->addWidget(texteditncl);
    layout2->addWidget(texteditlua);
    layout2->addWidget(window);
    layout2->addWidget(picture);
    layout2->addWidget(picture1);
    layout2->addWidget(view);

    windowg->setLayout(layout2);

    button1 = new QPushButton("Play");
    button2 = new QPushButton("Pause");
    button3 = new QPushButton("Stop");

    musicPlayer = NULL;

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
                QString typemidia = entity->getAttribute("type");
                if (comp != entity->getUniqueId())
                {
                    comp=entity->getUniqueId();
                    if ( filename.endsWith(".mp4") ||
                         filename.endsWith(".mpg")|| filename.endsWith(".mp2") ||
                         filename.endsWith(".mpe") || filename.endsWith(".mpg4") ||
                         filename.endsWith(".mng") || filename.endsWith(".qt") ||
                         filename.endsWith(".mov") || filename.endsWith(".avi"))
                    {


                        videoPlayer->play(Phonon::MediaSource(filename));
                        videoPlayer->setVisible(true);


                        if(videoPlayer->isVisible())

                        {
                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }

                            videoPlayer->stop();
                            picture->hide();
                            picture1->hide();
                            window->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            videoPlayer->play(Phonon::MediaSource(filename));
                            videoPlayer->show();

                        }

                        else
                        {
                            videoPlayer->show();
                            videoPlayer->play();
                            picture->hide();
                            picture1->hide();
                            window->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();


                        }
                    }
                    else if (filename.endsWith(".mp3") || filename.endsWith(".flv") ||
                             filename.endsWith(".wav") || filename.endsWith(".ua") ||
                             filename.endsWith(".mp2") || filename.endsWith(".mp4")||
                             filename.endsWith(".mpg4"))
                    {

                        button1->setFont(QFont("Comic Sans MS",10,QFont::Bold));
                        button2->setFont(QFont("Comic Sans MS",10,QFont::Bold));
                        button3->setFont(QFont("Comic Sans MS",10,QFont::Bold));


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

                        if(musicPlayer != NULL && window->isVisible())
                        {
                            delete musicPlayer;
                            musicPlayer->stop();

                        }


                        musicPlayer = new player(filename);
                        mediaobject = Phonon::createPlayer(Phonon::MusicCategory,
                                                           Phonon::MediaSource(filename));
                        QObject::connect(button1,SIGNAL(clicked()),musicPlayer,SLOT(play()));
                        QObject::connect(button2,SIGNAL(clicked()),musicPlayer,SLOT(pause()));
                        QObject::connect(button3,SIGNAL(clicked()),musicPlayer,SLOT(stop()));



                        window->resize(400,100);
                        window->setVisible(true);

                        if(videoPlayer->isVisible())
                        {
                            videoPlayer->stop();
                            videoPlayer->hide();


                        }
                        else
                        {

                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }

                            window->show();
                            picture->hide();
                            picture1->hide();
                            videoPlayer->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();



                        }
                    }
                    else if (filename.endsWith(".jpeg") || filename.endsWith(".png") ||
                             filename.endsWith(".bmp") || filename.endsWith(".jpg") ||
                             filename.endsWith(".jpe"))
                    {
                        picture->setPixmap(QPixmap(filename));
                        picture->show();
                        picture->setVisible(true);

                        if (videoPlayer->isVisible())
                        {
                            videoPlayer->stop();
                            videoPlayer->hide();

                        }
                        else
                        {
                            picture1->hide();
                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }
                            window->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            picture->show();

                        }
                    }

                    else if (filename.endsWith(".gif") )
                    {

                        QMovie *movie = new QMovie(filename);
                        picture1->setMovie(movie);
                        movie->start();
                        picture1->show();

                        if(videoPlayer->isVisible())
                        {
                            videoPlayer->stop();
                            videoPlayer->hide();

                        }
                        else
                        {
                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }
                            window->hide();
                            picture->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            picture1->show();
                        }
                    }


                    else if (filename.startsWith("http") ||filename.endsWith(".html") ||
                             filename.endsWith("htm") || filename.endsWith(".xhtml"))
                    {
                        view->load(QUrl(filename));
                        view->adjustSize();
                        view->show();

                        if(videoPlayer->isVisible())
                        {

                            videoPlayer->stop();
                            videoPlayer->hide();

                        }
                        else
                        {
                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            view->show();
                        }

                    }

                    else if (filename.endsWith(".txt"))
                    {

                        QFile inputFile(filename);
                        QTextStream in(&inputFile);
                        inputFile.open(QIODevice::ReadOnly);
                        textedit->setText(in.readAll());
                        textedit->setReadOnly(true);
                        textedit->setStyleSheet("QTextEdit { background-color : white; color : black; }");
                        textedit->show();

                        if(videoPlayer->isVisible())
                        {

                            videoPlayer->stop();
                            videoPlayer->hide();

                        }

                        else
                        {
                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            texteditncl->hide();
                            texteditlua->hide();
                            view->hide();
                            textedit->show();
                        }

                    }


                    else if (filename.endsWith(".ncl") && typemidia.contains("application/x-ginga-NCL"))
                    {

                        QFile inputFile(filename);
                        QTextStream in(&inputFile);
                        inputFile.open(QIODevice::ReadOnly);
                        texteditncl->setText(in.readAll());
                        texteditncl->setReadOnly(true);
                        texteditncl->setStyleSheet("QTextEdit { background-color : white; color : black; }");
                        texteditncl->show();

                        if(videoPlayer->isVisible())
                        {

                            videoPlayer->stop();
                            videoPlayer->hide();

                        }
                        else
                        {
                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            view->hide();
                            textedit->hide();
                            texteditlua->hide();
                            texteditncl->show();
                        }

                    }

                    else if (filename.endsWith(".lua") && typemidia.contains("application/x-ginga-NCLua"))
                    {

                        QFile inputFile(filename);

                        QTextStream in(&inputFile);
                        inputFile.open(QFile::ReadOnly);
                        texteditlua->setText(in.readAll());
                        texteditlua->setReadOnly(true);
                        texteditlua->setStyleSheet("QTextEdit { background-color : white; color : black; }");
                        texteditlua->show();

                        if(videoPlayer->isVisible())
                        {

                            videoPlayer->stop();
                            videoPlayer->hide();

                        }
                        else
                        {
                            if(musicPlayer != NULL)
                            {
                                musicPlayer->stop();
                            }
                            window->hide();
                            picture->hide();
                            picture1->hide();
                            view->hide();
                            textedit->hide();
                            texteditncl->hide();
                            texteditlua->show();
                        }

                    }

                }
            }
        }
    }
}
